#ifndef ARG_H
#define ARG_H

typedef struct {
  char *filePath;
} Arg;

Arg parseArgs(int argc, char *argv[]);

#endif // ARG_H
