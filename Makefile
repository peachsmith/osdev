# target should be the location of the cross compiler
TARGET=

# the build tools are assumed to be the GNU gcc toolchain
AS=$(TARGET)-as
CC=$(TARGET)-gcc

C_FLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

LNK_FLAGS=-ffreestanding -O2 -nostdlib

OBJ=kernel.o
BIN=myos.bin

all:


build:
	$(AS) boot.s -o boot.o
	$(CC) $(C_FLAGS) kernel.c -o kernel.o
	$(CC) -T linker.ld -o $(BIN) $(LNK_FLAGS) $(OBJ) -lgcc


clean:
	rm *.o *.bin

