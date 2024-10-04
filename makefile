#Aditya Srichandan 1st October 2024
# Compiler and options
CC := g++
RM := rm -f
DEBUG := -g
CFLAGS := -Wall -Wno-unused-variable -Wno-sign-compare #suppress warnings
LDFLAGS := -lX11

# Directories
INCDIR := include
LIBDIR := lib
SRCDIR := src
BINDIR := executable

# Object files and target
OBJFILES := $(SRCDIR)/main.o $(LIBDIR)/MazeRoutingFPGA.o $(LIBDIR)/FPGAVisualizeEZGL.o $(LIBDIR)/graphics.o
TARGET := $(BINDIR)/A1Router

# Build target
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(DEBUG) -o $(TARGET) $(OBJFILES) $(LDFLAGS)  # Put LDFLAGS after OBJFILES

# Compile object files
$(SRCDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/MazeRoutingFPGA.h $(INCDIR)/FPGAVisualizeEZGL.h $(INCDIR)/graphics.h
	$(CC) $(CFLAGS) $(DEBUG) -I$(INCDIR) -c $(SRCDIR)/main.cpp -o $(SRCDIR)/main.o

$(LIBDIR)/MazeRoutingFPGA.o: $(LIBDIR)/MazeRoutingFPGA.cpp $(INCDIR)/MazeRoutingFPGA.h
	$(CC) $(CFLAGS) $(DEBUG) -I$(INCDIR) -c $(LIBDIR)/MazeRoutingFPGA.cpp -o $(LIBDIR)/MazeRoutingFPGA.o

$(LIBDIR)/FPGAVisualizeEZGL.o: $(LIBDIR)/FPGAVisualizeEZGL.cpp $(INCDIR)/FPGAVisualizeEZGL.h
	$(CC) $(CFLAGS) $(DEBUG) -I$(INCDIR) -c $(LIBDIR)/FPGAVisualizeEZGL.cpp -o $(LIBDIR)/FPGAVisualizeEZGL.o

$(LIBDIR)/graphics.o: $(LIBDIR)/graphics.cpp $(INCDIR)/graphics.h
	$(CC) $(CFLAGS) $(DEBUG) -I$(INCDIR) -c $(LIBDIR)/graphics.cpp -o $(LIBDIR)/graphics.o

# Clean command
clean:
	$(RM) $(OBJFILES) $(TARGET) 
