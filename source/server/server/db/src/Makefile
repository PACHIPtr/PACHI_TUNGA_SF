CC = clang++-devel

BINDIR = ../../bin/
OBJDIR = $(BINDIR)/objdir/db
SRV_DIR = /usr/game
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)
# Compile flags
CFLAGS = -w -m32 -Wextra -std=c++2a -pipe -mtune=i686 -fstack-protector -static -O3 -g3 -Wno-invalid-source-encoding -Wno-unused-private-field -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-unused-variable -Wno-uninitialized

############ Core Libs ###################
############# libgame ####################
INCDIR += -I../../libgame/libgame
LIBDIR += -L../../libgame/lib
LIBS   += -lgame
############# libsql #####################
INCDIR += -I../../libsql/libsql
LIBDIR += -L../../libsql/lib
LIBS   += -lsql
############# Libthecore #################
INCDIR += -I../../libthecore/libthecore
LIBDIR += -L../../libthecore/lib
LIBS   += -lthecore

############# Externals ##################
############# MySQL ######################
INCDIR += -I../../../extern/mysql
LIBDIR += -L../../../extern/mysql/lib
LIBS   += -lmysqlclient -lz -pthread
############# Boost #####################
INCDIR += -I../../../extern/boost/

############# Source ####################
CPPFILE =	Config.cpp NetBase.cpp Peer.cpp PeerBase.cpp Main.cpp DBManager.cpp \
		Cache.cpp LoginData.cpp ClientManager.cpp ClientManagerPlayer.cpp ClientManagerLogin.cpp \
		ClientManagerBoot.cpp ClientManagerParty.cpp ClientManagerGuild.cpp GuildManager.cpp \
		PrivManager.cpp MoneyLog.cpp ItemAwardManager.cpp ClientManagerEventFlag.cpp Marriage.cpp \
		ItemIDRangeManager.cpp ClientManagerHorseName.cpp \
		ProtoReader.cpp CsvReader.cpp\

#########################################
CPPOBJS = $(CPPFILE:%.cpp=$(OBJDIR)/%.o)

TARGET = $(BINDIR)/release/db
default: $(TARGET)

# Compile phase for .cpp
$(OBJDIR)/%.o: %.cpp
	@echo compile $<
	@$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

# build finalize phase
$(TARGET): $(CPPOBJS)
	@$(CC) $(CFLAGS) $(LIBDIR) $(CPPOBJS) $(LIBS) -o $(TARGET)
clean:
	@find . -type f | xargs -n 5 touch
	@rm -f $(OBJDIR)/*
	@rm -f $(TARGET)
install:
	rm -rf $(SRV_DIR)/share/bin/db
	cp $(TARGET) $(SRV_DIR)/share/bin/db
