#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"

int main() {
    char * input;

    void * virtual_physical_memory = make_dummy_physical_memory();

    // 메모리 주소와 데이터 값을 출력
    unsigned char * p = (unsigned char *)virtual_physical_memory;
    for (size_t i = 0; i < 100; i++) { // 처음 100 바이트만 출력
        printf("주소: %p, 데이터: 0x%02X\n", (void *)(p + i), p[i]);
    }

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }
        
        if (strcmp(input, "minisystem") == 0) {
            minisystem();
        }
     
        else {
            system(input); 
        }
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return 0;
}
