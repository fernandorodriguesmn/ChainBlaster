CC = gcc

CFLAGS = -Wall $(shell pkg-config --cflags \
	allegro-5 \
	allegro_font-5 \
	allegro_ttf-5 \
	allegro_primitives-5 \
	allegro_image-5)

LIBS = $(shell pkg-config --libs \
	allegro-5 \
	allegro_font-5 \
	allegro_ttf-5 \
	allegro_primitives-5 \
	allegro_image-5) \
	-lm

TARGET = chain
SRC = chain.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
