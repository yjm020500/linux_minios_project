nasm -f bin -o boot/boot.bin boot/boot.asm
nasm -f bin -o kernel/kernel.bin kernel/kernel.asm

copy "boot\boot.bin" + "kernel\kernel.bin" /b "kernel.img"