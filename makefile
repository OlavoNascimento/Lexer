# Nome do executável
EXE=lexer

# Flags de compilação
CFLAGS = -fstack-protector-all -std=c99 -W -Wall

# Recebe todos os arquivos .c
SOURCES = $(wildcard src/*.c)
# Substitui a extensão dos arquivos .c para .o
OBJECTS = $(SOURCES:.c=.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	@echo "Criando executável..."
	gcc -o $@ $(OBJECTS)

# Específica que cada arquivo .o deve ser gerado a partir de seu correspondente .c
%.o: %.c
# $@ é o alvo atual (arquivo .o)
# $^ representa a dependência atual (arquivo .c)
	@echo "Compilando $^..."
	gcc $(CFLAGS) -I . -c -o $@ $^

clean:
	rm -f src/*.o
