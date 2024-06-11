#define PAGE_SIZE 4096 // 4KB 페이지 크기

#define TOTAL_MEMORY_SIZE 65536 // 64KB 전체 메모리 크기
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / PAGE_SIZE) // 총 프레임 수
// #define OFFSET_MASK 0xFFF // 12비트 오프셋 마스크

// ================ ================ ================

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

// ================ ================ ================

typedef struct { // page struct
    unsigned int data[PAGE_SIZE];
    int page_number;
    int matched_frame; // 각 page와 frame matching된 값
    int is_matched_frame; // 각 page와 frame matching 여부 판단
} Page;

typedef struct { // page manager struct
    Page * pages; // dynamic allocation
    int allocated_pages; // 총 몇개의 페이지인지
    int is_memory_loaded; // is making page table finished?
} PageManager;

// ================ ================ ================

// 프로세스 정보를 담는 구조체
typedef struct Process {
    PageManager * page_manager;
    struct Process * next;
    char process_name[30];
    int pane_num;
} Process;

// 링크드 리스트 관리를 위한 구조체
typedef struct ProcessPool {
    Process* head;
} ProcessPool;

// ================ ================ ================

FrameList * create_empty_frames_list();
void add_empty_frame_sorted(FrameList * empty_frames_list, Frame frame, FrameManager * fm);
void print_empty_frames_list(FrameList * empty_frames_list); 
int count_empty_frames(FrameList * empty_frames_list);
Frame get_first_empty_frame(FrameList * empty_frames_list, FrameManager * fm);
Frame remove_node_at_position(FrameList * empty_frames_list, int position);

FrameManager * create_frame_manager(unsigned char * dummy_physical_mem, FrameList * empty_frames_list);
void show_frame_status(FrameManager * frame_manager);
void first_frame_list_set(FrameList * empty_frames_list, Frame frame);

void free_frame_manager(FrameManager * frame_manager);
void free_empty_frames_list(FrameList * empty_frames_list);

// ================ ================ ================

ProcessPool * CreateProcessPool();
void addProcess(ProcessPool * pool, PageManager * page_manager, char * name, int pane_num);
Process * removeProcess(ProcessPool * pool, char * name);
void printProcesses(ProcessPool * pool);
void freeProcessPool(ProcessPool * pool);
void show_pp(ProcessPool * pp);

// ================ ================ ================

PageManager * create_page_manager(int total_page_size);
void set_page_data(PageManager * page_manager, int total_bytes, unsigned int * byte);
void remove_page_manager(PageManager * page_manager);

void show_total_page_status(PageManager * page_manager);
void show_page_status(PageManager* page_manager, int page_num);
void show_pf_table(PageManager *page_manager, FrameManager *frame_manager);

// ================ ================ ================

typedef struct WaitingNode {
    PageManager *page_manager;
    char process_name[20];
    struct WaitingNode *next;
} WaitingNode;

typedef struct WaitingQueue {
    WaitingNode *front;
    WaitingNode *rear;
} WaitingQueue;

WaitingQueue* create_waiting_queue();
void enqueue(WaitingQueue *queue, PageManager *page_manager, const char *process_name);
WaitingNode* dequeue(WaitingQueue *queue);
int is_waiting_queue_empty(WaitingQueue *queue);
int queue_size(WaitingQueue * queue);