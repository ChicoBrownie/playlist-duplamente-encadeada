#include <stdio.h>
#include "include/playlist.h"
#include "include/musica.h"
#include "include/utils.h"

int main() {
    PlaylistCircular playlist;
    inicializaPlaylist(&playlist);
    recuperaMusicas(&playlist);

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Adicionar nova música\n");
        printf("2. Reproduzir playlist\n");
        printf("3. Remover música\n");
        printf("4. Listar músicas\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao); getchar();

        if (opcao == 1) {
            Musica m = criarMusicaUsuario();
            printf("Onde deseja adicionar?\n");
            printf("1. Início\n2. Meio\n3. Fim\n4. Posição específica\n5. Descartar\nEscolha: ");
            int escolha, pos;
            scanf("%d", &escolha); getchar();
            switch (escolha) {
                case 1:
                    inserirMusicaInicio(&playlist, m);
                    salvarPlaylistNoArquivo(&playlist);
                    printf("Adicionada no início!\n");
                    break;
                case 2:
                    inserirMusicaPosicao(&playlist, m, (playlist.tamanho/2)+1);
                    salvarPlaylistNoArquivo(&playlist);
                    printf("Adicionada no meio!\n");
                    break;
                case 3:
                    inserirMusicaFim(&playlist, m);
                    salvarPlaylistNoArquivo(&playlist);
                    printf("Adicionada no fim!\n");
                    break;
                case 4:
                    printf("Digite a posição (1 a %d): ", playlist.tamanho+1);
                    scanf("%d", &pos); getchar();
                    inserirMusicaPosicao(&playlist, m, pos);
                    salvarPlaylistNoArquivo(&playlist);
                    printf("Adicionada na posição %d!\n", pos);
                    break;
                default:
                    printf("Música descartada.\n");
                    liberarMusica(&m);
            }
        } else if (opcao == 2) {
            if (playlist.inicio == NULL) {
                printf("Playlist vazia! Adicione músicas primeiro.\n");
            } else {
                reproduzirPlaylist(&playlist);
            }
        } else if (opcao == 3) {
            char nome[256];
            lerString("Nome da música para remover: ", nome, sizeof(nome));
            removerMusica(&playlist, nome);
            salvarPlaylistNoArquivo(&playlist);
        } else if (opcao == 4) {
            listarMusicas(&playlist);
        }
    } while (opcao != 0);
    printf("Programa encerrado.\n");
    return 0;
}