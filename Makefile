# Compiler and Compiler Flags
CC=gcc
CFLAGS=-Wall -g -Iinclude

# Linker flags
LDFLAGS=-lreadline -lm

# The build target executable:
TARGET=minios
# FORK_PROGRAM=kernel/20192490/print_os_name

# Source, Object files
SRCS=kernel/kernel.c kernel/system.c kernel/mem_s/print_util.c kernel/mem_s/mem_allocate.c 
OBJS=$(SRCS:.c=.o)

#Include directory
INCLUDE_DIR=include

#$(FORK_PROGRAM)... 추후에 추가 $(OBJS)->$^
all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Make fork program... 추후에 추가
#$(FORK_CHILD_PROGRAM): kernel/20192490/print_os_name.o
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up:
clean:
	rm -f $(OBJS) $(TARGET)
