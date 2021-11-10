#include "utils.h"

void terminarExecucao();
void Expulsao();

int main(int argc, char** argv){
	int s_fifo_fd;					//fifo do servidor
	int c_fifo_fd;					//fifo do cliente
	int j_fifo_fd;					//fifo para o jogo
	resposta_c resp;				//mensagem para fora do cliente
	pergunta_s perg;				//mensagem para o cliente
	int nperg;
	char c_fifo_fname[25];	//nome do fifo deste cliente
	char j_fifo_fname[25];
	fd_set fds;							//para o select
	int res_fds;						//valor a receber do select
	int meuEstado = 0;			//basicamente uma especie de estado para o cliente

	//cli euCliente;
	char nome[25];
	char temp;


	signal(SIGUSR1, Expulsao);
	signal(SIGUSR2, terminarExecucao);


	if (access(FIFO_SRV, F_OK) != 0){
		printf("ERRO - NAO FOI POSSIVEL ENCONTRAR O ARBITO\n");
		exit(1);
	}

	resp.pid = getpid();
	sprintf(c_fifo_fname, FIFO_CLI, resp.pid);

	if(mkfifo(c_fifo_fname,0600) == -1){
		printf("ERRO - NAO FOI POSSIVEL CRIAR O FIFO DO CLIENTE\n");
		exit(1);
	}
	printf("Cliente inicializado.\n");

	//s_fifo_fd = open(FIFO_SRV, O_WRONLY);				//bloqueante
	//if(s_fifo_fd == -1){
	//	printf("ERRO - O servidor nao existe\n");
	//	unlink(c_fifo_fname);
	//	exit(1);
	//}
	//printf("Servidor aberto.\n");

	c_fifo_fd = open(c_fifo_fname, O_RDWR);
	if(c_fifo_fd == -1){
		printf("ERRO - Falha a abrir fifo do cliente\n");
		close(s_fifo_fd);
		unlink(c_fifo_fname);
		exit(1);
	}

	resp.onServer = false;

	printf("Nome: "); fflush(stdout);

	do{
		//printf("COMANDO: ");	fflush(stdout);


		FD_ZERO(&fds);
		FD_SET(0, &fds);	//TECLADO
		FD_SET(c_fifo_fd, &fds);	//FIFO

		res_fds = select(c_fifo_fd+1, &fds, NULL, NULL, NULL);

		if(res_fds > 0 && FD_ISSET(0, &fds)){ //res_fds > 0 && FD_ISSET(0, &fds)

			scanf(" %[^\n]", resp.mensagem);

			if(meuEstado == 0){
				s_fifo_fd = open(FIFO_SRV, O_WRONLY);
				write(s_fifo_fd, & resp, sizeof(resposta_c));
				close(s_fifo_fd);
				resp.onServer = true;
			}
			else if(meuEstado == 1){
				//printf("Aguardando o jogo começar.\n");	fflush(stdout);
				s_fifo_fd = open(FIFO_SRV, O_WRONLY);
				write(s_fifo_fd, & resp, sizeof(resposta_c));
				close(s_fifo_fd);
			}
			else if(meuEstado == 3){
				j_fifo_fd = open(j_fifo_fname, O_WRONLY);
				write(j_fifo_fd, & resp, sizeof(resposta_c));
				close(j_fifo_fd);
			}
			// else if(meuEstado == 4){
			// 	//printf("Aguardando o jogo começar.\n");	fflush(stdout);
			// 	s_fifo_fd = open(FIFO_SRV, O_WRONLY);
			// 	write(s_fifo_fd, & resp, sizeof(resposta_c));
			// 	close(s_fifo_fd);
			// }



		}
		else if(res_fds > 0 && FD_ISSET(c_fifo_fd, &fds)){ //res_fds > 0 && FD_ISSET(c_fifo_fd, &fds)

				nperg = read(c_fifo_fd, &perg, sizeof(pergunta_s));

				if(nperg == sizeof(perg)){
					//printf("[recebeu]%s\n", perg.mensagem);
					//printf("[estado]%d\n", perg.estado);
					//printf("[o meu estado]%d\n", meuEstado);

					if(meuEstado == 0 && perg.estado == 1){
						printf("[Server] O seu nome é %s.\n", perg.mensagem);	fflush(stdout);
						strcpy(nome, perg.mensagem);
						meuEstado = 1;
						resp.onServer = true;
					}
					else if(meuEstado == 1 && perg.estado == 2){
						printf("%s", perg.mensagem);	fflush(stdout);
					}
					else if(perg.estado == 3 && meuEstado != 3){
						meuEstado = 3;
						printf("O jogo vai começar.\n %s", perg.mensagem); fflush(stdout);
						sprintf(j_fifo_fname, FIFO_JOGO, resp.pid);
					}
					else if(perg.estado == 3 && meuEstado == 3){
						printf("%s", perg.mensagem); fflush(stdout);
					}
					else if(perg.estado == 4 && meuEstado == 3){
						meuEstado = 0;																	//temporario
						resp.onServer = false;
						//strcpy(resp.mensagem, "#END");
						//j_fifo_fd = open(j_fifo_fname, O_WRONLY);
						//write(j_fifo_fd, & resp, sizeof(resposta_c));
						//close(j_fifo_fd);
						printf(" %s\nNovo nome: ", perg.mensagem); fflush(stdout);
					}
				}

		}

	}while(1);

	close(c_fifo_fd);
	unlink(c_fifo_fname);

	exit(1);
}

void Expulsao(){
		char temp[20],temp2[20];

			sprintf(temp, FIFO_CLI, getpid());
			sprintf(temp2, FIFO_JOGO, getpid());
			printf("VOCE FOI EXPLUSO\n");
			unlink(temp);
			unlink(temp2);

	exit(0);
}

void terminarExecucao(){
	char temp[20],temp2[20];

		sprintf(temp, FIFO_CLI, getpid());
		sprintf(temp2, FIFO_JOGO, getpid());
		int aux = open(temp2,O_WRONLY);
		write(aux,"a\n",2);
		printf("Terminou Execuçao\n");
		unlink(temp);
		unlink(temp2);

	exit(0);
}
