#include "../include/playlist.h"
#include "../include/musica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Inicializa a playlist circular
void inicializaPlaylist(PlaylistCircular *pl) {
    pl->inicio = NULL;
    pl->tamanho = 0;
}

// Insere música no fim da playlist circular
void inserirMusicaCircular(PlaylistCircular *pl, Musica m) {
    Musica *novo = (Musica*)malloc(sizeof(Musica));
    *novo = m;
    novo->prox = novo->ant = NULL;

    if (pl->inicio == NULL) {
        novo->prox = novo->ant = novo;
        pl->inicio = novo;
    } else {
        Musica *ultimo = pl->inicio->ant;
        novo->prox = pl->inicio;
        novo->ant = ultimo;
        ultimo->prox = novo;
        pl->inicio->ant = novo;
    }
    pl->tamanho++;
}

// Insere música no início
void inserirMusicaInicio(PlaylistCircular *pl, Musica m) {
    inserirMusicaCircular(pl, m);
    pl->inicio = pl->inicio->ant;
}

// Insere música no fim
void inserirMusicaFim(PlaylistCircular *pl, Musica m) {
    inserirMusicaCircular(pl, m);
}

// Insere música em posição específica
void inserirMusicaPosicao(PlaylistCircular *pl, Musica m, int pos) {
    if (pos <= 1) {
        inserirMusicaInicio(pl, m);
        return;
    }
    if (pos > pl->tamanho) {
        inserirMusicaFim(pl, m);
        return;
    }
    Musica *novo = (Musica*)malloc(sizeof(Musica));
    *novo = m;
    Musica *atual = pl->inicio;
    for (int i = 1; i < pos - 1; i++)
        atual = atual->prox;
    novo->prox = atual->prox;
    novo->ant = atual;
    atual->prox->ant = novo;
    atual->prox = novo;
    pl->tamanho++;
}

// Remove música pelo nome
void removerMusica(PlaylistCircular *pl, const char *nome) {
    if (!pl->inicio) {
        printf("Playlist vazia!\n");
        return;
    }
    Musica *atual = pl->inicio;
    Musica *remover = NULL;
    int encontrou = 0;

    do {
        if (strcmp(atual->nome, nome) == 0) {
            remover = atual;
            encontrou = 1;
            break;
        }
        atual = atual->prox;
    } while (atual != pl->inicio);

    if (!encontrou) {
        printf("Música não encontrada!\n");
        return;
    }

    if (remover->prox == remover && remover->ant == remover) {
        pl->inicio = NULL;
    } else {
        remover->ant->prox = remover->prox;
        remover->prox->ant = remover->ant;
        if (pl->inicio == remover)
            pl->inicio = remover->prox;
    }
    pl->tamanho--;

    liberarMusica(remover);
    free(remover);

    printf("Música removida!\n");
}

// Lista todas as músicas
void listarMusicas(PlaylistCircular *pl) {
    if (!pl->inicio) {
        printf("Nenhuma música cadastrada.\n");
        return;
    }
    printf("\n--- Lista de Músicas ---\n");
    Musica *atual = pl->inicio;
    int i = 1;
    do {
        printf("\nMúsica %d:\n", i++);
        imprimeMusica(atual);
        atual = atual->prox;
    } while (atual != pl->inicio);
}

// Salva toda a playlist no arquivo
void salvarPlaylistNoArquivo(PlaylistCircular *pl) {
    FILE *arq = fopen("musicaBD.txt", "w");
    if (!arq) return;
    if (!pl->inicio) {
        fclose(arq);
        return;
    }
    Musica *atual = pl->inicio;
    do {
        fprintf(arq, "Musica: %s\n", atual->nome);
        fprintf(arq, "Album: %s\n", atual->album);
        fprintf(arq, "Minutos: %d\n", atual->duracao.min);
        fprintf(arq, "Segundos: %d\n", atual->duracao.seg);
        for (int i = 0; i < atual->numArtista; i++)
            fprintf(arq, "Artista: %s\n", atual->artista[i]);
        for (int i = 0; i < atual->numEstilo; i++)
            fprintf(arq, "Estilo: %s\n", atual->estilo[i]);
        fprintf(arq, "FIM\n\n");
        atual = atual->prox;
    } while (atual != pl->inicio);
    fclose(arq);
}

// Recupera músicas do arquivo
void recuperaMusicas(PlaylistCircular *pl) {
    FILE *arq = fopen("musicaBD.txt", "r");
    if (!arq) return;

    char linha[256];
    Musica m;
    memset(&m, 0, sizeof(Musica));
    while (fgets(linha, sizeof(linha), arq)) {
        linha[strcspn(linha, "\n")] = '\0';

        if (strncmp(linha, "Musica: ", 8) == 0) {
            memset(&m, 0, sizeof(Musica));
            m.nome = strdup(linha + 8);
        } else if (strncmp(linha, "Album: ", 7) == 0) {
            m.album = strdup(linha + 7);
        } else if (strncmp(linha, "Minutos: ", 9) == 0) {
            m.duracao.min = atoi(linha + 9);
        } else if (strncmp(linha, "Segundos: ", 10) == 0) {
            m.duracao.seg = atoi(linha + 10);
        } else if (strncmp(linha, "Artista: ", 9) == 0) {
            m.artista = realloc(m.artista, (m.numArtista + 1) * sizeof(char*));
            m.artista[m.numArtista++] = strdup(linha + 9);
        } else if (strncmp(linha, "Estilo: ", 8) == 0) {
            m.estilo = realloc(m.estilo, (m.numEstilo + 1) * sizeof(char*));
            m.estilo[m.numEstilo++] = strdup(linha + 8);
        } else if (strcmp(linha, "FIM") == 0) {
            inserirMusicaCircular(pl, m);
            memset(&m, 0, sizeof(Musica));
        }
    }
    fclose(arq);
}

// Reprodução infinita da playlist circular
void reproduzirPlaylist(PlaylistCircular *pl) {
    extern int kbhit(void); // Definido em utils.c
    if (!pl->inicio) {
        printf("Playlist vazia!\n");
        return;
    }
    Musica *atual = pl->inicio;
    int reverso = 0;

    printf("Controles: 'p' = parar | 'k' = próxima | 'j' = anterior\n");

    while (1) {
        imprimeMusica(atual);
        int tempo = atual->duracao.min * 60 + atual->duracao.seg;
        int mudou = 0;

        for (int i = tempo; i >= 0; i--) {
            printf("\rTempo restante: %02d:%02d ", i / 60, i % 60);
            fflush(stdout);
            sleep(1);
            if (kbhit()) {
                char c = getchar();
                if (c == 'p' || c == 'P') {
                    printf("\nReprodução parada pelo usuário.\n");
                    return;
                }
                if (c == 'k' || c == 'K') {
                    atual = atual->prox;
                    reverso = 0;
                    mudou = 1;
                    break;
                }
                if (c == 'j' || c == 'J') {
                    atual = atual->ant;
                    reverso = 1;
                    mudou = 1;
                    break;
                }
            }
            if (i == 0) {
                printf("\rTempo restante: 00:00 ");
                fflush(stdout);
                sleep(1);
            }
        }
        printf("\n");
        if (!mudou) {
            atual = reverso ? atual->ant : atual->prox;
        }
    }
}