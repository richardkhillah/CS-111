#!/usr/local/cs/bin/python3

"""
NAME: Richard Khillah
EMAIL: RKhillah@ucla.edu
ID: 604853262

lab3b performs a file system audit by analyzing a csv summary of the
file system. The csv summary must adhear to the formatting convetions
described in P3A.html, despite not storing all data read in.

Exit Codes:
0 ... successful execution, no inconsistencies found.
1 ... unsuccessful execution, bad parameters or system call failure.
2 ... successful execution, inconsistencies found.

"""

import sys
import os.path

exitCode = 0

# There's no need to keep things we don't need, so for each
# line in a filesystem
# .csv for each object, store only the data we are auditing
class SuperBlock():
    def __init__(self, split_vals):
        self.block_count = int(split_vals[1])
        self.inode_count = int(split_vals[2])
        #self.block_size = int(split_vals[3])
        #self.inode_size = int(split_vals[4])
        #self.blocks_group = int(split_vals[5])
        #self.inodes_group = int(split_vals[6])
        self.first_free_inode = int(split_vals[7])

class Group():
    def __init__(self, split_vals):
        #self.group_num = int(split_vals[1])
        self.block_count = int(split_vals[2])
        self.inode_count = int(split_vals[3])
        self.num_free_blocks = int(split_vals[4])
        self.num_free_inodes = int(split_vals[5])
        #self.block_bmp_block = int(split_vals[6])
        #self.inode_bmp_block = int(split_vals[7])
        #self.first_inodes_block = int(split_vals[8])

class Inode():
    def __init__(self, split_vals):
        self.inumber = int(split_vals[1])
        self.type = split_vals[2]
        #self.mode = int(split_vals[3])
        #self.owner = int(split_vals[4])
        #self.group = int(split_vals[5])
        self.link_count = int(split_vals[6])
        #self.last_change = split_vals[7]
        #self.mod_time = split_vals[8]
        #self.acc_time = split_vals[9]
        #self.file_size = int(split_vals[10])
        self.block_count = int(split_vals[11])
        self.block_pointers = [int(x) for x in split_vals[12:]]

class DirEntry():
    def __init__(self, split_vals):
        self.parent_num = int(split_vals[1])
        #self.offset = int(split_vals[2])
        self.file_inode_num = int(split_vals[3])
        #self.entry_len = int(split_vals[4])
        #self.name_len = int(split_vals[5])
        self.name = split_vals[6]

class IndirectRef():
    def __init__(self, split_vals):
        self.parent_num = int(split_vals[1])
        #self.indir_lvl = int(split_vals[2])
        #self.offset = int(split_vals[3])
        self.block_num = int(split_vals[4])
        self.ref_block_num = int(split_vals[5])

if __name__ == '__main__':
    program_name = sys.argv[0]
    if len(sys.argv) != 2:
        print('Error: error getting input file', file=sys.stderr)
        print('Usage: {} filename'.format(filename), file=sys.stderr)
        #sys.exit(1)
        exit(1)

    super_block = None
    group = None
    free_blocks = set()
    free_inodes = set()
    inodes = []
    directory_entries = []
    indirects = []

    filename = sys.argv[1]

    if os.path.exists(filename):
        with open(filename, 'r') as file:
            try:
                for line in file:
                    line = line.rstrip()
                    split_vals = line.split(',')
                    l0 = split_vals[0]
                    if l0 == 'SUPERBLOCK':
                        super_block = SuperBlock(split_vals)
                    elif l0 == 'GROUP':
                        group = Group(split_vals)
                    elif l0 == 'BFREE':
                        free_blocks.add(int(split_vals[1]))
                    elif l0 == 'IFREE':
                        free_inodes.add(int(split_vals[1]))
                    elif l0 == 'DIRENT':
                        directory_entries.append(DirEntry(split_vals))
                    elif l0 == 'INODE':
                        inodes.append(Inode(split_vals))
                    elif l0 == 'INDIRECT':
                        indirects.append(IndirectRef(split_vals))
                pass
            except Exception as e:
                print('Uh oh, looks like there was an error reading contents of {}.'.format(filename), file=sys.stderr)
                print('Usage: {} filename.csv'.format(program_name), file=sys.stderr)
                exit(1)




    # file = open(filename, 'r')

    # # read in csv value. Here we are assuming correct .csv format
    # for line in file:
    #     line = line.rstrip()
    #     split_vals = line.split(',')
    #     l0 = split_vals[0]
    #     if l0 == 'SUPERBLOCK':
    #         super_block = SuperBlock(split_vals)
    #     elif l0 == 'GROUP':
    #         group = Group(split_vals)
    #     elif l0 == 'BFREE':
    #         free_blocks.add(int(split_vals[1]))
    #     elif l0 == 'IFREE':
    #         free_inodes.add(int(split_vals[1]))
    #     elif l0 == 'DIRENT':
    #         directory_entries.append(DirEntry(split_vals))
    #     elif l0 == 'INODE':
    #         inodes.append(Inode(split_vals))
    #     elif l0 == 'INDIRECT':
    #         indirects.append(IndirectRef(split_vals))

    # map block number to list of duplicate messages
    blocks = {}
    blocks_not_seen = set([i for i in range(8, super_block.block_count)])
    inodes_not_seen = set([i for i in range(super_block.first_free_inode, super_block.inode_count+1)])
    inode_link_counts = {}
    inodes_seen = set()

    for inode in inodes:
        inodes_seen.add(inode.inumber)

    for entry in directory_entries:
        # check for invalid inode num
        file_inumber = entry.file_inode_num
        parent_inumber = entry.parent_num
        if file_inumber < 1 or file_inumber > super_block.inode_count:
            print("DIRECTORY INODE {0} NAME {1} INVALID INODE {2}".format(parent_inumber, entry.name, file_inumber))
            exitCode = 2
        
        # check if inode in free inodes
        if file_inumber in free_inodes and file_inumber not in inodes_seen:
            print("DIRECTORY INODE {0} NAME {1} UNALLOCATED INODE {2}".format(parent_inumber, entry.name, file_inumber))
            exitCode = 2
        
        # increment inode count
        if file_inumber in inode_link_counts:
            inode_link_counts[file_inumber] += 1
        else:
            inode_link_counts[file_inumber] = 1

        if entry.name == "'.'":
            if file_inumber != parent_inumber:
                print("DIRECTORY INODE {0} NAME '.' LINK TO INODE {1} SHOULD BE {0}".format(parent_inumber, file_inumber))
                exitCode = 2

        if entry.name == "'..'":
            if parent_inumber == 2:
                if file_inumber != 2:
                    print("DIRECTORY INODE 2 NAME '..' LINK TO INODE {0} SHOULD BE 2".format(file_inumber))
                    exitCode = 2
            else:
                for parent in directory_entries:
                    if parent.parent_num == file_inumber:
                        if parent.file_inode_num == parent_inumber:
                            break
                    else:
                        if parent.file_inode_num == parent_inumber:
                           print("DIRECTORY INODE {1} NAME '..' LINK TO INODE {0} SHOULD BE {1}".format(file_inumber, parent_inumber, parent.parent_num))
                           exitCode = 2
                           break

    # check blocks
    for inode in inodes:
        # ensure right link count
        inumber = inode.inumber
        if inumber in inode_link_counts:
            if inode_link_counts[inumber] != inode.link_count:
                print("INODE {0} HAS {1} LINKS BUT LINKCOUNT IS {2}".format(inumber, inode_link_counts[inumber], inode.link_count))
                exitCode = 2
        elif inode.link_count != 0:
            print("INODE {0} HAS 0 LINKS BUT LINKCOUNT IS {1}".format(inumber, inode.link_count))
            exitCode = 2

        # check if inode was listed as free
        if inumber in free_inodes:
            print("ALLOCATED INODE {0} ON FREELIST".format(inumber))
            exitCode = 2

        # attempt to mark the inode as seen
        if inumber in inodes_not_seen:
            inodes_not_seen.remove(inumber)
        
        for i in range(15):
            block_type = 'BLOCK'
            offset = i
            if i == 12:
                offset = 12
                block_type = 'INDIRECT BLOCK'
            elif i == 13:
                offset = 256 + 12
                block_type = 'DOUBLE INDIRECT BLOCK'
            elif i == 14:
                offset = 256 * 256 + 256 + 12
                block_type = 'TRIPLE INDIRECT BLOCK'
            
            block_number = inode.block_pointers[i]
            if block_number > super_block.block_count or block_number < 0:
                print('INVALID {0} {1} IN INODE {2} AT OFFSET {3}'.format(block_type, block_number, inumber, offset))
                exitCode = 2
            if block_number < 5 and block_number > 0:
                print('RESERVED {0} {1} IN INODE {2} AT OFFSET {3}'.format(block_type, block_number, inumber, offset))
            if block_number in free_blocks:
                print('ALLOCATED BLOCK {0} ON FREELIST'.format(block_number))
                exitCode = 2

            if block_number != 0:
                if block_number in blocks:
                    blocks[block_number].append('DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(block_type, block_number, inumber, offset))
                else:
                    blocks[block_number]= ['DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(block_type, block_number, inumber, offset)]
                if block_number in blocks_not_seen:
                    blocks_not_seen.remove(block_number)
    
    for ref in indirects:
        if ref.block_num in blocks_not_seen:
            blocks_not_seen.remove(ref.block_num)
        if ref.ref_block_num in blocks_not_seen:
            blocks_not_seen.remove(ref.ref_block_num)

    # see if any block has multiple associations
    for l in blocks:
        if len(blocks[l]) > 1:
            for m in blocks[l]:
                print(m)
    
    # check for missing inodes
    missing_inodes = inodes_not_seen - free_inodes

    for n in missing_inodes:
        print("UNALLOCATED INODE {0} NOT ON FREELIST".format(n))
        exitCode = 2
    
    missing_blocks = blocks_not_seen - free_blocks
    for n in missing_blocks:
        print("UNREFERENCED BLOCK {0}".format(n))
        exitCode = 2

    exit(exitCode)
