#include <stdio.h>

int read_program(char * path) { // 프로그램 파일 경로
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    // 파일에서 바이너리 데이터를 읽어와서 직접 처리하는 과정을 여기에 추가합니다.
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
