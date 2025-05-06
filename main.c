/*
 *	---> CODIGO DESENVOLVIDO E TESTADO APENAS NO LINUX (6.12.24-0-lts, x86_64, musl) <---
 *	Se acontecer algum bug no Windows, por favor, tente rodar no Linux.
 *	
 * 	Para compilar o codigo usando MAKE, apenas rode:
 * 		- make
 * 		- make run
 * 	Usando gcc:
 * 		- gcc ./main.c -O3 -o jogo
 * 		- ./jogo
 *
 * 	Github: https://github.com/henriquemdimer/trabalho-grau-a
 * 	clang-format on
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Uma tentativa de fazer a função sleep crossplataforma usando macros
// Como estou desenvolvendo e testando no Linux (não tenho Windows instalado)
// Não sei se irá funcionar corretamente
void csleep(int seconds)
{
#ifdef _WIN32
        Sleep(seconds * 1000);
#else
        sleep(seconds);
#endif
}

struct Player
{
        int points;
        int is_bot;
        int id;
        int idx;
};

struct Player player_new(int is_bot, int idx)
{
        struct Player player = {
            .points = 0, .is_bot = is_bot, .idx = idx, .id = idx + 1};
        return player;
}

void ask(char *msg, char *template, void *target)
{
        printf("[ ? ] => %s", msg);
        // C não printa imediatamente quando \n não é usado
        // Então tenho que 'flushar' o buffer manualmente
        fflush(stdout);
        scanf(template, target);

        // Limpar o buffer
        while (getchar() != '\n')
                ;
}

int get_random(int min, int max) { return min + rand() % (max - min + 1); }

int play(struct Player player)
{
        int roundPoints = 0;
        while (1)
        {
                int roll = get_random(1, 6);
                if (roll == 1)
                {
                        printf("[ ! ] => Jogador %d perdeu a vez!\n",
                               player.id);
                        roundPoints = 0;
                        csleep(1);
                        break;
                }
                else
                {

                        roundPoints += roll;
                        char stop;

                        if (player.is_bot == 0)
                        {
                                char buffer[100];
                                sprintf(buffer,
                                        "Jogador %d tirou %d no dado, deseja "
                                        "continuar? (S/n): ",
                                        player.id, roll);

                                ask(buffer, "%c", &stop);
                        }
                        else
                        {
                                printf("[ ! ] => Jogador %d (BOT) tirou %d e "
                                       "está pensando...\n",
                                       player.id, roll);
                                int random = get_random(0, 100);

				// 50% de chance de parar
				// Talvez devo implementar um sistema de dificulades
				// onde o Bot utiliza de algumas estrategias/regras para continuar ou parar
				// baseado nos pontos dele e dos outros jogadores, mas estou sem tempo :)
                                if (random <= 50)
                                        stop = 's';
                                else
                                {

                                        stop = 'n';
                                }
                                csleep(2);
                        }

                        if (tolower(stop) == 'n')
                        {
                                printf("[ ! ] => Jogador %d "
                                       "decidiu parar essa rodada!\n",
                                       player.id);
                                csleep(1);
                                break;
                        }
                }
        }

        printf("[ ! ] => Jogador %d ganhou %d pontos nessa rodada!\n",
               player.id, roundPoints);
        return roundPoints;
}

void sort(struct Player players[], int size)
{
        for (int i = 0; i < size - 1; i++)
        {
                for (int j = 0; j < size - i - 1; j++)
                {
                        if (players[j].points < players[j + 1].points)
                        {
                                struct Player tmp = players[j];
                                players[j] = players[j + 1];
                                players[j + 1] = tmp;
                        }
                }
        }
}

void print_leaderboard(struct Player players[], int size)
{
        struct Player copy[size] = {};
        for (int i = 0; i < size; i++)
        {
                copy[i] = players[i];
        }

        sort(copy, size);
        for (int i = 0; i < size; i++)
        {
                printf("%d - Jogador %d: %d pontos\n", i + 1, copy[i].id,
                       copy[i].points);
        }
}

int main()
{
        printf("[ + ] => Iniciando jogo...\n");
        srand(time(NULL));

        int qtd;
        ask("Quantos jogadores irão jogar? ", "%d", &qtd);
        if (qtd < 1)
        {
                printf("[ E ] => É preciso ter no minimo um jogador!\n");
                return 1;
        }
        qtd++; // +1 para o bot!

        struct Player players[qtd] = {};

        for (int i = 0; i < qtd - 1; i++)
        {
                players[i] = player_new(0, i);
        }

        printf("[ + ] => Adicionando BOT como jogador número %d.\n", qtd);
        players[qtd - 1] = player_new(1, qtd - 1);

        int rounds = 0;
        int winner = -1;
        int diff = 0;

        while (1)
        {
                struct Player copy[qtd] = {};
                for (int i = 0; i < qtd; i++)
                {
                        copy[i] = players[i];
                }

                sort(copy, qtd);
                for (int i = 0; i < qtd; i++)
                {
                        if (copy[i].points >= 100)
                        {
                                winner = copy[i].idx;
                                break;
                        }
                }

                if (winner != -1)
                {
                        if (qtd > 1)
                        {
                                diff = copy[0].points - copy[1].points;
                        }

                        break;
                }

                printf("\n======= Rodada %d =======\n", ++rounds);
                print_leaderboard(players, qtd);
                csleep(2);

                for (int i = 0; i < qtd; i++)
                {
                        struct Player player = players[i];
                        printf("\n[ + ] => É a vez do jogador %d\n", player.id);
                        players[i].points += play(player);
                        csleep(2);
                }
        }

        printf("\n======= Fim de jogo! =======\n");
        print_leaderboard(players, qtd);
        printf("\n==> O JOGADOR %d VENCEU COM %d PONTOS (%d PONTOS DE "
               "DIFERENÇA COM O "
               "SEGUNDO LUGAR)! <==\n",
               players[winner].id, players[winner].points, diff);
        return 0;
}
