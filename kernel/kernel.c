#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
#include "print_util.h"

int pane_arr[4] = {0, 0, 0, 0}; // pane 제어 전역변수
//                 2  3  4  5  번 pane

int main() {
    system("clear");
    char * input;

    // 메모리 setting
    void * virtual_physical_memory = make_dummy_physical_memory(); // in memory_allocate.c

    // ProgrogramManager * pm = create_program_manager( ...... );
    // ProgramPool
    // ProgramQueue

    FrameList * fl = create_empty_frames_list();
    FrameManager * fm = create_frame_manager(virtual_physical_memory, fl);
    ProcessPool * pp = CreateProcessPool();
    WaitingQueue * wq = create_waiting_queue();
    
    //

    // 필요할 때 사용하자
    // const char * tmux_session_name = "terminal";
    // const char * tmux_pane = "1";

    print_minios("");
    printWithDelay("Welcome to the Memory Management Simulation.", 30000);
    print_minios("");
    printWithDelay("================================================", 10000);
    print_minios("");
    sleep(1);

    memory_view(virtual_physical_memory, 0, 25); // in memory_allocate.c

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            printWithDelay("Bye, See you next time.", 30000);
            sleep(1);

            free(input);
            freeProcessPool(pp);
            free_frame_manager(fm) ;
            free_empty_frames_list(fl);

            system("tmux kill-session -t terminal");
            break;
        }
        
        if (strcmp(input, "minisystem") == 0) {
            minisystem();
        }

        else if (strcmp(input, "help") == 0) {
            print_minios("");
            print_minios("show_m - 메모리 VIEW에서 볼 수 있는 매모리 그림을 업데이트 합니다. \n");
            print_minios("show_f - Frame Manager를 참조하여, 모든 frame의 번호, 상태(할당), 프레임의 첫주소를 볼 수 있습니다. \n");
            print_minios("show-efl - 가용 가능 프레임 리스트를 볼 수 있습니다. \n");
            print_minios("show_pp - Process Pool을 참조하여, 현재 메모리에 적재된 프로그램을 볼 수 있습니다. \n");
            print_minios("execute - 실행하고자 하는 프로그램을 입력받아 메모리에 적재합니다. \n");
            print_minios("terminate - 실행 중인 프로그램을 종료 시킵니다. \n");
        }

        else if (strcmp(input, "show_m") == 0) {
            size_t first, end;

            print_minios("Memory addresses are accessible from 0 to 65535.");
            print_minios("");
            printf("Which address should you print first : ");
            scanf("%zu", &first);
            printf("Which address should you print end : ");
            scanf("%zu", &end);
            print_minios("");
            memory_view(virtual_physical_memory, first, end);
        }
     
        else if (strcmp(input, "show_f") == 0) {
            show_frame_status(fm);
        }
        
        else if (strcmp(input, "show_efl") == 0) {
            print_empty_frames_list(fl);
        }

        else if(strcmp(input, "show_pp") == 0){
            show_pp(pp);
        }

        else if (strcmp(input, "execute") == 0) {
            execute(virtual_physical_memory, fl, fm, pp, wq);
        }

        else if (strcmp(input, "terminate") == 0) {
            terminate(virtual_physical_memory, fl, fm, pp, wq);
        }

        else {
            system(input); 
            print_minios("");
        }
    }

    return 0;
}
