CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRCDIR = src
OBJDIR = obj
BUILDDIR = build

TARGETS = q3 q4

OBJFILES = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(TARGETS)))
BINARIES = $(addprefix $(BUILDDIR)/, $(TARGETS))

all: $(BINARIES)

$(BUILDDIR)/%: $(OBJDIR)/%.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking $(@F)..."
	$(CC) $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETS): %: $(BUILDDIR)/%

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) $(BUILDDIR)

.PHONY: all clean $(TARGETS)
