#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void regras() {
	printf("Advinha o numero\nO computador ira gerar um numero aleatorio entre dois limites\n");
}

int pontos=0;
void terminar() {
	printf("O jogo foi encerrado");
	printf("Parabens voce conseguiu pontos%d", pontos);
	exit(1);
}

int StringToInteger(char string[]){
	int num, j=1, total=0, i = strlen(string)-1;

		for(i; i>=0;i--){
			num = string[i];
			num = num - 48;
			total += (num * j);
			j = j * 10;
		}
	return total;
}

int main()
{
	int guess = 0;
	char stringGuess[150] = "";
	int dificuldade = 2;
	int random;
	srand ( time(NULL) );

	regras();
	signal(SIGUSR2, regras);
	signal(SIGUSR1, terminar);

	while (1) {
		random = rand() % dificuldade + 1;
		//printf("\nRandom-> %d", random);			//teste

		printf("\nQue numero foi gerado entre 1 a %d \n", dificuldade);	fflush(stdout);
		scanf(" %s", stringGuess);


		guess = StringToInteger(stringGuess);
		//printf("\nadvinhado-> %d", guess);			//teste
		printf("Introduziu-> %d\n", guess);

		if (strcmp(stringGuess, "#END") == 0)
		break;

		if (guess == random) {
			printf("Acertou\n");
			pontos++;
			dificuldade = dificuldade * 2;
		}
		else{
			printf("Errou - numero->%d\n", random);
			if (pontos > 0) {pontos--; dificuldade = 2;}
		}

	}
	printf("Parabens voce conseguiu %dpontos", pontos);
	exit(pontos);
}
