#ifndef MUSICA_H
#define MUSICA_H

typedef struct {
    int min;
    int seg;
} Duracao;

typedef struct Musica {
    char *nome;
    char *album;
    Duracao duracao;
    char **artista;
    int numArtista;
    char **estilo;
    int numEstilo;
    struct Musica *ant, *prox;
} Musica;

Musica criarMusicaUsuario();
void imprimeMusica(Musica *m);
void salvarMusicaNoArquivo(Musica *m);
void liberarMusica(Musica *m);

#endif