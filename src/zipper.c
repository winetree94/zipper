#include <stdlib.h>
#include "zipper.h"

Zip *zip_open(char *path)
{
    Zip *zip = malloc(sizeof(Zip));
    if (zip == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    zip->file = fopen(path, "rb");
    if (zip->file == NULL)
    {
        printf("Failed to open file: %s\n", path);
        free(zip);
        return NULL;
    }
    return zip;
}

void zip_close(Zip *zip)
{
    fclose(zip->file);
    free(zip);
}

void zip_read_eocd(
    Zip *zip,
    EndOfCentralDirectoryRecord *eocd)
{
    fseek(zip->file, 0, SEEK_END);
    int file_size = ftell(zip->file);

    for (int i = EOCD_MIN_SIZE; i <= file_size; i++)
    {
        uint32_t signature;
        fseek(zip->file, -i, SEEK_END);
        fread(&signature, sizeof(uint32_t), 1, zip->file);
        if (signature != EOCD_SIGNATURE)
        {
            continue;
        }
        fseek(zip->file, -i, SEEK_END);
        fread(eocd, EOCD_MIN_SIZE, 1, zip->file);

        eocd->start = i;
        eocd->end = i + EOCD_MIN_SIZE + eocd->comment_length;
        break;
    }
}

void zip_read_eocd_comment(
    Zip *zip,
    char *result,
    EndOfCentralDirectoryRecord *record)
{
    fseek(zip->file, -record->comment_length, SEEK_END);
    fread(result, record->comment_length, 1, zip->file);
    result[record->comment_length] = '\0';
}

void zip_read_cdfh(
    Zip *zip,
    CentralDirectoryFileHeader *result,
    int start,
    int size)
{
    int index = 0;

    fseek(zip->file, start, SEEK_SET);
    while (ftell(zip->file) < (start + size))
    {
        uint32_t signature;
        fread(&signature, sizeof(uint32_t), 1, zip->file);
        if (signature != CDFH_SIGNATURE)
        {
            // signature 찾지 못한 경우 1바이트씩 건너뛸 수 있게
            fseek(zip->file, -3, SEEK_CUR);
            continue;
        }
        fseek(zip->file, -sizeof(uint32_t), SEEK_CUR);
        fread(result + index, CDFH_MIN_SIZE, 1, zip->file);
        (result + index)->start = ftell(zip->file) - CDFH_MIN_SIZE;
        (result + index)->end = ftell(zip->file);
        index++;
    }
}

void zip_read_cdfh_file_name(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *header)
{
    fseek(zip->file, header->end, SEEK_SET);
    fread(result, header->filename_length, 1, zip->file);
    result[header->filename_length] = '\0';
}

void zip_read_cdfh_extra(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *header)
{
    fseek(zip->file, header->end + header->filename_length, SEEK_SET);
    fread(result, header->extra_field_length, 1, zip->file);
    result[header->extra_field_length] = '\0';
}

void zip_read_cdfh_comment(
    Zip *zip,
    char *result,
    CentralDirectoryFileHeader *header)
{
    fseek(
        zip->file,
        header->end + header->filename_length + header->extra_field_length,
        SEEK_SET);
    fread(
        result,
        header->file_comment_length,
        1,
        zip->file);
    result[header->file_comment_length] = '\0';
}