# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262


CC=gcc
CFLAGS= -Wall -Wextra
PT_FLAGS= -pthread

EXENAME=
SOURCE=

DIST_NAME=
DIST_FILES=README Makefile common.h common.c ${SOURCE} 

RM_FILES=

default:

${EXENAME}: ${SOURCE} common.h


dist: 
	tar -czvf ${DIST_NAME}.tar.gz ${SOURCE} ${DIST_FILES}
clean:
	rm -rf *tar.gz ${EXENAME} ${RM_FILES}