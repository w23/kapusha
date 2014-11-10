#!/bin/sh

# BORKEN :( cc -Wall -Werror -g -DDEBUG math.c ../src/math/math.c ../src/ooo/ooo.c -I../include -DKP_GCC_ATOMICS -DKP_OS_POSIX -pthread -lm -o math && $1 ./math
cc -Wall -Werror -g -DDEBUG msgqueue.c ../src/core/core.c ../src/sys/posix.c -I../include -DKP_GCC_ATOMICS -DKP_OS_POSIX -pthread -o msgqueue && $1 ./msgqueue
