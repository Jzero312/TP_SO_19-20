#include "utils.h"

void verifica_var_amb(char *dir,int *max) {
	//defenir diretorios dos jogos
	if (getenv("GAMEDIR"))
		strcpy(dir, getenv("GAMEDIR"));
	else
		strcpy(dir, GAME_DIR);

	//defenir numero maximo de jogadores
	if (getenv("MAXPLAYER")) {
		*max = atoi(getenv("MAXPLAYER"));
		if (*max > 30)
			*max = 30;
	}
	else
		*max = MAX_PLAYER;
}

char* RemoveQuestion(char* mensagem){
	for(int i=0,j=2;i<strlen(mensagem);i++){								//remover 1 pergunta do mygame
		if(mensagem[i]=='\n')
			j--;
		if(j==0)
			mensagem[i+1]='\0';
	}
	return mensagem;
}



char** verificarJogos(char dir[],int *c){
	DIR *dr = opendir(dir);
	struct dirent *de;
	char **temp=NULL;

	if (dr == NULL)
	{
			return NULL;
	}

	while ((de = readdir(dr)) != NULL){
		if(de->d_name[0]=='g' && de->d_name[1]=='_'){
				(*c)++;
				if(temp==NULL){
						temp=malloc(sizeof(char*));
				}else{
						temp=realloc(temp,(*c)*sizeof(char*));
				}
				temp[(*c)-1]=malloc(sizeof(char)*50);
				strcpy(temp[(*c)-1],de->d_name);
		}
}

	closedir(dr);
	return temp;
}

int countLoogedUsers(cli *Users, int max){
	int loggedUserCount = 0;
	for(int i=0; i<max; i++){
		if((Users+i)->aceite==1)
			loggedUserCount++;
	}
	return loggedUserCount;
}

int countPlayingUsers(cli *Users){
	int playingUserCount = 0;
	for(int i=0; i<MAX_PLAYER; i++){
		if((Users+i)->aceite==1 && (Users+i)->ingame==true)
			playingUserCount++;
	}
	return playingUserCount;
}

int addUser(pergunta_s *mandarCli, cli *c, cli *Users, int *n, int max,char jogo[50]){
	int i;

	for(i=0;i<max;i++)
  		if((Users+i)->aceite==-1)
  			break;

  	 if(i==max)
  	 {
  	 	printf("\nNao existem vagas\n"); fflush(stdout);
  	 	strcpy(mandarCli->mensagem, "Nao existem vagas");
  	 	return -1;
  	 }

		 //verificar nome
  	 char nome[15], nomeaux[40];
  	 int j,st,x;
  	 strcpy(nome,c->nome);
  	 for(j=0,st=1,x=0;j<max;j++)
  	 {
    		if((Users+j)->aceite==1)
			if(strcmp((Users+j)->nome,nome)==0)
      			{
				if(x)
					nome[strlen(nome)-1]='\0';

        			sprintf(nomeaux,"%s%d",nome,st++);
      				strcpy(nome,nomeaux);
				x=1;
        			j=-1;
      			}
			}

	*n = *n + 1;
	strcpy(c->nome,nome);
	strcpy(mandarCli->mensagem,nome);
	strcpy((Users+i)->nome,nome);
	strcpy((Users+i)->nomeJogo,jogo);
	mandarCli->estado = 1;

	c->aceite = 1;

  (Users+i)->pid=c->pid;
  (Users+i)->aceite=1;
	(Users+i)->suspenso=false;
	(Users+i)->ingame=false;

  return i;
	}


	void mostraJogadores(cli *clientes, int max){

			for(int i=0;i<max;i++)
  				if((clientes+i)->aceite==1){
  					printf("\nNome: %s\nJogo: %s\n", (clientes+i)->nome, (clientes+i)->nomeJogo);
						if((clientes+i)->suspenso)
							printf("O jogador esta suspenso\n");
						else
							printf("O jogador esta em jogo\n");
					}
	}



	int kickPlayer(cli *Users,char nome[],int max, char motivo[]){
		int i;

		for(i=0;i<max;i++)
  			if((Users+i)->aceite==1)
  				if(strcmp(nome, (Users+i)->nome)==0){
  					(Users+i)->aceite = -1;
  					//strcpy((Users+i)->res, motivo);
  					kill((Users+i)->pid, SIGUSR1);
  					return 0;
  				}
  		return -1; //erro
	}


		int suspendePlayer(cli *Users,char nome[],int max,bool suspenso){
			int i;

			for(i=0;i<max;i++)
	  			if((Users+i)->aceite==1)
	  				if(strcmp(nome, (Users+i)->nome)==0){
	  					(Users+i)->suspenso = suspenso;
	  					return 0;
	  				}
	  		return -1; //erro
		}

void* thread_comunicacao(void* c){
	cli* cliente = (cli*)c;
	int p_write[2]= {-1,-1},p_read[2]= {-1,-1},res,child_id;

	if ( pipe(p_write) < 0  ||  pipe(p_read) < 0 ) {printf("[Server] Erro criar pipes de %s\n", cliente->nome); return NULL;}

	cliente->ingame=true;

  child_id=fork();

  if(child_id==0){     //filho

		close(p_write[1]); close(p_read[0]);
		dup2(p_write[0], 0);	close(p_write[0]);
		dup2(p_read[1], 1);	close(p_read[1]);

		char dir[101],game_dir[50];
		int aux;

		verifica_var_amb(game_dir, &aux);
		sprintf(dir,"%s/%s",game_dir,cliente->nomeJogo);
		execlp(dir,dir,NULL);
		fprintf(stderr,"[Server] Erro a executar jogo de %s\n", cliente->nome);

    exit(1);
  }
  else {	//pai

			close(p_write[0]);
			close(p_read[1]);
			int n,x,pontos=0;
			char g_fifo_name[15];
			int g_fifo_fd;
			char c_fifo_name[15];
			int c_fifo_fd;
			pergunta_s perg;
			resposta_c resp;
			char mygame[250];										//info do jogo
			bool fistTimeConnecting=true;				//primeira vez que o jogador entra
			bool mg=false;											//my game
			bool cmd=false;											//comando, para verificar se o user introduziu um comando

			perg.estado = 3;

			sprintf(g_fifo_name, FIFO_JOGO, cliente->pid);
			sprintf(c_fifo_name, FIFO_CLI, cliente->pid);
			if(mkfifo(g_fifo_name,0600) == -1){
				perror("mkfifo() error");
				kill(child_id, SIGKILL);
				close(p_write[1]);
				close(p_read[0]);
				pthread_exit(NULL);
			}

			g_fifo_fd = open(g_fifo_name, O_RDWR);
			if(g_fifo_fd == -1){
				perror("open() error");
				close(g_fifo_fd);							//faço close se deu erro?!?
				unlink(g_fifo_name);
				close(p_write[1]);
				close(p_read[0]);
				pthread_exit(NULL);
			}

			printf("[Server] Jogo de %s iniciado com sucesso!\n", cliente->nome);

			do{
				if(!cliente->suspenso){
					if(cmd){
						if(mg){
							mg=false;
							strcpy(perg.mensagem,mygame);
						}else{
							strcpy(perg.mensagem,"[Arbitro] - Comando invalido\n");
						}
						cmd=false;

					}else{
						n=read(p_read[0],perg.mensagem,sizeof(perg.mensagem));		//le jogo
						perg.mensagem[n]='\0';
					}
					if(fistTimeConnecting){//guarda mygame
						fistTimeConnecting=false;
						strcpy(mygame,perg.mensagem);
						RemoveQuestion(mygame);
					}

					c_fifo_fd = open(c_fifo_name, O_WRONLY);
					if(c_fifo_fd == -1){
						perror("open() error");
						close(g_fifo_fd);
						unlink(g_fifo_name);
						close(p_write[1]);
						close(p_read[0]);
						pthread_exit(NULL);
					}
					write(c_fifo_fd, &perg, sizeof(pergunta_s));	//manda msg para cliente
					close(c_fifo_fd);

					x = read(g_fifo_fd, &resp, sizeof(resposta_c));		//lê resposta do cliente

					if(!cliente->ingame){											//torneio vai terminar
						write(p_write[1],"#END\n\0",6);					//escreve #sair para acabar o jogo

						int points;															//passar os pontos da msg para a var
						wait(&points);
						WIFEXITED(points);
						cliente->pontos=WEXITSTATUS(points);
						printf("[Server] O jogador %s obteve %d pontos.\n",cliente->nome,cliente->pontos);

						kill(child_id, SIGKILL);								//terminar jogo
						close(g_fifo_fd);
						unlink(g_fifo_name);
						close(p_write[1]);
						close(p_read[0]);
						pthread_exit(NULL);
					}

					if(resp.mensagem[0]=='#'){	//mensagem para o arbito
						cmd=true;
						if(strcmp(resp.mensagem,"#mygame")==0){
							mg=true;
						}
						else if(strcmp(resp.mensagem,"#quit")==0){
							cliente->aceite=-1;

							write(p_write[1],"#END\n\0",6);
							kill(child_id, SIGKILL);
							close(g_fifo_fd);
							unlink(g_fifo_name);
							kill(cliente->pid,SIGUSR2);
							close(p_write[1]);
							close(p_read[0]);
							printf("[Server] O jogador %s abandonou a partida!\n", cliente->nome);
							pthread_exit(NULL);
						}
					}else	{	//envia msg para o jogo
						strcat(resp.mensagem, "\n\0");
						write(p_write[1],resp.mensagem,strlen(resp.mensagem));
					}
				}else{ //quando o jogo esta suspenso

					read(g_fifo_fd, &resp, sizeof(resposta_c));
					strcpy(perg.mensagem,"[Server] Voce esta suspenso\n");
					c_fifo_fd = open(c_fifo_name, O_WRONLY);
					if(c_fifo_fd == -1){
						perror("open() error");
						close(g_fifo_fd);
						unlink(g_fifo_name);
						pthread_exit(NULL);
					}
					write(c_fifo_fd, &perg, sizeof(pergunta_s));
					close(c_fifo_fd);
				}
			}while(true);
	}
	pthread_exit(NULL);
}

int kickall(cli *Users,pthread_t *threads_cli,int max){
	int i;

	for(i=0;i<max;i++)
			if((Users+i)->aceite==1 && (Users+i)->ingame==true){
					(Users+i)->ingame=false;
					kill((Users+i)->pid, SIGUSR2);
					pthread_join(*(threads_cli+i), NULL);

			}else if((Users+i)->aceite==1 && (Users+i)->ingame==false){
				kill((Users+i)->pid, SIGUSR2);
			}
		return -1;
}

void StartGamesOfLoggedUsers(pthread_t *threads_cli, cli *clientes){

	for(int i = 0; i<MAX_PLAYER; i++){
		if((clientes+i)->aceite==1)
			pthread_create((threads_cli+i),NULL,thread_comunicacao,(clientes+i));
	}
}

void TerminaOsJogos(cli *Users,pthread_t *threads_cli){
	char c_fifo_fname[15],g_fifo_fname[15];
	int c_fifo_fd,g_fifo_fd;
	pergunta_s mandarCli;

	mandarCli.estado = 4;

	//acabar jogos e calcular posiçoes
	int maxP=0,maxJ=0;
	for(int j=0;j<MAX_PLAYER;j++){
			if((Users+j)->aceite==1 && (Users+j)->ingame==true){
				(Users+j)->ingame=false;
				sprintf(g_fifo_fname, FIFO_JOGO, (Users+j)->pid); //poder saber a quem responder
				g_fifo_fd = open(g_fifo_fname, O_WRONLY);
				//mandar uma msg e os postos + classificaçao
				strcpy(mandarCli.mensagem,"a\n");
				write(g_fifo_fd, &mandarCli, sizeof(pergunta_s));
				close(g_fifo_fd);
				pthread_join(*(threads_cli+j), NULL);
				if((Users+j)->pontos>maxP)
				{
					maxP=(Users+j)->pontos;
					maxJ=j;
				}
			}
	}
	printf("[Server] O jogador vencedor foi %s com %d pontos.\n",(Users+maxJ)->nome,(Users+maxJ)->pontos);
	// enviar msg aos jogadores
	for(int i=0;i<MAX_PLAYER;i++){
			if((Users+i)->aceite==1 && (Users+i)->ingame==false){
					sprintf(c_fifo_fname, FIFO_CLI, (Users+i)->pid); //poder saber a quem responder
					c_fifo_fd = open(c_fifo_fname, O_WRONLY);
					//mandar uma msg e os postos + classificaçao
					sprintf(mandarCli.mensagem, "O jogo terminou!\nVoce objeteve %d pontos\nO jogador vencedor foi %s com %d pontos\n",(Users+i)->pontos,(Users+maxJ)->nome,(Users+maxJ)->pontos);
					write(c_fifo_fd, &mandarCli, sizeof(pergunta_s));
					close(c_fifo_fd);
					(Users+i)->aceite=-1;
					(Users+i)->ingame=false;

			}
		}
		printf("[Server] Todos os jogos terminaram!!\n");
		return;
}

void* thread_TimeToEndGame(void* c){
	Timer* timer = (Timer*)c;
	int PlayingUsers = 0;
	timer->flagContinue = true;

	printf("[Server] O torneiro irá terminar dentro de %d segundos\n", timer->tempoDeJogo);
	 while(1){
		 sleep(1);
		 timer->tempoDeJogo = timer->tempoDeJogo - 1;
		 PlayingUsers = countPlayingUsers(timer->clientes);

		 if(timer->tempoDeJogo == 0){
			 				*timer->startGames = false;
			 				TerminaOsJogos(timer->clientes, timer->threads_cli);
							pthread_exit(NULL);
		 }
		 else if(PlayingUsers == 1){
			 				*timer->startGames = false;
							TerminaOsJogos(timer->clientes, timer->threads_cli);
							pthread_exit(NULL);
		 }
		 else if(PlayingUsers == 0){ //nunca chegamos aqui, acho eu
			 pthread_exit(NULL);
		 }

		 pthread_mutex_lock(timer->mutexStartGame);
		 if(timer->flagContinue == false){
			 pthread_mutex_unlock(timer->mutexStartGame);
			 break;
		 }
		 if(*timer->startGames == false){
			 pthread_mutex_unlock(timer->mutexStartGame);
			 break;
		 }
		 pthread_mutex_unlock(timer->mutexStartGame);
	 }
	pthread_exit(NULL);
}



void* thread_TimeToStartTheGame(void* c){
	Timer* timer = (Timer*)c;
	timer->flagContinue = true;
	printf("[Server] O torneiro irá começar dentro de %d segundos\n", timer->time);
	 while(1){
		 sleep(1);

		 pthread_mutex_lock(timer->mutexTimeToStart);
		 timer->time = timer->time - 1;
		 pthread_mutex_unlock(timer->mutexTimeToStart);

		 if(timer->time == 0){
			 if(countLoogedUsers((cli*) (timer->clientes), MAX_PLAYER) < 2){
				 printf("[Server] Numero de jogadores insuficiente para começar partida.\n");
				 pthread_exit(NULL);
			 }
			 else{
				 StartGamesOfLoggedUsers((pthread_t*)(timer->threads_cli),(cli*) (timer->clientes));
				 *timer->startGames = true;
				 pthread_create((pthread_t *)timer->thread_tempo_jogo, NULL, thread_TimeToEndGame, timer);
				 pthread_exit(NULL);
			 }
		 }

		 pthread_mutex_lock(timer->mutexStartGame);
		 if(timer->flagContinue == false){
			 pthread_mutex_unlock(timer->mutexStartGame);
			 break;
		 }
		 if(*timer->startGames == true){
			 pthread_mutex_unlock(timer->mutexStartGame);
			 break;
		 }
		 pthread_mutex_unlock(timer->mutexStartGame);
	 }
	pthread_exit(NULL);
}


int main(int argc, char** argv) {

	int s_fifo_fd;					//fifo do servidor
	int c_fifo_fd;					//fifo do cliente
	char c_fifo_fname[25];	//nome do fifo cliente para responder
	int num;								//bits de leitura servidor
	pergunta_s mandarCli;		//resposta a mandar para o clientes
	resposta_c receberCli;	//o que vai receber do cliente
	bool startGames = false;

	char game_dir[50], cmd[40], **nomeJogos=NULL;
	int max_player, duracao_camp,tempo_espera, nPlayers=0, numJogos=0;

	fd_set fds;							//para o select
	int res_fds;						//valor a receber do select

	//cli c;
	bool termina = false;

	struct timeval tempo;

	srand ( time(NULL) );


				/*------------------Argumentos & outras cenas------------------*/
	duracao_camp = 60;
	tempo_espera = 60;
	if(argc == 3){
			duracao_camp = atoi(argv[1]);
			tempo_espera = atoi(argv[2]);
	}
	if(argc == 2){
			duracao_camp = atoi(argv[1]);
	}


	verifica_var_amb(game_dir, &max_player);
	nomeJogos=verificarJogos(game_dir,&numJogos);

	cli clientes[max_player];


	pthread_t threads_cli[max_player];							//Declaraçao das Threads
	pthread_t threadTimeToStartGame;
	pthread_t threadTimeToEndGame;

	pthread_mutex_t mutexTimeToStart;
	pthread_mutex_t mutexStartGame;
	//pthread_mutex_t mutexAddingPlayer;

	Timer infoTimers = {.clientes = clientes, .time = tempo_espera, .tempoDeJogo = duracao_camp,
											.flagContinue = true, .startGames = &startGames, .threads_cli = threads_cli,
											.thread_tempo_jogo = &threadTimeToEndGame, .mutexStartGame = &mutexStartGame,
											.mutexTimeToStart = &mutexTimeToStart};

  	for(int i=0;i<max_player;i++)
  	{
  		clientes[i].aceite=-1;
  		clientes[i].pid=-1;
  	}

			/*------------------Servidor------------------*/


	//verificar se ja existe algum server a correr
	if (access(FIFO_SRV, F_OK) != 0){
		mkfifo(FIFO_SRV, 0600);
		printf("Servidor iniciado...\n");
		}
	else {
		fprintf(stderr, "ERRO Ja existe um servidor a funcionar!\n");
		exit(1);
	}


	s_fifo_fd = open(FIFO_SRV, O_RDWR);
	if(s_fifo_fd == -1){
		fprintf(stderr, "ERRO a abrir fifo do servidor.\n");
		exit(1);
	}
	do{
		printf("\nCOMANDO: ");	fflush(stdout);

		FD_ZERO(&fds);
		FD_SET(0, &fds);	//TECLADO
		FD_SET(s_fifo_fd, &fds);	//FIFO
		//tempo.tv_sec = 5; tempo.tv_usec = 0;
		res_fds = select(s_fifo_fd+1, &fds, NULL, NULL, NULL);

		if(res_fds > 0 && FD_ISSET(0, &fds)){
			scanf(" %[^\n]",cmd);
			printf("processar o cmd -> %s\n", cmd);

				if(strcmp(cmd, "exit")==0){
					kickall(clientes,threads_cli,max_player);
					termina = true;
				}
				else if(strcmp(cmd, "players")==0){
					mostraJogadores(clientes, max_player);

				}
				else if(strcmp(cmd, "games")==0){
					for(int i=0;i<numJogos;i++)
						printf("\n%s", nomeJogos[i]);
				}
				else if(cmd[0]=='k'){
					memmove(cmd, cmd+1, strlen(cmd));
					kickPlayer(clientes, cmd, max_player,"Voce foi expulso do servidor!\n");
				}
				else if(strcmp(cmd, "start")==0){
					if(countLoogedUsers(clientes, max_player)>=2){
						if(startGames == false){										//verificar se estao pelo menos 2 jogadores
							  pthread_mutex_lock(&mutexStartGame);
							  infoTimers.flagContinue = false;
								startGames = true;
								pthread_mutex_unlock(&mutexStartGame);

								pthread_join(threadTimeToStartGame, NULL);
								infoTimers.tempoDeJogo = duracao_camp;
								pthread_create(&threadTimeToEndGame, NULL, thread_TimeToEndGame, &infoTimers);
								StartGamesOfLoggedUsers(threads_cli, clientes);
						}
						else
							printf("[Server] O torneio já se encontra a decorrer!\n");
						}
					else
						printf("[Server] É necessario 2 jogadores para começar!\n");

				}
				else if(strcmp(cmd, "end")==0){
					if(startGames == true){
						pthread_mutex_lock(&mutexStartGame);
						infoTimers.flagContinue = false;
						startGames = false;
						pthread_mutex_unlock(&mutexStartGame);

						pthread_join(threadTimeToEndGame, NULL);
						TerminaOsJogos(clientes, threads_cli);
						infoTimers.time = tempo_espera;
						infoTimers.tempoDeJogo = duracao_camp;
					}
					else
						printf("Nao existem jogos a decorrer!\n");
				}
				else if(cmd[0]=='s'){
					memmove(cmd, cmd+1, strlen(cmd));
					suspendePlayer(clientes, cmd, max_player,true);
				}
				else if(cmd[0]=='r'){
					memmove(cmd, cmd+1, strlen(cmd));
					suspendePlayer(clientes, cmd, max_player,false);
				}
		}
		else if(res_fds>0 && FD_ISSET(s_fifo_fd, &fds)){

				num = read(s_fifo_fd, &receberCli, sizeof(resposta_c));

				if(num == sizeof(resposta_c)){

					//printf("[jogador disse]%s\n", receberCli.mensagem);
					//printf("[jogador pid]%d\n", receberCli.pid);
					//printf("[jogador conectado]%d\n", receberCli.onServer);
						printf("[%d] %s\n", receberCli.pid, receberCli.mensagem);
						if(receberCli.onServer == false){
							cli c;
							strcpy(c.nome, receberCli.mensagem);
							c.pid = receberCli.pid;

							int numCli = addUser(&mandarCli ,&c, clientes, &nPlayers, max_player, nomeJogos[rand()%numJogos]);
							if(countLoogedUsers(clientes, max_player) == 2 && startGames == false){
								infoTimers.time = tempo_espera;
								pthread_create(&threadTimeToStartGame,NULL,thread_TimeToStartTheGame,&infoTimers);
							}
						}


						else{
							if(receberCli.mensagem[0]=='#'){
								if(strcmp(receberCli.mensagem, "#quit")==0){
									for(int i = 0; i<max_player; i++)
										if(receberCli.pid == clientes[i].pid){
											clientes[i].aceite = -1;
											break;
											}
									kill(receberCli.pid,SIGUSR2);
								}
							}
						else if(countLoogedUsers(clientes, max_player) < 2){
							mandarCli.estado = 2;
							strcpy(mandarCli.mensagem,"[Server] Falta 1 jogador para começar o torneio!\n");
						}
						else if(startGames == false){
							mandarCli.estado = 2;
							pthread_mutex_lock(&mutexTimeToStart);
							sprintf(mandarCli.mensagem,"[Server] Falta %d segundos para iniciar o torneio\n", infoTimers.time);
							pthread_mutex_unlock(&mutexTimeToStart);
						}
						else  if(startGames == true){
							mandarCli.estado = 2;
							strcpy(mandarCli.mensagem,"[Server] O torneio encontra-se a decorrer aguarde que acabe!\n");
						}

						}

							sprintf(c_fifo_fname, FIFO_CLI, receberCli.pid); //poder saber a quem responder
							c_fifo_fd = open(c_fifo_fname, O_WRONLY);
							num = write(c_fifo_fd, &mandarCli, sizeof(pergunta_s));
							close(c_fifo_fd);

							//printf("[jogador recebeu]%s\n", mandarCli.mensagem);
							//printf("[estado]%d\n", mandarCli.estado);
				}
	}
}while(termina != true);

	close(s_fifo_fd);
	unlink(FIFO_SRV);
	pthread_mutex_destroy(&mutexTimeToStart);
	pthread_mutex_destroy(&mutexStartGame);

	exit(5);




}
