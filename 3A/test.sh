#! /bin/bash

OUTFILE=output.csv
CHECKFILE=trivial.csv

S_OF=sorted_output.csv
S_CF=sorted_checkfile.csv

./lab3a trivial.img >$OUTFILE

sort $OUTFILE >$S_OF
sort $CHECKFILE >$S_CF

echo sorted outfile sorted checkfile
diff $S_OF $S_CF

rm -rf $S_OF $S_CF $OUTFILE