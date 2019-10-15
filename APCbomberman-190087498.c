/*Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programação de Computadores –
2/2019
Aluno(a): Gabriel Ligoski
Matricula: 190087498
Turma: A
Versão do compilador: gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Descricao: Jogo bomberman feito em C para o primeiro trabalho de APC 19.2
*/
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*---------------------------------------------------------------------------*/
/*determina o sistema operacional para o clear */
#ifdef _WIN32
#define CLEAR "cls"
#else /*em qualquer outro SO*/
#define CLEAR "clear"
#endif
/*---------------------------------------------------------------------------*/
/*valor medio que a bomba levara para explodir (segundos)*/
#define MEDIA_TEMPO 7
/* erro da medida de tempo que a bomba levara para explodir (segundos)*/
#define ERRO_TEMPO 3
/* expressao que gera um numero aleatorio no intervalo [M - E, M + E]*/
#define RAND(M, E) (M + (rand()%(2*E + 1) - E))
/*---------------------------------------------------------------------------*/
/*variaveis Globais*/
int vetorGlobalL, vetorGlobalC, playerLinha, playerColuna = 1, contadorInimigo=0, EndGame=0;
/*mapa processual gerado*/
void on_start(char MatrizTabuleiro[vetorGlobalL][vetorGlobalC], int vetorGlobalL, int vetorGlobalC)/*A fazer: Encerrar quando ganhar ou perder e nao esperar rodar o loop denovo*/
{
	/*contadores*/
	int i, z;
	/*loops para criar as bordas do mapa e paredes*/
	for (i = 0; i < vetorGlobalL; ++i)
	{
		for (z = 0; z < vetorGlobalC; ++z)
		{
			if (i!=vetorGlobalL-1)
			MatrizTabuleiro[i][z] = ' ';
			if (i%2==0 && z%2==0 && i!=vetorGlobalL-1 && z!=vetorGlobalC-1)
			MatrizTabuleiro[i][z] = '+';

			MatrizTabuleiro[vetorGlobalL-1][z] = '_';
			MatrizTabuleiro[0][z] = '_';
		}
		MatrizTabuleiro[i][0] = '|';
		MatrizTabuleiro[i][vetorGlobalC-1] = '|';
	}
	/*Mapa dinamico gerado aqui*/
	for (i = 1; i < vetorGlobalL-1; ++i)
	{
		for (z = 1; z < vetorGlobalC-1; ++z)
		{
			/*20% de chance de ter '#' por vetor valido*/
			if (2==RAND(2,2) && MatrizTabuleiro[i][z]!='+')
			{
				MatrizTabuleiro[i][z] = '#';
			}
			/*5% de chance de ter '@' por vetor valido*/
			if (2==RAND(10,10) && MatrizTabuleiro[i][z]!='+')
			{
				MatrizTabuleiro[i][z] = '@';
				contadorInimigo+=1;
			}
		}
		/*caso nao haja inimigos gere ao menos um*/
		if (contadorInimigo==0 && i==vetorGlobalL-2){
			contadorInimigo=1;
			MatrizTabuleiro[1][1] = '@';
		}
	}
}

/*Programa Principal*/
int main (){
	/*inicia número aleatorio*/
	srand(time(0));
	/* Inicia Contadores e Vetores*/
	int i, z, vetor, vetorBombaLinha, vetorBombaColuna, bombaAcionada=0;
	/*Inicia contadores de tempo*/
	time_t tempoReal = time(NULL);
	time_t tempoDetonamento;
	/*Configurando o Tabuleiro e atualizando em tempo real*/
	time_t tempoExecucao = time(NULL) - tempoReal;
	/*Inicia o Tabuleiro e tamanho (funciona melhor com impares por estetica) */
	printf("Insira Tamanho do Tabuleiro:\n");
	scanf("%d", &vetor);
	if (vetor<5){
		printf("Valor Invalido, insira um numero maior que 5!\n");
		EndGame=5;
	}
	char Tabuleiro[vetor][vetor*2];
	vetorGlobalL = vetor;
	vetorGlobalC = vetor*2;
	/*declarando funcoes*/
	void on_start(char Tabuleiro[vetorGlobalL][vetorGlobalC], int vetorGlobalL, int vetorGlobalC);
	void movimentos(char Tabuleiro[vetorGlobalL][vetorGlobalC], int vetorX, int vetorY);
	/*Iniciando Tabuleiro*/
	on_start(Tabuleiro, vetorGlobalL, vetorGlobalC);
	Tabuleiro[vetor-2][1] = '&';
	playerLinha = vetor-2;
	/*Loop para rodar o jogo por 200segundos*/
	while(tempoExecucao <= 200 && EndGame==0){
		if (contadorInimigo==0)
			EndGame = 3;
		/*Checa se a bomba explodiu neste momento*/
		else if (tempoDetonamento<=time(NULL) && bombaAcionada==1)
		{
			/*faz son*/
			printf("\a");
			bombaAcionada=0;
			for (i = vetorBombaLinha-1; i < vetorBombaLinha+2; ++i)
		    {
		        for (z = vetorBombaColuna-1; z < vetorBombaColuna+2; ++z)
		        {
		            if (Tabuleiro[i][z]!='|' && Tabuleiro[i][z]!='_' && Tabuleiro[i][z]!='+')
		            {
		            	if (Tabuleiro[i][z]=='&')
		            		EndGame=4;
		            	if (Tabuleiro[i][z]=='@')
		            		contadorInimigo--;
		                Tabuleiro[i][z] = ' ';
		            }
		        }
		    }
		}
		/*Monitora os movimentos do Player*/
        char Input;
		if (Input=='s' && (Tabuleiro[playerLinha-1][playerColuna]== ' ' || Tabuleiro[playerLinha-1][playerColuna]=='@'))
		{
			movimentos(Tabuleiro, 0, -1);
		}
		if (Input=='a' && (Tabuleiro[playerLinha][playerColuna-1]== ' ' || Tabuleiro[playerLinha][playerColuna-1]=='@'))
		{
			movimentos(Tabuleiro, -1, 0);
		}
		if (Input=='x' && (Tabuleiro[playerLinha+1][playerColuna]== ' ' || Tabuleiro[playerLinha+1][playerColuna]=='@'))
		{
			movimentos(Tabuleiro, 0, 1);
		}
		if (Input=='d' && (Tabuleiro[playerLinha][playerColuna+1]== ' ' || Tabuleiro[playerLinha][playerColuna+1]=='@'))
		{
			movimentos(Tabuleiro, 1, 0);
		}
		/*saida do jogo*/
		if (Input=='e')
			EndGame=5;
		/*bomba!*/
		if (Input=='b')
		{
			/*verifica se ha alguma bomba ativa*/
			if (bombaAcionada==0)
			{
				/*Muda cor para Verde*/
				printf("\033[1;31m");
				printf("\a");
				Tabuleiro[playerLinha][playerColuna] = '*';
				vetorBombaLinha = playerLinha;
				vetorBombaColuna = playerColuna;
				tempoDetonamento = time(NULL)+RAND(MEDIA_TEMPO, ERRO_TEMPO);
				bombaAcionada=1;
			}
		}
    /*---------------------------------------------------------------------------------------------------------------------------------------*/
		/*limpa a tela de acordo com SO*/
        system(CLEAR);
        system(CLEAR);
		/*Imprime o tabuleiro e informacoes*/
		printf("\033[1;35m");
		printf("\nInstrucoes:\n\n\tInimigos>@\tParedes Indestrutiveis>+\tParedes Quebraveis>#\n\n");
		printf("Movimentacao:\n\n\tS-cima\tA-esquerda\tX-baixo\tD-direita\n\n\tB-bomba\t\tE-sair\n\n");
		printf("Inimigos restantes: %d\n", contadorInimigo);
		/*Muda cor*/
		printf("\033[1;32m");
		for (i = 0; i < vetor; ++i)
		{
			for (z = 0; z < vetor*2; ++z)
			{
				printf("%c", Tabuleiro[i][z]);
			}
		printf("\n");
		}
		printf("Tempo restante: %ld\n", 200-tempoExecucao);
		tempoExecucao = time(NULL) - tempoReal;
		scanf(" %c", &Input);
	}
	if (tempoExecucao==201)
		EndGame = 2;
	/*Muda cor*/
	printf("\033[1;36m");
	switch(EndGame){
		case 1:
			printf("\n---------------------------------\n");
			printf("You Died\n");
			printf("---------------------------------\n");
			break;
		case 2:
			printf("\n---------------------------------\n");
			printf("Tempo Encerrado!\n");
			printf("---------------------------------\n");
			break;
		case 3:
			printf("\n---------------------------------\n");
			printf("You Win!!!\n");
			printf("---------------------------------\n");
			break;
		case 4:
			printf("\n---------------------------------\n");
			printf("Kabum! voce virou cinzas!\n");
			printf("---------------------------------\n");
			break;
		case 5:
			break;
	}
	printf("\n---------------------------------\n");
	printf("Jogo Encerrado.\nAperte Enter Para sair.\n");
	printf("---------------------------------\n");
	getchar();/*armazena lixo*/
	getchar();/*pausa o codigo*/
	/*Volta cor ao normal*/
	printf("\033[0m");
    return 0;
}

/*funcao que controla a logistica de movimento in-game*/
void movimentos(char MatrizTabuleiro[vetorGlobalL][vetorGlobalC],int vetorX,int vetorY){
	if (MatrizTabuleiro[playerLinha][playerColuna]=='*')
	{
		playerColuna += vetorX;
		playerLinha += vetorY;
		if (MatrizTabuleiro[playerLinha][playerColuna]=='@')
		{
			EndGame=1;
		}
		MatrizTabuleiro[playerLinha][playerColuna] = '&';
	}
	else{
		MatrizTabuleiro[playerLinha][playerColuna] = ' ';
		playerColuna += vetorX;
		playerLinha += vetorY;
		if (MatrizTabuleiro[playerLinha][playerColuna]=='@')
		{
			EndGame=1;
		}
		MatrizTabuleiro[playerLinha][playerColuna] = '&';
	}
}
