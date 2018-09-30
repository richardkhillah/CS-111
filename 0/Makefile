# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

#TODO: modularize testing and/or error reporting.

CC=gcc
CFLAGS= -Wall -Wextra

DEBUG= -g

MKDIR := mkdir -vp
ECHO := echo
SHELL := /bin/sh

EXIT0=0
EXIT1=1
EXIT2=2
EXIT3=3
EXIT4=4
EXIT5=5

SEGFAULT=139

all: lab0

lab0: lab0.c
	$(CC) $(CFLAGS) -o $@ $<

#TODO make this part of lab0 rule.
d_lab0: lab0.c
	$(CC) $(DEBUG) $(CFLAGS) -o $@ $<

dist:
	tar -cvzf lab0-604853262.tar.gz Makefile lab0.c README backtrace.png breakpoint.png

check: check_input \
	check_input_permissions_denied \
	check_output_permissions_denied \
	check_input_output \
	check_catch_segfault \
	check_segfault

clean: clean_test_dir
	rm -f lab0 

clean_test_dir:
	rm -rf $(test_dir)


test_dir=test_dir
.ONESHELL:
check_input:
	@$(MKDIR) $(test_dir)
	@$(ECHO) -n Testing \`--input\` on readable file...
	@$(ECHO) "Testing \`--input\` option..." > $(test_dir)/$@_input.txt
	@./lab0 --input=$(test_dir)/$@_input.txt > \
		$(test_dir)/$@_redirect.txt 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT0) ]]
	@then
		@if [[ `cmp -s $(test_dir)/$@_input.txt \
			$(test_dir)/$@_redirect.txt >/dev/null` -eq 0 ]]
		@then
			@echo -e "\t passed"
		@else
			@echo -e "\t failed"
		@fi
	@else
		@echo -e "\t failed"
	@fi

check_input_permissions_denied:
	@$(ECHO) -n Testing \`--input\` on non-readable file...
	@touch $(test_dir)/$@_input.txt
	@chmod -r $(test_dir)/$@_input.txt
	@./lab0 --input=$(test_dir)/$@_input.txt 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

good_input=good_input_permissions.txt
check_output_permissions_denied:
	@$(ECHO) -n Testing \`--output\` to non-writable file...
	touch $(test_dir)/$@.txt ;  chmod -wr $(test_dir)/$@.txt
	touch $(test_dir)/$(good_input) ; chmod +r $(test_dir)/$(good_input)
	@./lab0 --input=$(test_dir)/$(good_input) \
		--output=$(test_dir)/$@.txt \
		2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_input_output: check_input
	@$(MKDIR) $(test_dir)
	@touch $(test_dir)/$@_input.txt ; chmod +rw $(test_dir)/$@_input.txt
	@$(ECHO) -n Testing \`--input --output\` options with readable files...
	@$(ECHO) "Testing \`--input --output\` options..." > $(test_dir)/$@_input.txt

	@./lab0 --input=$(test_dir)/$@_input.txt \
		--output=$(test_dir)/$@_output.txt \
		2>&1>/dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT0) ]]
	@then
		@if [[ -s $(test_dir)/$@_output.txt ]]
		@then
			@echo -e "\t passed"
		@else
			@echo -e "\t failed"
		@fi
	@else
		@echo -e "\t failed"
	@fi

check_segfault:
	@$(ECHO) -n Testing \`--segfault\` option...
	@./lab0 --segfault 2>&1>/dev/null 
	@if [[ $$? -eq $(SEGFAULT) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_catch:
	@$(ECHO) -n Testing \`--catch\` option...
	@./lab0 --catch > 2>&1>/dev/null 2>&1>/dev/null
	@if [[ $$? -ne $(EXIT0) ]]
	@then
		@echo -e "\t failed"
	@else
		@echo -e "\t ok"
	@fi

check_catch_segfault:
	@$(ECHO) -n Testing \`--catch --segfault\` options...
	@./lab0 --catch --segfault 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT4) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_no_args:
	@$(ECHO) -n Testing executable witout arguments...
	@./lab0 2>&1>/dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT5) ]]
	@then 
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_invalid_arg:
	@$(ECHO) -n Testing executable with invalid  argument...
	@./lab0 input 2>&1>/dev/null 2>&1>/dev/null 
	@if [ $$? -eq 1 ] 
	@then 
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi
