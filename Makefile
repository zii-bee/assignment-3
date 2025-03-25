CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./include
LDFLAGS = -pthread

# Executable name
TARGET = statistical_analysis

# Source files
SRCS = src/main.c src/array_utils.c src/thread_analysis.c src/single_thread_analysis.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f src/*.o $(TARGET)

# Phony targets
.PHONY: all clean