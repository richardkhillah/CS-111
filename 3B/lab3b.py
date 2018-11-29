#!/usr/local/cs/bin/python3

import sys

USAGE = 'Usage: ./lab3b file'

class SuperBlock():
    def __init__(self, csv):
        self.block_count = int(csv[1])
        self.inode_count = int(csv[2])
        self.block_size = int(csv[3])
        self.inode_size = int(csv[4])
        self.groups_per_block = int(csv[5])
        self.inodes_per_group = int(csv[6])
        self.first_free_inode = int(csv[7])

class Group():
    def __init__(self, csv):
        self.group_number = int(csv[1])
        self.block_count = int(csv[2])
        self.inode_count = int(csv[3])
        self.free_blocks_count = int(csv[4])
        self.free_inodes_count = int(csv[5])
        self.block_bitmap = int(csv[6])
        self.inode_bitmap = int(csv[7])
        self.inode_table = int(csv[8])

class Inode():
    def __init__(self, csv):
        self.inumber = int(csv[1])
        self.filetype = csv[2]
        self.mode = int(csv[3])
        self.owner = int(csv[4])
        self.group = int(csv[5])
        self.link_count = int(csv[6])
        self.time_last_inode_change = csv[7]
        self.modification_time = csv[8]
        self.last_access_time = csv[9]
        self.file_size = int(csv[10])
        self.num_blocks_on_disk = int(csv[11])
        self.block_pointers = [int(x) for x in csv[12:]]

class DirectoryEntry():
    def __init__(self, csv):
        self.parent_inumber = int(csv[1])
        self.local_offset = int(csv[2])
        self.file_inumber = int(csv[3])
        self.entry_length = int(csv[4])
        self.name_length = int(csv[5])
        self.name = csv[6]

class IndirectReference():
    def __init__(self, csv):
        self.parent_inumber = int(csv[1])
        self.level_of_indirection = int(csv[2])
        self.local_offset = int(csv[3])
        self.block_number = int(csv[4])
        self.reference_block_number = int(csv[5])

def main():
	parser = argparse.ArgumentParser()
	program_name = parser.prog # get program name

	if len(sys.argv) != 2:
		print('Error: Invalid number of arguments')
		print('Usage: ./{} <filename>'.format(program_name))
		sys.exit(1)

	superblock = None
    group = None
    free_blocks = set()
    free_inodes = set()
    inodes = []
    directory_entries = []
    indirect_references = []
    
    # if len(sys.argv) != 2:
    #     print('lab3b: Invalid arguments passed')
    #     print(USAGE)
    #     sys.exit(1)

    file_name = sys.argv[1]
    file = open(file_name, 'r')

    # parse the csv
    for line in file:
        line = line.rstrip()
        split_line = line.split(',')
        if split_line[0] == 'SUPERBLOCK':
            superblock = SuperBlock(split_line)
        elif split_line[0] == 'GROUP':
            group = Group(split_line)
        elif split_line[0] == 'BFREE':
            free_blocks.add(int(split_line[1]))
        elif split_line[0] == 'IFREE':
            free_inodes.add(int(split_line[1]))
        elif split_line[0] == 'DIRENT':
            directory_entries.append(DirectoryEntry(split_line))
        elif split_line[0] == 'INODE':
            inodes.append(Inode(split_line))
        elif split_line[0] == 'INDIRECT':
            indirect_references.append(IndirectReference(split_line))

    # map block number to list of duplicate messages
    blocks = {}
    blocks_not_seen = set([i for i in range(8, superblock.block_count)])
    inodes_not_seen = set([i for i in range(superblock.first_free_inode, superblock.inode_count+1)])
    inode_link_counts = {}
    inodes_seen = set()

    for inode in inodes:
        inodes_seen.add(inode.inumber)

    for entry in directory_entries:
    	file_inumber = file_inumber
"""
PICK UP HERE!!!!
"""
        # check for invalid inode num
        if file_inumber < 1 or file_inumber > superblock.inode_count:
            print("DIRECTORY INODE {0} NAME {1} INVALID INODE {2}".format(entry.parent_inumber, entry.name, file_inumber))
        
        # check if inode in free inodes
        if file_inumber in free_inodes and file_inumber not in inodes_seen:
            print("DIRECTORY INODE {0} NAME {1} UNALLOCATED INODE {2}".format(entry.parent_inumber, entry.name, file_inumber))
        
        # increment inode count
        if file_inumber in inode_link_counts:
            inode_link_counts[file_inumber] += 1
        else:
            inode_link_counts[file_inumber] = 1

        if entry.name == "'.'":
            if file_inumber != entry.parent_inumber:
                print("DIRECTORY INODE {0} NAME '.' LINK TO INODE {1} SHOULD BE {0}".format(entry.parent_inumber, file_inumber))
        if entry.name == "'..'":
            if entry.parent_inumber == 2:
                if file_inumber != 2:
                    print("DIRECTORY INODE 2 NAME '..' LINK TO INODE {0} SHOULD BE 2".format(file_inumber))
            else:
                for parent in directory_entries:
                    if parent.parent_inumber == file_inumber:
                        if parent.file_inumber == entry.parent_inumber:
                            break
                    else:
                        if parent.file_inumber == entry.parent_inumber:
                           print("DIRECTORY INODE {1} NAME '..' LINK TO INODE {0} SHOULD BE {1}".format(file_inumber, entry.parent_inumber, parent.parent_inumber))
                           break

    # check blocks
    for inode in inodes:
        # ensure right link count
        if inode.inumber in inode_link_counts:
            if inode_link_counts[inode.inumber] != inode.link_count:
                print("INODE {0} HAS {1} LINKS BUT LINKCOUNT IS {2}".format(inode.inumber, inode_link_counts[inode.inumber], inode.link_count))
        elif inode.link_count != 0:
            print("INODE {0} HAS 0 LINKS BUT LINKCOUNT IS {1}".format(inode.inumber, inode.link_count))

        # check if inode was listed as free
        if inode.inumber in free_inodes:
            print("ALLOCATED INODE {0} ON FREELIST".format(inode.inumber))

        # attempt to mark the inode as seen
        if inode.inumber in inodes_not_seen:
            inodes_not_seen.remove(inode.inumber)
        
        for i in range(15):
            t = 'BLOCK'
            offset = i
            if i == 12:
                offset = 12
                t = 'INDIRECT BLOCK'
            elif i == 13:
                offset = 256 + 12
                t = 'DOUBLE INDIRECT BLOCK'
            elif i == 14:
                offset = 256 * 256 + 256 + 12
                t = 'TRIPLE INDIRECT BLOCK'
            
            b_num = inode.block_pointers[i]
            if b_num > superblock.block_count or b_num < 0:
                print('INVALID {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inumber, offset))
            if b_num < 5 and b_num > 0:
                print('RESERVED {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inumber, offset))
            if b_num in free_blocks:
                print('ALLOCATED BLOCK {0} ON FREELIST'.format(b_num))

            if b_num != 0:
                if b_num in blocks:
                    blocks[b_num].append('DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inumber, offset))
                else:
                    blocks[b_num]= ['DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inumber, offset)]
                if b_num in blocks_not_seen:
                    blocks_not_seen.remove(b_num)
    
    for ref in indirect_references:
        if ref.block_number in blocks_not_seen:
            blocks_not_seen.remove(ref.block_number)
        if ref.reference_block_number in blocks_not_seen:
            blocks_not_seen.remove(ref.reference_block_number)

    # see if any block has multiple associations
    for l in blocks:
        if len(blocks[l]) > 1:
            for msg in blocks[l]:
                print(msg)
    
    # check for missing inodes
    missing_inodes = inodes_not_seen - free_inodes

    for n in missing_inodes:
        print("UNALLOCATED INODE {0} NOT ON FREELIST".format(n))
    
    missing_blocks = blocks_not_seen - free_blocks
    for n in missing_blocks:
        print("UNREFERENCED BLOCK {0}".format(n))

if __name__ == '__main__':
    main()
