#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

void release_pane(int * pane_arr, int pane_id) {
    int index = pane_id - 2; // pane_id는 2부터 시작하므로 인덱스는 pane_id - 2
    if (index >= 0 && index < 4) {
        pane_arr[index] = 0;
    } else {
        printf("Invalid pane_id: %d\n", pane_id);
    }
}

void terminate(void * virtual_physical_memory, FrameList * fl, FrameManager * fm, ProcessPool * pp) {
    int pane_num;
    char process_name[30]; // 프로그램 이름을 저장할 배열

    printf("종료할 프로그램을 입력하세요 : ");
    scanf(" %s", process_name); // 사용자로부터 파일 이름을 입력 받음

    Process * process_to_remove = removeProcess(pp, process_name);
    if (process_to_remove == NULL) {
        return;
    }

    pane_num = process_to_remove->pane_num;
    PageManager * page_manager = process_to_remove -> page_manager;
    
    for (int i = 0; i < page_manager->allocated_pages ; i++) {
        Frame frame_to_return = fm -> frames[(page_manager -> pages[i].matched_frame)];

        unsigned char * addr_ptr;
        addr_ptr = (virtual_physical_memory + (int)frame_to_return.first_address);
    
        for (int j = 0; j < PAGE_SIZE; j++) {
            *(addr_ptr + j) = 0x00;
        }

        add_empty_frame_sorted(fl, frame_to_return, fm);
        printf("    [ %d번 frame 반환 ] \n", page_manager -> pages[i].matched_frame);
    }

    remove_page_manager(page_manager); 
    printf("\n[ %s ] is terminated ! \n\n", process_name);
    memory_view(virtual_physical_memory, 0, 20);

    release_pane(pane_arr, pane_num); // pane 관리 배열에 pane 반환

    char tmux_command[64];
    const char * terminal = "terminal";
    snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%d C-c", terminal, pane_num); // Ctrl+C로 프로그램 종료
    system(tmux_command);
    snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%d \"clear\" C-m", terminal, pane_num); // pane clear
    system(tmux_command);

    return;
}