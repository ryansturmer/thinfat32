#ifndef __FAT32_H
#define __FAT32_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define TF_MAX_PATH 256
#define TF_FILE_HANDLES 5

#define TF_FLAG_DIRTY 0x01
#define TF_FLAG_OPEN 0x02
#define TF_FLAG_SIZECHANGED 0x04
#define TF_FLAG_ROOT 0x08

#define TYPE_FAT12 0
#define TYPE_FAT16 1
#define TYPE_FAT32 2

#define TF_MARK_BAD_CLUSTER32 0x0ffffff7
#define TF_MARK_BAD_CLUSTER16 0xfff7
#define TF_MARK_EOC32 0x0ffffff8
#define TF_MARK_EOC16 0xfff8

#define LFN_ENTRY_CAPACITY 13       // bytes per LFN entry

#define TF_ATTR_DIRECTORY 0x10
//  #define TF_DEBUG 1


#ifdef DEBUG

    #define dbg_printf(...) printf(__VA_ARGS__)
    #define dbg_printHex(x,y) printHex(x,y)

#ifdef TF_DEBUG
typedef struct struct_TFStats {
    unsigned long sector_reads;
    unsigned long sector_writes;
} TFStats;

    #define tf_printf(...) printf(__VA_ARGS__)
    #define tf_printHex(x,y) printHex(x,y)
#else
    #define tf_printf(...) 
    #define tf_printHex(x,y)
#endif  // TF_DEBUG

#else   // DEBUG
    #define dbg_printf(...)
    #define dbg_printHex(x,y)
    #define tf_printf(...) 
    #define tf_printHex(x,y)
#endif  // DEBUG

#define LSN(CN, bpb) SSA + ((CN-2) * bpb->SectorsPerCluster)

#ifndef min
#define min(x,y)  (x<y)? x:y  
#define max(x,y)  (x>y)? x:y  
#endif

    
// Ultimately, once the filesystem is checked for consistency, you only need a few
// things to keep it up and running.  These are:
// 1) The type (fat16 or fat32, no fat12 support)
// 2) The number of sectors per cluster
// 3) Everything needed to compute indices into the FATs, which includes:
//    * Bytes per sector, which is fixed at 512
//    * The number of reserved sectors (pulled directly from the BPB)
// 4) The current sector in memory.  No sense reading it if it's already in memory!

typedef struct struct_tfinfo {
    // FILESYSTEM INFO PROPER
    uint8_t type; // 0 for FAT16, 1 for FAT32.  FAT12 NOT SUPPORTED
    uint8_t sectorsPerCluster;
    uint32_t firstDataSector;
    uint32_t totalSectors;
    uint16_t reservedSectors;
    // "LIVE" DATA
    uint32_t currentSector;
    uint8_t sectorFlags;
    uint32_t rootDirectorySize;
    uint8_t buffer[512];
} TFInfo;

/////////////////////////////////////////////////////////////////////////////////

typedef struct struct_TFFILE {
    uint32_t parentStartCluster;
    uint32_t startCluster;
    uint32_t currentClusterIdx;
    uint32_t currentCluster;
    short currentSector;
    short currentByte;
    uint32_t pos;
    uint8_t flags;
    uint8_t attributes;
    uint8_t mode;
    uint32_t size;
    uint8_t filename[TF_MAX_PATH];
} TFFile;


#define TF_MODE_READ 0x01
#define TF_MODE_WRITE 0x02
#define TF_MODE_APPEND 0x04
#define TF_MODE_OVERWRITE 0x08

#define TF_ATTR_READ_ONLY 0x01
#define TF_ATTR_HIDDEN 0x02
#define TF_ATTR_SYSTEM 0x04
#define TF_ATTR_VOLUME_LABEL 0x08
#define TF_ATTR_DIRECTORY 0x10
#define TF_ATTR_ARCHIVE 0x20
#define TF_ATTR_DEVICE 0x40 // Should never happen!
#define TF_ATTR_UNUSED 0x80


int read_sector(uint8_t *data, uint32_t blocknum);
int write_sector(uint8_t *data, uint32_t blocknum);
// New error codes
#define TF_ERR_NO_ERROR 0
#define TF_ERR_BAD_BOOT_SIGNATURE 1
#define TF_ERR_BAD_FS_TYPE 2

#define TF_ERR_INVALID_SEEK 1

// FS Types
#define TF_TYPE_FAT16 0
#define TF_TYPE_FAT32 1

// New backend functions
int tf_init(void);
int tf_fetch(uint32_t sector);
int tf_store(void);
uint32_t tf_get_fat_entry(uint32_t cluster);
int tf_set_fat_entry(uint32_t cluster, uint32_t value);
int tf_unsafe_fseek(TFFile *fp, int32_t base, long offset);
TFFile *tf_fnopen(uint8_t *filename, const uint8_t *mode, int n);
int tf_free_clusterchain(uint32_t cluster);
int tf_create(uint8_t *filename);
void tf_release_handle(TFFile *fp);
TFFile *tf_parent(uint8_t *filename, const uint8_t *mode, int mkParents);
int tf_shorten_filename(uint8_t *dest, uint8_t *src, uint8_t num);

// New frontend functions
int tf_init();
int tf_fflush(TFFile *fp);
int tf_fseek(TFFile *fp, int32_t base, long offset);
int tf_fclose(TFFile *fp);
int tf_fread(uint8_t *dest,  int size,  TFFile *fp);
int tf_find_file(TFFile *current_directory, uint8_t *name);
int tf_compare_filename(TFFile *fp, uint8_t *name);
uint32_t tf_first_sector(uint32_t cluster);
uint8_t *tf_walk(uint8_t *filename, TFFile *fp);
TFFile *tf_fopen(uint8_t *filename, const uint8_t *mode);
int tf_fwrite(uint8_t *src, int size, int count, TFFile *fp);
int tf_fputs(uint8_t *src, TFFile *fp);
int tf_mkdir(uint8_t *filename, int mkParents);
int tf_remove(uint8_t *filename);
void tf_print_open_handles(void);

uint32_t tf_find_free_cluster();
uint32_t tf_find_free_cluster_from(uint32_t c);

uint32_t tf_initializeMedia(uint32_t totalSectors);
uint32_t tf_initializeMediaNoBlock(uint32_t totalSectors, int start);

// hidden functions... IAR requires that all functions be declared
TFFile *tf_get_free_handle();
uint8_t upper(uint8_t c);

// New Datas
extern TFInfo tf_info;
extern TFFile tf_file;

#endif
