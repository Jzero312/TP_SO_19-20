#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdbool.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>

#define GAME_DIR "jogos"
#define MAX_PLAYER 30
#define FIFO_CLI "fifo_%d"
#define FIFO_JOGO "fifo_jogo_%d"
#define FIFO_SRV "fifo_server"

#define MSG_MAX 150
#define NOME_MAX 30

typedef struct {
	char nome[NOME_MAX];
	char nomeJogo[50];
	int pontos;
	pid_t pid;
	int aceite;
	bool suspenso;
	bool ingame;
}cli, pCli;

typedef struct {
	cli *clientes;
	int time;
	int tempoDeJogo;
	bool flagContinue;
	bool *startGames;
	pthread_t *threads_cli;
	pthread_t *thread_tempo_jogo;
	pthread_mutex_t *mutexTimeToStart;
	pthread_mutex_t *mutexStartGame;
	pthread_mutex_t *mutexAddingPlayer;
} Timer;


typedef struct {
	pid_t pid;
	char mensagem[MSG_MAX];
	bool onServer;
} resposta_c;

typedef struct {
	char mensagem[MSG_MAX];
	int estado; // 0 = ñ pertence aqui,1=receber o nome, 2=esta no servidor, a aguardar inicio do jogo, 3-o jogo começou, 4-o jogo terminou e quero as pontuaçoes
} pergunta_s;

#endif
