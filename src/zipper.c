#include <stdlib.h>
#include "zipper.h"

void printSomething()
{
    printf("Hello, World! \n");
}

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
        printf("Memory allocation failed\n");
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
    int eocd_min_size = sizeof(EndOfCentralDirectoryRecord);

    fseek(zip->file, 0, SEEK_END);
    int file_size = ftell(zip->file);

    for (int i = eocd_min_size; i <= file_size; i++)
    {
        uint32_t signature;
        fseek(zip->file, -i, SEEK_END);
        fread(&signature, sizeof(uint32_t), 1, zip->file);
        if (signature != EOCD_SIGNATURE)
        {
            continue;
        }
        fseek(zip->file, -i, SEEK_END);
        fread(eocd, eocd_min_size, 1, zip->file);
        // 동적 메모리 할당을 여기서 해버리면 쓰는 곳에서 힘들다.
        // eocd.comment = malloc(eocd.comment_length + 1);
        // fread(eocd.comment, eocd.comment_length, 1, file);
        // eocd.comment[eocd.comment_length] = '\0';
        break;
    }
}

void zip_read_eocd_comment(
    Zip *zip,
    char *result,
    int comment_length)
{
    fseek(zip->file, -comment_length, SEEK_END);
    fread(result, comment_length, 1, zip->file);
    result[comment_length] = '\0';
}

void zip_read_cdfh(
    Zip *zip,
    CentralDirectoryFileHeader *result,
    int start,
    int size)
{
    fseek(zip->file, start, SEEK_SET);
    fread(result, size, 1, zip->file);
}