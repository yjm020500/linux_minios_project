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

void terminate(void * virtual_physical_memory, FrameList * fl, FrameManager * fm, ProcessPool * pp, WaitingQueue * wq) {
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


    int dequeue_num = 0;
    int size = queue_size(wq);

    printf("현재 Waiting Queue에 대기중인 프로세스 [ %d ]개 \n\n" , size);

    while(dequeue_num < size) {
        int pane_num = get_first_available_pane(pane_arr);

        if(pane_num == -1) {
            print_minios("\n 프로그램을 실행할 공간이 없습니다. \n");
            return;
        }

        WaitingNode * first_dequeued = dequeue(wq);
        printf("첫번째 대기프로세스 [ %s ] dequeued! \n" , first_dequeued->process_name);
        printf("필요한 frame 수 [ %d ] / 현재 가용 가능한 frame 수 [ %d ] \n\n" , first_dequeued->page_manager->allocated_pages, count_empty_frames(fl));

        if ( first_dequeued -> page_manager -> allocated_pages <= count_empty_frames(fl) ) {
            printf("메모리에 적재 가능 ! \n\n");
        
            for (int i = 0; i < first_dequeued -> page_manager -> allocated_pages; i++) {
                Frame frame_out = get_first_empty_frame(fl, fm);
                first_dequeued -> page_manager -> pages[i].matched_frame = frame_out.frame_number;
                first_dequeued -> page_manager -> pages[i].is_matched_frame = 1;

                unsigned char * addr_ptr;
                addr_ptr = (virtual_physical_memory + (int)frame_out.first_address);
    
                for (int j = 0; j < PAGE_SIZE; j++) {
                    *(addr_ptr + j) = first_dequeued -> page_manager -> pages[i].data[j];
                }

                printf(" [ %d번 page => %d번 frame에 매핑 ] \n", 
                first_dequeued -> page_manager -> pages[i].page_number, first_dequeued -> page_manager -> pages[i].matched_frame);
             }

            // kernel에서 보여지는 영역 시작
            first_dequeued -> page_manager -> is_memory_loaded = 1;
            show_pf_table(first_dequeued -> page_manager, fm);
            addProcess(pp, first_dequeued->page_manager, first_dequeued->process_name, pane_num); // 프로세서 pool에 넣는다.
            print_minios("프로그램 적재(loading) 완료 \n");
            memory_view(virtual_physical_memory, 0, 20);

            char tmux_command[64];
            const char * terminal = "terminal";
            char full_path[40];
            strcpy(full_path, "program/");
            strcat(full_path, first_dequeued->process_name);
            snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%d './%s' C-m", terminal, pane_num, full_path);
            system(tmux_command);

            dequeue_num++;
        } else {
            printf("메모리에 적재 불가능... \n\n");
            enqueue(wq, first_dequeued->page_manager, first_dequeued->process_name);
            dequeue_num++;
        }
    }
   

    return;
}