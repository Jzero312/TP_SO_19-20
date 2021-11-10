#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void regras() {
	printf("Jogo das Capitais\nInsira o capital do pais perguntado\n");
}

int pontos=0;
void terminar() {
	printf("O jogo foi encerrado");
	printf("Parabens voce conseguiu pontos%d", pontos);
	exit(1);
}

int main()
{

	const char* nomes[176][2] = {
		{"Argélia","Argel"},

		{"Botswana","Gaborone"},

		{"Burkina Faso","Ouagadougou"},

		{"Burundi","Bujumbura"},

		{"Cabo Verde","Praia"},

		{"Camarões","Yaoundé"},

		{"Chade","NDjamena"},

		{"Comores","Moroni"},

		{"Djibouti","Djibouti"},

		{"Egito","Cairo"},

		{"Eritreia","Asmara"},

		{"Gabão","Libreville"},

		{"Gana","Acra"},

		{"Guiné","Conacri"},

		{"Gâmbia","Banjul"},

		{"Lesoto","Maseru"},

		{"Libéria","Monróvia"},

		{"Líbia","Trípoli"},

		{"Madagáscar","Antananarivo"},

		{"Malawi","Lilongwe"},

		{"Mali","Bamako"},

		{"Marrocos","Rabat"},

		{"Mauritânia","Nouakchott"},

		{"Maurícia","Port Louis"},

		{"Moçambique","Maputo"},

		{"Namíbia","Windhoek"},

		{"Nigéria","Abuja"},

		{"Níger","Niamey"},

		{"Quénia","Nairobi"},

		{"Ruanda","Kigali"},

		{"Senegal","Dakar"},

		{"Serra Leoa","Freetown"},

		{"Seychelles","Victoria"},

		{"Somália","Mogadíscio"},

		{"Sudão",""},

		{"Sudão do Sul","Juba"},

		{"São Tomé e Príncipe","São Tomé"},

		{"Togo","Lomé"},

		{"Tunísia","Tunis"},

		{"Uganda","Kampala"},

		{"Zâmbia","Lusaka"},

		{"Zimbabwe","Harare"},

		{"Bahamas","Nassau"},

		{"Barbados","Bridgetown"},

		{"Belize","Belmopan"},

		{"Costa Rica","São José"},

		{"Cuba","Havana"},

		{"Dominica","Roseau"},

		{"El Salvador","São Salvador"},

		{"Guatemala","Cidade da Guatemala"},

		{"Haiti","Porto Príncipe"},

		{"Honduras","Tegucigalpa"},

		{"Jamaica","Kingston"},

		{"Nicarágua","Manágua"},

		{"Panamá","Cidade de Panamá"},

		{"Santa Lúcia","Castries"},

		{"São Cristóvão e Nevis","Basseterre"},

		{"São Vicente e Granadinas","Kingstown"},

		{"Trinidad e Tobago","Porto de Espanha"},

		{"Argentina","Buenos Aires"},

		{"Brasil","Brasília"},

		{"Chile","Santiago"},

		{"Colômbia","Bogotá"},

		{"Equador","Quito"},

		{"Guiana","Georgetown"},

		{"Paraguai","Assunção"},

		{"Peru","Lima"},

		{"Suriname","Paramaribo"},

		{"Uruguai","Montevidéo"},

		{"Venezuela","Caracas"},

		{"Afeganistão","Cabul"},

		{"Arábia Saudita","Riad"},

		{"Azerbaijão","Baku"},

		{"Bangladesh","Dacca"},

		{"Barein","Manama"},

		{"Brunei","Bandar Seri Begawan"},

		{"Butão","Timphu"},

		{"Camboja","Phnom Penh"},

		{"Cazaquistão","Astana"},

		{"Catar","Doha"},

		{"China","Pequim"},

		{"Cingapura","Cidade de Cingapura"},

		{"Coreia do Norte","Pyongyang"},

		{"Coreia do Sul","Seul"},

		{"Emirados Árabes Unidos","Abu Dhabi"},

		{"Filipinas","Manila"},

		{"Iêmen","Sana"},

		{"Índia","Nova Délhi"},

		{"Indonésia","Jacarta"},

		{"Irã","Teerã"},

		{"Iraque","Bagdá"},

		{"Israel","Jerusalém"},

		{"Japão","Tóquio"},

		{"Jordânia","Amã"},

		{"Kuwait","Cidade do Kuwait"},

		{"Laos","Vietiane"},

		{"Líbano","Beirute"},

		{"Malásia","Kuala Lumpur"},

		{"Maldivas","Male"},

		{"Mianmar","Nay Puy Taw"},

		{"Mongólia","Ulan Bator"},

		{"Nepal","Katmandu"},

		{"Omã","Mascate"},

		{"Paquistão","Islamabad"},

		{"Quirguistão","Bishkek"},

		{"Síria","Damasco"},

		{"Sri Lanka","Colombo"},

		{"Tailândia","Bangcoc"},

		{"Tajidquistão","Muchambe"},

		{"Timor Leste","Dili"},

		{"Turcomenistão","Ashkhabad"},

		{"Uzbequistão","Tashkent"},

		{"Vietnã","Hanói"},

		{"Albânia","Tirana"},

		{"Alemanha","Berlim"},

		{"Andorra","Andorra la Vella"},

		{"Armênia","Erevan"},

		{"Áustria","Viena"},

		{"Azerbaijão","Baku"},

		{"Bielorrússia","Minsk"},

		{"Bélgica","Bruxelas"},

		{"Bulgária","Sófia"},

		{"República Tcheca","Praga"},

		{"Cazaquistão","Astana"},

		{"Chipre","Nicósia"},

		{"Croácia","Zagreb"},

		{"Dinamarca","Copenhagem"},

		{"Eslováquia "},

		{"Eslovênia","Liubliana"},

		{"Espanha","Madrid"},

		{"Estônia","Tallinn"},

		{"Finlândia","Helsinque"},

		{"França","Paris"},

		{"Grécia","Atenas"},

		{"Geórgia","Tbilisi"},

		{"Hungria","Budapeste"},

		{"Irlanda","Dublin"},

		{"Islândia","Reiquiavique"},

		{"Itália","Roma"},

		{"Letônia","Riga"},

		{"Liechtenstein","Vaduz"},

		{"Lituânia","Vilnius"},

		{"Luxemburgo","Luxemburgo"},

		{"República da Macedônia","Skopje"},

		{"Malta","Valetta"},

		{"Moldávia","Chisinau"},

		{"Mônaco","Monaco-Ville"},

		{"Montenegro","Podgorica"},

		{"Noruega","Oslo"},

		{"Países Baixos","Amsterdã"},

		{"Polônia","Varsóvia"},

		{"Portugal","Lisboa"},

		{"Reino Unido","Londres"},

		{"Romênia","Bucareste"},

		{"Rússia","Moscou"},

		{"Turquia","Ancara"},

		{"San Marino","San Marino"},

		{"Sérvia","Belgrado"},

		{"Suécia","Estocolmo"},

		{"Suíça","Berna"},

		{"Ucrânia","Kiev"},

		{"Austrália","Camberra"},

		{"Fiji","Suva"},

		{"Ilhas Marshall","Majuro"},

		{"Ilhas Salomão","Honiara"},

		{"Kiribati","Taraua"},

		{"Micronésia","Paliquir"},

		{"Nauru","Yaren"},

		{"Nova Zelândia","Wellington"},

		{"Palau","Melequeoque"},

		{"Papua Nova Guiné","Porto Moresby"},

		{"Samoa","Apia"},

		{"Tonga","Nucualofa"},

		{"Tuvalu","Funafuti"},

		{"Vanuatu","Porto Vila"}
								};
	char capital[150];
	int random;

	regras();
	//printf("Eu teho i PID= %d .. \n", getpid());
	signal(SIGUSR2, regras);
	signal(SIGUSR1, terminar);
	srand ( time(NULL) );
	while (1) {
		random = rand() % 176;

		printf("\nQual é a capiatal de %s:\n", nomes[random][0]);	fflush(stdout);
		scanf(" %s", capital);
		if (strcmp(capital, "#END") == 0)
			break;
		if (strcmp(capital, nomes[random][1])==0) {
			printf("Acertou\n");
			pontos++;
		}
		if (strcmp(capital, nomes[random][1])!=0){
			printf("Errou era %s\n",nomes[random][1]);
			if (pontos > 0)
				pontos--;
		}


	}
	printf("Parabens voce conseguiu %dpontos", pontos);
	exit(pontos);
}
