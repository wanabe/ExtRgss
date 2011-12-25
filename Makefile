INCDIRS=-I$(LOADSO_DIR)
LIBDIRS=-L$(LOADSO_DIR)

LOADSO_DIR=LoadSo
LDFLAGS=$(LIBDIRS) -shared
EXT=ExtRgss.so
CFLAGS=$(INCDIRS) -Wall -DDLL_NAME="\"$(EXT)\""
OBJS=ext_rgss.o graphics.o
LIBS=d3dx9.lib -ld3d9 -lloadso
GAME=./Game.exe
HEADERS=ext_rgss.h

all: $(EXT)

pack: ExtRgss.zip

ExtRgss.zip: $(EXT) msvcrt-ruby191.dll $(LOADSO_DIR)/README.txt $(LOADSO_DIR)/load_so.rb $(LOADSO_DIR)/COPYING.txt $(LOADSO_DIR)/COPYING.ja.txt $(LOADSO_DIR)/BSDL
	cp $(LOADSO_DIR)/README.txt README-LoadSo.txt
	cp $(LOADSO_DIR)/load_so.rb $(LOADSO_DIR)/COPYING.txt $(LOADSO_DIR)/COPYING.ja.txt $(LOADSO_DIR)/BSDL .
	zip ExtRgss.zip $(EXT) msvcrt-ruby191.dll ext_rgss.rb load_so.rb README*.txt load_so.rb COPYING* BSDL
	rm README-LoadSo.txt load_so.rb COPYING* BSDL

test: all msvcrt-ruby191.dll
	$(GAME)

$(EXT): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXT) $(OBJS) $(LIBS)

ext_rgss.o: $(LOADSO_DIR)/rgss.h ext_rgss.h graphics.h
graphics.o: $(LOADSO_DIR)/rgss.h ext_rgss.h graphics.h

msvcrt-ruby191.dll: $(LOADSO_DIR)/msvcrt-ruby191.dll
	cp $(LOADSO_DIR)/$@ $@
