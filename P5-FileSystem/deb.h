#ifndef DEB_H
#define DEB_H

// ============================================================================
// deb.h - functions to help debug the BFS FileSystem
// ============================================================================

#include <ctype.h>
#include <stdio.h>
#include "alias.h"

i32 debDumpDbn   (i32 dbn, i32 size);
i32 debDumpDir   ();
i32 debDumpInodes();
i32 debDumpSuper ();

#endif