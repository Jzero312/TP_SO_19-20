#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void regras() {
	printf("Jogo das Traduçoes (Ingles)\nTraduza as respostas de Ingles para Portugues\n");
}

int pontos=0;
void terminar() {
	printf("O jogo foi encerrado");
	printf("Parabens voce conseguiu pontos%d", pontos);
	exit(1);
}

int main()
{

	const char* perguntas[77][2] = {
		{"time", "tempo"},
		{"people", "pessoas"},
		{"way", "modo"},
		{"water", "água"},
		{"words", "respostas"},
		{"man", "homem"},
		{"work", "trabalho"},
		{"word", "resposta"},
		{"part", "parte"},
		{"place", "lugar"},
		{"things", "coisas"},
		{"years", "anos"},
		{"number", "número"},
		{"men", "homens"},
		{"name", "nome"},
		{"mister", "senhor"},
		{"home", "casa"},
		{"air", "ar"},
		{"line", "linha"},
		{"end", "final"},
		{"sound", "som"},
		{"house", "casa"},
		{"world", "mundo"},
		{"school", "escola"},
		{"form", "forma"},
		{"food", "comida"},
		{"children", "crianças"},
		{"feet", "pés"},
		{"land", "terra"},
		{"side", "lado"},
		{"boy", "menino"},
		{"life", "vida"},
		{"head", "cabeça"},
		{"kind", "tipo"},
		{"page", "página"},
		{"earth", "terra"},
		{"hand", "mão"},
		{"year", "ano"},
		{"mother", "mãe"},
		{"light", "luz"},
		{"parts", "partes"},
		{"country", "país"},
		{"father", "pai"},
		{"picture", "imagem"},
		{"eyes", "olhos"},
		{"times", "vezes"},
		{"help", "ajuda"},
		{"thought", "pensamento"},
		{"show", "show"},
		{"night", "noite"},
		{"story", "historia"},
		{"boys", "meninos"},
		{"long", "extenso"},
		{"little", "pouco"},
		{"good", "bom"},
		{"new", "novo"},
		{"same", "mesmo"},
		{"right", "direito"},
		{"different", "diferente"},
		{"old", "velho"},
		{"great", "otimo"},
		{"small", "pequeno"},
		{"big", "grande"},
		{"own", "próprio"},
		{"last", "passado"},
		{"left", "esquerda"},
		{"large", "grande"},
		{"important", "importante"},
		{"enough", "suficiente"},
		{"second", "segunda"},
		{"first", "primeiro"},
		{"well", "bem"},
		{"live", "vivo"},
		{"high", "alta"},
		{"following", "seguinte"},
		{"used", "usado"},
		{"even", "igual"}};

	char resposta[150],lixo;
	int random;

	regras();
	signal(SIGUSR2, regras);
	signal(SIGUSR1, terminar);
	srand ( time(NULL) );

	while (1) {
		random = rand() % 76;

		printf("\nQual é a traduçao de %s:\n", perguntas[random][0]);
		fflush(stdout);
		//read(0,resposta,sizeof(resposta));
		//fflush(stdin);
		scanf(" %s", resposta);
		printf("resp :%s:\n", resposta);

		if (strcmp(resposta, "#END") == 0)
			break;

		if (strcmp(resposta, perguntas[random][1])==0) {
			printf("Acertou\n");
			pontos++;
		}
		if (strcmp(resposta, perguntas[random][1])!=0 && strcmp(resposta, "sair") != 0){
			printf("Errou - Resposta->%s\n", perguntas[random][1]);
			if (pontos > 0)
				pontos--;
		}
	}
	printf("Parabens voce conseguiu %dpontos", pontos);
	exit(pontos);
}
