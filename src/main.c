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

    if (eocd.signature != EOCD_SIGNATURE) {
        printf("올바른 zip 파일이 아닙니다.");
    }

    char *eocd_comment = malloc(eocd.comment_length + 1);
    zip_read_eocd_comment(zip, eocd_comment, eocd.comment_length);

    printf("eocd.signature: %x\n", eocd.signature);
    printf("eocd.disk_number: %d\n", eocd.disk_number);
    printf("eocd.central_directory_disk_number: %d\n", eocd.central_directory_disk_number);
    printf("eocd.number_of_entries_on_this_disk: %d\n", eocd.number_of_entries_on_this_disk);
    printf("eocd.total_number_of_entries: %d\n", eocd.total_number_of_entries);
    printf("eocd.central_directory_size: %d\n", eocd.central_directory_size);
    printf("eocd.central_directory_offset: %d\n", eocd.central_directory_offset);
    printf("eocd.comment_length: %d\n", eocd.comment_length);
    printf("eocd.comment: %s\n", eocd_comment);

    zip_close(zip);
    free(eocd_comment);

    return 0;
}
