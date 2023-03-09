// ============================================================================
// deb.c - functions to help debug the BFS FileSystem
// ============================================================================

#include "bfs.h"
#include "deb.h"

// ============================================================================
// Dump block DBN
// ============================================================================
i32 debDumpDbn(i32 dbn, i32 size) {
  i8 buf[BYTESPERBLOCK] = {0};

  i8*  buf8  = (i8*) buf;
  i16* buf16 = (i16*)buf;
  i32* buf32 = (i32*)buf;

  bioRead(dbn, buf);

  printf("\n");
  if (size == 1) {
    for (int i = 0; i < BYTESPERBLOCK; ++i) {
      printf("%02x ", buf8[i]);
      if ((i + 1) % 16 == 0) {
        for (int i = 0; i < 16; ++i) {
          char c = buf8[i];
          if (!isprint(c)) c = '.';
          printf("%c", c);
        }
        printf("\n");
      }
    }
  } else if (size == 2) {
    for (int i = 0; i < BYTESPERBLOCK / sizeof(i16); ++i) {
      printf("%04x ", buf16[i]);
      if ((i + 1) % 8 == 0) printf("\n");
    }
  } else if (size == 4) {
    for (int i = 0; i < BYTESPERBLOCK / sizeof(i32); ++i) {
      printf("%08x ", buf32[i]);
      if ((i + 1) % 4 == 0) printf("\n");
    }
  } else {
    printf("debDumpDbn: size must be 1, 2 or 4 \n");
  }

  return 0;
}



// ============================================================================
// Dump the Dir
// ============================================================================
i32 debDumpDir() {
  i8 buf[BYTESPERBLOCK] = {0};
  bioRead(DBNDIR, buf);
  Dir* dir = (Dir*)buf;

  printf("\n");
  for (int inum = 0; inum < NUMINODES; ++inum) {
    printf("[%02d]  %s \n", inum, dir->fname[inum]);
  }
  printf("\n"); fflush(stdout);

  return 0;
}



// ============================================================================
// Dump the Inodes
// ============================================================================
i32 debDumpInodes() {
  i8 buf[BYTESPERBLOCK] = {0};
  bioRead(DBNINODES, buf);

  Inode* inodes = (Inode*) buf;

  printf("\n");
  for (int inum = 0; inum < NUMINODES; ++inum) {
    Inode inode = inodes[inum];
    printf("[%d] size = %d \n", inum, inode.size);
    for (i32 d = 0; d < NUMDIRECT; ++d) {
      printf("    [%d] direct[%d] = %d \n", inum, d, inode.direct[d]);
    }
    printf("        indirect  = %d \n", inode.indirect);
  }
  printf("\n"); fflush(stdout);

  return 0;
}


// ============================================================================
// Dump the Superblock
// ============================================================================
i32 debDumpSuper() {
  i8 buf[BYTESPERBLOCK] = {0};

  bioRead(DBNSUPER, buf);

  Super* super = (Super*)buf;

  printf("\n");
  printf("Super.numBlocks = %d \n", super->numBlocks);
  printf("Super.numInodes = %d \n", super->numInodes);
  printf("Super.firstFree = %d \n", super->firstFree);
  printf("\n"); fflush(stdout);

  // Check that remainder of Superblock is all zeroes

  for (i32 b = sizeof(Super); b < BYTESPERBLOCK; ++b) {
    if (buf[b] != 0) {
      printf("Super[%d] == %02x, should be 0x00 \n", b, buf[b]);
    }
  }
  fflush(stdout);

  return 0;
}

