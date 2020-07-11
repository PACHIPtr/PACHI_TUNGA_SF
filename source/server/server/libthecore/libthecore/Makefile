BIN = ../lib/libthecore.a
CXX = clang++-devel
OBJDIR = ../../bin/objdir/libthecore
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)
# Compile flags
CFLAGS = -m32 -Wall -std=c++2a -pipe -mtune=i686 -fstack-protector -static -Wno-invalid-source-encoding
############# Source ####################
CPPFILE=buffer.cpp fdwatch.cpp gost.cpp hangul.cpp heart.cpp kstbl.cpp log.cpp main.cpp memcpy.cpp poly.cpp signal.cpp socket.cpp tea.cpp utils.cpp

CPPOBJS = $(CPPFILE:%.cpp=$(OBJDIR)/%.o)
TARGET = ../lib/libthecore.a

# Compile phase for .cpp
$(OBJDIR)/%.o: %.cpp
	@echo -e "\033[0;36m [Mythsia] Compiling:\033[0m" $<
	@$(CXX) $(INCDIR) $(CFLAGS) -c $^ -o $@

$(TARGET): $(CPPOBJS)
	@ar cru $(TARGET) $^
	@ranlib $(TARGET)

default: $(TARGET)
all: $(TARGET)
clean:
	@rm -f $(OBJDIR)/*
	@rm -f $(TARGET)