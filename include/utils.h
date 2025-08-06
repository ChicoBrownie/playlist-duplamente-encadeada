#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void lerLinha(FILE *arq, char *buffer, int tam);
void lerString(const char *msg, char *dest, int tam);
int kbhit(void);

#endif