#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
#include "print_util.h"

int main() {

    system("clear");

    char * input;

    void * virtual_physical_memory = make_dummy_physical_memory();

    void * start_address = (void *)virtual_physical_memory;
    void * end_address = (void *)(virtual_physical_memory + (64*1024) - 1);

    prepare_dummy_physical_memory_reorder(start_address, end_address);

    // 메모리 주소와 데이터 값을 출력 (윗단은 진짜 64KB 메모리 처럼 재구성, 아랫단은 진짜 값)
    unsigned char * p = (unsigned char *)virtual_physical_memory;

    for (size_t i = 0; i < 64*1024 ; i++) {
        print_minios("--------------------------------------------");
        printf("| ADDRESS |  0x%04zX  | DATA |  0b", i);
        print_binary(p[i]);
        print_minios("  |");
        print_minios("--------------------------------------------");
    }

    // for (size_t i = 0; i < 100; i++) { // 처음 100 바이트만 출력
    //     printf("주소: %p, 데이터: 0b", (void *)(p + i));
    //     print_binary(p[i]); // (void *)(p + i) 가 메모리 주소, p[i]가 그 메모리의 값 (if, i=0  (void *)(p), p[0])
    //     printf("\n");
    // }


    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            system("tmux kill-session -t terminal");
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
