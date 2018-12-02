# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash
UID="604853262"
PGM="lab4b"
TARBALL="$PGM-604853262.tar.gz"
TEMP=./tmp


# begin testing
echo
p=2
s="c"
echo "... $PGM supports --scale, --period, --log"
./$PGM --period=$p --scale=$s --log="LOGFILE" <<-EOF
SCALE=F
PERIOD=1
START
STOP
LOG test
OFF
EOF

ret=$?
if [ $ret -ne 0 ]
then
	echo "RETURNS RC=$ret"
	let errors+=1
fi

if [ ! -f LOGFILE ]
then
	echo "did not create a log file"
	let errors+=1
else
	rm LOGFILE
fi

