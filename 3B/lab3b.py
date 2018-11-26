#!/usr/local/bin/python3

# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

import argparse, sys

def main():
	parser = argparse.ArgumentParser()
	program_name = parser.prog # get program name
	
	if len(sys.argv) != 2:
		print('Error: Invalid number of arguments')
		print('Usage: {} filename'.format(program_name))

if __name__ == '__main__':
	main()