#include <stdio.h>

void main() {
    int x, y;

    printf("첫 번째 값을 입력하세요: ");
    scanf("%d", &x);
    printf("두 번째 값을 입력하세요: ");
    scanf("%d", &y);

	printf("%d + %d = %d\n", x, y, x + y);
}