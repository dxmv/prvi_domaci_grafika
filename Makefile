CC = gcc
IN = main.c src/main_state.c src/player.c src/stars.c src/laser.c src/planet.c src/enemies.c src/collision.c src/heart.c src/item.c src/glad/glad.c
OUT = main.out
CFLAGS = -Wall -DGLFW_INCLUDE_NONE
IFLAGS = -I. -I./include

UNAME_S := $(shell uname -s)

# Platform specific linker flags
ifeq ($(UNAME_S),Darwin)
    LFLAGS = -L$(shell brew --prefix)/lib -lglfw -lm \
             -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
    IFLAGS += -I$(shell brew --prefix)/include
else
    LFLAGS = -lglfw -ldl -lm
endif

.SILENT all: clean build run

clean:
	rm -f $(OUT)

build: $(IN) include/main_state.h include/stb_image.h 
	$(CC) $(IN) -o $(OUT) $(CFLAGS) $(LFLAGS) $(IFLAGS)

run: $(OUT)
	./$(OUT)
