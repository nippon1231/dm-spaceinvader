BIN = $(GDK)/bin
LIB = $(GDK)/lib

LIBMD = $(LIB)/libmd.a
LIBGCC = $(GDK)/m68k-elf/lib/gcc/m68k-elf/6.3.0/libgcc.a

CC = $(BIN)/m68k-elf-gcc
AS = $(BIN)/m68k-elf-as
AR = $(BIN)/m68k-elf-ar
LD = $(BIN)/m68k-elf-ld
NM = $(BIN)/m68k-elf-nm
OBJCPY = $(BIN)/m68k-elf-objcopy
RESCOMP = $(BIN)/rescomp

OPTION = -m68000 -Wall -Wextra -O3 -fomit-frame-pointer -fno-builtin
INCS = -I. -I$(GDK)/inc -I$(GDK)/res -Iinc -Ires
FLAGS = $(OPTION) -c $(INCS)

all: out.bin

out.bin: out.elf
	$(OBJCPY) -O binary $< $@

out.elf: src/main.o src/game.o src/enemies.o src/player.o src/bullets.o res/resources.o
	$(CC) $(OPTION) -T $(GDK)/md.ld -nostdlib src/main.o src/game.o src/enemies.o src/player.o src/bullets.o res/resources.o $(LIBMD) $(LIBGCC) -o $@

src/%.o: src/%.c res/resources.h
	$(CC) $(FLAGS) $< -o $@

res/resources.h: res/resources.res
	$(RESCOMP) res/resources.res res/resources.h

res/resources.o: res/resources.res res/resources.h
	$(RESCOMP) res/resources.res res/resources.s
	$(AS) -m68000 res/resources.s -o res/resources.o

clean:
	rm -f src/*.o res/*.o res/resources.s res/resources.h out.bin out.elf

