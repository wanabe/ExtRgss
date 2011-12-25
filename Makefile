INCDIRS=-I$(LOADSO_DIR)
LIBDIRS=-L$(LOADSO_DIR)

LOADSO_DIR=LoadSo
LDFLAGS=$(LIBDIRS) -shared
EXT=ExtRgss.so
CFLAGS=-Wall -DDLL_NAME="\"$(EXT)\""
OBJS=ext_rgss.o graphics.o
LIBS=d3dx9.lib -ld3d9 -lloadso
GAME=./Game.exe
HEADERS=ext_rgss.h

all: $(EXT)

pack: ExtRgss.zip

ExtRgss.zip: $(EXT) msvcrt-ruby191.dll $(LOADSO_DIR)/load_so.rb ext_rgss.rb
	cp $(LOADSO_DIR)/load_so.rb load_so.rb
	zip ExtRgss.zip $(EXT) msvcrt-ruby191.dll load_so.rb ext_rgss.rb
	rm load_so.rb

test: all msvcrt-ruby191.dll
	$(GAME)

$(EXT): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXT) $(OBJS) $(LIBS)

ext_rgss.o: $(LOADSO_DIR)/rgss.h ext_rgss.h graphics.h
graphics.o: $(LOADSO_DIR)/rgss.h ext_rgss.h graphics.h

msvcrt-ruby191.dll: $(LOADSO_DIR)/msvcrt-ruby191.dll
	cp $(LOADSO_DIR)/$@ $@
