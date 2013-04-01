#include "thinfat32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat32_ui.h"

#define NO_ERROR 0
#define FILE_OPEN_ERROR -1
#define DATA_READ_ERROR -2
#define DATA_WRITE_ERROR -3
#define DATA_MISMATCH_ERROR -4

// Tests
int basic_read(char *input_fle, char *expected);
int basic_write(char *input_file, char *write_string);
int basic_append(char *input_file, char *write_string);

int main(int argc, char **argv) {
    TFFile *fp;
    char data;
    int rc;

    printf("\r\nFAT32 Filesystem Test");
    printf("\r\n-----------------------");
    tf_init();

    // BASIC WRITE, Root directory, LFN
    printf("\r\n[TEST] Basic LFN write test ") ;
    if(rc = test_basic_write("/test_longfilename0.txt", "Hello, World!")) {
        printf("failed with error code 0x%x", rc) ;
    }else { printf(" PASSED."); }

    // BASIC READ, Root directory, LFN
    printf("\r\n[TEST] Basic LFN read test ") ;
    if(rc = test_basic_read("/test_longfilename0.txt", "Hello, World!")) {
        printf("failed with error code 0x%x", rc) ;
    }else { printf(" PASSED.\n"); }

    // BASIC WRITE, Root directory, 8.3 filename
    if(rc = test_basic_write("/test0.txt", "Hello, World!")) {
        printf("\r\n[TEST] Basic 8.3 write test failed with error code 0x%x", rc);
    }else { printf("\r\n[TEST] Basic 8.3 write test PASSED."); }
    // BASIC READ, Root directory, 8.3 filename
    if(rc = test_basic_read("/test0.txt", "Hello, World!")) {
        printf("\r\n[TEST] Basic 8.3 read test failed with error code 0x%x", rc) ;
    }else { printf("\r\n[TEST] Basic 8.3 read test PASSED."); }
    
    return 0;
}

/*
 * Open a file, read its contents, if the contents match, return 0
 *If the contents don't match, or any other error occurs, return 
 * an appropriate error code.
 */
int test_basic_read(char *input_file, char *expected) {
    TFFile *fp;
    char data[128];
    int size = strlen(expected);
    fp = tf_fopen(input_file, "r");
    int i=0;

    if(fp) {
        while(!tf_fread(&(data[i]), 1, fp)) {i+=1;}
        data[i+1] = '\x00';
        //printf("\r\n\r\nexpected: %s", expected);
        //printf("\r\n\r\nreceived: %s", data);
        if(strcmp(data, expected)) {
            tf_fclose(fp);
            return DATA_MISMATCH_ERROR;
        }
        else {
            tf_fclose(fp);
            return NO_ERROR;
        }

    }
    else {
        return FILE_OPEN_ERROR;
    }
}

/*
 * Open a file, write a string to it, return 0.
 * Return an appropriate error code if there's any problem.
 */
int test_basic_write(char *input_file, char *write_string) {
    TFFile *fp;
    int rc;

    fp = tf_fopen(input_file, "w");
    
    if(fp) {
        printf("\r\n[TEST] writing data to file: %s", write_string);
        rc = tf_fwrite(write_string, 1, strlen(write_string), fp);
        if(rc<1) {
            tf_fclose(fp);
            return DATA_WRITE_ERROR;
        }
        else {
            tf_fclose(fp);
            return NO_ERROR;
        }
        
    }
    else {
        return FILE_OPEN_ERROR;
    }
}


/*
 * Open a file, append a string to it, return 0.
 * Return an appropriate error code if there's any problem.
 */
int test_basic_append(char *input_file, char *write_string) {
    TFFile *fp;
    int rc;

    fp = tf_fopen(input_file, "a");
    
    if(fp) {
        rc = tf_fwrite(write_string, 1, strlen(write_string), fp);
        if(rc) {
            tf_fclose(fp);
            return DATA_WRITE_ERROR;
        }
        else {
            tf_fclose(fp);
            return NO_ERROR;
        }
        
    }
    else {
        return FILE_OPEN_ERROR;
    }
}
