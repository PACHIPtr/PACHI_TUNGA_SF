BIN = ../lib/libgame.a
CXX = clang++-devel
OBJDIR = ../../bin/objdir/libgame
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)
# Compile flags
CFLAGS = -m32 -Wall -std=c++2a -pipe -mtune=i686 -fstack-protector -static -Wno-invalid-source-encoding
############# Source ####################
CPPFILE = attribute.cpp grid.cpp targa.cpp

CPPOBJS = $(CPPFILE:%.cpp=$(OBJDIR)/%.o)
TARGET = ../lib/libgame.a

# Compile phase for .cpp
$(OBJDIR)/%.o: %.cpp
	@echo compile $<
	@$(CXX) $(CFLAGS) -c $^ -o $@

$(TARGET): $(CPPOBJS)
	@ar cru $(TARGET) $^
	@ranlib $(TARGET)

default: $(TARGET)
all: $(TARGET)
clean:
	@rm -f $(OBJDIR)/*
	@rm -f $(TARGET)
