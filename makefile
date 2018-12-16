#NOURRIT GABRIEL
#NOEL JULES


CC=gcc
CPPFLAGS= -I./inc/
EXEC= bin/dictionnaire
#on recupère nos .c et .h
SRC=$(wildcard src/*.c)
HDR=$(wildcard inc/*.h)
#generation des .o
BIN=$(SRC:.c=.o)

ifeq ($(NBNOEUD),)
CFLAGS= -Wall
else
ifeq ($(DEBUG),0)
CFLAGS= -Wall -DNBNOEUD=${NBNOEUD}
else
CFLAGS= -Wall -DNBNOEUD=${NBNOEUD} -DDEBUG
endif
endif

all: $(EXEC)

#on utilise la regle implicite .c.o
$(EXEC): $(BIN)
	$(CC) $(BIN) -o $(EXEC)
	mv src/*.o bin/

#on fait dépendre chaque .o de chaque point .h
#si un .h est modifié tout est recompilé !
$(OBJ)::$(HDR)


#si il y a un fichier de meme nom dans /src la regle n'est pas executé
#.PHONY reconstruit les dépendances
.PHONY: clean run tar

#lancer le programme
run:
	bin/Lanceur.sh
	bin/$(EXEC)

clean:
	@(rm -rf bin/*.o $(EXEC)  bin/*~ src/*~ inc/*~ *~ 2>/dev/null || true)

tar:
	tar -zcvf NOURRIT_Gabriel_NOEL_Jules.tar.gz ./src ./inc ./bin makefile README
