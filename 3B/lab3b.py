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
		self.inumber					= int(csv[1])
		self.file_type					= csv[2]
		self.mode						= int(csv[3])
		self.owner						= int(csv[4])
		self.group						= int(csv[5])
		self.link_count					= int(csv[6])
		self.time_of_last_Inode_change	= int(csv[7])
		self.modification_time			= csv[8]
		self.time_of_last_access		= csv[9]
		self.file_size					= csv[10]
		self.num_blocks_on_disk			= int(csv[11])
		self.block_pointers				= [int(x) for x in csv[12:]]
		
class DirectoryEntry():
	def __init__(self, csv):
		self.parent_inumber		= int(csv[1])
		self.local_offset		= int(csv[2])
		self.inumber			= int(csv[3])
		self.entry_length		= int(csv[4])
		self.name_length		= int(csv[5])
		self.name				= csv[6]
		
class IndirectReference():
	def __init__(self, csv):
		self.owner_inumber			= int(csv[1])
		self.level_of_indirection	= int(csv[2])
		self.local_offset			= int(csv[3])
		self.inidrect_block_num		= int(csv[4])
		self.reference_block_num	= int(csv[5])

def main():
	parser = argparse.ArgumentParser()
	program_name = parser.prog # get program name
	
	if len(sys.argv) != 2:
		print('Error: Invalid number of arguments')
		print('Usage: ./{} <filename>'.format(program_name))

	filename = sys.argv[1]
	file = open(filename, 'r')
									# c's mapping
	superblock 			= None		# super_block
	group 				= None		# group
	free_blocks 		= set()		# free_block
	free_inodes 		= set()		# free_inodes
	inodes 				= []		# type Inode()				# inodes
	directory_entries	= []		# type DirectoryEntry()		# dir_entries
	indirect_references	= []		# type IndirectReference()	# indirects

	# read in all .csv lines, grouping as you go
	for line in file: 
		line = rstrip()
		elements = line.split(',')
		if(elements[0] == 'SUPERBLOCK'):
			superblock = SuperBlock(elements)
		elif(elements[0] == 'GROUP'):
			group = Group(elements)
		elif(elements[0] == 'BFREE'):
			free_blocks.add(int(elements[1]))
		elif(elements[0] == 'IFREE'):
			free_inodes.add(int(elements[1]))
		elif(elements[0] == 'DIRENT'):
			directory_entries.append(DirectoryEntry(elements))
		elif(elements[0] == 'INODE'):
			inodes.append(Inode(elements))
		elif(elements[0] == 'INDIRECT'):
			indirect_references.append(IndirectReference(elements))

	# acknowledged_inodes = set()
	# # Acknowledge each Inode by adding it's number to inodes[]
	# for node in inodes:
	# 	acknowledged_inodes.add(node.inumber)
	# processed_inodes = set()

	"""
	create a list of all the blocks and inodes we know about and have not 
	processed. As we process elements from either set, remove the element
	so as to indicate that the element (either block or inode) was seen and 
	processed.
	"""
	blocks_not_seen = set([i for i in range(8, superblock.s_blocks_count)])
	inodes_not_seen = set([i for i in range(superblock.s_first_ino, superblock.s_inodes_count+1)])
	# Block Consistency Audits
	for inode in inodes:
		# If any block pointer is not valid, print an error message
		for i in range(15):
			b = 'BLOCK'
			offset = i
			if i == 12:
				b = 'INDIRECT BLOCK'
				offset = 12
			if i == 13:
				b = 'DOUBLE INDIRECT BLOCK'
				offset = 12 + 256
			if i == 14:
				b = 'TRIPPLE INDIRECT BLOCK'
				offset = 12 + 256 + 256*256

			block_number = inode.block_pointers[i]

			if block_number > superblock.s_blocks_count or block_number < 0:
				print('INVALID {0} {1} IN INODE {2} AT OFFSET {3}'.format(b, block_number, inode.inumber, offset))
			if block_number < 5 and block_number > 0:
				print('RESERVED {0} {1} IN INODE {2} AT OFFSET {3}'.format(b, block_number, inode.inumber, offset))

			# A block that is allocated to some file might also appear on the free list
			if block_number in free_blocks:
				print('ALLOCATED BLOCK {0} ON FREELIST'.format(block_number))

			######CHECK THIS
			if block_number != 0:
				if block_number in blocks:
					blocks[].append('DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(b, block_number, inode.inumber, offset))
				else:
					blocks[block_number] = ['DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(b, block_number, inode.inumber, offset)]
				# Remove block from the set of blocks_not_seen
				if block_number in blocks_not_seen:
					blocks_not_seen.remove(block_number)

		# I-node Allocation Audit: check whether this inode on the freelist
		if inode.inumber in free_inodes:
			print('ALLOCATED INODE {0} ON FREELIST')
		# Indicate that this I-node was seen
		if inode.inumber in free_inodes:
			inodes_not_seen.remove(inode.inumber)

	# DIRECTORY CONSISTENCY AUDITS

	
	# Print duplicate block findings
	for dup in blocks:
		if len(blocks[dup] > 1):
			for dup_message in dup:
				print(dup_message)

	# Figure out whether a block is multiply referenced, 
	for mref in indirect_references:
		if mref.inidrect_block_num in blocks_not_seen:
			blocks_not_seen.remove(mref.block_number)
		if mref.reference_block_num in blocks_not_seen:
			blocks_not_seen.remove(mref.reference_block_num)

	# If a block is not referenced by any file and is not on the free list,
	# note.
	unreferenced_blocks = blocks_not_seen - free_blocks # the set of unreferenced blocks
	for ublock in unreferenced_blocks:
		print('UNREFERENCED BLOCK {0}'.format(ublock))

	# Unallocated inodes ought to be on the free list; report when this is not the case.
	unallocated_inodes = inodes_not_seen - free_inodes
	for n in unallocated_inodes:
		print('UNALLOCATED INODE {0} NOT ON FREELIST')



if __name__ == '__main__':
	main()