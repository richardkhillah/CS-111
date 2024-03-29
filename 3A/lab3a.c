// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

// include more headers below
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ext2_fs.h"

/* file system struct, containing all the necessary information about 
 * the file_system to be read
 */

typedef struct fs {
	int fd;
	unsigned int block_size;
	struct ext2_super_block* su_block;
	struct ext2_group_desc* group;

	int node_count;
	int dir_count;
	struct ext2_inode* iNodes;
	struct ext2_inode* dNodes;
	int* valid_nodes;
	int* valid_dNodes;
	
} fs_t;

void init_fs(fs_t** file_system, char const* fs_img){
	// Allocate memory for file system (superblock and group)
	fs_t* tmp = (fs_t *)malloc(sizeof(fs_t));
	if(tmp != NULL){
		tmp->su_block = (struct ext2_super_block*)malloc(sizeof(struct ext2_super_block));
		tmp->group = (struct ext2_group_desc*)malloc(sizeof(struct ext2_group_desc));
		if(tmp->su_block == NULL || tmp->group == NULL) {
			fatal_error("Error initiailzizing filesystem fs", NULL, 1);
		}
	} else {
		fatal_error("Error initiailzizing filesystem fs", NULL, 1);
	}

	if((tmp->fd = open(fs_img, O_RDONLY)) < 0) {
		fatal_error("Error opening filesystem... Ensure correct filesystem image", (void *)usage, 1);
	}

	/* get file system superblock information */
	tmp->block_size = 1024;
	pread(tmp->fd, tmp->su_block, 1024, 1024);
	tmp->block_size <<= tmp->su_block->s_log_block_size; // convert to base 10

	/* get file system group information */
	pread(tmp->fd, tmp->group, 32, 2048);

	tmp->node_count = 0;
	tmp->dir_count = 0;
	tmp->iNodes = NULL;
	tmp->dNodes = NULL;
	tmp->valid_nodes = NULL;
	tmp->valid_dNodes = NULL;

	*file_system = tmp;
}

int destroy_fs(fs_t* file_system) {
	close(file_system->fd);

	free(file_system->su_block);
	free(file_system->group);
	if(file_system-> iNodes != NULL) free(file_system->iNodes);
	if(file_system->dNodes != NULL) free(file_system->dNodes);
	if(file_system->valid_nodes != NULL) free(file_system->valid_nodes);
	if(file_system->valid_dNodes != NULL) free(file_system->valid_dNodes);

	free(file_system);
	return 0;
}

const char* SUPERBLOCK = "SUPERBLOCK";
const char* GROUP = "GROUP";

void scan_directory(fs_t* fs, int offset, int i) {
	unsigned int byte_count = fs->block_size * offset;
	int count = 0;

	while(byte_count < fs->block_size * (offset+1)){
		int temp;
		pread(fs->fd, &temp, 4, byte_count);
		if (temp == 0){
			pread(fs->fd, &temp, 2, byte_count + 4);
			byte_count += temp;
			count += temp;
			continue;
		}
		struct ext2_dir_entry* current_dir = (struct ext2_dir_entry *)malloc(sizeof(struct ext2_dir_entry));

		pread(fs->fd, current_dir, 263, byte_count);
		char* name = malloc((current_dir->name_len+1)*sizeof(char));

		pread(fs->fd, name, current_dir->name_len, byte_count + 8);
		name[current_dir->name_len] = '\0';
		dprintf(STDOUT_FILENO, "DIRENT,");
		dprintf(STDOUT_FILENO, "%d,", fs->			valid_dNodes[i]+1);
		dprintf(STDOUT_FILENO, "%d,", 				count);
		dprintf(STDOUT_FILENO, "%d,", current_dir->	inode);
		dprintf(STDOUT_FILENO, "%d,", current_dir->	rec_len);
		dprintf(STDOUT_FILENO, "%d,", current_dir->	name_len);
		dprintf(STDOUT_FILENO, "\'%s\'", 			name);
		dprintf(STDOUT_FILENO, "\n");
		
		byte_count += current_dir->rec_len;
		count += current_dir->rec_len;
	}
}

void scan_indirect_directory(fs_t* fs, int offset, int count, int i){
	int temp;
	if (offset != 0){
		for(unsigned int j = 0; j < fs->block_size/4; j++){
			pread(fs->fd, &temp, 4, offset*fs->block_size + j*4);
			if(count == 0){
				if(temp != 0) scan_directory(fs, offset, i);
			}
			else
				scan_indirect_directory(fs, temp, count-1, i);
		}
	}
}

void scan_indirect_block(fs_t* fs, int offset, int count, int i, int logic){
	int temp;
	if (offset != 0){
		for(unsigned int j = 0; j < fs->block_size/4; j++){
			pread(fs->fd, &temp, 4, fs->block_size*offset + j*4);
			if (temp != 0){
				dprintf(STDOUT_FILENO,"INDIRECT,%d,%d,%d,%d,%d\n", fs->valid_nodes[i]+1, count, logic + (int)(j*pow(256,count-1)), offset, temp);
				if (count != 1)
					scan_indirect_block(fs, temp, count-1, i, logic + (int)(j*pow(256,count-1)));
			}
		}
	}
}

void print_summary(fs_t* fs, const char* stype) {
	if(stype == SUPERBLOCK){
		dprintf(STDOUT_FILENO, "SUPERBLOCK,");
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_blocks_count);
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inodes_count);
		dprintf(STDOUT_FILENO, "%d,", fs->				block_size);
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inode_size);
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_blocks_per_group);
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inodes_per_group);
		dprintf(STDOUT_FILENO, "%d", fs->su_block->		s_first_ino);
		dprintf(STDOUT_FILENO, "\n");
	}

	if(stype == GROUP) {
		dprintf(STDOUT_FILENO, "GROUP,0,");
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->		s_blocks_count);
		dprintf(STDOUT_FILENO, "%d,", fs->su_block->		s_inodes_count);
		dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_free_blocks_count);
		dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_free_inodes_count);
		dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_block_bitmap);
		dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_inode_bitmap);
		dprintf(STDOUT_FILENO, "%d", fs->group->			bg_inode_table);
		dprintf(STDOUT_FILENO, "\n");
	}
}

void get_free_entries(fs_t* fs) {
	for(unsigned int i = 0; i < fs->block_size; i++) {
	   	int tempB = 0,
	   		tempI = 0,
	   		ones = 1;
	   	pread(fs->fd, &tempB, 1, fs->group->bg_block_bitmap*fs->block_size+i); 
	   	pread(fs->fd, &tempI, 1, fs->group->bg_inode_bitmap*fs->block_size+i); 
	   	
	   	for (unsigned int j = 0; j < 8; j++) {
	   		// free blocks
	   		if ((tempB & ones) == 0) {
	   			dprintf(STDOUT_FILENO,"BFREE,%d\n", i*8 + j+1);
	   		}
	   		if ((tempI & ones) == 0) {
	   			dprintf(STDOUT_FILENO,"IFREE,%d\n", i*8 + j+1);
	   		} else if(((tempI & ones) != 0) && (i*8 + j < fs->su_block->s_inodes_per_group)){
	   			fs->valid_nodes[fs->node_count] = i*8 + j+1;
	   			fs->node_count++;
	   		}

	   		ones <<= 1;
	   	}
	}
}

void get_Inode_summary(fs_t* fs) {
	for(int i = 0; i < fs->node_count; i++){
		int k = fs->valid_nodes[i];
		pread(fs->fd, &fs->iNodes[i], 128, fs->group->bg_inode_table*fs->block_size + k*128);

		if(fs->iNodes[i].i_mode == 0 && fs->iNodes[i].i_links_count == 0)
			continue;

		char cbuff[20],mbuff[20],abuff[20];
		
		time_t temp = fs->iNodes[i].i_ctime;
		struct tm* ctime = gmtime(&temp);
		strftime(cbuff,18,"%D %H:%M:%S",ctime);

		temp = fs->iNodes[i].i_mtime;
		struct tm* mtime = gmtime(&temp);
		strftime(mbuff,18,"%D %H:%M:%S",mtime);

		temp = fs->iNodes[i].i_atime;
		struct tm* atime = gmtime(&temp);
		strftime(abuff,18,"%D %H:%M:%S",atime);
		
		char c;
		if(fs->iNodes[i].i_mode&0x4000) {
			c = 'd';
			pread(fs->fd, &fs->dNodes[fs->dir_count], 128, fs->group->bg_inode_table*fs->block_size + k*128);
			fs->valid_dNodes[fs->dir_count] = k;
			fs->dir_count++;
		}
		else if(fs->iNodes[i].i_mode&0x8000) c = 'f';
		else if(fs->iNodes[i].i_mode&0xA000) c = 's';
		else c = '?';

		long tempSize = (((long)fs->iNodes[i].i_dir_acl) << 32) | fs->iNodes[i].i_size;
		dprintf(STDOUT_FILENO,"INODE,");
		dprintf(STDOUT_FILENO, "%d,", k+1);
		dprintf(STDOUT_FILENO, "%c,", c);
		dprintf(STDOUT_FILENO, "%o,", fs->iNodes[i].	i_mode & 0x0FFF);
		dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_uid);
		dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_gid);
		dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_links_count);
		dprintf(STDOUT_FILENO, "%s,", 					cbuff);
		dprintf(STDOUT_FILENO, "%s,", 					mbuff);
		dprintf(STDOUT_FILENO, "%s,", 					abuff);
		dprintf(STDOUT_FILENO, "%ld,", 					tempSize);
		dprintf(STDOUT_FILENO, "%d", fs->iNodes[i].		i_blocks);

		for (int j = 0; j < 15; j++)
			dprintf(STDOUT_FILENO,",%d",fs->iNodes[i].	i_block[j]);
		dprintf(STDOUT_FILENO,"\n");
	}
}

void get_directory_entries(fs_t* fs) {
	for(int i = 0; i < fs->dir_count; i++){
		for(int j = 0; j < 12; j++){
			int offset = fs->dNodes[i].i_block[j];
			if(offset != 0) scan_directory(fs, offset, i);
		}
		scan_indirect_directory(fs, fs->dNodes[i].i_block[12], 0, i);
		scan_indirect_directory(fs,fs->dNodes[i].i_block[13], 1, i);
		scan_indirect_directory(fs,fs->dNodes[i].i_block[14], 2, i);
	}
}

void get_indirect_block_refs(fs_t* fs) {
	for(int i = 0; i < fs->node_count; i++){
		scan_indirect_block(fs, fs->iNodes[i].i_block[12],1,i,12);
		scan_indirect_block(fs, fs->iNodes[i].i_block[13],2,i,268);
		scan_indirect_block(fs, fs->iNodes[i].i_block[14],3,i,65804);
	}
}

void usage() {
	fprintf(stderr, "usage: %s file_system.img\n", program_name);
}

int main(int argc, char const *argv[]) {
	set_program_name(argv[0]);

	if(argc != 2) fatal_error("Wrong number of arguments", (void *)usage, 1);

	fs_t* fs;
	init_fs(&fs, argv[1]);

	/* superblock and group summaries */
	print_summary(fs, SUPERBLOCK);
	print_summary(fs, GROUP);

	/* get free block and free I-node entries */
	fs->valid_nodes = (int *)malloc(fs->su_block->s_inodes_count * sizeof(int));
   	if (fs->valid_nodes == NULL) fatal_error("Internal error allocating memory", NULL, 1);
   	
   	get_free_entries(fs);
		
	/* get I-node summary, directory entries, and indirect blcok references */
	fs->iNodes = (struct ext2_inode *)	malloc(fs->su_block->s_inodes_count * sizeof(struct ext2_inode));
	fs->dNodes = (struct ext2_inode *)	malloc(fs->node_count * sizeof(struct ext2_inode));
	fs->valid_dNodes = (int *)			malloc(fs->node_count * sizeof(int));
	if( fs->iNodes == NULL || fs->dNodes == NULL || fs->valid_dNodes == NULL)
		fatal_error("Internal error allocating memory", NULL, 1);

	get_Inode_summary(fs);
	get_directory_entries(fs);
	get_indirect_block_refs(fs);
	
	/* free all memory allocated for file system fs */
	destroy_fs(fs);
	return 0;
}
