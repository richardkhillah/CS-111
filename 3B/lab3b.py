#!/usr/local/bin/python3

import sys

USAGE = 'Usage: ./lab3b file'

class SuperBlock():
    def __init__(self, csv):
        self.num_blocks = int(csv[1])
        self.num_inodes = int(csv[2])
        self.block_size = int(csv[3])
        self.inode_size = int(csv[4])
        self.blocks_group = int(csv[5])
        self.inodes_group = int(csv[6])
        self.first_free_inode = int(csv[7])

class Group():
    def __init__(self, csv):
        self.group_num = int(csv[1])
        self.num_blocks = int(csv[2])
        self.num_inodes = int(csv[3])
        self.num_free_blocks = int(csv[4])
        self.num_free_inodes = int(csv[5])
        self.block_bmp_block = int(csv[6])
        self.inode_bmp_block = int(csv[7])
        self.first_inodes_block = int(csv[8])

class Inode():
    def __init__(self, csv):
        self.inode_num = int(csv[1])
        self.type = csv[2]
        self.mode = int(csv[3])
        self.owner = int(csv[4])
        self.group = int(csv[5])
        self.link_count = int(csv[6])
        self.last_change = csv[7]
        self.mod_time = csv[8]
        self.acc_time = csv[9]
        self.file_size = int(csv[10])
        self.num_blocks = int(csv[11])
        self.block_pointers = [int(x) for x in csv[12:]]

class DirEntry():
    def __init__(self, csv):
        self.parent_num = int(csv[1])
        self.offset = int(csv[2])
        self.file_inode_num = int(csv[3])
        self.entry_len = int(csv[4])
        self.name_len = int(csv[5])
        self.name = csv[6]

class IndirectRef():
    def __init__(self, csv):
        self.parent_num = int(csv[1])
        self.indir_lvl = int(csv[2])
        self.offset = int(csv[3])
        self.block_num = int(csv[4])
        self.ref_block_num = int(csv[5])

def main():
    super_block = None
    group = None
    free_blocks = set()
    free_inodes = set()
    inodes = []
    directory_entries = []
    indirects = []

    if len(sys.argv) != 2:
        print('lab3b: Invalid arguments passed')
        print(USAGE)
        sys.exit(1)

    filename = sys.argv[1]

    file = open(filename, 'r')

    # parse the csv
    for line in file:
        line = line.rstrip()
        split_vals = line.split(',')
        sval0 = split_vals[0]
        if sval0 == 'SUPERBLOCK':
            super_block = SuperBlock(split_vals)
        elif sval0 == 'GROUP':
            group = Group(split_vals)
        elif sval0 == 'BFREE':
            free_blocks.add(int(split_vals[1]))
        elif sval0 == 'IFREE':
            free_inodes.add(int(split_vals[1]))
        elif sval0 == 'DIRENT':
            directory_entries.append(DirEntry(split_vals))
        elif sval0 == 'INODE':
            inodes.append(Inode(split_vals))
        elif sval0 == 'INDIRECT':
            indirects.append(IndirectRef(split_vals))

    # map block number to list of duplicate messages
    blocks = {}
    blocks_not_seen = set([i for i in range(8, super_block.num_blocks)])
    inodes_not_seen = set([i for i in range(super_block.first_free_inode, super_block.num_inodes+1)])
    inode_link_counts = {}
    inodes_seen = set()

    for inode in inodes:
        inodes_seen.add(inode.inode_num)

    for entry in directory_entries:
        # check for invalid inode num
        if entry.file_inode_num < 1 or entry.file_inode_num > super_block.num_inodes:
            print("DIRECTORY INODE {0} NAME {1} INVALID INODE {2}".format(entry.parent_num, entry.name, entry.file_inode_num))
        
        # check if inode in free inodes
        if entry.file_inode_num in free_inodes and entry.file_inode_num not in inodes_seen:
            print("DIRECTORY INODE {0} NAME {1} UNALLOCATED INODE {2}".format(entry.parent_num, entry.name, entry.file_inode_num))
        
        # increment inode count
        if entry.file_inode_num in inode_link_counts:
            inode_link_counts[entry.file_inode_num] += 1
        else:
            inode_link_counts[entry.file_inode_num] = 1

        if entry.name == "'.'":
            if entry.file_inode_num != entry.parent_num:
                print("DIRECTORY INODE {0} NAME '.' LINK TO INODE {1} SHOULD BE {0}".format(entry.parent_num, entry.file_inode_num))
        if entry.name == "'..'":
            if entry.parent_num == 2:
                if entry.file_inode_num != 2:
                    print("DIRECTORY INODE 2 NAME '..' LINK TO INODE {0} SHOULD BE 2".format(entry.file_inode_num))
            else:
                for parent in directory_entries:
                    if parent.parent_num == entry.file_inode_num:
                        if parent.file_inode_num == entry.parent_num:
                            break
                    else:
                        if parent.file_inode_num == entry.parent_num:
                           print("DIRECTORY INODE {1} NAME '..' LINK TO INODE {0} SHOULD BE {1}".format(entry.file_inode_num, entry.parent_num, parent.parent_num))
                           break

    # check blocks
    for inode in inodes:
        # ensure right link count
        if inode.inode_num in inode_link_counts:
            if inode_link_counts[inode.inode_num] != inode.link_count:
                print("INODE {0} HAS {1} LINKS BUT LINKCOUNT IS {2}".format(inode.inode_num, inode_link_counts[inode.inode_num], inode.link_count))
        elif inode.link_count != 0:
            print("INODE {0} HAS 0 LINKS BUT LINKCOUNT IS {1}".format(inode.inode_num, inode.link_count))

        # check if inode was listed as free
        if inode.inode_num in free_inodes:
            print("ALLOCATED INODE {0} ON FREELIST".format(inode.inode_num))

        # attempt to mark the inode as seen
        if inode.inode_num in inodes_not_seen:
            inodes_not_seen.remove(inode.inode_num)
        
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
            if b_num > super_block.num_blocks or b_num < 0:
                print('INVALID {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inode_num, offset))
            if b_num < 5 and b_num > 0:
                print('RESERVED {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inode_num, offset))
            if b_num in free_blocks:
                print('ALLOCATED BLOCK {0} ON FREELIST'.format(b_num))

            if b_num != 0:
                if b_num in blocks:
                    blocks[b_num].append('DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inode_num, offset))
                else:
                    blocks[b_num]= ['DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}'.format(t, b_num, inode.inode_num, offset)]
                if b_num in blocks_not_seen:
                    blocks_not_seen.remove(b_num)
    
    for ref in indirects:
        if ref.block_num in blocks_not_seen:
            blocks_not_seen.remove(ref.block_num)
        if ref.ref_block_num in blocks_not_seen:
            blocks_not_seen.remove(ref.ref_block_num)

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
