CC = clang++-devel
BINDIR = ../../bin/
OBJDIR = $(BINDIR)/objdir/game
SRV_DIR = /usr/game
$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)

# Compile Flags
CFLAGS = -w -Wall -m32 -std=c++2a -pipe -g3 -O3 -fstack-protector -static -Wno-invalid-source-encoding -Wno-unused-private-field -Wno-unused-parameter -Wno-sign-compare -Wno-unused-variable -Wno-uninitialized
#Show warnings
#CFLAGS += -Wall
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
############# Cryptopp #######################
INCDIR += -I../../../extern/
LIBDIR += -L../../../extern/cryptopp/lib
LIBS   += -lcryptopp

########### Game Exclusive ##############
############# Lua #######################
INCDIR += -I../../../extern/lua
LIBDIR += -L../../../extern/lua/lib
LIBS   += -llua

############# Minilzo ##################
INCDIR += -I../../../extern/minilzo
LIBDIR += -L../../../extern/minilzo/lib
LIBS   += -lminilzo -lmd

############# devIl ####################
INCDIR += -I../../../extern/devil
LIBDIR += -L../../../extern/devil/lib
LIBS   += -lIL -lpng -ltiff -lmng -llcms -ljpeg -ljbig -llzma

############# urielac ####################
LIBDIR += -L../../../extern/urielac
LIBS   += -lurielac

############# Source ####################



CPPFILE = FSM.cpp MarkConvert.cpp MarkImage.cpp MarkManager.cpp OXEvent.cpp ani.cpp\
		  arena.cpp banword.cpp battle.cpp blend_item.cpp buffer_manager.cpp building.cpp\
		  char.cpp char_affect.cpp char_battle.cpp char_change_empire.cpp char_horse.cpp char_item.cpp char_manager.cpp\
		  char_quickslot.cpp char_resist.cpp char_skill.cpp char_state.cpp PetSystem.cpp cmd.cpp cmd_emotion.cpp cmd_general.cpp\
		  cmd_gm.cpp cmd_oxevent.cpp config.cpp constants.cpp crc32.cpp cube.cpp db.cpp desc.cpp\
		  desc_client.cpp desc_manager.cpp desc_p2p.cpp dungeon.cpp entity.cpp\
		  entity_view.cpp event.cpp event_queue.cpp exchange.cpp file_loader.cpp fishing.cpp gm.cpp guild.cpp\
		  guild_manager.cpp guild_war.cpp horse_rider.cpp horsename_manager.cpp input.cpp input_auth.cpp input_db.cpp\
		  input_login.cpp input_main.cpp input_p2p.cpp\
		  item.cpp item_addon.cpp item_attribute.cpp item_manager.cpp item_manager_idrange.cpp locale.cpp\
		  locale_service.cpp log.cpp login_data.cpp lzo_manager.cpp marriage.cpp\
		  messenger_manager.cpp mining.cpp mob_manager.cpp motion.cpp p2p.cpp packet_info.cpp\
		  party.cpp polymorph.cpp priv_manager.cpp pvp.cpp\
		  questevent.cpp questlua.cpp questlua_affect.cpp questlua_arena.cpp questlua_building.cpp\
		  questlua_danceevent.cpp questlua_dungeon.cpp questlua_game.cpp questlua_global.cpp\
		  questlua_guild.cpp questlua_horse.cpp questlua_pet.cpp questlua_item.cpp questlua_marriage.cpp\
		  questlua_npc.cpp questlua_oxevent.cpp questlua_party.cpp questlua_pc.cpp\
		  questlua_quest.cpp questlua_target.cpp questmanager.cpp questnpc.cpp questpc.cpp\
		  refine.cpp TempleOchao.cpp questlua_TempleOchao.cpp regen.cpp safebox.cpp sectree.cpp sectree_manager.cpp shop.cpp\
		  skill.cpp start_position.cpp target.cpp text_file_loader.cpp trigger.cpp utils.cpp vector.cpp war_map.cpp\
		  wedding.cpp xmas_event.cpp map_location.cpp\
		  BlueDragon.cpp BlueDragon_Binder.cpp DragonLair.cpp questlua_dragonlair.cpp skill_power.cpp affect.cpp\
		  buff_on_attributes.cpp dragon_soul_table.cpp DragonSoul.cpp\
		  group_text_parse_tree.cpp char_dragonsoul.cpp questlua_dragonsoul.cpp\
		  shop_manager.cpp shopEx.cpp item_manager_read_tables.cpp New_PetSystem.cpp questlua_petnew.cpp questlua_mysql.cpp char_cards.cpp\
		  MeleyLair.cpp questlua_MeleyLair.cpp game_events.cpp event_manager.cpp\
		  combat_zone.cpp support_shaman.cpp questlua_supportshaman.cpp item_combination.cpp auto_event_manager.cpp\
		  nearby_scanner.cpp minigame_attendance.cpp minigame_catch_king.cpp minigame_other.cpp boss_tracking.cpp\
		  daily_boss.cpp vid.cpp char_mailbox.cpp inventory.cpp char_battle_zodiac.cpp questlua_zodiac_temple.cpp\
		  zodiac_temple.cpp abuse.cpp MountSystem.cpp cipher.cpp features.cpp questlua_utils.cpp AutoNotice.cpp auto_query.cpp main.cpp\


CPPOBJS	= $(CPPFILE:%.cpp=$(OBJDIR)/%.o)
TARGET  = $(BINDIR)/release/game
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
	rm -rf $(SRV_DIR)/share/bin/game
	cp $(TARGET) $(SRV_DIR)/share/bin/game
