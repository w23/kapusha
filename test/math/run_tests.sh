#!/bin/sh

cc -g -DDEBUG math.c ../../src/math/math.c -I../../include -DKP_GCC_ATOMICS -lm -o math && $1 ./math