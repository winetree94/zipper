//
// Created by winetree94 on 8/8/2024.
//
#include <string.h>
#include "string.h"

void substring(
    char *source,
    char *destination,
    int start,
    int end
) {

}

// strlen 은 1바이트당 1로 측정함
// 한글 등 utf-8 이 필요한 인코딩에선 문자형당 가변 바이트를 가지기 때문에
// 별도의 측정 로직이 필요함
// 
// 0xxxxxxx: 1바이트 문자 (ASCII)
// 110xxxxx: 2바이트 문자 시작
// 1110xxxx: 3바이트 문자 시작
// 11110xxx: 4바이트 문자 시작
// 10xxxxxx: 연속 바이트
unsigned long utf8_strlen(const char *str) {
    unsigned long length = 0;
    while (*str) {
        if ((*str & 0xC0) != 0x80) { // 0x80 (10xxxxxx) is a continuation byte
            length++;
        }
        str++;
    }
    return length;
}

char *slice(
    char *origin,
    int start,
    int end
) {

}