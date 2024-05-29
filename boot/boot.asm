[org 0] 
[bits 16]
	jmp 0x07C0:start
	
start :
	mov ax, cs
	mov ds, ax
	
	mov ax, 0xB800
	mov es, ax
	mov di, 0
	mov ax, word [msgBack]
	mov cx, 0x7FF
paint :
    mov word [es:di], ax;
    add di, 2;
    dec cx;
    jnz paint;
    
read:
    mov ax, 0x1000      ; ES:BX = 1000:0000
    mov es, ax          ;
    mov bx, 0           ; 0x10000번지에 커널을 로드
 
    mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로 
    mov al, 1           ; 1 섹터를 읽을 것이다.
    mov ch, 0           ; 0번째 Cylinder
    mov cl, 2           ; 2번째 섹터부터 읽기 시작한다.
    mov dh, 0           ; Head=0
    mov dl, 0           ; Drive=0  A:드라이브
    int 0x13            ; Read!
 
    jc read             ; 에러가 나면, 다시 함.
 
    jmp 0x1000:0000     ; kernel.bin 이 위치한 곳으로 점프한다.
 
    msgBack db '.', 0x37            ; 배경색으로 사용할 데이터

    times 510-($-$$) db 0           ; 여기서부터 509번지까지 0으로 채움
    dw 0xAA55                       ; 510번지에 0xAA를, 511번지에 0x55를 넣어둔다
    


