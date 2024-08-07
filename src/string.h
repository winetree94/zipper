#ifndef STRING_H
#define STRING_H

void substring(
    char *source,
    char *destination,
    int start,
    int end
);

unsigned long utf8_strlen(const char*);

#endif // STRING_H
