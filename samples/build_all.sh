#!/bin/sh

for i in *.c
do
  SAMPLE=`basename $i .c`
  make PRODUCT=$SAMPLE $@
done
