#include "graficos.h"/*incluindo o ponto h*/

void nome_console()/*nessecita de ajustes tratamento do A acentuado exemplo: á */
{
	setTituloConsole("Petrobras PN-15m-28/03/2015");/*funçao da biblioteca console_v1.5.4.h*/
	system ("pause");/*pausa a execução do programa */
}

void maximiza_console()/* incompleta  */
{
	COORD posi, maxd;/*variaveis do tipo COORD posição largura e altura*/
	
	maxd = MaxDimensaoJanela();/*atribuição de função do tipo COORD */
	posi = getPosicaoJanela();/*atribuição de função do tipo COORD */
	
	/*nao e nescessario da print nisso so pra esclarecer*/

	printf("posicao atual x: %d\n",posi.X);/*obtem posição atual do console*/
	printf("posicao atual y: %d\n",posi.Y);/*obtem posição atual do console*/
	printf("dimensao maxima x: %d\n", maxd.X);/*obtem o valor maximo de maximização do console*/
	printf("dimensao maxima y: %d\n", maxd.Y);/*obtem o valor maximo de maximização do console*/

	setDimensaoJanela(maxd.X, maxd.Y);/*chama função para maximizar a janela do console*/
	setDimensaoJanela(posi.X, posi.Y);/*dando erro na biblioteca do windows fica parado resolução maxima*/
			

}