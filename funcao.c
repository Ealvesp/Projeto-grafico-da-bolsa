#include "graficos.h"/*incluindo o ponto h*/

void nome_console()/*nessecita de ajustes tratamento do A acentuado exemplo: á */
{
	setTituloConsole("Petrobras PN-15m-28/03/2015");/*funçao da biblioteca console_v1.5.4.h*/

}

void maximiza_console()
{
	COORD maxd;/*variaveis do tipo COORD largura e altura*/

	maxd = MaxDimensaoJanela();/*atribuição de procedimento*/
	
	setDimensaoJanela(maxd.X, maxd.Y);/*chama função para maximizar a janela do console*/
}
		
void velas()
{
	COLORREF cor_vermelha = RGB(255, 0, 0);
	COLORREF cor_verde = RGB(0, 255, 0);
	
	system("cls");
	linha(50*2, 50*2, 100*2, 100*2, cor_verde);
	putpixel(50, 100, cor_vermelha); 
	putpixel(110, 100, cor_verde);
	putpixel(130, 100, cor_vermelha); 
	putpixel(160, 100, cor_verde); 

}
void icone()
{
	int icone[16][16] ={
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {2, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2},
        {2, 2, 2, 1, 2, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2},
        {2, 2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2},
        {2, 2, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2},
        {2, 2, 1, 2, 2, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 2},
        {2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2,  1,1, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };

    int i, j;

    COLORREF cor;
    
    while(1)
    {
        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < 16; j++)
            {


                /*Escolhe cor amarela onde tem 0*/
                if (icone[i][j] == 0) cor = RGB(249,194,39);

                 /*Escolhe cor branca onde tem 1*/
                if (icone[i][j] == 1) cor = RGB(255,255,255);

                /*Escolhe cor verde onde tem 2*/
                if (icone[i][j] == 2) cor = RGB(8,118,73);

                putpixel(j,i,cor);

	     }
    	}
     }
}
