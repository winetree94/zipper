#include <stdio.h>
#include <string.h>
#include "arg.h"

const char *FILE_PATH = "--file-path";

/**
 *
 * 
 */
char *get_arg_value(const char *arg, const char *findArgs)
{
    if (arg == NULL || findArgs == NULL) 
    {
        return NULL;
    }
    char *findResult = strstr(arg, findArgs);

    if (findResult != arg)
    {
        return NULL;
    }

    return (char *)(arg + strlen(findArgs) + 1);
}

Arg parseArgs(const int argc, char *argv[])
{
    printf("argc: %d \n", argc);

    char *filePath;

    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
        filePath = get_arg_value(arg, FILE_PATH);
    }

    const Arg result = {
        .filePath = filePath
    };

    printf("file path: %s \n", result.filePath);

    return result;
}
