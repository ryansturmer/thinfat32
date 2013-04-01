#ifndef _THINTERNAL_H
#define _THINTERNAL_H

#define false 0
#define true 1

#pragma pack(push, 1)

// Starting at offset 36 into the BPB, this is the structure for a FAT12/16 FS
typedef struct struct_BPBFAT1216_struct {
    uint8_t     BS_DriveNumber;           // 1
    uint8_t     BS_Reserved1;             // 1
    uint8_t     BS_BootSig;               // 1
    uint32_t          BS_VolumeID;              // 4
             uint8_t     BS_VolumeLabel[11];       // 11
         uint8_t     BS_FileSystemType[8];     // 8
} BPB1216_struct;

// Starting at offset 36 into the BPB, this is the structure for a FAT32 FS
typedef struct struct_BPBFAT32_struct {
    uint32_t          FATSize;             // 4
    uint16_t    ExtFlags;              // 2
    uint16_t    FSVersion;             // 2
    uint32_t          RootCluster;           // 4
    uint16_t    FSInfo;                // 2
    uint16_t    BkBootSec;             // 2
    uint8_t     Reserved[12];          // 12
    uint8_t     BS_DriveNumber;            // 1
    uint8_t     BS_Reserved1;              // 1
    uint8_t     BS_BootSig;                // 1
    uint32_t          BS_VolumeID;               // 4
             uint8_t     BS_VolumeLabel[11];        // 11
             uint8_t     BS_FileSystemType[8];      // 8
} BPB32_struct;

typedef struct struct_BPB_struct {
    uint8_t     BS_JumpBoot[3];            // 3
             uint8_t     BS_OEMName[8];             // 8
    uint16_t    BytesPerSector;        // 2
    uint8_t     SectorsPerCluster;     // 1
    uint16_t    ReservedSectorCount;   // 2
    uint8_t     NumFATs;               // 1
    uint16_t    RootEntryCount;        // 2
    uint16_t    TotalSectors16;        // 2
    uint8_t     Media;                 // 1
    uint16_t    FATSize16;             // 2
    uint16_t    SectorsPerTrack;       // 2
    uint16_t    NumberOfHeads;         // 2
    uint32_t          HiddenSectors;         // 4
    uint32_t          TotalSectors32;        // 4
    union {
        BPB1216_struct fat16;
        BPB32_struct fat32;
    } FSTypeSpecificData;
} BPB_struct;

typedef struct struct_FatFile83 {
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creationTimeMs;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessTime;
    uint16_t eaIndex;
    uint16_t modifiedTime;
    uint16_t modifiedDate;
    uint16_t firstCluster;
    uint32_t fileSize;
} FatFile83;

typedef struct struct_FatFileLFN {
    uint8_t sequence_number;
    uint16_t name1[5];      // 5 Chars of name (UTF 16???)
    uint8_t attributes;     // Always 0x0f
    uint8_t reserved;       // Always 0x00
    uint8_t checksum;       // Checksum of DOS Filename.  See Docs.
    uint16_t name2[6];      // 6 More chars of name (UTF-16)
        uint16_t firstCluster;  // Always 0x0000
    uint16_t name3[2];
} FatFileLFN;

typedef union struct_FatFileEntry {
    FatFile83 msdos;
    FatFileLFN lfn;
} FatFileEntry;

#pragma pack(pop)


// "Legacy" functions
uint32_t fat_size(BPB_struct *bpb);
int total_sectors(BPB_struct *bpb);
int root_dir_sectors(BPB_struct *bpb);
int cluster_count(BPB_struct *bpb);
int fat_type(BPB_struct *bpb);
int first_data_sector(BPB_struct *bpb);
int first_sector_of_cluster(BPB_struct *bpb, int N);
int data_sectors(BPB_struct *bpb);
int fat_sector_number(BPB_struct *bpb, int N);
int fat_entry_offset(BPB_struct *bpb, int N);
int fat_entry_for_cluster(BPB_struct *bpb, uint8_t *buffer, int N);

#endif
