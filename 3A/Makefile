# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

UID=604853262
LAB_NUMBER=lab3
LAB_PART=a
EXECUTABLE_NAME=$(LAB_NUMBER)$(LAB_PART)
EXECUTABLE_SOURCE=$(EXECUTABLE_NAME).c
TEST_SCRIPT=test.sh
CHECK_SCRIPT=P3A_check.sh


CC=gcc

CFLAGS= -Wall -Wextra -std=gnu99 -g
MFLAGS= -lm

DEFAULT=Makefile README utils.c utils.h

EXTRAS=ext2_fs.h


default: build

build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
	$(CC) $(CFLAGS) -o $@ $(EXECUTABLE_SOURCE) utils.c $(MFLAGS)

dist: build
	tar -cvzf $(LAB_NUMBER)$(LAB_PART)-$(UID).tar.gz $(DEFAULT) $(EXECUTABLE_NAME) $(EXECUTABLE_SOURCE) $(EXTRAS) $(TEST_SCRIPT)

clean:
	rm -rf *tar.gz

test: 
	./$(TEST_SCRIPT)

check:
	./$(CHECK_SCRIPT) $(UID)
