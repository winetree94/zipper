//
// Created by winetree94 on 8/7/2024.
//

#ifndef ZIPPER_H
#define ZIPPER_H

#include <stdio.h>

void printSomething();

#include <stdint.h>

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

#pragma pack(pop)  // 이전 패킹 설정 복원

#endif //ZIPPER_H
