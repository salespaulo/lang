#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <serpente.h>

// VARIAVEIS GLOBAIS
int INICIO, FIM, TAM, PONTOS, N = NNIVEIS-1;
char DIRECAO;

void printscr(void)
{
  clrscr();
  desliga_cursor();
  janelamenu();
}

void main()
{
  char cod, *itens[NITENS] = {"Jogo","Nivel","Recorde","Instrucoes","Sair"};
  int pos = 0;

  le_recorde();

  for( ; ; )
  {
     printscr();
     imprime(itens,pos);
     if((cod=getch())==0) cod = getch();
     
     switch(cod)
     {
       case P_ESQ:  pos = pos > 0 ? pos-1 : NITENS-1;   break;
       case TAB:    
       case P_DIR:  pos = pos < NITENS-1 ? pos+1 : 0;   break;
       case ENTER:  acao(pos);
     }
  }
}

void acao(int pos)
{
  switch(pos)
  {
     case 0:  JOGO();        break;
     case 1:  NIVEL();       break;
     case 2:  RECORDES();    break;
     case 3:  INSTRUCOES();  break;
     case 4:  liga_cursor();
              clrscr();
              exit(1);
  }
  limpajanela(2,4,78,24);  /* limpa area interna da janela menu */
  while(kbhit())  getch(); /* descarrega buffer do teclado */
}


void imprime(char *mat[], int pos)
{
   gotoxy(2,2);  
   
   for(int j = 0; j < NITENS; j++)
   {
      if(j==pos)  {  textcolor(BLACK);   textbackground(LIGHTGRAY);   }
      cprintf("    %s     ",*(mat+j));
      textcolor(LIGHTGRAY);
      textbackground(BLACK);  
   } 
}

int MOVIMENTA(void)
{
   coordenada aux;
   
   if(calc_nova_pos(&aux))  { FIM_JOGO();  return 0;  }
  
   if(comida.x==aux.x && comida.y==aux.y)  /* Atingiu o alvo? */
   {
     crescer(aux);     /* Incrementa serpente */
     nova_refeicao();  /* Cria outro alvo */
     PONTOS++;
   }
   else  desloca(aux);  /* Desloca serpente */
  
   return 1;
}

void JOGO(void)
{
  int TEMPO = N*12+20;
  INICIO = PONTOS = 0;
  FIM = TAM = TAM_INICIAL-1;
  DIRECAO = P_DIR;

  janelajogo(MIN_X, MIN_Y, MAX_X, MAX_Y);

  for(int i = 0; i <= TAM; i++)
  {
    serpente[i].x = (MIN_X+MAX_X)/2+i;
    serpente[i].y = (MIN_Y+MAX_Y)/2;
    gotoxy(serpente[i].x, serpente[i].y);  putch('1');
  }
  
  randomize();
  nova_refeicao();

  do
  {
     delay(TEMPO);
     if(kbhit())
       LER_TECLADO();
  } while(MOVIMENTA());
}

void FIM_JOGO(void)
{
//  limpajanela(2,4,78,24);
  gotoxy(4,8);  printf("\aPONTUACAO: %d", PONTOS);

  if(PONTOS > rec[N].pontos) /* Superou recorde anterior? */  
  {
     gotoxy(4,10);  printf("\aNOVO RECORDE");
     gotoxy(4,12);  printf("Digite seu nome:");

     liga_cursor();
     ler_nome(rec[N].nome);
     desliga_cursor();
   
     rec[N].pontos = PONTOS;   /* Salva pontuacao do jogador */
     grava_recorde();
  }
  else  delay(1500);  /* Atraso */
}

void LER_TECLADO(void)
{
  char tecla = getch();

  if(tecla=='p' || tecla=='P')  tecla = getch();  /* PAUSA jogo */
  if(tecla==0)
  {
    tecla = getch();
    if((tecla==P_ESQ   && DIRECAO!=P_DIR  ) ||
       (tecla==P_DIR   && DIRECAO!=P_ESQ  ) ||
       (tecla==P_CIMA  && DIRECAO!=P_BAIXO) ||
       (tecla==P_BAIXO && DIRECAO!=P_CIMA ))  DIRECAO = tecla;
  }
}

int calc_nova_pos(coordenada *aux)
{
   *aux = serpente[FIM]; 
   
   switch(DIRECAO)
   {
     case P_ESQ  :  aux->x--; break;
     case P_DIR  :  aux->x++; break;
     case P_CIMA :  aux->y--; break;
     case P_BAIXO:  aux->y++;
   }

   /* Verifica se a serpente atingiu uma das paredes */
   if(aux->x==MIN_X || aux->x==MAX_X || aux->y==MIN_Y || aux->y==MAX_Y)
     return 1; 

   /* Verifica se a serpente atingiu sua cauda */
   for(int i = 0; i <= TAM; i++)
      if(i!=INICIO && aux->x==serpente[i].x && aux->y==serpente[i].y)
        return 1;  

   return 0;
}

void crescer(coordenada aux)
{
   serpente[++TAM] = aux;   /* Acrescenta outro quadrado */
   
   /* Reordena serpente */
   if(FIM=TAM-1)  FIM = TAM;
   else
   {
      int fim = FIM;
      do {  aux = serpente[fim];         
            serpente[fim] = serpente[TAM];
            serpente[TAM] = aux;
            fim = fim ? fim-1 : TAM-1;
      } while(fim != TAM-1);
   } 
   
   sound(600);  
   delay(30);
   nosound();
   gotoxy(serpente[FIM].x, serpente[FIM].y);  putch('1');  /* Novo quadrado */
}

void desloca(coordenada aux)
{
   gotoxy(serpente[INICIO].x, serpente[INICIO].y);  putch(' ');
   serpente[INICIO] = aux;
   gotoxy(serpente[INICIO].x, serpente[INICIO].y);  putch('1');

   INICIO = INICIO == TAM ? 0 : INICIO+1;
   FIM = FIM == TAM ? 0 : FIM+1;
}

void nova_refeicao(void)
{
   int i;
   
   do {  comida.x = MIN_X+1+random(MAX_X-MIN_X-1);
         comida.y = MIN_Y+1+random(MAX_Y-MIN_Y-1);
         
         for(i = 0; i <= TAM; i++)
           if(comida.x==serpente[i].x && comida.y==serpente[i].y)  break;
      } while(i <= TAM);

   gotoxy(comida.x, comida.y);  putch('0');
}

void NIVEL(void)
{  int tecla = 0;
   gotoxy(4,4);  printf("NIVEL ATUAL: %d", N+1);
   gotoxy(4,6);  printf("NOVO NIVEL(1-%d): ", NNIVEIS);  

   liga_cursor();
   
   while((tecla < 1)||(tecla > NNIVEIS))
   {
     gotoxy(21,6);
     fflush(stdin);
     scanf("%d",&tecla);
   }

   N = tecla;
   //if(tecla >= '1' && tecla <= '0'+NNIVEIS)  N  = (tecla-'0')-1;

   delay(250);
   desliga_cursor();
}


void RECORDES(void)
{
   for(int i = 0; i < NNIVEIS; i++)
   {
      gotoxy(5,i+6);   printf("Nome:				"
			      "Pontos:				"
			      "Nivel:	");
      gotoxy(11,i+6);  printf("%s",rec[i].nome);
      gotoxy(40,i+6);  printf("%D",rec[i].pontos);
      gotoxy(66,i+6);  printf("%D",i+1);  
   }
   
   if(getch()==0)  getch();   /* PAUSA */
}

void INSTRUCOES(void)
{
   gotoxy(4,6);   printf("A serpente crescera se voce a alimentar.");
   gotoxy(4,7);   printf("Utilize as setas para controlar a serpente.");
   gotoxy(4,8);   printf("Nao atinja as paredes nem a cauda.");
   gotoxy(4,9);   printf("Tecla 'P' pausa o jogo.");

   if(getch()==0)  getch();   /* PAUSA */   
}


void janelamenu(void)
{
   for(int x = 2; x < 80; x++)  {  gotoxy(x,1);   putch('=');
				   gotoxy(x,3);   putch('=');
				   gotoxy(x,25);  putch('=');  }

   for(int y = 2; y < 25; y++)  {  gotoxy(1,y);   putch(')');
				   gotoxy(79,y);  putch(')');  }

   gotoxy(1,1);   putch('-');   gotoxy(79,1);     putch('-');
   gotoxy(1,3);   putch('-');   gotoxy(79,3);     putch('-');
   gotoxy(1,25);  putch('-');   gotoxy(79,25);    putch('-');
}

void janelajogo(int x1, int y1, int x2, int y2)
{
  for(int x = x1; x <= x2; x++)    {  gotoxy(x,y1);  putch(']');
				      gotoxy(x,y2);  putch(']');  }

  for(int y = y1+1; y <= y2; y++)  {  gotoxy(x1,y);  putch(']');
				      gotoxy(x2,y);  putch(']');  }
}