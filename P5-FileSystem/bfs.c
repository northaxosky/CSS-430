// ============================================================================
// bfs.c
// ============================================================================

#include "bfs.h"

// ============================================================================
// Allocate a free disk block for the file whose Inode number is 'inum' and
// assign it to FBN 'fbn' in the file's Inode.  On success, return the DBN
// allocated.  On failure, abort
// ============================================================================
i32 bfsAllocBlock(i32 inum, i32 fbn) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);
  if (fbn  < 0)       FATAL(EBADFBN);
  if (fbn  > MAXFBN)  FATAL(EBADFBN);

  // Grab the next free block in the BFS disk

  i32 dbn = bfsFindFreeBlock();

  // Update the corresponding Inode, or IndirectBlock

  i8 buf8[BYTESPERBLOCK] = {0};           // 1-block buffer
  bioRead(DBNINODES, buf8);
 
  Inode* pinodes = (Inode*)buf8;          // array of Inodes
  Inode* pinode  = &pinodes[inum];        // target Inode

  if (fbn < NUMDIRECT) {                  // in direct[] array?
    pinode->direct[fbn] = dbn;
    bioWrite(DBNINODES, buf8);
    return dbn;
  } else {                                // in indirect block?
    i16 buf16[I16SPERBLOCK]= {0};
    i32 dbnIndirect = pinode->indirect;   // DBN of indirect block

    if (dbnIndirect == 0) {               // not yet allocated
      dbnIndirect = bfsFindFreeBlock();
      pinode->indirect = dbn;
    }

    bioRead(dbnIndirect, buf16);

    buf16[fbn - NUMDIRECT] = dbn;
    bioWrite(dbnIndirect, buf16);
  }

  return dbn;                             // allocated DBN

}



// ============================================================================
// Create file 'fname'.  Find a free inum; ie, free slot in the Directory.
// Leave the size of the file as zero, until the user performs a write, or a
// seek into the file.  On success, return the file's inum.  On failure, abort
// ============================================================================
i32 bfsCreateFile(str fname) {

  if (fname == NULL) FATAL(ENULLPTR);

  if (strlen(fname) > FNAMESIZE - 1) FATAL(EBIGFNAME);  // fname too big

  i8 buf[BYTESPERBLOCK] = {0};

  bioRead(DBNDIR, buf);

  Dir* dir = (Dir*)buf;

  for (int inum = 0; inum < NUMINODES; ++inum) {        // search Directory
    if (strlen(dir->fname[inum]) == 0) {                // free slot
      strcpy(dir->fname[inum], fname);
      bioWrite(DBNDIR, dir);
      bfsRefOFT(inum);
      return inum;
    }
  }

  FATAL(EDIRFULL);                                      // Directory full
  return 0;                                             // pacify compiler
}



// ============================================================================
// Dereference file with Inode number 'inum' in the Open File Table.  If
// refcount reaches 0, free up that entry in the OFT
// ============================================================================
i32 bfsDerefOFT(i32 inum) {
  i32 ofte = bfsFindOFTE(inum);
  --g_oft[ofte].refs;
  if (g_oft[ofte].refs == 0) {
    g_oft[ofte].inum = 0;
    g_oft[ofte].curs = 0;
  }
  return 0;
}



// ============================================================================
// Extend file 'inum' out to FBN 'fbn'
// ============================================================================
i32 bfsExtend(i32 inum, i32 fbn) {
  i32 size = bfsGetSize(inum);
  i32 fbnLast = (size + 1) / BYTESPERBLOCK;
  for (i32 f = fbnLast; f <= fbn; ++f) {
    bfsAllocBlock(inum, f);
  }
  return 0;
}



// ============================================================================
// Use Inode to find the DBN used to store file block 'fbn'.  Return ENODBN
// if not yet mapped
// ============================================================================
i32 bfsFbnToDbn(i32 inum, i32 fbn) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);
  if (fbn  < 0)       FATAL(EBADFBN);
  if (fbn  > MAXFBN)  FATAL(EBADFBN);

  Inode inode;
  
  bfsReadInode(inum, &inode);

  if (fbn < NUMDIRECT) {            // in direct[] array?
    i32 dbn = inode.direct[fbn];
    return (dbn == 0) ? ENODBN : dbn;
  }

  // fbn is not in direct, so check indirect block.  If it doesn't exist,
  // then allocate an empty indirect block.  But return ENODBN for the
  // caller to handle grabing a new data block.

  if (inode.indirect == 0) {      // no indirect block yet allocated
    i32 dbn = bfsFindFreeBlock();
    inode.indirect = dbn;
    bfsWriteInode(inum, &inode);
    return ENODBN;
  }

  // Check the indirect block

  i16 buf[NUMINDIRECT] = {0};
  bioRead(inode.indirect, buf);

  i32 dbn = buf[fbn - NUMDIRECT];
  return (dbn == 0) ? ENODBN : dbn;
}



// ============================================================================
// Convert FileDescriptor (user-visible) to Inum (internal)
// ============================================================================
i32 bfsFdToInum(i32 fd) { 
  i32 inum = fd - INUMTOFD; 
  if (inum < 0) FATAL(EBADINUM);
  return inum;
}




// ============================================================================
// Find 'inum' in the Open File Table (OFT).  If not found, create an entry.
// Return the index within the OFT.  On failure, EOFTFULL
// ============================================================================
i32 bfsFindOFTE(i32 inum) {
  for (int i = 0; i < NUMOFTENTRIES; ++i) {
    if (g_oft[i].inum == inum) return i;
  }
  
  // Not found, so look for an empty OFTE

  for (int i = 0; i < NUMOFTENTRIES; ++i) {
    if (g_oft[i].inum == 0) {
      g_oft[i].inum = inum;
      g_oft[i].curs = 0;
      g_oft[i].refs = 1;
      return i;
    }
  }
  FATAL(EOFTFULL);      // no-return
  return 0;             // pacify compiler
}



// ============================================================================
// Allocate the next free block from the Freelist.  Adjust Freelist
// accordingly.  On success, return DBN.  FATAL otherwise
// ============================================================================
i32 bfsFindFreeBlock() {
  i8 buf8[BYTESPERBLOCK] = {0};
  bioRead(DBNSUPER, buf8);
  Super* super = (Super*)buf8;

  i32 dbn = super->firstFree;
  if (dbn == 0) FATAL(EDISKFULL);

  i16 buf16[I16SPERBLOCK] = {0};      // for next free block
  bioRead(dbn, buf16);

  super->firstFree = buf16[0];        // new head of Freelist

  bioWrite(DBNSUPER, buf8);           // update SuperBlock

  return dbn;
}


// ============================================================================
// Initialize the Freelist
// ============================================================================
i32 bfsInitFreeList() {
  i16 buf[I16SPERBLOCK] = {0};
  i32 ret = 0;

  for (int dbn = NUMMETA; dbn < BLOCKSPERDISK - 1; ++dbn) {
    buf[0] = dbn + 1;
    bioWrite(dbn, (i8*)buf);
  }

  buf[0] = 0;
  bioWrite(BLOCKSPERDISK - 1, (i8*)buf);      // end of Freelist

  return ret;
}



// ============================================================================
// Write the initial Dir block, of all zeroes, into DBN 2
// ============================================================================
i32 bfsInitDir(FILE* fp) {
  if (fp == NULL) FATAL(ENULLPTR);
  i8 buf[BYTESPERBLOCK] = {0};
  return bioWrite(DBNDIR, buf);
}



// ============================================================================
// Write the initial Inodes block, of all zeroes, into DBN 1
// ============================================================================
i32 bfsInitInodes(FILE* fp) {
  if (fp == NULL) FATAL(ENULLPTR);
  i8 buf[BYTESPERBLOCK] = {0};
  return bioWrite(DBNINODES, buf);
}



// ============================================================================
// Initialize the Open File Table to all zeroes
// ============================================================================
i32 bfsInitOFT() {
  for (i32 i = 0; i < NUMOFTENTRIES; ++i) {
    g_oft[i].inum = 0;
    g_oft[i].curs = 0;
    g_oft[i].refs = 0;
  }
  return 0;
}


// ============================================================================
// Write the initial Super block into DBN 0
// ============================================================================
i32 bfsInitSuper(FILE* fp) {

  if (fp == NULL) FATAL(ENULLPTR);

  Super sb;
  sb.numBlocks = BLOCKSPERDISK;           // eg: 100
  sb.numInodes = NUMINODES;               // eg: 8
  sb.firstFree = NUMMETA;                 // eg: 3

  i8 buf[BYTESPERBLOCK] = {0};
  memcpy(buf, &sb, sizeof(Super));

  return bioWrite(DBNSUPER, buf);
}



// ============================================================================
// Convert between inum (internal) and FileDescriptor (user-visible)
// ============================================================================
i32 bfsInumToFd(i32 inum) { return inum + INUMTOFD; }


// ============================================================================
// Lookup 'fname' in the Directory.  If found, return its inum.  If not,
// return EFNF
// ============================================================================
i32 bfsLookupFile(str fname) {

  if (fname == NULL) FATAL(ENULLPTR);

  i8 buf[BYTESPERBLOCK] = {0};

  bioRead(DBNDIR, buf);

  Dir* dir = (Dir*)buf;

  for (int inum = 0; inum < NUMINODES; ++inum) {
    if (strcmp(fname, dir->fname[inum]) == 0) {
      bfsRefOFT(inum);
      return inum;
    }
  }

  return EFNF;

}



// ============================================================================
// Read FBN 'fbn' for the file whose inum is 'inum' into 'buf'
// ============================================================================
i32 bfsRead(i32 inum, i32 fbn, i8* buf) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);
  if (fbn  < 0)       FATAL(EBADFBN);
  if (fbn  > MAXFBN)  FATAL(EBADFBN);

  i32 dbn = bfsFbnToDbn(inum, fbn);

  bioRead(dbn, buf);
  return 0;
}


// ============================================================================
// Read the Inodes block.  Extract and return the Inode whose number is 'inum'.
// On success, return 0.  On failure, abort
// ============================================================================
i32 bfsReadInode(i32 inum, Inode* inode) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);
  if (inode == NULL)  FATAL(ENULLPTR);

  i8 buf[BYTESPERBLOCK] = {0};

  bioRead(DBNINODES, buf);

  Inode* inodes = (Inode*)buf;

  memcpy(inode, &inodes[inum], sizeof(Inode));
  return 0;
}



// ============================================================================
// Reference file with Inode number 'inum' in the Open File Table
// ============================================================================
i32 bfsRefOFT(i32 inum) {
  i32 ofte = bfsFindOFTE(inum);
  ++g_oft[ofte].refs;
  return 0;
}



// ============================================================================
// Set cursor position for the file open on File Descriptor 'fd' to 'newCurs'
// ============================================================================
i32 bfsSetCursor(i32 inum, i32 newCurs) {

  if (inum < 0) FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);

  i32 ofte = bfsFindOFTE(inum);
  g_oft[ofte].curs = newCurs;
  return 0;
}



// ============================================================================
// Return the cursor position for the file open on File Descriptor 'fd'
// ============================================================================
i32 bfsTell(i32 fd) {
  i32 inum = bfsFdToInum(fd);
  i32 ofte = bfsFindOFTE(inum);
  return g_oft[ofte].curs;
}



// ============================================================================
// Return the size of the file whose Inode number is 'inum'
// ============================================================================
i32 bfsGetSize(i32 inum) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);

  Inode inode;
  bfsReadInode(inum, &inode);

  return inode.size;
}



// ============================================================================
// Set size of file 'inum' to 'size
// ============================================================================
i32 bfsSetSize(i32 inum, i32 size) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);

  Inode inode;
  bfsReadInode(inum, &inode);
  
  inode.size = size;
  bfsWriteInode(inum, &inode);
  return 0;
}



// ============================================================================
// Update the Inodes block on disk with the info in 'inode'
// ============================================================================
i32 bfsWriteInode(i32 inum, Inode* inode) {

  if (inum < 0)       FATAL(EBADINUM);
  if (inum > MAXINUM) FATAL(EBADINUM);
  if (inode == NULL)  FATAL(ENULLPTR);

  i8 buf[BYTESPERBLOCK];
  bioRead(DBNINODES, buf);
  Inode* inodes = (Inode*)buf;
  memcpy(&inodes[inum], inode, sizeof(Inode));
  bioWrite(DBNINODES, buf);

  return 0;
}

