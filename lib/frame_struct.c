#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096 // 4KB 페이지 크기
#define OFFSET_MASK 0xFFF // 12비트 오프셋

#define TOTAL_MEMORY_SIZE 65536 // 64KB 전체 메모리 크기
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / PAGE_SIZE) // 총 프레임 수

typedef struct { 
    int frame_number;
    int is_allocated;
    size_t first_address;
} Frame; // 각 frame을 관리

typedef struct {
    Frame frames[TOTAL_FRAMES];
    unsigned char * mem;
} FrameManager; // 전체 frame을 관리

// status가 0인 frame들을 관리하는 양방향 연결 리스트
typedef struct FrameNode {
    Frame frame;
    struct FrameNode * prev;
    struct FrameNode * next;
} FrameNode;

typedef struct {
    FrameNode * head;
    FrameNode * tail;
} FrameList;
//

FrameList * create_empty_frames_list() { // 비어 있는 frame들을 관리하는 양방향 연결 리스트 생성
    FrameList * empty_frames_list = (FrameList *)malloc(sizeof(FrameList));
    empty_frames_list->head = NULL;
    empty_frames_list->tail = NULL;

    return empty_frames_list;
}

void add_empty_frame_to_list(FrameList * empty_frames_list, Frame frame) {
    FrameNode * new_node = (FrameNode *)malloc(sizeof(FrameNode));
    new_node -> frame = frame;
    new_node -> prev = NULL;
    new_node -> next = NULL;

    if (empty_frames_list -> head == NULL) {
        empty_frames_list -> head = new_node;
        empty_frames_list -> tail = new_node;
    } else {
        empty_frames_list -> tail -> next = new_node;
        new_node -> prev = empty_frames_list -> tail;
        empty_frames_list -> tail = new_node;
    }
}

void print_empty_frames_list(FrameList * empty_frames_list) {
    FrameNode * current = empty_frames_list -> head;
    printf("\n[ HEAD ] \n\n");
    while (current != NULL) {
        printf("Frame Number: %3d  First address : %6zu \n", current -> frame.frame_number, current -> frame.first_address);
        current = current->next;
    }
    printf("\n[ TAIL ] \n\n");
}

// 빈 프레임 연결 리스트 내의 노드 개수 세기
int count_empty_frames(FrameList * empty_frames_list) {
    int count = 0;
    FrameNode * current = empty_frames_list -> head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// 빈 프레임 연결 리스트에서 맨 앞의 프레임 가져오기, 
// 항상 먼저 연결 리스트 개수 확인한 후에 실행 (개수가 0 이면 에러)
Frame get_first_empty_frame(FrameList * empty_frames_list) {
    if (empty_frames_list -> head == NULL) {
        Frame error;
        error.frame_number = -1;
        error.is_allocated = -1;
        error.first_address = -1;
        printf("No empty frame ! \n");
        return error;
    }

    FrameNode * first_node = empty_frames_list -> head;
    Frame first_frame = first_node -> frame;

    if (empty_frames_list -> head == empty_frames_list -> tail) {
        // Only one node in the list
        empty_frames_list -> head = NULL;
        empty_frames_list -> tail = NULL;
    } else {
        empty_frames_list -> head = first_node -> next;
        empty_frames_list -> head -> prev = NULL;
    }

    free(first_node);

    return first_frame;
}

// empty frames list 생성 후에 포인터 전달
FrameManager * create_frame_manager(unsigned char * dummy_physical_mem, FrameList * empty_frames_list) {
    // 전체 frame을 묶어서 관리하는 자료구조
    FrameManager * frame_manager = (FrameManager *)malloc(sizeof(FrameManager)); 
    frame_manager -> mem = dummy_physical_mem; // 할당받은 dummy 물리 메모리 공간을 연결

    size_t addr = 0;
    
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frame_manager -> frames[i].frame_number = i; // 정해지고 변하지 않음
        frame_manager -> frames[i].is_allocated = 0; // 계속 변하는 값
        frame_manager -> frames[i].first_address = addr; // 정해지고 변하지 않음
        addr += 4096;

        // 처음 frame manager가 setting 될 때, 모든 frame을 양방향 연결 리스트에 연결
        if (frame_manager->frames[i].is_allocated == 0) {
            add_empty_frame_to_list(empty_frames_list, frame_manager->frames[i]);
        }
    }

    printf("Frame 관리 자료구조 세팅 완료 ...");

    return frame_manager;
}

void show_frame_status(FrameManager * frame_manager) {
    printf("%15s  %9s  %15s \n", "Frame Number", "Status", "First Address");
    printf("   ======================================== \n");
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        printf("%13d  %9d  %17zu \n", 
        frame_manager->frames[i].frame_number, frame_manager->frames[i].is_allocated, frame_manager->frames[i].first_address);
    }
}

// 동적 할당 메모리 free 함수 모음

void free_frame_manager(FrameManager * frame_manager) {
    free(frame_manager);
}

void free_empty_frames_list(FrameList * empty_frames_list) {
    FrameNode * current = empty_frames_list -> head;
    while (current != NULL) {
        FrameNode * temp = current;
        current = current -> next;
        free(temp);
    }
    free(empty_frames_list);
}