#include "../include/musica.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria uma música a partir da entrada do usuário
Musica criarMusicaUsuario() {
    Musica m;
    char buffer[256];
    m.nome = m.album = NULL;

    lerString("Nome da música: ", buffer, sizeof(buffer));
    m.nome = strdup(buffer);

    lerString("Álbum: ", buffer, sizeof(buffer));
    m.album = strdup(buffer);

    // Artistas
    m.numArtista = 0;
    m.artista = NULL;
    char resp[8];
    do {
        printf("Artista: \n");
        for (int i = 0; i < m.numArtista; i++) {
            printf("  %s\n", m.artista[i]);
        }
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        m.artista = realloc(m.artista, (m.numArtista + 1) * sizeof(char*));
        m.artista[m.numArtista++] = strdup(buffer);
        lerString("Adicionar mais um artista? (s/n): ", resp, sizeof(resp));
    } while (resp[0] == 's' || resp[0] == 'S');

    // Estilos
    m.numEstilo = 0;
    m.estilo = NULL;
    do {
        printf("Estilo %d: ", m.numEstilo + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        m.estilo = realloc(m.estilo, (m.numEstilo + 1) * sizeof(char*));
        m.estilo[m.numEstilo++] = strdup(buffer);
        lerString("Adicionar mais um estilo? (s/n): ", resp, sizeof(resp));
    } while (resp[0] == 's' || resp[0] == 'S');

    printf("Duração (minutos): ");
    scanf("%d", &m.duracao.min); getchar();
    printf("Duração (segundos): ");
    scanf("%d", &m.duracao.seg); getchar();

    m.ant = m.prox = NULL;
    return m;
}

// Imprime os dados de uma música
void imprimeMusica(Musica *m) {
    printf("\n--- Tocando ---\n");
    printf("Nome: %s\n", m->nome);
    printf("Álbum: %s\n", m->album);
    printf("Duração: %d:%02d\n", m->duracao.min, m->duracao.seg);
    printf("Artistas: ");
    for (int i = 0; i < m->numArtista; i++) {
        printf("%s", m->artista[i]);
        if (i < m->numArtista - 1) printf(", ");
    }
    printf("\nEstilos: ");
    for (int i = 0; i < m->numEstilo; i++) {
        printf("%s", m->estilo[i]);
        if (i < m->numEstilo - 1) printf(", ");
    }
    printf("\n");
}

// Salva uma música no arquivo
void salvarMusicaNoArquivo(Musica *m) {
    FILE *arq = fopen("musicaBD.txt", "a");
    if (!arq) return;
    fprintf(arq, "Musica: %s\n", m->nome);
    fprintf(arq, "Album: %s\n", m->album);
    fprintf(arq, "Minutos: %d\n", m->duracao.min);
    fprintf(arq, "Segundos: %d\n", m->duracao.seg);
    for (int i = 0; i < m->numArtista; i++)
        fprintf(arq, "Artista: %s\n", m->artista[i]);
    for (int i = 0; i < m->numEstilo; i++)
        fprintf(arq, "Estilo: %s\n", m->estilo[i]);
    fprintf(arq, "FIM\n");
    fclose(arq);
}

// Libera memória de uma música
void liberarMusica(Musica *m) {
    if (!m) return;
    free(m->nome);
    free(m->album);
    for (int i = 0; i < m->numArtista; i++) free(m->artista[i]);
    free(m->artista);
    for (int i = 0; i < m->numEstilo; i++) free(m->estilo[i]);
    free(m->estilo);
}