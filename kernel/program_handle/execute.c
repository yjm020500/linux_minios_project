#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

#define PAGE_SIZE 4096 // 4KB 페이지 크기
#define OFFSET_MASK 0xFFF // 12비트 오프셋


int read_program(char * path) { // 프로그램 파일 경로
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    int byte;
    long total_bytes = 0;
    
    while ((byte = fgetc(fp)) != EOF) {
        // 읽어온 바이트 값을 이용하여 원하는 작업을 수행합니다.
        // 예를 들어, 바이너리 데이터의 특정 부분을 확인하거나 처리할 수 있습니다.
        // 여기서는 총 읽은 바이트 수를 카운트합니다.
        total_bytes++;
        printf("%02X ", byte); // 예시로 읽어온 바이트 값을 16진수로 출력하는 예시
    }

    printf("\nTotal bytes read: %ld\n", total_bytes);

    fclose(fp);
    return 0;
}

int execute() {
    char path[30]; // 프로그램 이름을 저장할 배열
    char full_path[40];

    printf("실행할 프로그램을 입력하세요 : ");
    scanf("%s", path); // 사용자로부터 파일 경로를 입력 받음

    strcpy(full_path, "program/");
    strcat(full_path, path);

    // read_program 함수 호출
    int result = read_program(full_path);
    if (result != 0) {
        printf("디렉토리에 입력한 프로그램이 없습니다. \n");
        return 1;
    }

    return 0;
}