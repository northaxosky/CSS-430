#!/bin/bash

rm -f a.out

gcc -Wall -Wextra -Wno-sign-compare *.c

./a.out
