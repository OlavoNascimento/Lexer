#include <stdbool.h>
#include <stdio.h>

#define LEXEME_MAX_SIZE 101

// Tipos de caracteres que o lexer reconhece
typedef enum {
    LETTER,
    DIGIT,
    // Operador matemático ou parêntesis
    SYMBOL
} CharacterTypes;

// Tokens retornados pelo lexer
typedef enum {
    // Número
    INT_LIT,
    // Identificador
    IDENTIFIER,
    // Declaração de variável
    ASSIGN_OP,
    // Soma
    ADD_OP,
    // Subtração
    SUB_OP,
    // Multiplicação
    MULT_OP,
    // Divisão
    DIV_OP,
    // Parênteses esquerdo
    LEFT_PAREN,
    // Parênteses direito
    RIGHT_PAREN
} Tokens;

// Retorna verdadeiro caso um caractere seja uma letra.
bool isAlpha(char character) {
    if (character >= 'A' && character <= 'Z')
        return true;
    if (character >= 'a' && character <= 'z')
        return true;
    return false;
}

// Retorna verdadeiro caso um caractere seja um dígito.
bool isDigit(char character) {
    return character >= '0' && character <= '9';
}

// Adiciona o caractere passado a função ao vetor lexeme
void addChar(char *lexeme, int *lexeme_size, char character) {
    if (*(lexeme_size) < LEXEME_MAX_SIZE - 1) {
        lexeme[*(lexeme_size)] = character;
        (*lexeme_size)++;
        lexeme[*(lexeme_size)] = '\0';
    } else {
        printf("Erro - lexema é muito grande\n");
    }
}

// Retorna o tipo a que um caractere pertence.
CharacterTypes getCharType(char character) {
    CharacterTypes character_type;
    if (character != EOF) {
        if (isAlpha(character)) {
            character_type = LETTER;
        } else if (isDigit(character)) {
            character_type = DIGIT;
        } else {
            character_type = SYMBOL;
        }
    } else {
        character_type = EOF;
    }
    return character_type;
}

// Recebe o próximo caractere do arquivo de entrada até encontrar um caractere que não é espaço ou nova linha.
char getNonBlank(FILE *input_file) {
    char character = getc(input_file);
    while (character == ' ' || character == '\n') {
        character = getc(input_file);
    }
    return character;
}

// Define o token para um caractere que não letra ou dígito.
Tokens lookup(char *lexeme, int *lexeme_size, char character) {
    Tokens token = -1;
    switch (character) {
        case '(':
            addChar(lexeme, lexeme_size, character);
            token = LEFT_PAREN;
            break;
        case ')':
            addChar(lexeme, lexeme_size, character);
            token = RIGHT_PAREN;
            break;
        case '+':
            addChar(lexeme, lexeme_size, character);
            token = ADD_OP;
            break;
        case '-':
            addChar(lexeme, lexeme_size, character);
            token = SUB_OP;
            break;
        case '*':
            addChar(lexeme, lexeme_size, character);
            token = MULT_OP;
            break;
        case '/':
            addChar(lexeme, lexeme_size, character);
            token = DIV_OP;
            break;
    }
    return token;
}

// Analisador léxico para expressões aritméticas simples
Tokens lex(FILE *input_file) {
    // Recebe o primeiro caractere que não é um espaço ou nova linha
    char next_character = getNonBlank(input_file);
    // Recebe o tipo do caractere atual
    int character_type = getCharType(next_character);

    // String contendo o lexema construído até o momento
    char lexeme[LEXEME_MAX_SIZE];
    // Tamanho atual do lexema
    int lexeme_size = 0;

    // Token gerado pelo programa
    int next_token;
    switch (character_type) {
        // Reconhecer identificadores
        case LETTER:
            addChar(lexeme, &lexeme_size, next_character);
            next_character = getc(input_file);
            // Lê o arquivo de entrada até encontrarmos um caractere que não é uma letra ou dígito
            while (character_type == LETTER || character_type == DIGIT) {
                addChar(lexeme, &lexeme_size, next_character);
                next_character = getc(input_file);
                character_type = getCharType(next_character);
            }
            // O caractere lido não é uma letra ou digito, retornamos ele ao arquivo de entrada já que não pertence ao
            // lexema atual
            ungetc(next_character, input_file);
            next_token = IDENTIFIER;
            break;
        // Reconhecer literais inteiros
        case DIGIT:
            addChar(lexeme, &lexeme_size, next_character);
            next_character = getc(input_file);
            // Lê o arquivo de entrada ate encontrarmos um caractere que não é um dígito
            while (character_type == DIGIT) {
                addChar(lexeme, &lexeme_size, next_character);
                next_character = getc(input_file);
                character_type = getCharType(next_character);
            }
            // O caractere lido não é um digito, retornamos ele ao arquivo de entrada já que não pertence ao lexema
            // atual
            ungetc(next_character, input_file);
            next_token = INT_LIT;
            break;
        // Parênteses e operadores
        case SYMBOL:
            next_token = lookup(lexeme, &lexeme_size, next_character);
            // Caractere encontrado não é um lexema reconhecido pelo programa
            if (next_token == -1) {
                printf("ERRO: Caractere %c não é um lexema válido!\n", next_character);
                return EOF;
            }
            break;
        // Fim do arquivo
        case EOF:
            next_token = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;
    }
    printf("Próximo lexema é %s, próximo token é %d\n", lexeme, next_token);
    return next_token;
}

int main() {
    // Arquivo de entrada
    FILE *input_file = fopen("expressao.txt", "r");
    if (input_file != NULL) {
        int next_token;
        do {
            next_token = lex(input_file);
        } while (next_token != EOF);
    } else {
        printf("Não foi possível abrir o arquivo expressao.txt\n");
    }
    return 0;
}
