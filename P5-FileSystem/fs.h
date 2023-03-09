#ifndef FS_H
#define FS_H

// ===================================================================
// fs.h - File System user interface
// ===================================================================

#include <stdio.h>
#include "alias.h"
#include "errors.h"

i32 fsClose (i32 fd);
i32 fsCreate(str name);
i32 fsFormat();
i32 fsMount();
i32 fsOpen  (str fname);
i32 fsRead  (i32 fd, i32 numb,   void* buf);
i32 fsSeek  (i32 fd, i32 offset, i32   whence);
i32 fsSize  (i32 fd);
i32 fsTell  (i32 fd);
i32 fsWrite (i32 fd, i32 numb,   void* buf);

#endif