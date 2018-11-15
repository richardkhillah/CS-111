#!/bin/bash
UID="604853262"
PGM="lab4b"
TARBALL="$PGM-604853262.tar.gz"
TEMP=./tmp


# make a testing directory and move a copy of executable there
if [ -d $TEMP ]
then
	echo Deleting old $TEMP
	rm -rf $TEMP
fi
mkdir $TEMP
tar -xzvf $TARBALL -C $TEMP
cd $TEMP

# begin testing
echo
p=2
s="c"
echo "... $PGM supports --scale, --period, --log --debug"
./$PGM --period=$p --scale=$s --log="LOGFILE" --debug <<-EOF
# SCALE=F
# PERIOD=1
# START
# STOP
# LOG test
# OFF
EOF

ret=$?
if [ $ret -ne 0 ]
then
	echo "RETURNS RC=$ret"
	let errors+=1
fi

if [ -f LOGFILE ]
then
	echo "did not create a log file"
	let errors+=1
else
	echo "... $PGM supports and logs all sensor commands"
	for c in SCALE=F PERIOD=1 START STOP OFF SHUTDOWN "LOG test"
	do
		grep "$c" LOGFILE > /dev/null
		if [ $? -ne 0 ]
		then
			echo "DID NOT LOG $c command"
			let errors+=1
		else
			echo "    $c ... RECOGNIZED AND LOGGED"
		fi
	done

	if [ $errors -gt 0 ]
	then
		echo "   LOG FILE DUMP FOLLOWS   "
		cat LOGFILE
	fi
fi


# Remove testing directory
cd ..
rm -rf $TEMP