CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRCDIR = src
OBJDIR = obj
BUILDDIR = build

# Available targets
Q2_TARGET = $(BUILDDIR)/q2
Q4_TARGET = $(BUILDDIR)/q4
Q6_TARGET = $(BUILDDIR)/q6
Q7_TARGET = $(BUILDDIR)/q7
Q8_1_TARGET = $(BUILDDIR)/q8_1
Q8_2_TARGET = $(BUILDDIR)/q8_2

# Default target
all: q2 q4 q6 q7 q8_1 q8_2

# Individual targets
q2: $(Q2_TARGET)
q4: $(Q4_TARGET)
q6: $(Q6_TARGET)
q7: $(Q7_TARGET)
q8_1: $(Q8_1_TARGET)
q8_2: $(Q8_2_TARGET)

$(Q2_TARGET): $(OBJDIR)/q2.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q2..."
	$(CC) $(CFLAGS) -o $(Q2_TARGET) $(OBJDIR)/q2.o

$(Q4_TARGET): $(OBJDIR)/q4.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q4..."
	$(CC) $(CFLAGS) -o $(Q4_TARGET) $(OBJDIR)/q4.o

$(Q6_TARGET): $(OBJDIR)/q6.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q6..."
	$(CC) $(CFLAGS) -o $(Q6_TARGET) $(OBJDIR)/q6.o

$(Q7_TARGET): $(OBJDIR)/q7.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q7..."
	$(CC) $(CFLAGS) -o $(Q7_TARGET) $(OBJDIR)/q7.o

$(Q8_1_TARGET): $(OBJDIR)/q8_1.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q8_1..."
	$(CC) $(CFLAGS) -o $(Q8_1_TARGET) $(OBJDIR)/q8_1.o

$(Q8_2_TARGET): $(OBJDIR)/q8_2.o
	@mkdir -p $(BUILDDIR)
	@echo "Linking q8_2..."
	$(CC) $(CFLAGS) -o $(Q8_2_TARGET) $(OBJDIR)/q8_2.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) $(BUILDDIR)

.PHONY: all q2 q7 clean
