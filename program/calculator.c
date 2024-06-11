#include <stdio.h>
#include <stdlib.h>

int main()
{
    int chara;
    int number = 0; 
    int num1; 
    int num2;
    printf("add=1,sub=2,mul=3,div=4\n");
    scanf("%d", &chara);
    printf("Enter first number\n");
    scanf("%d", &num1);
    printf("Enter second number\n");
    scanf("%d", &num2);
    if(chara==1){
        number=num1+num2;
        printf("result : %d \n", number);
    }
    else if(chara == 2){
        number=num1-num2;
        printf("result : %d \n", number);
    }
    else if(chara==3){
        number=num1*num2;
        printf("result : %d \n", number);
    }
    else if(chara==4){
        number=num1/num2;
        printf("result : %d \n", number);
    } else {
        printf("무슨 계산을 하라는거야....");
    }

    return 0;
}
