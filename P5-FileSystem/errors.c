// ============================================================================
// errors.c
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void pause() {
  printf("\nHit any key to finish ");
  getchar();
  exit(0);
}



void RepTest(int err, str file, int line) {
  RepError(err);
  printf(" in file %s at line %d \n", file, line);
  pause();
}


void RepError(i32 e) {
  switch(e) {
    case EBADDBN:
      printf("\nERROR: Bad DBN: negative or too large \n");    pause(); break;
    case EBADFBN:
      printf("\nERROR: Bad FBN: negative or too large \n");    pause(); break;
    case EBADINUM:
      printf("\nERROR: Bad Inum: negative or too large \n");   pause(); break;
    case EBADCURS:
      printf("\nERROR: Bad cursor within file \n");           pause(); break;
    case EBADREAD:
      printf("\nERROR: Error writing to BFS disk \n");         pause(); break;
    case EBADWRITE:
      printf("\nERROR: Error writing to BFS disk \n");         pause(); break;
    case EBIGFNAME:
      printf("\nERROR: Filename too big \n");                  pause(); break;
    case EBIGNUMB:
      printf("\nERROR: Read or write is too big \n");          pause(); break;
    case EDIRFULL:
      printf("\nERROR: Directory is already full \n");         pause(); break;
    case EDISKCREATE:
      printf("\nERROR: Failure creating BFS disk \n");         pause(); break;
    case EDISKFULL:
      printf("\nERROR: Disk is full \n");                      pause(); break;
    case EEXISTS:
      printf("\nERROR: Format would destroy current disk \n"); pause(); break;
    case EFNF:
      printf("\nERROR: File Not Found \n");                    pause(); break;
    case ENEGNUMB:
      printf("\nERROR: Negative # bytes in read or write \n"); pause(); break;
    case ENODBN:
      printf("\nERROR: No DBN yet allocated - non-fatal \n");  pause(); break;
    case ENODISK:
      printf("\nERROR: Cannot open the BFS disk \n");          pause(); break;
    case ENOMEM:
      printf("\nERROR: Failure to malloc memory \n");          pause(); break;
    case ENULLPTR:
      printf("\nERROR: About to deref a null pointer \n");     pause(); break;
    case ENYI:
      printf("\nERROR: Function Note Yet Implemented \n");     pause(); break;
    case EOFTFULL:
      printf("\nERROR: OpenFileTable is full \n");             pause(); break;
    case EBADWHENCE:
      printf("\nERROR: Invalid 'whence' in fsSeek \n");        pause(); break;
    default:
      printf("\nERROR: Miscellaneous error \n");               pause(); break;
  }
}

