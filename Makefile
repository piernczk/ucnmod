SRCDIR ?= ./src
INCDIR ?= ./include
OBJDIR ?= ./bin
BINDIR ?= ./bin
LD = $(CC)
CFLAGS += -Wall -I$(INCDIR)

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ifeq ($(OS),Windows_NT)
	TARGET ?= ucnmod.exe
else
	TARGET ?= ucnmod
endif

all: $(BINDIR)/$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(LD) -o $@ $(LFLAGS) $(OBJECTS)

.PHONY: all clean
.SUFFIXES:
.SUFFIXES: .c .o .exe
