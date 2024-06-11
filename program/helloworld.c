#include <stdio.h>
#include <stdlib.h>

int mian(void){
 while(1){
    int num;
    printf("if you want to start, enter 1\n");
    scanf("%d", &num);
    if(num == 1) {break;}
 }
printf("hello world!");
}
