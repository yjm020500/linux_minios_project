#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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

    const char * tmux_session_name = "terminal";
    const char * tmux_pane = "1";

    print_minios("");
    printWithDelay("Welcome to the Memory Management Simulation.", 30000);
    print_minios("");
    printWithDelay("===========================================", 30000);
    print_minios("");
    sleep(1);

    prepare_dummy_physical_memory_reorder(start_address, end_address);
    memory_view(virtual_physical_memory, 0, 25, tmux_session_name, tmux_pane);

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            printWithDelay("Bye, See you next time.", 30000);
            sleep(1);
            free(input);
            system("tmux kill-session -t terminal");
            break;
        }
        
        if (strcmp(input, "minisystem") == 0) {
            minisystem();
        }

        else if (strcmp(input, "show_memory") == 0) {
            size_t first, end;

            print_minios("Memory addresses are accessible from 0 to 65535.");
            print_minios("");
            printf("Which address should you print first : ");
            scanf("%zu", &first);
            printf("Which address should you print end : ");
            scanf("%zu", &end);
            print_minios("");
            memory_view(virtual_physical_memory, first, end, tmux_session_name, tmux_pane);
        }
     
        else if (strcmp(input, "help") == 0) {
            print_minios("Sorry, It will be implemented soon.");
            print_minios("");
        }

        else if (strcmp(input, "execute") == 0) {

        }

        else if (strcmp(input, "terminate") == 0) {

        }

        else {
            system(input); 
            print_minios("");
        }
    }

    return 0;
}
