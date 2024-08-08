//
// Created by winetree94 on 8/7/2024.
//

#ifndef ZIPPER_H
#define ZIPPER_H

#include <stdio.h>
#include <stdint.h>

// 현재 아키텍쳐(x86, x64)의 포인터의 사이즈
#define POINTER_SIZE sizeof(void *)
#define EOCD_SIGNATURE 0x06054b50
#define EOCD_MIN_SIZE 22
#define CDFH_SIGNATURE 0x02014b50
#define CDFH_MIN_SIZE 46
#define LFH_SIGNATURE 0x04034b50
#define LFH_MIN_SIZE 32

/**
 * file stream 바이트를 그대로 struct 에 대입하기 위해
 * 1바이트 경계로 패킹을 설정
 */
#pragma pack(push, 1)

typedef struct
{
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

typedef struct
{
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

    // extra
    size_t start;
    size_t end;
} CentralDirectoryFileHeader;

typedef struct
{
    uint32_t signature;
    uint16_t disk_number;
    uint16_t central_directory_disk_number;
    uint16_t number_of_entries_on_this_disk;
    uint16_t total_number_of_entries;
    uint32_t central_directory_size;
    uint32_t central_directory_offset;
    uint16_t comment_length;

    // extra
    size_t start;
    size_t end;
} EndOfCentralDirectoryRecord;

#pragma pack(pop) // 이전 패킹 설정 복원

typedef struct
{
    FILE *file;
} Zip;

/**
 * 지정된 경로의 zip 파일을 엽니다.
 * 파일이 올바른 zip 형식이 아닌 경우 NULL 을 반환합니다.
 */
Zip *zip_open(char *path);

/**
 * zip 파일에서 EOCD 테이블을 읽습니다.
 */
void zip_read_eocd(
    Zip *zip,
    EndOfCentralDirectoryRecord *record);

/**
 * zip 파일에서 EOCD 테이블의 comment 를 읽습니다.
 */
void zip_read_eocd_comment(
    Zip *zip,
    char *result,
    EndOfCentralDirectoryRecord *record);

/**
 * zip 파일에서 CDFH 테이블 목록을 읽습니다.
 */
void zip_read_cdfh(
    Zip *zip,
    CentralDirectoryFileHeader *records,
    int start,
    int size);

/**
 * zip 파일의 CDFH 테이블에서 파일 이름을 읽습니다.
 */
void zip_read_cdfh_file_name(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *record);

/**
 * zip 파일의 CDFH 테이블에서 comment 를 읽습니다.
 */
void zip_read_cdfh_comment(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *record);

/**
 * zip 파일의 CDFH 테이블에서 extra field 를 읽습니다.
 */
void zip_read_cdfh_extra(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *record);

/**
 * zip 파일을 닫습니다. 메모리가 반환됩니다.
 */
void zip_close(Zip *zip);

#endif // ZIPPER_H
