all:
	gcc -c arbitro.c
	gcc utils.h -pthread arbitro.c -o arbitro -lpthread
	gcc -c cliente.c
	gcc utils.h cliente.c -o cliente
	gcc -c jogos/G_001.c
	gcc jogos/G_001.c -o jogos/g_001
	gcc -c jogos/G_002.c
	gcc jogos/G_002.c -o jogos/g_002
	gcc -c jogos/G_003.c
	gcc jogos/G_003.c -o jogos/g_003
	gcc -c jogos/G_004.c
	gcc jogos/G_004.c -o jogos/g_004

arbitro :
	gcc -c arbitro.c
	gcc utils.h -pthread arbitro.c -o arbito -lpthread

cliente :
	gcc -c cliente.c
	gcc utils.h cliente.c -o cliente

jogo1 :
	gcc -c jogos/G_001.c
	gcc jogos/G_001.c -o jogos/g_001
jogo2 :
	gcc -c jogos/G_002.c
	gcc jogos/G_002.c -o jogos/g_002
jogo3 :
	gcc -c jogos/G_003.c
	gcc jogos/G_003.c -o jogos/g_003
jogo4 :
	gcc -c jogos/G_004.c
	gcc jogos/G_004.c -o jogos/g_004

clean:
	$(RM) arbitro arbitro.o
	$(RM) cliente cliente.o
	$(RM) jogos/G_001 jogos/g_001.o
	$(RM) jogos/G_002 jogos/g_002.o
	$(RM) jogos/G_003 jogos/g_003.o
	$(RM) jogos/G_003 jogos/g_004.o
	rm fifo*
