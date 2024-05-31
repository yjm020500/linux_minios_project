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

// 1바이트(메모리 한칸의 크기 - 안의 값이 16진수로 표시됨)를 2진수로 만들어 다른 pane에 넘겨주기 위해 저장
void print_binary_to_file(unsigned char byte, FILE *fp) {
    for (int i = 7; i >= 0; i--) {
        fprintf(fp, "%d", (byte >> i) & 1);
    }
}