// ============================================================================
// bio.c - low level Block IO functions
// ============================================================================

#include "bfs.h"
#include "bio.h"

// ============================================================================
// Read 512 bytes from block number 'dbn' in the BFS disk into buffer 'buf'
// ============================================================================
i32 bioRead(i32 dbn, void* buf) {

  if (dbn < 0)             FATAL(EBADDBN);
  if (dbn > BLOCKSPERDISK) FATAL(EBADDBN);

  FILE* fp = fopen(BFSDISK, "rb+");
  if (fp == NULL) FATAL(ENODISK);

  i32 boff = dbn * BYTESPERBLOCK;
  i32 ret  = fseek(fp, boff, SEEK_SET);
  if (ret != 0) { fclose(fp); FATAL(ret); }

  i32 numb = fread(buf, 1, BYTESPERBLOCK, fp);
  if (numb != BYTESPERBLOCK) { fclose(fp); FATAL(EBADREAD); }

  fclose(fp);
  return 0;
}


// ============================================================================
// Write 512 bytes from 'buf' into block number 'dbn' of the BFS disk
// ============================================================================
i32 bioWrite(i32 dbn, void* buf) {

  FILE* fp = fopen(BFSDISK, "rb+");
  if (fp == NULL) FATAL(ENODISK);

  i32 boff = dbn * BYTESPERBLOCK;
  i32 ret  = fseek(fp, boff, SEEK_SET);
  if (ret != 0) { fclose(fp); FATAL(ret); }

  i32 numb = fwrite(buf, 1, BYTESPERBLOCK, fp);
  if (numb != BYTESPERBLOCK) { fclose(fp); FATAL(EBADWRITE); }

  fclose(fp);

  return 0;
}
