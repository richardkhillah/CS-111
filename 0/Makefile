# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#TODO: modularize testing and/or error reporting.

CC=gcc
CFLAGS= -Wall -Wextra

DEBUG= -g

MKDIR := mkdir -vp
ECHO := echo

all: lab0

lab0: lab0.c
	$(CC) $(CFLAGS) -o $@ $<

dist:
	tar -cvzf lab0-604853262.tar.gz Makefile lab0.c README \
backtrace.png breakpoint.png

check: check_input check_input_permissions_denied check_input_output check_catch_segfault

#TODO make this part of lab0 rule.
d_lab0: lab0.c
	$(CC) $(DEBUG) $(CFLAGS) -o $@ $<

test_dir=test_dir
check_input:
	@$(MKDIR) $(test_dir)
	@$(ECHO) -n Testing \`--input\` on readable file...
	@$(ECHO) "Testing \`--input\` option..." > $(test_dir)/test_input.txt
	@./lab0 --input=$(test_dir)/test_input.txt > $(test_dir)/$@.txt; \
	if [[ $$? -ne 0 ]]; then \
		echo "\t failed"; \
	else \
		if [[ `cmp $(test_dir)/test_input.txt $(test_dir)/check_input.txt` -ne 0 ]]; then \
			echo "\t failed"; \
		else \
			echo "\t ok"; \
		fi; \
	fi;

check_input_permissions_denied:
	@$(ECHO) -n Testing \`--input\` on non-readable file...;
	@touch $(test_dir)/$@.txt ;  chmod -r $(test_dir)/$@.txt
	@./lab0 --input=$(test_dir)/$@.txt 2>&1> /dev/null ; \
	if [[ $$? -ne 2 ]]; then \
		echo "\t failed"; \
	else \
		echo "\t ok"; \
	fi;

check_input_output: check_input
	@$(MKDIR) $(test_dir)
	@$(ECHO) -n Testing \`--input\` \`--output\` options...
	@$(ECHO) "Testing \`--input\` \`--output \` options..." > $(test_dir)/test_input_output.txt
	@./lab0 --input=$(test_dir)/test_input_output.txt --output=$(test_dir)/$@.txt; \
	if [[ $$? -ne 0 ]]; then \
		echo "\t failed"; \
	else \
		if [[ `cmp -s $(test_dir)/test_input_output.txt \
			$(test_dir)/check_input_output.txt >/dev/null` -ne 0 ]]; then \
			echo "\t failed"; \
		else \
			echo "\t ok"; \
		fi; \
	fi;

check_catch:
	@$(ECHO) -n Testing \`--catch\` option...
	@./lab0 --catch; \
	if [[ $$? -ne 0 ]]; then \
		echo "\t failed"; \
	else \
		echo "\t ok"; \
	fi;

check_segfault:
	@$(ECHO) -n Testing \`--segfault\` option...

check_catch_segfault:
	@$(ECHO) -n Testing \`--catch --segfault\` options...
	@./lab0 --catch --segfault 2>&1> /dev/null; \
	if [[ $$? -ne 4 ]]; then \
		echo "\t failed"; \
	else \
		echo "\t ok"; \
	fi;

clean: clean_test_dir
	rm -f lab0

clean_test_dir:
	rm -rf $(test_dir)
