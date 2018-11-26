#!/usr/local/bin/python3

# NAME: Richard Khillah
# EMAIL: RKhillah@ucla.edu
# ID: 604853262

import argparse, sys

class SuperBlock():
	def __init__(self, csv):

		self.s_blocks_count 	= int(csv[1])
		self.s_inodes_count 	= int(csv[2])
		self.block_size 		= int(csv[3])
		self.s_inode_size 		= int(csv[4])
		self.s_blocks_per_group = int(csv[5])
		self.s_inodes_per_group = int(csv[6])
		self.s_first_ino 		= int(csv[7])
		
class Group():
	def __init__(self, csv):
		self.group_number			= int(csv[1])
		self.s_blocks_count			= int(csv[2])
		self.s_inodes_count			= int(csv[3])
		self.bg_free_blocks_count	= int(csv[4])
		self.bg_free_inodes_count	= int(csv[5])
		self.bg_block_bitmap		= int(csv[6])
		self.bg_inode_bitmap		= int(csv[7])
		self.bg_inode_table			= int(csv[8])

class Inode():
	def __init__(self, csv):
		self.INODE						= int(csv[1])
		self.inode_number				= csv[2]
		self.file_type					= int(csv[3])
		self.mode						= int(csv[4])
		self.owner						= int(csv[5])
		self.group						= int(csv[6])
		self.link_count					= int(csv[7])
		self.time_of_last_Inode_change	= csv[8]
		self.modification_time			= csv[9]
		self.time_of_last_access		= csv[10]
		self.file_size					= int(csv[11])
		self.num_blocks_on_disk			= [int(x) for x in csv[12:]]
		
class DirectoryEntry():
	def __init__(self, csv):
		self.parent_inode_num	= int(csv[1])
		self.local_offset		= int(csv[2])
		self.inode_num			= int(csv[3])
		self.entry_length		= int(csv[4])
		self.name_length		= int(csv[5])
		self.name				= csv[6]
		
class IndirectReference():
	def __init__(self, csv):
		self.owner_inode_num		= int(csv[1])
		self.level_of_indirection	= int(csv[2])
		self.local_offset			= int(csv[3])
		self.indrect_block_num		= int(csv[4])
		self.reference_block_num	= int(csv[5])

def main():
	parser = argparse.ArgumentParser()
	program_name = parser.prog # get program name
	
	if len(sys.argv) != 2:
		print('Error: Invalid number of arguments')
		print('Usage: ./{} <filename>'.format(program_name))

if __name__ == '__main__':
	main()