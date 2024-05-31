#ifndef MY_STRUCT_H  // 중복 포함 방지를 위한 헤더 가드
#define MY_STRUCT_H

// 구조체 정의
typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

#endif // MY_STRUCT_H
