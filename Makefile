CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror -std=c99 -g
LDFLAGS =

TARGET = lab3c
SRCDIR = src
HEADERS_DIR = $(SRCDIR)/include

OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(BINDIR)/$(TARGET) 

$(BINDIR)/$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

valgrind: $(BINDIR)/$(TARGET)
	valgrind --leak-check=full $^

clean:
	rm -rf $(OBJDIR) $(BINDIR)
