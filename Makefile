ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: chain.exe

chain.exe: chain.o
	gcc -o chain.exe chain.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

chain.o: chain.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c chain.c		

	
	
clean:
	del chain.o 
	del chain.exe
