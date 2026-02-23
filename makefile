# Makefile for compiling all .c files in src/ and main file (assuming main.c is in src/)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = test/taskMain.c

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/program

# Source files: all .c files in SRCDIR
SOURCES = $(wildcard $(SRCDIR)/*.c)	
# Object files: replace .c with .o and move to OBJDIR
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Compile .c files into .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Include dependency files
DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)

# Generate dependency files
$(OBJDIR)/%.d: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean