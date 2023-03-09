#ifndef ERRORS_H
#define ERRORS_H

#include "alias.h"

#define FATAL(err) { printf("\nERROR: File %s, Line %d \n", __FILE__, __LINE__); \
                     RepTest(err, __FILE__, __LINE__); }

void RepTest(int err, str file, int line);

#define EBADCURS    -1    // invalid cursor (byte offset into file)
#define EBADDBN     -2    // invalid DBN
#define EBADFBN     -3    // invalid FBN
#define EBADINUM    -4    // invalid inum
#define EBADREAD    -5    // error reading from BFS disk
#define EBADWHENCE  -6    // Invalide 'whence' in fsSeek
#define EBADWRITE   -7    // error writing to BFS disk
#define EBIGFNAME   -8    // filename too big
#define EBIGNUMB    -9    // number of bytes to transfer too big
#define EDIRFULL    -10   // Directory full
#define EDISKCREATE -11   // Failed to create new BFS disk
#define EDISKFULL   -12   // BFS disk has no free blocks
#define EEXISTS     -13   // BFS disk already exists, so don't format it!
#define EFNF        -14   // File Not Found
#define ENEGNUMB    -15   // negative number of bytes to transfer
#define ENODBN      -16   // no DBN yet allocated - non fatal
#define ENODISK     -17   // cannot open BFSDISK
#define ENOMEM      -18   // no memory (malloc failed)
#define ENULLPTR    -19   // about to deref a NULL pointer
#define ENYI        -20   // not yet implemented
#define EOFTFULL    -21   // OpenFileTable is full

void pause();
void RepError(i32 ret);

#endif