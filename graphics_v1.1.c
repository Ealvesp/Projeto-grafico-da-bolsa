


/* Esta diretiva foi colocada para que o programa que utilizar esta biblioteca possa ser compilado corretamente. 
     Isso porque a fun��o GetConsoleWindow precisa disso.
     A partir deste momento s� rodar� para Windows 2000 ou superior pois desabilitar� todas as outras fun��es que 
	  sejam espec�ficas de vers�es anteriores (Ex. win95, win98)	 
	 Observa��es: 
	 1) ao inv�s do c�digo 0x0500 poderia-se usar a macro Windows2000 definida em w32api.h pelo gcc para windows.
	     Contudo, essa macro pode estar definida com um nome em outros compiladores (borland, visual studio, etc). 
	     Portanto, para garantir maior compatibilidade, manteve-se a escrita do valor em hexadecimal, mesmo perdendo
	     na legibilidade final 
	 2) Os c�digos e significado dessa macro podem ser consultados em:
	      http://msdn.microsoft.com/en-us/library/aa383745.aspx */
#if (_WIN32_WINNT < 0x0500)
    #undef _WIN32_WINNT 
    #define _WIN32_WINNT 0x0500 /* 0x0500 corresponde a vers�o do Windows 2000 */
#endif

#include <windows.h> /* GetDC() dentro de winuser.h que � chamada em windows.h */
#include <math.h>

#include "graphics_v1.1.h" /* vetor SIN_ACOS e todos os prot�tipos das fun��es deste arquivo */

/* plotar 1 pixel nas coordenadas (x,y) com a cor "cor" */
void putpixel(int x, int y, COLORREF cor)
{
     HWND hWnd; /* handle Window */
     HDC hDC;   /* handle Device Context */

     hWnd = GetConsoleWindow();  /* armazenar o controlador da janela do prompt de comando */
     hDC = GetDC(hWnd);          /* armazenar o controlador da �rea de desenho na janela do prompt de comando */
	 
     SetPixel(hDC, x, y, cor);   /* Plotar o pixel na posi��o (x,y) com a cor "cor" */
   
     ReleaseDC(hWnd, hDC); /* liberar o controlador da �rea gr�fica */
}

/* retornar a cor de um pixel especificado pelas coordenadas x,y */
COLORREF getpixel(int x, int y)
{
     COLORREF cor;
	 
     HWND hWnd; /* handle Window */
     HDC hDC;   /* handle Device Context */

     hWnd = GetConsoleWindow();  /* armazenar o controlador da janela do prompt de comando */
     hDC = GetDC(hWnd);          /* armazenar o controlador da �rea de desenho na janela do prompt de comando */

	 cor = GetPixel(hDC, x, y);  /* armazenar a cor do pixel da coordenada (x,y) */
	 
     ReleaseDC(hWnd, hDC); /* liberar o controlador da �rea gr�fica */
	 
   return(cor);   
}

/* Algoritmo de Bresenham para desenho de um reta de cor 
   igual ao par�metro "cor", do ponto (x1,y1) ao ponto (x2,y2).
*/
void linha(int x1, int y1, int x2, int y2, COLORREF cor)
{
     int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

     dx = x2 - x1;      /* the horizontal distance of the line */
     dy = y2 - y1;      /* the vertical distance of the line */
     dxabs = abs(dx);
     dyabs = abs(dy);
     sdx = (dx >= 0)? +1: -1;
     sdy = (dy >= 0)? +1: -1;
     x = dyabs>>1;
     y = dxabs>>1;
     px = x1;
     py = y1;

     if( dxabs >= dyabs ) /* the line is more horizontal than vertical */
     {
         for( i = 0; i < dxabs; i++ )
         {
             y += dyabs;
             if( y >= dxabs )
             {
                 y -= dxabs;
                 py += sdy;
             }
             px += sdx;
             putpixel(px,py,cor);
         }
     }
     else /* the line is more vertical than horizontal */
     {
         for(i = 0; i < dyabs; i++)
         {
             x += dxabs;
             if( x >= dyabs )
             {
                 x -= dyabs;
                 px += sdx;
             }
             py += sdy;
             putpixel(px, py, cor);
         }
     }

}

/* Desenha um c�rculo de centro (x,y), com raio "raio" e a linha com a cor "cor" */
void circulo(int x,int y, int raio, COLORREF cor)
{

  double n = 0, invraio = 1/(float)raio;

  int dx = 0, dy = raio - 1;

  while( dx <= dy )
  {
	 putpixel( x + dy, y - dx, cor );  /* 1o octante */
	 putpixel( x + dx, y - dy, cor );  /* 2o octante */
	 putpixel( x - dx, y - dy, cor );  /* 3o octante */
	 putpixel( x - dy, y - dx, cor );  /* 4o octante */
	 putpixel( x - dy, y + dx, cor );  /* 5o octante */
	 putpixel( x - dx, y + dy, cor );  /* 6o octante */
	 putpixel( x + dx, y + dy, cor );  /* 7o octante */
	 putpixel( x + dy, y + dx, cor );  /* 8o octante */

	 dx++;
	 n += invraio;

	 dy = raio * sin(acos(n));
  }
}

