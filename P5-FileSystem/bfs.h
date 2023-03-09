#ifndef BFS_H
#define BFS_H

// ===================================================================
// bfs.h - API to Bothell File System
// ===================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alias.h"
#include "bio.h"
#include "errors.h"

#define BYTESPERBLOCK 512
#define I16SPERBLOCK  256
#define BLOCKSPERDISK 100
#define BYTESPERDISK  (BLOCKSPERDISK * BYTESPERBLOCK)
#define NUMINODES     8
#define MAXINUM       NUMINODES - 1
#define NUMMETA       3
#define MINDBN        3
#define BFSDISK       "BFSDISK"
#define NUMDIRECT     5
#define NUMINDIRECT   BYTESPERBLOCK / sizeof(i16)
#define MAXFBN        NUMDIRECT + NUMINDIRECT
#define FNAMESIZE     16

#define DBNSUPER      0
#define DBNINODES     1
#define DBNDIR        2

#define INUMTOFD      5

#define NUMOFTENTRIES 20


typedef struct {          // SuperBlock
  i16 numBlocks;          // total # of blocks in BFSDISK = 1,000
  i16 numInodes;          // total # of inodes = 8
  i16 firstFree;          // DBN of first free block
} Super;



typedef struct {          // Inode
  i32 size;               // # of bytes in file
  i16 direct[NUMDIRECT];  // DBNs for first 5 FBNs
  i16 indirect;           // DBN of the indirect table
} Inode;



typedef struct {          // Dir
  char fname[NUMINODES][FNAMESIZE];
} Dir;


typedef struct {          // Open File Table Entry
  i32 inum;               // inum of file. O => slot not used
  i32 refs;               // # processes fsOpen'd this file
  i32 curs;               // cursor into file
} OFTE;

OFTE g_oft[NUMOFTENTRIES];

i32 bfsAllocBlock(i32 inum, i32 fbn);
i32 bfsCreateFile(str fname);
i32 bfsDerefOFT(i32 inum);
i32 bfsExtend(i32 inum, i32 fbn);
i32 bfsFbnToDbn(i32 inum,   i32 fbn);
i32 bfsFdToInum(i32 fd);
i32 bfsFindFreeBlock();
i32 bfsFindOFTE(i32 inum);
i32 bfsGetSize(i32 inum);
i32 bfsInitDir();
i32 bfsInitFreeList();
i32 bfsInitInodes();
i32 bfsInitOFT();
i32 bfsInitSuper(FILE* fp);
i32 bfsInumToFd(i32 inum);
i32 bfsLookupFile(str fname);
i32 bfsRead(i32 inum, i32 fbn, i8* buf);
i32 bfsReadInode(i32 inum, Inode* inode);
i32 bfsRefOFT(i32 inum);
i32 bfsSetCursor(i32 inum, i32 newCurs);
i32 bfsSetSize(i32 inum, i32 size);
i32 bfsTell(i32 fd);
i32 bfsWriteInode(i32 inum, Inode* inode);

#endif