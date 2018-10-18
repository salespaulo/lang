#define   BACKSPACE		8
#define   TAB			9
#define   ENTER		       13
#define   ESC		       27
#define   P_CIMA	       72
#define   P_ESQ		       75
#define   P_DIR		       77
#define   P_BAIXO	       80
	
#define   MIN_X			8
#define   MIN_Y			4
#define   MAX_X		       72
#define   MAX_Y		       24	

#define   TAM_INICIAL		5
#define   NITENS		5
#define   NNIVEIS		6
#define   MAXNAME	       15	
#define   MAXQD		       ((MAX_X-MIN_X-1)*(MAX_Y-MIN_Y+1))	

struct coordenada  { int x, y }  comida, serpente[MAXQD];

struct recordes    { char nome[MAXNAME];
		     int pontos;  }  rec[NNIVEIS];	

void JOGO(void);
void NIVEL(void);
void INSTRUCOES(void);
void RECORDES(void);
void FIM_JOGO(void);
void LER_TECLADO(void);
void MOVIMENTACAO(void);

void janelamenu(void);
void janelajogo(int x1, int y1, int x2, int y2);
void imprime(char *mat[], int pos);
void acao(int pos);
void crescer(coordenada aux);
void desloca(coordenada aux);
void nova_refeicao(void);
int  calc_nova_pos(coordenada *aux);
void ler_nome(char *s);

void desliga_cursor(void);
void liga_cursor(void);
void limpajanela(char x1, char y1, char x2, char y2);
void grava_recorde(void);
void le_recorde(void);

void ler_nome(char *s)
{
  char tecla;
  int n = 0;
  
  while((tecla = getch()) != ENTER)
  {
    if(tecla == BACKSPACE && n)  {  cputs("\b \b");  n--;  }

    if(tecka != BACKSPACE && tecka != ESC && n < MAXNAME-1)
      putch(*(s+n++) = tecla);
  }
  *(s+n) = NULL;
}


void limpajanela(char x1, char y1, char x2, char y2)
{
  union REGS regs;
  
  regs.h.ah = 0x06;
  regs.h.al = y2-y1+1;  
  regs.h.bh = 7;    
  regs.h.ch = y1-1;
  regs.h.cl = x1-1;
  regs.h.dh = y2-1;
  regs.h.dl = x2-1;

  int86(0x01,&regs,&regs);
}

void liga_cursor(void)
{
  union REGS regs;
  regs.h.ah = 0x01;
  regs.h.ch = 6;
  regs.h.cl = 7;

  int86(0x01,&regs,&regs);
}

void desliga_cursor(void)
{
  union REGS regs;
  regs.h.ah = 0x01;
  regs.h.ch = 0x20;

  int86(0x01,&regs,&regs);
}


void le_recorde(void)
{
  FILE *fptr;
  
  if((fptr = fopen("maxponto.rec", "rb")) != NULL)
  {
    fread(&rec, sizeof(rec[0]), NNIVEIS, fptr);
    fclose(fptr);
  }
}

void grava_recorde(void)
{
  FILE *fptr;
  
  if((fptr = fopen("maxponto.rec", "wb")) != NULL)
  {
    fwrite(&rec, sizeof(rec[0]), NNIVEIS, fptr);
    fclose(fptr);
  }
}

