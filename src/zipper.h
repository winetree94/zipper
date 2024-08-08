//
// Created by winetree94 on 8/7/2024.
//

#ifndef ZIPPER_H
#define ZIPPER_H

// 현재 아키텍쳐(x86, x64)의 포인터의 사이즈
#define POINTER_SIZE sizeof(void*)

#define EOCD_SIGNATURE 0x06054b50
#define CDS_SIGNATURE 0x02014b50
#define LFH_SIGNATURE 0x04034b50

#include <stdio.h>
#include <stdint.h>

void printSomething();


#pragma pack(push, 1)  // 1바이트 경계로 패킹 설정

typedef struct {
    uint32_t signature;
    uint16_t version;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t filename_length;
    uint16_t extra_field_length;
} LocalFileHeader;

typedef struct {
    uint32_t signature;
    uint16_t version_made_by;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t filename_length;
    uint16_t extra_field_length;
    uint16_t file_comment_length;
    uint16_t disk_number_start;
    uint16_t internal_file_attributes;
    uint32_t external_file_attributes;
    uint32_t relative_offset_of_local_header;
} CentralDirectoryFileHeader;

typedef struct {
    uint32_t signature;
    uint16_t disk_number;
    uint16_t central_directory_disk_number;
    uint16_t number_of_entries_on_this_disk;
    uint16_t total_number_of_entries;
    uint32_t central_directory_size;
    uint32_t central_directory_offset;
    uint16_t comment_length;
} EndOfCentralDirectoryRecord;

typedef struct {
    FILE *file;
} Zip;

#pragma pack(pop)  // 이전 패킹 설정 복원

/**
 * 지정된 경로의 zip 파일을 엽니다.
 * 파일이 올바른 zip 형식이 아닌 경우 NULL 을 반환합니다.
 */
Zip *zip_open(char* path);

void zip_read_eocd(Zip *, EndOfCentralDirectoryRecord *);

void zip_read_eocd_comment(Zip *, char *, int);

void zip_read_cdfh(CentralDirectoryFileHeader *, FILE *, int start, int size);

void zip_close(Zip *zip);

#endif //ZIPPER_H
