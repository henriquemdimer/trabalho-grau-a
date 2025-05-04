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
#include <stdio.h>

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
        printf("%s", msg);
	// C não printa imediatamente quando \n não é usado
        // Então tenho que 'flushar' o buffer manualmente
        fflush(stdout);
        scanf(template, target);

        // Limpar o buffer
        while (getchar() != '\n')
                ;
}

int main()
{
        printf("Iniciando jogo...\n");

        int qtd;
        ask("Quantos jogadores irão jogar? ", "%d", &qtd);
        qtd++; // +1 para o bot!

        struct Player jogadores[qtd] = {};

        for (int i = 0; i < qtd - 1; i++)
        {
                jogadores[i] = player_new(0, i);
        }

        printf("Adicionando BOT como jogador número %d.\n", qtd);
        jogadores[qtd - 1] = player_new(1, qtd - 1);

        while (1)
        {
                char stop;
                ask("Deseja continuar? (S/n): ", "%c", &stop);
                if (tolower(stop) == 'n')
                        break;
        }

        printf("Adeus!\n");
        return 0;
}
