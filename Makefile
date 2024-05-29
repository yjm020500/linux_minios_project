# Compiler and Compiler Flags
CC=gcc
CFLAGS=-Wall -g -Iinclude

# Linker flags
LDFLAGS=-lreadline

# The build target executable:
TARGET=minios
FORK_CHILD_PROGRAM=kernel/20192490/print_os_name

# Source, Object files
SRCS=kernel/kernel.c kernel/system.c kernel/20192490/mini_fork.c kernel/20192490/pipe.c kernel/20192490/monte_carlo.c
OBJS=$(SRCS:.c=.o)

#Include directory
INCLUDE_DIR=include

all: $(TARGET) $(FORK_CHILD_PROGRAM) $(SHM_CONSUMER_PROGRAM)

$(TARGET): $(OBJS) kernel/20192490/shm_com.o
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LDFLAGS) -lrt -lm

# Make fork child program 
$(FORK_CHILD_PROGRAM): kernel/20192490/print_os_name.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up:
clean:
	rm -f $(OBJS) $(TARGET)
