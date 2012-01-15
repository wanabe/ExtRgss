INCDIRS=-I$(LOADSO_DIR)
LIBDIRS=-L$(LOADSO_DIR)

LOADSO_DIR=LoadSo
LDFLAGS=$(LIBDIRS) -shared
EXT=ExtRgss.so
CFLAGS=$(INCDIRS) -Wall -DDLL_NAME="\"$(EXT)\""
OBJS=ext_rgss.o ext_rgss.def graphics.o bitmap.o sprite.o rect.o window.o
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

ext_rgss.h: $(LOADSO_DIR)/rgss.h
	touch ext_rgss.h

ext_rgss.o: ext_rgss.h graphics.h bitmap.h rect.h sprite.h window.h
graphics.o: ext_rgss.h graphics.h bitmap.h sprite.h window.h
bitmap.o  : ext_rgss.h graphics.h bitmap.h
rect.o    : ext_rgss.h graphics.h sprite.h
sprite.o  : ext_rgss.h graphics.h rect.h sprite.h
window.o  : ext_rgss.h graphics.h bitmap.h rect.h window.h

msvcrt-ruby191.dll: $(LOADSO_DIR)/msvcrt-ruby191.dll
	cp $(LOADSO_DIR)/$@ $@
