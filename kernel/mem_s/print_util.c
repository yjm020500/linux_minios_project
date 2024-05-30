#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep 함수가 포함된 헤더 파일

void printWithDelay(const char *str, int delay) {
    while (*str) {
        printf("%c", *str);
        fflush(stdout); // 출력 버퍼를 비워 즉시 출력되도록 함
        usleep(delay); // 지정한 시간만큼 지연 (마이크로초 단위)
        str++;
    }
    printf("\n");
}

void print_minios(char* str) {
        printf("%s\n",str);
}
