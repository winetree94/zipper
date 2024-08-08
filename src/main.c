#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "zipper.h"
#include "arg.h"

int main(int argc, char *argv[])
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("현재 작업 디렉토리: %s \n", cwd);
    }
    else
    {
        exit(1);
    }

    Arg args = parseArgs(argc, argv);

    printf("file path: %s \n", args.filePath);

    Zip *zip = zip_open(args.filePath);

    if (zip == NULL)
    {
        printf("파일 열기 실패");
        return 1;
    }

    EndOfCentralDirectoryRecord eocd;
    zip_read_eocd(zip, &eocd);

    if (eocd.signature != EOCD_SIGNATURE)
    {
        printf("올바른 zip 파일이 아닙니다.");
        zip_close(zip);
        return 1;
    }

    printf("----- End Of Central Directory ------\n");
    char *eocd_comment = malloc(eocd.comment_length + 1);
    zip_read_eocd_comment(zip, eocd_comment, &eocd);

    printf("eocd.signature: %x\n", eocd.signature);
    printf("eocd.disk_number: %d\n", eocd.disk_number);
    printf("eocd.central_directory_disk_number: %d\n", eocd.central_directory_disk_number);
    printf("eocd.number_of_entries_on_this_disk: %d\n", eocd.number_of_entries_on_this_disk);
    printf("eocd.total_number_of_entries: %d\n", eocd.total_number_of_entries);
    printf("eocd.central_directory_size: %d\n", eocd.central_directory_size);
    printf("eocd.central_directory_offset: %d\n", eocd.central_directory_offset);
    printf("eocd.comment_length: %d\n", eocd.comment_length);
    printf("eocd.comment: %s\n", eocd_comment);
    printf("eocd.start: %zx\n", eocd.start);
    printf("eocd.end: %zx\n", eocd.end);

    CentralDirectoryFileHeader *cdfh = malloc(
        sizeof(CentralDirectoryFileHeader) * eocd.total_number_of_entries);
    zip_read_cdfh(zip, cdfh, eocd.central_directory_offset, eocd.central_directory_size);

    if (cdfh == NULL)
    {
        printf("파일 정보가 없습니다.");
        free(eocd_comment);
        zip_close(zip);
        return 1;
    }

    printf("----- End Of Central Directory ------\n");

    for (int i = 0; i < eocd.total_number_of_entries; i ++) {
        printf("----- Central Directory File Header ------\n");
        CentralDirectoryFileHeader *ptr = cdfh + i;
        char *file_name = malloc(ptr->filename_length + 1);
        char *comment = malloc(ptr->file_comment_length + 1);
        char *extra = malloc(ptr->extra_field_length + 1);
        zip_read_cdfh_file_name(zip, file_name, ptr);
        zip_read_cdfh_comment(zip, comment, ptr);
        zip_read_cdfh_extra(zip, extra, ptr);

        printf("cdfh.index: %d\n", i);
        printf("cdfh.signature: %x\n", ptr->signature);
        printf("cdfh.version_made_by: %d\n", ptr->version_made_by);
        printf("cdfh.version_needed: %d\n", ptr->version_needed);
        printf("cdfh.flags: %d\n", ptr->flags);
        printf("cdfh.compression: %d\n", ptr->compression);
        printf("cdfh.mod_time: %d\n", ptr->mod_time);
        printf("cdfh.mod_date: %d\n", ptr->mod_date);
        printf("cdfh.crd32: %d\n", ptr->crc32);
        printf("cdfh.compressed_size: %d\n", ptr->compressed_size);
        printf("cdfh.uncompressed_size: %d\n", ptr->uncompressed_size);
        printf("cdfh.filename_length: %d\n", ptr->filename_length);
        printf("cdfh.extra_field_length: %d\n", ptr->extra_field_length);
        printf("cdfh.file_comment_length: %d\n", ptr->file_comment_length);
        printf("cdfh.disk_number_start: %d\n", ptr->disk_number_start);
        printf("cdfh.internal_file_attributes: %d\n", ptr->internal_file_attributes);
        printf("cdfh.external_file_attributes: %d\n", ptr->external_file_attributes);
        printf("cdfh.relative_offset_of_local_header: %d\n", ptr->relative_offset_of_local_header);

        printf("cdfh.file_name: %s\n", file_name);
        printf("cdfh.comment: %s\n", comment);
        printf("cdfh.extra: %s\n", extra);

        printf("cdfh.start: %zx\n", ptr->start);
        printf("cdfh.end: %zx\n", ptr->end);

        free(file_name);
        free(comment);
        free(extra);
        printf("----- Central Directory File Header ------\n");
    }

    zip_close(zip);
    free(eocd_comment);
    free(cdfh);

    return 0;
}
