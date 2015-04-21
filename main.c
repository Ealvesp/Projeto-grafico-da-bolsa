#include "graficos.h"/*incluindo o ponto h*/

int main (int argc, char *argv[])/*função principal*/
{
	EVENTO tecla;
	COORD tam_ini, posi;

	argc=argc;/*corrigir warnings de argumentos nao ultilzados */
	argv=argv;/*corrigir warnings de argumentos nao ultilzados */
	tam_ini = tamanhoJanelaConsole();/*atribuição de procedimento*/
	posi = getPosicaoJanela();/*atribuição de procedimento*/
	
	do 
	{	
		tecla = Evento();

		nome_console();/*chama procedimendo para mudar o nome do console*/
		maximiza_console();/*chamada do procedimento para maximizar janela do console*/
		velas();/*chama função para criar as velas*/

		if (tecla.teclado.codigo_tecla == ESC)/*verifica ponto de parada do programa*/
			{
				setPosicaoJanela(posi.X, posi.Y);/*coloca na posição inicial*/
				setDimensaoJanela(tam_ini.X, tam_ini.Y);/*coloca no tamanho inicial*/
				break;
			}
	}while(1);/*se verdade sai*/

	return(0);
}
