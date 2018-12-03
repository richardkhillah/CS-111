# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#!/bin/bash
UID="604853262"
TCP_PGM="lab4c_tcp"
TLS_PGM="lab4c_tls"
TARBALL="$PGM-604853262.tar.gz"
TEMP=./tmp

FAKEID=369713302
HOST=lever.cs.ucla.edu
TCP_PORT=18000
TLS_PORT=19000
LOG=log.txt

# begin testing
echo Running TCP program
./$PGM --id=$(FAKEID) --host=$(HOST) $(TCP_PORT) --log=$(LOG)
