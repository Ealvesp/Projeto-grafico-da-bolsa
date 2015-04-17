#include "graficos.h"/*incluindo o ponto h*/

void nome_console()/*nessecita de ajustes tratamento do A acentuado exemplo: á */
{
	setTituloConsole("Petrobras PN-15m-28/03/2015");/*funçao da biblioteca console_v1.5.4.h*/
	
}

void maximiza_console()
{
	COORD posi, maxd, tam_ini;/*variaveis do tipo COORD largura e altura*/
	
	tam_ini = tamanhoJanelaConsole();/*atribuição de procedimento*/
	maxd = MaxDimensaoJanela();/*atribuição de procedimento*/
	posi = getPosicaoJanela();/*atribuição de procedimento*/
	
	
	setDimensaoJanela(maxd.X, maxd.Y);/*chama função para maximizar a janela do console*/
	system ("pause");
	setPosicaoJanela(posi.X, posi.Y);/*coloca na posição inicial*/
	setDimensaoJanela(tam_ini.X, tam_ini.Y);/*coloca no tamanho inicial*/
}