#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"

void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }
        
        if (strcmp(input, "minisystem") == 0) {
            minisystem();
        }
     
        else if (strcmp(input, "up_down_game") == 0) {
            up_down_game();
        }
        
        else if (strcmp(input, "mini_fork") == 0) {
            mini_fork();    
        }

	    else if (strcmp(input, "shm_com") == 0) {
	        shm_com();
	    }

        else if (strcmp(input, "pipe_com") == 0) {
            pipe_com();
	    }
        
        else if (strcmp(input, "monte_carlo") == 0) {
            monte_carlo();
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

void print_minios(char* str) {
        printf("%s\n",str);
}
