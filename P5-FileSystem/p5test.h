#ifndef P5TEST_H
#define P5TEST_H

#include <assert.h>       // assert
#include <stdio.h>        // fopen, printf, 
#include <string.h>       // memset

#include "alias.h"        // i32, etc
#include "fs.h"           // fsOpen, etc

#define BLOCKS        50
#define BYTESPERBLOCK 512
#define BUFSIZE       2000

void check(i32 testnum, i8* buf, i32 start, i32 size, i32 val);
void checkCursor(i32 testnum, i32 expected, i32 actual);
void createP5();
void test1(i32 fd);
void test2(i32 fd);
void test3(i32 fd);
void test4(i32 fd);
void p5test();

#endif