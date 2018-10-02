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

SEGFAULT=139

all: lab0

lab0: lab0.c
	$(CC) $(DEBUG) $(CFLAGS) -o $@ $<

#TODO make this part of lab0 rule.
d_lab0: lab0.c
	$(CC) $(DEBUG) $(CFLAGS) -o $@ $<

dist:
	@tar -cvzf lab0-604853262.tar.gz lab0.c Makefile backtrace.png breakpoint.png README

check: create_tst \
	check_i \
	check_o \
	check_io \
	check_s \
	check_sc \
	check_sci \
	check_sco \
	check_scio \
	check_badarg

clean: 
	@rm -rf lab0 $(TST) *tar.gz

TST=tst
goodintxt=goodinput.txt
badintxt=badinput.txt
badouttxt=badoutput.txt

goodinput=$(TST)/$(goodintxt)
badinput=$(TST)/$(badintxt)
badoutput=$(TST)/$(badouttxt)

.ONESHELL:
create_tst:
	@$(MKDIR) $(TST) >/dev/null;
	@$(ECHO) "This is a good input file..." > $(goodinput)
	@$(ECHO) "This is a bad input file..." > $(badinput)
	@touch $(badoutput)

	@chmod +r $(goodinput)
	@chmod -rw $(badinput)
	@chmod -rw $(badoutput)


check_i: create_tst
# --input=goodinput
	@$(ECHO) -n Testing --input=readable_file ...
	@./lab0 --input=$(goodinput) > $(TST)/$@good.txt 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT0) ]]
	@then
		@if [[ `cmp -s $(goodinput) $(TST)/$@good.txt >/dev/null` -eq 0 ]]
		@then
			@echo -e "\t passed"
		@else
			@echo -e "\t failed"
		@fi
	@else
		@echo -e "\t failed"
	@fi
# --input=badinput
	@$(ECHO) -n Testing --input=unreadable_file ...
	@./lab0 --input=$(badinput) 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

# --input=doesnotexit
	@$(ECHO) -n Testing --input=does_not_exist ...
	@./lab0 --input=does_not_exit.txt 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_o:
# --input=badoutput
	@$(ECHO) -n Testing --output=unreadable_file ...
	@./lab0 --output=$(badoutput) 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_io :create_tst
#--output=output_check_iogood.txt
	@$(ECHO) -n Testing --input=readable_file --output=STDOUT ...
	@./lab0 --input=$(goodinput) --output=$(TST)/output_$@good.txt 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT0) ]]
	@then
		@if [[ -s $(TST)/output_$@good.txt ]]
		@then
			@echo -e "\t passed"
		@else
			@echo -e "\t failed"
		@fi
	@else
		@echo -e "\t failed"
	@fi

#--input=badinput --output=output_check_iobad.txt
	@$(ECHO) -n Testing --input=unreadable_file and --output=STDIO ...
	@./lab0 --input=$(badinput) --output=$(TST)/output_$@bad.txt 2>&1>/dev/null \
		2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@if [[ -s $(TST)/output_$@bad.txt ]]
		@then
			@echo -e "\t failed"
		@else
			@echo -e "\t passed"
		@fi
	@else
		@echo -e "\t failed"
	@fi

#--input=goodinput --output=badoutput
	@$(ECHO) -n Testing --input=readable_file --output=unreadable_file ...
	@./lab0 --input=$(goodinput) --output=$(badoutput) 2>&1>/dev/null \
		2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) ]]
	@then
		@if [[ -s $(TST)/output_$@bad.txt ]]
		@then
			@echo -e "\t failed"
		@else
			@echo -e "\t passed"
		@fi
	@else
		@echo -e "\t failed"
	@fi



#--input=badinput --output=badoutput
	@$(ECHO) -n Testing --input=unreadable_file --output=unreadable_file ...
	@./lab0 --input=$(badinput) --output=$(badoutput) 2>&1>/dev/null \
		2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) || $(EXIT3) ]]
	@then
		@if [[ -s $(TST)/output_$@bad.txt ]]
		@then
			@echo -e "\t failed"
		@else
			@echo -e "\t passed"
		@fi
	@else
		@echo -e "\t failed"
	@fi


check_s :create_tst
# --segfault
	@$(ECHO) -n Testing --segfault ...
	@./lab0 --segfault 2>&1>/dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(SEGFAULT) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


check_sc :create_tst
# --segfault --catch
	@$(ECHO) -n Testing --catch --segfault ...
	@./lab0 --catch --segfault 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT4) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


check_sci :create_tst
# --segfault --catch --input=goodinput
	@$(ECHO) -n Testing --catch --segfault --input=readable_file ...
	@./lab0 --catch --segfault --input=$(goodinput) 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT4) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

# --segfault --catch --input=badinput
	@$(ECHO) -n Testing --catch --segfault --input=unreadable_file ...
	@./lab0 --catch --segfault --input=$(badinput) 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_sco: create_tst
# --segfault --catch --output=output_$@good.txt
	@$(ECHO) -n Testing --catch --segfault --output=readable_file ...
	@./lab0 --catch --segfault --output=$(TST)/output_$@good.txt 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT4) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

# --segfault --catch --output=output_$@bad.txt
	@$(ECHO) -n Testing --catch --segfault --output=unreadable_file ...
	@./lab0 --catch --segfault --output=$(badoutput) 2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


check_scio :create_tst
# --segfault --catch --input=goodinput --output=output_goodscio.txt
	@$(ECHO) -n Testing --catch --segfault --input=readable_file --output=readable_file ...
	@./lab0 --catch --segfault --input=$(goodinput) --output=$(TST)/output_$@good.txt \
		2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT4) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


# --segfault --catch --input=badinput --output=output_badscio.txt
	@$(ECHO) -n Testing --catch --segfault --input=unreadable_file --output=readable_file ...
	@./lab0 --catch --segfault --input=$(badinput) --output=output_$@bad.txt \
		2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


# --segfault --catch --input=goodinput --output=badoutput
	@$(ECHO) -n Testing --catch --segfault --input=readable_file --output=unreadable_file ... 
	@./lab0 --catch --segfault --input=$(goodinput) --output=$(badoutput) \
		2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi


# --segfault --catch --input=badinput --output=badoutput
	@$(ECHO) -n Testing --catch --segfault --input=unreadable_file --output=unreadable_file ...
	@./lab0 --catch --segfault --input=$(badinput) --output=$(badoutput) \
		2>&1> /dev/null 2>&1>/dev/null
	@if [[ $$? -eq $(EXIT3) || $(EXIT2) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

check_badarg:
	@$(ECHO) -n Testing --invalid_argument ...
	@./lab0 --invalid_argument 2>&1>/dev/null 2>&1>/dev/null
	@if [[  $$? -eq $(EXIT1) ]]
	@then
		@echo -e "\t passed"
	@else
		@echo -e "\t failed"
	@fi

