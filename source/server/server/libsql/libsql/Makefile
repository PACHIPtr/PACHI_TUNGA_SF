BIN = ../lib/libsql.a
CXX = clang++-devel
OBJDIR = ../../bin/objdir/libsql
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)
# Compile flags
CFLAGS = -m32 -Wall -std=c++2a -pipe -mtune=i686 -fstack-protector -static -Wno-invalid-source-encoding

############# Externals ##################
############# MySQL ######################
INCDIR += -I../../../extern/mysql

############# Source ####################
CPPFILE=AsyncSQL.cpp Semaphore.cpp Statement.cpp Tellwait.cpp

CPPOBJS = $(CPPFILE:%.cpp=$(OBJDIR)/%.o)
TARGET = ../lib/libsql.a


# Compile phase for .cpp
$(OBJDIR)/%.o: %.cpp
	@echo compile $<
	@$(CXX) $(INCDIR) $(CFLAGS) -c $^ -o $@

$(TARGET): $(CPPOBJS)
	@ar cru $(TARGET) $^
	@ranlib $(TARGET)

default: $(TARGET)
all: $(TARGET)
clean:
	@rm -f $(OBJDIR)/*
	@rm -f $(TARGET)