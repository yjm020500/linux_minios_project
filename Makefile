# Compiler and Compiler Flags
CC=gcc
CFLAGS=-Wall -g -Iinclude -Ilib

# Linker flags
LDFLAGS=-lreadline -lm

# The build target executable:
TARGET1=powerOn
TARGET2=minios
# FORK_PROGRAM=kernel/20192490/print_os_name

# Source, Object files
SRCS=kernel/kernel.c \
     kernel/system.c \
	 kernel/mem_s/mem_allocate.c \
	 kernel/mem_s/dummy_memory_management.c \
	 kernel/program_handle/execute.c \
	 kernel/program_handle/terminate.c \
	 lib/print_util.c \
	 lib/structlib.c \
	  
OBJS=$(SRCS:.c=.o)

#Include directory
INCLUDE_DIR=include

PROGRAM_SRCS=$(wildcard program/*.c)
PROGRAM_EXES=$(PROGRAM_SRCS:.c=)

all: $(TARGET1) $(TARGET2) $(PROGRAM_EXES)

$(TARGET1): boot/boot.o
	$(CC) $(CFLAGS) -o $(TARGET1) boot/boot.o $(LDFLAGS)

$(TARGET2): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJS) $(LDFLAGS)

# Compile program folder .c files to executables
program/%: program/%.c
	$(CC) -Wall -Iinclude -Ilib -o $@ $<

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up:
clean:
	rm -f $(OBJS) $(TARGET) $(PROGRAM_EXES)
