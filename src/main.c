#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "zipper.h"
#include "arg.h"

int main(int argc, char *argv[])
{
    // char *name = "hansol박한솔";

    // printf("name: %s \n", name);
    // printf("size of name: %ld \n", sizeof(name));
    // printf("len of name: %ld \n", strlen(name));
    // printf("name + 8: %s \n", name + 8);

    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("현재 작업 디렉토리: %s \n", cwd);
    }
    else
    {
        exit(1);
    }

    Arg args = parseArgs(argc, argv);

    printf("file path: %s \n", args.filePath);

    FILE *file;

    file = fopen(args.filePath, "rb");

    if (file == NULL) {
        perror("파일 열기 실패");
        return 1;
    } else {
        printf("%s 파일을 열었습니다.\n", args.filePath);
    }

    fseek(file, 0, SEEK_END);
    // EndOfCentralDirectoryRecord eocd;
    // fread(&eocd, sizeof(EndOfCentralDirectoryRecord), 1, file);

    // printf("eocd: %x \n", eocd.signature);

    // if (eocd.signature != 0x06054b50) {
    //     printf("zip 파일이 아닙니다.");
    //     exit(1);
    // }

    // 파일 내용 읽기 (줄 단위로)
    // while (fgets(buffer, sizeof(buffer), file) != NULL) {
    //     printf("%s", buffer);
    // }

    fclose(file);

    return 0;
}
