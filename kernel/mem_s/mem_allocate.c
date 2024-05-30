#include <stdio.h>
#include <stdlib.h>
#include "system.h"

void * memory_set(void * ptr, int value, size_t num) {
    unsigned char * p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void * make_dummy_physical_memory() {
    int delay = 30000; // 30,000 마이크로초 = 30 밀리초
    const char * booting_message = "Allocating virtual physical memory... [Size] 64KB";

    printWithDelay(booting_message, delay);
    print_minios(" ");

    // 64KB 메모리 할당
    size_t size = 64 * 1024; // 64 * 2^10 Byte
    void * mem = malloc(size);

    // 메모리 할당 실패 확인
    if (mem == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return NULL;
    }

    // memory_set 함수를 사용하여 할당된 메모리를 0x00로 초기화
    memory_set(mem, 0x00, size);

    return mem;
}