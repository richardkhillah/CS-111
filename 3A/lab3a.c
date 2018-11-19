#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "ext2_fs.h"
#include "utils.h"

#ifdef DEV
/* file system struct, containing all the necessary information about 
 * the file_system to be read
 */

/*
	typedef struct ref_tracker {
		void** ptrs;
		int num_ptrs;
	} rt_t;

	rt_t* references = NULL;

	int inti_ref_tracker(void) {
		if(references == NULL) {
			rt_t* tmp = (rt_t *)malloc(sizeof(rt_t));
			if(tmp == NULL) {
				fatal_error("internal error while initiailzizing reference tracker", NULL, 1);
			}
			tmp->num_ptrs = 0;
			references = tmp;

			if(references == NULL) {
				fatal_error("internal error while initiailzizing reference tracker", NULL, 1);
			}
			return 0;
		} else {
			return -1;
		}
	}

	int destroy(void) {
		if( references != NULL && references->num_ptrs < 1) {
			// there are resouces to destroy, err.. return to the OS...
			int i = 0;
			while( i < references->num_ptrs) {
				free(references->ptrs[i]);
				i++;
			}
		} else if ( !1 ) {
			// there are no resources to destroy
			exit(1);
		} else {
			// some other weird thing happened...
			fprintf(stderr, "some weird shiiiit happening\n");
			exit(1);
		}
		free(references);
		exit(0);
	}
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

	/* setup file system super block */
	tmp->block_size = 1024;
	pread(tmp->fd, tmp->su_block, 1024, 1024);
	tmp->block_size <<= tmp->su_block->s_log_block_size; // convert to base 10

	tmp->node_count = 0;
	tmp->dir_count = 0;
	tmp->iNodes = NULL;
	tmp->dNodes = NULL;
	tmp->valid_nodes = NULL;
	tmp->valid_dNodes = NULL;

	*file_system = tmp;
}

void directory(fs_t* fs, int offset, int i) {
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

void indDirectory(fs_t* fs, int offset, int count, int i){
	int temp;
	if (offset != 0){
		for(unsigned int j = 0; j < fs->block_size/4; j++){
			pread(fs->fd, &temp, 4, offset*fs->block_size + j*4);
			if(count == 0){
				if(temp != 0) directory(fs, offset, i);
			}
			else
				indDirectory(fs, temp, count-1, i);
		}
	}
}

void indirect(fs_t* fs, int offset, int count, int i, int logic){
	int temp;
	if (offset != 0){
		for(unsigned int j = 0; j < fs->block_size/4; j++){
			pread(fs->fd, &temp, 4, fs->block_size*offset + j*4);
			if (temp != 0){
				dprintf(STDOUT_FILENO,"INDIRECT,%d,%d,%d,%d,%d\n", fs->valid_nodes[i]+1, count, logic + (int)(j*pow(256,count-1)), offset, temp);
				if (count != 1)
					indirect(fs, temp, count-1, i, logic + (int)(j*pow(256,count-1)));
			}
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

void usage() {
	fprintf(stderr, "usage: %s file_system.img\n", program_name);
}

int main(int argc, char const *argv[])
{
	set_program_name(argv[0]);

	if(argc != 2) fatal_error("Wrong number of arguments", (void *)usage, 1);

	fs_t* fs;
	init_fs(&fs, argv[1]);

	// print information to stdout:
	dprintf(STDOUT_FILENO, "SUPERBLOCK,");
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_blocks_count);
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inodes_count);
	dprintf(STDOUT_FILENO, "%d,", fs->				block_size);
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inode_size);
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_blocks_per_group);
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->	s_inodes_per_group);
	dprintf(STDOUT_FILENO, "%d", fs->su_block->		s_first_ino);
	dprintf(STDOUT_FILENO, "\n");

	// get group summary information and print it to stdout
	pread(fs->fd, fs->group, 32, 2048);
	dprintf(STDOUT_FILENO, "GROUP,0,");
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->		s_blocks_count);
	dprintf(STDOUT_FILENO, "%d,", fs->su_block->		s_inodes_count);
	dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_free_blocks_count);
	dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_free_inodes_count);
	dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_block_bitmap);
	dprintf(STDOUT_FILENO, "%d,", fs->group->			bg_inode_bitmap);
	dprintf(STDOUT_FILENO, "%d", fs->group->			bg_inode_table);
	dprintf(STDOUT_FILENO, "\n");

	/* get free block and I-node entries and print them to stdout */
	fs->valid_nodes = (int *)malloc(fs->su_block->s_inodes_count * sizeof(int));
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
	
	// get I-node summary and print to stdout
	fs->iNodes = (struct ext2_inode *)	malloc(fs->su_block->s_inodes_count * sizeof(struct ext2_inode));
	fs->dNodes = (struct ext2_inode *)	malloc(fs->node_count * sizeof(struct ext2_inode));
	fs->valid_dNodes = (int *)			malloc(fs->node_count * sizeof(int));

	if( fs->iNodes == NULL ||
		fs->dNodes == NULL ||
		fs->valid_dNodes == NULL)
		fatal_error("Internal error allocating memory", NULL, 1);

	/* I-node summary */
	get_Inode_summary(fs);
	// for(int i = 0; i < fs->node_count; i++){
	// 	int k = fs->valid_nodes[i];
	// 	pread(fs->fd, &fs->iNodes[i], 128, fs->group->bg_inode_table*fs->block_size + k*128);

	// 	if(fs->iNodes[i].i_mode == 0 && fs->iNodes[i].i_links_count == 0)
	// 		continue;

	// 	char cbuff[20],mbuff[20],abuff[20];
		
	// 	time_t temp = fs->iNodes[i].i_ctime;
	// 	struct tm* ctime = gmtime(&temp);
	// 	strftime(cbuff,18,"%D %H:%M:%S",ctime);

	// 	temp = fs->iNodes[i].i_mtime;
	// 	struct tm* mtime = gmtime(&temp);
	// 	strftime(mbuff,18,"%D %H:%M:%S",mtime);

	// 	temp = fs->iNodes[i].i_atime;
	// 	struct tm* atime = gmtime(&temp);
	// 	strftime(abuff,18,"%D %H:%M:%S",atime);
		
	// 	char c;
	// 	if(fs->iNodes[i].i_mode&0x4000) {
	// 		c = 'd';
	// 		pread(fs->fd, &fs->dNodes[fs->dir_count], 128, fs->group->bg_inode_table*fs->block_size + k*128);
	// 		fs->valid_dNodes[fs->dir_count] = k;
	// 		fs->dir_count++;
	// 	}
	// 	else if(fs->iNodes[i].i_mode&0x8000) c = 'f';
	// 	else if(fs->iNodes[i].i_mode&0xA000) c = 's';
	// 	else c = '?';

	// 	long tempSize = (((long)fs->iNodes[i].i_dir_acl) << 32) | fs->iNodes[i].i_size;
	// 	dprintf(STDOUT_FILENO,"INODE,");
	// 	dprintf(STDOUT_FILENO, "%d,", k+1);
	// 	dprintf(STDOUT_FILENO, "%c,", c);
	// 	dprintf(STDOUT_FILENO, "%o,", fs->iNodes[i].	i_mode & 0x0FFF);
	// 	dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_uid);
	// 	dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_gid);
	// 	dprintf(STDOUT_FILENO, "%d,", fs->iNodes[i].	i_links_count);
	// 	dprintf(STDOUT_FILENO, "%s,", 					cbuff);
	// 	dprintf(STDOUT_FILENO, "%s,", 					mbuff);
	// 	dprintf(STDOUT_FILENO, "%s,", 					abuff);
	// 	dprintf(STDOUT_FILENO, "%ld,", 					tempSize);
	// 	dprintf(STDOUT_FILENO, "%d", fs->iNodes[i].		i_blocks);

	// 	for (int j = 0; j < 15; j++)
	// 		dprintf(STDOUT_FILENO,",%d",fs->iNodes[i].	i_block[j]);
	// 	dprintf(STDOUT_FILENO,"\n");
	// }

	// get directory entries and print to stdout
	// DIRENT
	for(int i = 0; i < fs->dir_count; i++){
		for(int j = 0; j < 12; j++){
			int offset = fs->dNodes[i].i_block[j];
			if(offset != 0) directory(fs, offset, i);
		}
		indDirectory(fs, fs->dNodes[i].i_block[12], 0, i);
		indDirectory(fs,fs->dNodes[i].i_block[13], 1, i);
		indDirectory(fs,fs->dNodes[i].i_block[14], 2, i);
	}

	// get indeirect block references and print to stdout
	// INDIRECT
	for(int i = 0; i < fs->node_count; i++){
		indirect(fs, fs->iNodes[i].i_block[12],1,i,12);
		indirect(fs, fs->iNodes[i].i_block[13],2,i,268);
		indirect(fs, fs->iNodes[i].i_block[14],3,i,65804);
	}
	
	
	destroy_fs(fs);
	
	return 0;
}
#endif
