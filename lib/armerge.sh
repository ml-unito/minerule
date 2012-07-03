#!/bin/bash

echo Cleaning up...
rm -f libmr.a
cd objects
rm -f *.o
FILES=`ls ../*.a`

echo Extracting object files...
for f in $FILES; do
    ar x $f
done

echo Building up the archive...
ar rcs libmr.a *.o
mv libmr.a ..
