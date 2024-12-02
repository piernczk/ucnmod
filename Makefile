SRCDIR ?= ./src
INCDIR ?= ./include
OBJDIR ?= ./bin
BINDIR ?= ./bin
LD = $(CC)
CFLAGS += -Wall -I$(INCDIR)

INCLUDES := $(wildcard $(INCDIR)/*.h)

ifeq ($(OS),Windows_NT)
	TARGET ?= ucnmod.exe
	SOURCES = $(SRCDIR)/win.c
else
	TARGET ?= ucnmod
	SOURCES = $(SRCDIR)/linux.c
endif

SOURCES += $(SRCDIR)/main.c $(SRCDIR)/command.c $(SRCDIR)/commands.c
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

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
