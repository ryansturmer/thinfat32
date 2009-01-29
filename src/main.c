#include "fat32.h"
#include <stdio.h>
#include <stdlib.h>
#include "fat32_ui.h"
int main(int argc, char **argv) {
	char data[1024];
	TFFile *fp;
	printf("FAT32 Filesystem Test Program\n");
	printf("-----------------------------\n");
	tf_init();
	
	fp = tf_fopen("/subdir/this is not a short filename at all.txt","w");
	
	if(fp) {
		/*
		printf("FILE CONTENTS:\n--------------\n");
			while(!tf_fread(data, 1, fp)) {
				printf("%c", data[0]);
			} 
		printf("\n--------------\n");
		*/
		tf_fputs("This file does not has a short name!", fp);
		tf_fclose(fp);
	}
	else {
		printf("File not found.\n");
	}
		printf("\n\n");
	
	return 0;
}
