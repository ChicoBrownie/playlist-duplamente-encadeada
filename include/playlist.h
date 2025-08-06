#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "musica.h"

typedef struct {
    Musica *inicio;
    int tamanho;
} PlaylistCircular;

void inicializaPlaylist(PlaylistCircular *pl);
void inserirMusicaInicio(PlaylistCircular *pl, Musica m);
void inserirMusicaFim(PlaylistCircular *pl, Musica m);
void inserirMusicaPosicao(PlaylistCircular *pl, Musica m, int pos);
void inserirMusicaCircular(PlaylistCircular *pl, Musica m);
void removerMusica(PlaylistCircular *pl, const char *nome);
void listarMusicas(PlaylistCircular *pl);
void salvarPlaylistNoArquivo(PlaylistCircular *pl);
void recuperaMusicas(PlaylistCircular *pl);
void reproduzirPlaylist(PlaylistCircular *pl);

#endif