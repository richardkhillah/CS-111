#!/usr/local/cs/bin/python3

"""
NAME: Richard Khillah
EMAIL: RKhillah@ucla.edu
ID: 604853262

lab3b analyzes a file system summary (a .csv file generated from lab3a)
and reports on all found inconsistencies. It generates
    Block Consistency Audits - determine whether every block pointer in
        every Inode, block (direct, indirect, double-indirect, and
        triple-indirect) is valid
    I-node Allocation Audits - determine whether or not an Inode is 
        allocated.
    Directory Consistency Audits - determine whether every allocated
        I-node is referred to by the number of directory entries that
        is equal to the reference count recorded in the I-node.
and reports findings to stdout.

Exit Codes:
0 ... successful execution, no inconsistencies found.
1 ... unsuccessful execution, bad parameters or system call failure.
2 ... successful execution, inconsistencies found.

"""

import sys
import os.path

exitCode = 0
program_name = None

# There's no need to keep things we don't need, so for each
# line in the split csv file, store only the data we are auditing
class SuperBlock():
    def __init__(self, split_vals):
        self.block_count = int(split_vals[1])
        self.inode_count = int(split_vals[2])
        self.first_free_inode = int(split_vals[7])

class Group():
    def __init__(self, split_vals):
        self.block_count = int(split_vals[2])
        self.inode_count = int(split_vals[3])
        self.num_free_blocks = int(split_vals[4])
        self.num_free_inodes = int(split_vals[5])

class Inode():
    def __init__(self, split_vals):
        self.inumber = int(split_vals[1])
        self.type = split_vals[2]
        self.link_count = int(split_vals[6])
        self.block_count = int(split_vals[11])
        self.block_pointers = [int(x) for x in split_vals[12:]]

class DirectoryEntry():
    def __init__(self, split_vals):
        self.parent_inumber = int(split_vals[1])
        self.file_inumber = int(split_vals[3])
        self.name = split_vals[6]

class IndirectReference():
    def __init__(self, split_vals):
        self.parent_inumber = int(split_vals[1])
        self.block_num = int(split_vals[4])
        self.ref_block_num = int(split_vals[5])

def process(csv):
    global exitCode

    # Ensure that the file passed in by the user is valid
    if not os.path.exists(csv):
        sys.stderr.write('%s: Unable to open report %s\n' % (program_name, csv))
        exitCode = 1
        return

    # Take Inventory. Scan the file, appropiately storing each line in an 
    # appropiate structure prior to moving onto the next line.
    super_block = None
    group = None
    free_blocks = set()
    free_inodes = set()
    inodes = []
    directory_entries = []
    indirects = []   

    with open(csv, 'r') as file:
        try:
            for line in file:
                line = line.rstrip() # remove any whitespace characters that *may have*
                                     # made it's way into a line
                fields = line.split(',')
                l0 = fields[0]
                if l0 == 'SUPERBLOCK':
                    super_block = SuperBlock(fields)
                elif l0 == 'GROUP':
                    group = Group(fields)
                elif l0 == 'BFREE':
                    free_blocks.add(int(fields[1]))
                elif l0 == 'IFREE':
                    free_inodes.add(int(fields[1]))
                elif l0 == 'DIRENT':
                    directory_entries.append(DirectoryEntry(fields))
                elif l0 == 'INODE':
                    inodes.append(Inode(fields))
                elif l0 == 'INDIRECT':
                    indirects.append(IndirectReference(fields))
            pass
        except Exception as e:
            sys.stderr.write('Unable to process a line in %s\n' % (csv))
            exitCode = 1
            return


    blocks = {}
    # When we first start, we will not have seen any blocks. We remove
    # blocks we have audited.
    blocks_not_audited = set([i for i in range(8, super_block.block_count)])
    inodes_not_audited = set([i for i in range(super_block.first_free_inode,
                                             super_block.inode_count+1)])
    inode_link_counts = {}
    inodes_seen = set()

    for inode in inodes:
        inodes_seen.add(inode.inumber)

    for entry in directory_entries:
        # check for invalid inode num
        file_inumber = entry.file_inumber
        parent_inumber = entry.parent_inumber
        if file_inumber < 1 or file_inumber > super_block.inode_count:
            sys.stdout.write("DIRECTORY INODE %d NAME %s INVALID INODE %d" %
                             (parent_inumber, entry.name, file_inumber))
            exitCode = 2
        
        # check if inode in free inodes
        if file_inumber in free_inodes and file_inumber not in inodes_seen:
            sys.stdout.write("DIRECTORY INODE %d NAME %s UNALLOCATED INODE %s" %
                             (parent_inumber, entry.name, file_inumber))
            exitCode = 2
        
        # increment inode count
        if file_inumber in inode_link_counts:
            inode_link_counts[file_inumber] += 1
        else:
            inode_link_counts[file_inumber] = 1

        if entry.name == "'.'" and file_inumber is not parent_inumber:
            sys.stdout.write("DIRECTORY INODE %d NAME '.' LINK TO INODE %d SHOULD BE %d" %
                             (parent_inumber, file_inumber))
            exitCode = 2

        if entry.name == "'..'":
            if parent_inumber == 2 and file_inumber != 2:
                sys.stdout.write("DIRECTORY INODE 2 NAME '..' LINK TO INODE %d SHOULD BE 2" %
                                 (file_inumber))
                exitCode = 2
            else:
                for parent in directory_entries:
                    if parent.parent_inumber == file_inumber:
                        if parent.file_inumber == parent_inumber:
                            break
                    else:
                        if parent.file_inumber == parent_inumber:
                            sys.stdout.write("DIRECTORY INODE %d NAME '..' LINK TO INODE %d SHOULD BE %d" %
                                            (file_inumber, parent_inumber, parent.parent_inumber))
                            exitCode = 2
                            break

    # check blocks
    for inode in inodes:
        # ensure right link count
        inumber = inode.inumber
        if inumber in inode_link_counts:
            if inode_link_counts[inumber] != inode.link_count:
                sys.stdout.write("INODE %d HAS %d LINKS BUT LINKCOUNT IS %d" %
                                 (inumber, inode_link_counts[inumber], inode.link_count))
                exitCode = 2
        elif inode.link_count != 0:
            sys.stdout.write("INODE %d HAS 0 LINKS BUT LINKCOUNT IS %d" %
                             (inumber, inode.link_count))
            exitCode = 2

        # check if inode was listed as free
        if inumber in free_inodes:
            sys.stdout.write("ALLOCATED INODE %d ON FREELIST" % (inumber))
            exitCode = 2

        # attempt to mark the inode as seen
        if inumber in inodes_not_audited:
            inodes_not_audited.remove(inumber)
        
        # An Inode can have associated with it 15 blocks:
        # 12 direct blocks and up to 3 indirect blocks (either direct, 
        # double indirect block, or triple indirect), so we figure out what
        # type of block we are dealing with.
        for i in range(15): 
            block_type = "BLOCK"
            offset = i
            if i == 12:
                offset = 12
                block_type = "INDIRECT BLOCK"
            elif i == 13:
                offset = 256 + 12
                block_type = "DOUBLE INDIRECT BLOCK"
            elif i == 14:
                offset = 256 * 256 + 256 + 12
                block_type = "TRIPLE INDIRECT BLOCK"
            
            block_number = inode.block_pointers[i]  # cache this to save memory references
            
            # Data blocks are either allocated or allocaed. If allocated, 
            # the block pointer should be valid or zero. If the block number is
            # higher than the number of blocks known to be allocated, the block
            # is not legal and is therefore invalid. If the block number is less than 0
            # then common sense tells us something is wrong and the block is obviously 
            # invalid.
            if block_number > super_block.block_count or block_number < 0:
                sys.stdout.write("INVALID %s %d IN INODE %d AT OFFSET %d" %
                                 (block_type, block_number, inumber, offset))
                exitCode = 2

            # A reserved block is one that should not be allocaed because it 
            # is reserved for filesystem metadata use and so we should not find
            # reserved blocks in our audit... but of course we do... such is life.
            if block_number < 5 and block_number > 0:
                sys.stdout.write("RESERVED %s %d IN INODE %d AT OFFSET %d" %
                                 (block_type, block_number, inumber, offset))

            # This is logically trivial: if a block is allocated and it is on the
            # free list, then the file system manager might be corrupted... or
            # maybe someone fiddled aroudn and mucked things up... either
            # way, report the discrepency.
            if block_number in free_blocks:
                sys.stdout.write("ALLOCATED BLOCK %d ON FREELIST" % (block_number))
                exitCode = 2

            # Now, We check authenticity of each block. 
            if block_number != 0:
                if block_number in blocks:
                    blocks[block_number].append("DUPLICATE %s %d IN INODE %d AT OFFSET %d" %
                                                (block_type, block_number, inumber, offset))
                else:
                    blocks[block_number]= ["DUPLICATE %s %d IN INODE %d AT OFFSET %d" %
                                            (block_type, block_number, inumber, offset)]

                if block_number in blocks_not_audited:
                    blocks_not_audited.remove(block_number)
    
    for reference in indirects:
        if reference.block_num in blocks_not_audited:
            blocks_not_audited.remove(reference.block_num)
        if reference.ref_block_num in blocks_not_audited:
            blocks_not_audited.remove(reference.ref_block_num)

    # see if any block has multiple associations
    for l in blocks:
        if len(blocks[l]) > 1:
            for m in blocks[l]:
                #sys.stdout.write(m)
                print(m)
    
    # check for missing inodes
    missing_inodes = inodes_not_audited - free_inodes

    for n in missing_inodes:
        sys.stdout.write("UNALLOCATED INODE %d NOT ON FREELIST" % (n))
        exitCode = 2
    
    missing_blocks = blocks_not_audited - free_blocks
    for n in missing_blocks:
        sys.stdout.write("UNREFERENCED BLOCK %d" % (n))
        exitCode = 2


if __name__ == '__main__':
    program_name = sys.argv[0]

    if len(sys.argv) != 2:
        sys.stderr.write('Error getting input file\n')
        sys.stderr.write('Usage: %s filename\n' % (program_name))
        exit(1)

    filename = sys.argv[1]

    process(filename)


    
    
    exit(exitCode)
