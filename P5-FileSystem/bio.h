#ifndef BIO_H
#define BIO_H

// ===================================================================
// bio.h - Block IO interface.  Simulates kernel-mode read and write
// functions to the BFS disk
// ===================================================================

#include <stdio.h>

#include "alias.h"

i32 bioRead (i32 dbn, void* buf);
i32 bioWrite(i32 dbn, void* buf);

#endif