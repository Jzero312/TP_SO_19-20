#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void regras() {
	printf("Caculos (Soma e Subtracao)\nO computador ira gerar uma conta para voce calcular\n");
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
	int resultado = 0;
	int resposta = 0;
	char stringGuess[10] = "";
	int dificuldade = 10;
	int pMenbro, sMenbro, Operacao;
	char sc;

	regras();
	signal(SIGUSR2, regras);
	signal(SIGUSR1, terminar);
	srand ( time(NULL) );

	while (1) {
		pMenbro = rand() % dificuldade + 1;
		sMenbro = rand() % dificuldade + 1;
		Operacao = rand() % 2;

		switch (Operacao) {
			case 0: //+
			resultado = pMenbro + sMenbro;
			sc = '+';
			break;
			case 1: //-
			if(pMenbro < sMenbro) {int aux = sMenbro; sMenbro = pMenbro; pMenbro = aux;}
			resultado = pMenbro - sMenbro;
			sc = '-';
			break;
		}

		printf("\n%d %c %d =  ", pMenbro, sc, sMenbro);
		fflush(stdout);
		scanf(" %s", stringGuess);

		resposta = StringToInteger(stringGuess);
		//printf("\nadvinhado-> %d", guess);			//teste
		if (strcmp(stringGuess, "#END") == 0)
			break;

		if (resposta == resultado) {
			printf("Acertou\n");
			pontos++;
			dificuldade += 15;
		}
		else{
			printf("Errou\n");
			if (pontos > 0) {pontos--; dificuldade = 10;}
		}

		fflush(stdout);
	}
	printf("Parabens voce conseguiu %dpontos", pontos);
	exit(pontos);
}
