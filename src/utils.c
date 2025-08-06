#include "../include/utils.h"
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// Lê uma linha do arquivo e remove o '\n'
void lerLinha(FILE *arq, char *buffer, int tam) {
    if (fgets(buffer, tam, arq)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Lê uma string do usuário
void lerString(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

// Verifica se uma tecla foi pressionada (não bloqueante)
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}