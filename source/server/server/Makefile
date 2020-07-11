libgame: .
	$(MAKE) -C $@/libgame clean
	$(MAKE) -C $@/libgame

libsql: .
	$(MAKE) -C $@/libsql clean
	$(MAKE) -C $@/libsql

libthecore: .
	$(MAKE) -C $@/libthecore clean
	$(MAKE) -C $@/libthecore

game: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

db: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

libs: 
	@echo "--------------------------------------"
	@echo "Library Build Start"
	@echo "--------------------------------------"

	$(MAKE) -C libgame/libgame clean
	$(MAKE) -C libgame/libgame

	$(MAKE) -C libsql/libsql clean
	$(MAKE) -C libsql/libsql

	$(MAKE) -C libthecore/libthecore clean
	$(MAKE) -C libthecore/libthecore

	@echo "--------------------------------------"
	@echo "Library Build End"
	@echo "--------------------------------------"
all: 
	@echo "--------------------------------------"
	@echo "Full Build Start"
	@echo "--------------------------------------"

	$(MAKE) -C libgame/libgame clean
	$(MAKE) -C libgame/libgame

	$(MAKE) -C libsql/libsql clean
	$(MAKE) -C libsql/libsql

	$(MAKE) -C libthecore/libthecore clean
	$(MAKE) -C libthecore/libthecore

	$(MAKE) -C game/src clean
	$(MAKE) -C game/src

	$(MAKE) -C db/src clean
	$(MAKE) -C db/src
	@echo "--------------------------------------"
	@echo "Full Build End"
	@echo "--------------------------------------"

clean:
	@echo "--------------------------------------"
	@echo "Clean Build Start"
	@echo "--------------------------------------"


	$(MAKE) -C libgame/libgame clean
	$(MAKE) -C libsql/libsql clean
	$(MAKE) -C libthecore/libthecore clean
	$(MAKE) -C game/src clean
	$(MAKE) -C db/src clean
	
	@echo "--------------------------------------"
	@echo "Clean Build End"
	@echo "--------------------------------------"
