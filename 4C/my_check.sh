# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash
#UID="604853262"
TCP_PGM="lab4c_tcp"
TLS_PGM="lab4c_tls"
TARBALL="$PGM-604853262.tar.gz"

FAKEID=369713302
HOST=lever.cs.ucla.edu
TCP_PORT=18000
TLS_PORT=19000
LOG=log.txt

# begin testing
echo Running tests...
#echo ./lab4c_tcp --id=369713302 --host=lever.cs.ucla.edu 18000 --log=tcp_log.txt
#./$TCP_PGM --id=$FAKEID --host=$HOST $TCP_PORT --log=$LOG
echo ./lab4c_tls --id=369713302 --host=lever.cs.ucla.edu 19000 --log=tls_log.txt
./$TLS_PGM --id=$FAKEID --host=$HOST $TLS_PORT --log=$LOG

#./lab4c_tcp --id=369713302 --host=lever.cs.ucla.edu 18000 --log=tcp_log.txt
#./lab4c_tls --id=369713302 --host=lever.cs.ucla.edu 19000 --log=tls_log.txt
