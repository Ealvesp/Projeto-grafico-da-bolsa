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

#include <windows.h> /* ExitProcess() */
#include <stdio.h>  /* printf() e fprintf() */
#include "console_v1.5.4.h" /* cabe�alho de todas as novas fun��es definidas neste arquivo */


/* C avan�ado */
/* Usa chamadas a API do windows */
/* maiores informa��es podem ser consultadas no link:
http://msdn.microsoft.com/pt-br/library/windows/desktop/ms682073%28v=vs.85%29.aspx */




/* Retorna uma estrutura contendo o tamanho da janela de console atual:
   typedef struct _COORD{
                        SHORT X;   -> Largura da janela
                        SHORT Y;   -> Altura da janela
                        } COORD;
                        (
*/
COORD tamanhoJanelaConsole(void)
{
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    SMALL_RECT srWindow;
    COORD dimJanela;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if(! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
         printf("GetConsoleScreenBufferInfo(%ld)\n", GetLastError());

    srWindow = csbiInfo.srWindow;
    dimJanela.Y = srWindow.Bottom - srWindow.Top + 1;
    dimJanela.X = srWindow.Right - srWindow.Left + 1;

    return dimJanela;
}

/* Retorna a resolu��o atual do monitor */
COORD getResolucaoMonitor(void)
{
	HWND hDesktop; /* identificador da Desktop Window */
	RECT coordenadasDesktopWindow;
	COORD resolucaoMonitor;

	/* Armazenar o controlador da janela do Desktop */
	hDesktop = GetDesktopWindow();

	/* Armazenar as dimens�es do Desktop: left, top, right, bottom. Por exemplo: 0, 0, 1280, 800 */
	GetWindowRect(hDesktop, &coordenadasDesktopWindow);

	/* Armazenar a resolu��o atual do monitor em pixels */
	resolucaoMonitor.X = coordenadasDesktopWindow.right;
	resolucaoMonitor.Y = coordenadasDesktopWindow.bottom;

	return resolucaoMonitor;
}

/* Retorna uma estrutura contendo as dimens�es m�ximas poss�veis que a janela de console
   pode ter, em n�mero de caracteres, na resolu��o de tela atual e ser inteiramente vis�vel na tela.
*/
COORD MaxDimensaoJanela(void)
{
	HWND hConsole;  /* identificador da Console Window */
	HANDLE hStdout;
	CONSOLE_FONT_INFO ConsoleCurrentFont;
	COORD MaxDimensoes;

	/* Obter o identificador da janela de console */
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/* Obter o tamanho m�ximo poss�vel para a janela de console.
       Equivalente a escrever:
	   MaxDimensoes.X = GetSystemMetrics(SM_CXFULLSCREEN) / (float) ConsoleCurrentFont.dwFontSize.X;
	   MaxDimensoes.Y = GetSystemMetrics(SM_CYFULLSCREEN) / (float) ConsoleCurrentFont.dwFontSize.Y;
	*/
	MaxDimensoes = GetLargestConsoleWindowSize(hConsole);

	/* Ajustando essa dimens�es para ficarem dentro da �rea vis�vel:
       - na altura desconsiderar espa�o ocupado pela DesktopTaskBar (getSystemMetrics(SM_CYSIZEFRAME))
       - na largura desconsidera espa�o ocupado pelas bordas laterais da pr�pria janela	 */
	    /* armazenar o controlador da janela atual */
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		/* obter o tamanho da fonte atual */
        GetCurrentConsoleFont(hStdout, FALSE, &ConsoleCurrentFont);
	    MaxDimensoes.Y = MaxDimensoes.Y - ( GetSystemMetrics(SM_CYSIZEFRAME) / (float) ConsoleCurrentFont.dwFontSize.Y );
        MaxDimensoes.X = MaxDimensoes.X - ( GetSystemMetrics( SM_CYSIZEFRAME) * 2 / (float) ConsoleCurrentFont.dwFontSize.X);

    return(MaxDimensoes);
}


/* Recuperar a posi��o da janela de console atual na tela */
COORD getPosicaoJanela(void)
{

	COORD canto_superior_esquerdo; /* canto_superior_esquerdo.Y = linha superior da janela ;
                                      canto_superior_esquerdo.X = = coluna mais a esquerda da janela */
    HWND janela; /* vari�vel respons�vel por armazenar o identificador da janela de console */
    RECT rect; /* estrutura que conter� as posi��es do canto superior esquerdo da janela de console */

    /* Armazenar o controlador da janela atual */
	janela = GetConsoleWindow();

    /* Obter as coordenadas do canto superior da janela de console */
	GetWindowRect(janela, &rect);

    /* Armazenar as coordenadas do canto superior esquerdo da janela de console */
    canto_superior_esquerdo.Y = rect.top;
    canto_superior_esquerdo.X = rect.left;

	return (canto_superior_esquerdo);
}

/* definir a posi��o atual do console na tela */
void setPosicaoJanela(int left, int top)
{
     HWND janela; /* vari�vel respons�vel por armazenar o identificador da janela de console */

     /* Obter o identificador da janela de console */
	 janela = GetConsoleWindow();

	 /* Posicionar a janela nas coordenadas passadas como par�metro. Para detalhes da fun��o SetWindowPos
                  consultar: http://msdn.microsoft.com/en-us/library/ms633545%28VS.85%29.aspx	 */
	 SetWindowPos(janela,
	              HWND_TOP,
				  left,
				  top,
				  0, 0,
				  SWP_NOSIZE);
}

/* definir as dimens�es da janela de console */
void setDimensaoJanela(int largura_nova, int altura_nova)
{
    HANDLE hStdout;
    SMALL_RECT srWindow;
    COORD novoBufferSize;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	int largura_atual, altura_atual;

    /* armazenar o controlador da janela atual */
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);


	/* ajustar o buffer para a nova dimens�o da janela em n�mero de caracteres.
	   Isso porque, se o buffer n�o tiver um tamanho igual ou maior, em caracteres,
	   � largura e � altura da janela do prompt de comando, o Windows impede o redimensionamento
	   da janela */

        /*---------------------------------*/
		/* ajustar a nova altura da janela */
		/*---------------------------------*/
	   	  /* obter o tamanho do buffer da janela atual */
	         GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
		     largura_atual = csbiInfo.dwSize.X;
		     altura_atual = csbiInfo.dwSize.Y;
		  /* ajustar o buffer da altura */
		     novoBufferSize.X = largura_atual;
	         novoBufferSize.Y = altura_nova;
          /* armazenar a nova altura da janela do prompt de comando */
             srWindow.Top = 0;
             srWindow.Bottom = altura_nova - 1;
             srWindow.Left = 0;
             srWindow.Right = largura_atual - 1;
		if (altura_nova > altura_atual)
		{
		    /* primeiro redimensionar o buffer para depois redimensionar a janela */
               if (!SetConsoleScreenBufferSize(hStdout, novoBufferSize))
	               printf("Erro: SetConsoleScreenBufferSize(%ld) \n", GetLastError());
               if ( ! SetConsoleWindowInfo(hStdout, TRUE, &srWindow))
                   printf("Erro: SetConsoleWindowInfo(%ld) \n", GetLastError());
		}
		else if (altura_nova < altura_atual) /* especifiquei o if para n�o ter que executar nada quando for o caso altura_nova == altura_atual */
		{
		    /* primeiro redimensionar a janela para depois redimensionar o buffer */
               if ( ! SetConsoleWindowInfo(hStdout, TRUE, &srWindow))
                   printf("Erro: SetConsoleWindowInfo(%ld) \n", GetLastError());
               if (!SetConsoleScreenBufferSize(hStdout, novoBufferSize))
	               printf("Erro: SetConsoleScreenBufferSize(%ld) \n", GetLastError());
		}

        /*----------------------------------*/
		/* ajustar a nova largura da janela */
		/*----------------------------------*/
	   	  /* obter novamente o tamanho do buffer da janela atual. O buffer da altura pode ter sido alterado no ajuste da altura */
	         GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
		     altura_atual = csbiInfo.dwSize.Y;
		  /* ajustar o buffer da largura */
		     novoBufferSize.X = largura_nova;
	         novoBufferSize.Y = altura_atual;
          /* armazenar a nova largura da janela do prompt de comando */
             srWindow.Top = 0;
             srWindow.Bottom = altura_nova - 1;
             srWindow.Left = 0;
             srWindow.Right = largura_nova - 1;
		if (largura_nova > largura_atual)
		{
		    /* primeiro redimensionar o buffer para depois redimensionar a janela */
               if (!SetConsoleScreenBufferSize(hStdout, novoBufferSize))
	               printf("Erro: SetConsoleScreenBufferSize(%ld) \n", GetLastError());
               if ( ! SetConsoleWindowInfo(hStdout, TRUE, &srWindow))
                   printf("Erro: SetConsoleWindowInfo(%ld) \n", GetLastError());
		}
		else if (largura_nova < largura_atual) /* especifiquei o if para n�o ter que executar nada quando for o caso largura_nova == largura_atual */
		{
		    /* primeiro redimensionar a janela para depois redimensionar o buffer */
               if ( ! SetConsoleWindowInfo(hStdout, TRUE, &srWindow))
                   printf("Erro: SetConsoleWindowInfo(%ld) \n", GetLastError());
               if (!SetConsoleScreenBufferSize(hStdout, novoBufferSize))
	               printf("Erro: SetConsoleScreenBufferSize(%ld) \n", GetLastError());
		}


}

/* Definir se � para tornar visivel ou invisivel a barra de tarefas (task bar) */
void setEstadoBarraTarefas(VISIBILIDADE estado)
{
     HWND hwnd;

	 /* Armazenar um controlador para a barra de tarefas, com base no seu nome */
	 hwnd = FindWindow("Shell_TrayWnd", "");

	 /* Definir o estado de visibilidade da barra de tarefas conforme o valor passado como par�metro */
	 ShowWindow(hwnd, estado);
}

/* Retorna uma estrutura do tipo COORD onde:
   - Campo X de COORD contem a largura da fonte (em pixels) dos caracteres na janela do console
   - Campo Y de COORD contem a altura da fonte (em pixels) dos caracteres na janela do console
*/
COORD getTamanhoFonteConsole(void)
{
    HANDLE hStdout; /* controlador da janela de console */
	CONSOLE_FONT_INFO ConsoleCurrentFont; /* estrutura que armazenar� as dimens�es da fonte atual */

    /* armazenar o controlador da janela atual */
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    /* obter as dimensoes da fonte atual para o tamanho de janela atual (par�metro FALSE).
       Se retornar FALSE ent�o a fun��o falhou.	*/
	if (!GetCurrentConsoleFont(hStdout, FALSE, &ConsoleCurrentFont))
         ErrorExit("getTamanhoFonteConsole");

    return (ConsoleCurrentFont.dwFontSize);
}


/* Retornar o t�tulo da janela de console */
char *getTituloConsole(void)
{
     HWND janela;    /* vari�vel que armazenar� o identificador da janela de console */
	 LPTSTR titulo;  /* vari�vel que armazenar� o t�tulo da janela de console */
	 int numero_caracteres_titulo;

	 /* Obter o identificador da janela de console */
	 janela = GetConsoleWindow();

	 /* Obter quantos caracteres tem o t�tulo da janela */
	 numero_caracteres_titulo = GetWindowTextLength(janela);

	 /* Alocar espa�o para o texto do t�tulo mais o '\0'*/
	 titulo = (LPTSTR) malloc(numero_caracteres_titulo + 1);

	 /* Obter o t�tulo da janela atual */
     GetWindowText(janela, titulo, numero_caracteres_titulo + 1);

	 return titulo;
}

/* Definir o t�tulo da janela de console */
void setTituloConsole(char *titulo)
{
    HWND janela;    /* vari�vel que armazenar� o identificador da janela de console */

    /* Obter o identificador da janela de console */
	janela = GetConsoleWindow();

	/* Definir o titulo da janela como sendo a string passada como par�metro */
	SetWindowText(janela, titulo);
}

/* Definir se � para ligar ou desligar o cursor piscante na tela */
void setCursorStatus(STATUS s)
{
    CONSOLE_CURSOR_INFO info;
    HANDLE hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    char buf[255];
    DWORD dwError;
    static DWORD dwSizeCursor; /* vari�vel que armazenar� o tamanho atual do cursor para quando for lig�-lo */

    /* armazenar as informa��es sobre o cursor atualmente */
    GetConsoleCursorInfo(hOutput, &info);
    switch (s)
    {
            case LIGAR:
                 info.bVisible = TRUE; /* turn the cursor on */
                 info.dwSize = dwSizeCursor;

                 break;
            case DESLIGAR:
                 info.bVisible = FALSE; /* turn the cursor off */
                 dwSizeCursor = info.dwSize; /* armazenar o tamanho do cursor */
                 break;
    }
       if( SetConsoleCursorInfo(hOutput,&info) == 0 )
       {
           dwError = GetLastError();
           printf("SetConsoleCurInfo failed(%ld)", dwError);
           FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, dwError,0,buf,sizeof(buf),0);
           printf("%s", buf);
       }
}

/* Esta fun��o retorna as cores, do texto e de fundo, definidas para a janela de prompt atual */
CORES getCoresJanela(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	CORES cores;
    HANDLE hOutput;

    /* armazenar o controlador da janela atual */
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    /* armazenar as informa��es sobre o cursor atualmente */
    GetConsoleScreenBufferInfo(hOutput, &csbi);
	cores.CorTexto = (csbi.wAttributes & 0x0F);
	cores.CorFundo = (csbi.wAttributes & 0xF0) >> 4;

    return cores;
}

int hit(TYPE_HIT tipoEvento)
{
   HANDLE hStdin;
   DWORD fdwMode;
   DWORD nEventos;
   INPUT_RECORD *eventos;
   DWORD leidos;
   int existeEvento = 0;
   unsigned int i;

    /* Armazenar o controlador padr�o de entrada ( standard input handle ) */
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    /* Habilitar a entrada de eventos na janela de console e tamb�m de eventos do mouse */
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (! SetConsoleMode(hStdin, fdwMode) )
        ErrorExit("SetConsoleMode");

   /* Armazenar em nEventos o n�mero de eventos n�o lidos no buffer */
   GetNumberOfConsoleInputEvents(hStdin, &nEventos);

   if(nEventos > 0) /* se houver algum evento aguardando ser lido no buffer */
   {
      /* Alocar mem�ria para o n�mero de eventos existentes no buffer */
      eventos = (INPUT_RECORD *)malloc(nEventos * sizeof(INPUT_RECORD));
	  /* Ler todos os eventos do buffer sem remov�-los do buffer */
      PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), eventos, nEventos, &leidos);
	  /* Varrer todos os eventos existentes no buffer para identificar se h� algum daqueles que foi
	     pedido para ser identificado por meio do par�metro tipoEvento */
	  for(i = 0;  i < nEventos; i++)
	  {
	      if( ( eventos[i].EventType == KEY_EVENT && (tipoEvento == KEYBOARD_HIT || tipoEvento == ALL) )
		     ||
			  ( eventos[i].EventType == MOUSE_EVENT && (tipoEvento == MOUSE_HIT || tipoEvento == ALL) )
			 ||
			  ( eventos[i].EventType == WINDOW_BUFFER_SIZE_EVENT && (tipoEvento == ALL) )
			)
			 existeEvento = 1;
	  }
	  free( eventos ); /* liberar a mem�ria alocada para os eventos */
   }
   return existeEvento;
}


/*
*/
/* Fun��o que aguarda e retorna um evento originado, ou do teclado, ou do mouse */
EVENTO Evento(void)
{
    HANDLE hStdin;
    DWORD cNumRead, fdwMode;
    INPUT_RECORD irInBuf;
	EVENTO Dados_evento;
	static DWORD status_anterior_botoes = 0;

    /* Armazenar o controlador padr�o de entrada ( standard input handle) */
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");


    /* Habilitar a entrada de eventos na janela de console e tamb�m de eventos do mouse */
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (! SetConsoleMode(hStdin, fdwMode) )
        ErrorExit("SetConsoleMode");


	/* Esperar pelos eventos */
	if (! ReadConsoleInput(	hStdin,      /* input buffer handle */
							&irInBuf,    /* Buffer onde os eventos lidos ser�o armazenados */
							1,           /* tamanho do buffer de leitura, no caso, irInBuf */
							&cNumRead) )
		ErrorExit("ReadConsoleInput");


	/* identificar e tratar adequadamente o evento lido */
	switch(irInBuf.EventType)
	{
		   case KEY_EVENT: /* keyboard input  */
			    Dados_evento.tipo_evento = KEY_EVENT;
				Dados_evento.teclado.codigo_tecla = irInBuf.Event.KeyEvent.wVirtualKeyCode; /* armazenar o c�digo da tecla pressionada */
                if (irInBuf.Event.KeyEvent.bKeyDown)
				    Dados_evento.teclado.status_tecla = PRESSIONADA;
				else
                    Dados_evento.teclado.status_tecla = LIBERADA;
                Dados_evento.teclado.status_teclas_controle = irInBuf.Event.KeyEvent.dwControlKeyState;

                /* se foi pressionada a tecla SHIFT guardar se foi pressionado o SHIFT ESQUERDO ou SHIFT DIREITO. Ver: http://msdn.microsoft.com/pt-br/library/windows/desktop/ms646301%28v=vs.85%29.aspx */
                if((GetKeyState(VK_LSHIFT) & BIT_SHORT_MAIS_SIGNIFICATIVO) == TECLA_PRESSIONADA)  /* se o bit mais significativo de um n�mero de tamanho short (2 bytes) tiver o valor 1 ent�o � porque a tecla est� pressionada */
				   Dados_evento.teclado.status_teclas_controle |= SHIFT_ESQUERDO_PRESSIONADO;
                if((GetKeyState(VK_RSHIFT) & BIT_SHORT_MAIS_SIGNIFICATIVO) == TECLA_PRESSIONADA)  /* se o bit mais significativo de um n�mero de tamanho short (2 bytes) tiver o valor 1 ent�o � porque a tecla est� pressionada */
				   Dados_evento.teclado.status_teclas_controle |= SHIFT_DIREITO_PRESSIONADO;
			    break;

		   case MOUSE_EVENT: /* mouse input */
			    Dados_evento.tipo_evento = MOUSE_EVENT; /* armazenar que foi gerado um evento do mouse */
			    Dados_evento.mouse.posicao = irInBuf.Event.MouseEvent.dwMousePosition; /* armazenar a posi��o na tela em que o usu�rio clicou */
			    Dados_evento.mouse.botao_pressionado = irInBuf.Event.MouseEvent.dwButtonState; /* armazenar o bot�o pressionado */

                Dados_evento.mouse.status_teclas_controle = irInBuf.Event.MouseEvent.dwControlKeyState;
                /* se foi pressionada a tecla SHIFT guardar se foi pressionado o SHIFT ESQUERDO ou SHIFT DIREITO. Ver: http://msdn.microsoft.com/pt-br/library/windows/desktop/ms646301%28v=vs.85%29.aspx */
                if((GetKeyState(VK_LSHIFT) & BIT_SHORT_MAIS_SIGNIFICATIVO) == TECLA_PRESSIONADA)  /* se o bit mais significativo de um n�mero de tamanho short (2 bytes) tiver o valor 1 ent�o � porque a tecla est� pressionada */
				   Dados_evento.teclado.status_teclas_controle |= SHIFT_ESQUERDO_PRESSIONADO;
                if((GetKeyState(VK_RSHIFT) & BIT_SHORT_MAIS_SIGNIFICATIVO) == TECLA_PRESSIONADA)  /* se o bit mais significativo de um n�mero de tamanho short (2 bytes) tiver o valor 1 ent�o � porque a tecla est� pressionada */
				   Dados_evento.teclado.status_teclas_controle |= SHIFT_DIREITO_PRESSIONADO;

				   Dados_evento.mouse.flags_qual_evento_mouse = irInBuf.Event.MouseEvent.dwEventFlags;

				/* Identificar se foi liberado um bot�o que tenha sido pressionado anteriormente, ou seja,
                   foi dado um clique com aquele bot�o. Somente foi escrito c�digo para reconhecer o clique de
				   3 bot�es de um mouse. Mouses que tenham mais bot�es, os demais bot�es n�o ser�o reconhecidos */
				Dados_evento.mouse.botao_clicou = 0;
				if ((status_anterior_botoes & FROM_LEFT_1ST_BUTTON_PRESSED) == 1 && (Dados_evento.mouse.botao_pressionado & FROM_LEFT_1ST_BUTTON_PRESSED) == 0)
				     Dados_evento.mouse.botao_clicou +=  FROM_LEFT_1ST_BUTTON_RELEASED;
				if (((status_anterior_botoes & RIGHTMOST_BUTTON_PRESSED) >> 1) == 1 && (Dados_evento.mouse.botao_pressionado & RIGHTMOST_BUTTON_PRESSED) == 0)
				     Dados_evento.mouse.botao_clicou +=  RIGHTMOST_BUTTON_RELEASED;
				if (((status_anterior_botoes & FROM_LEFT_2ND_BUTTON_PRESSED) >> 2) == 1 && (Dados_evento.mouse.botao_pressionado & FROM_LEFT_2ND_BUTTON_PRESSED) == 0)
				     Dados_evento.mouse.botao_clicou +=  FROM_LEFT_2ND_BUTTON_RELEASED;
				status_anterior_botoes = irInBuf.Event.MouseEvent.dwButtonState;
                break;

           case WINDOW_BUFFER_SIZE_EVENT: /* scrn buf. resizing */
                /* printf("Evento de redimensionamento"); */
                break;

           case FOCUS_EVENT:  /* descartar focus events */

           case MENU_EVENT:   /* descartar menu events */
                break;

           default:
                ErrorExit("Unknown event type");
                break;
        }

	return Dados_evento;
}


/* fun��o que coleta e apresenta todas as informa��es pass�veis de serem coletadas sobre um
   evento e, seja ele um evento do teclado, ou do mouse.
   H� duas possibilidades do n�vel de detalhamento das informa��es que a fun��o pode apresentar:
   MINIMO e MAXIMO (constantes definidas como #define)  */
void IdentificarEvento(EVENTO e, int nivel_detalhe)
{
     switch(e.tipo_evento) /* identificar o tipo do evento recebido como par�metro */
	 {
	        case MOUSE_EVENT:
			     printf("\nEvento do mouse");
				 printf("\n-------------------");
				 printf("\n-> Posicao: %d (Linha), %d (Coluna)", e.mouse.posicao.Y, e.mouse.posicao.X);
				 printf("\n\n-> Botoes do mouse:");
				 printf("\n   - Primeiro botao da esquerda para a direita esta pressionado: %s", (e.mouse.botao_pressionado & FROM_LEFT_1ST_BUTTON_PRESSED)?("SIM"):("NAO"));
				 printf("\n   - Segundo botao da esquerda para a direita foi pressionado: %s", (e.mouse.botao_pressionado & FROM_LEFT_2ND_BUTTON_PRESSED)?("pressionado"):("nao pressionado"));
				 printf("\n   - Terceiro botao da esquerda para a direita foi pressionado: %s", (e.mouse.botao_pressionado & FROM_LEFT_3RD_BUTTON_PRESSED)?("pressionado"):("nao pressionado"));
				 printf("\n   - Quarto botao da esquerda para a direita foi pressionado: %s", (e.mouse.botao_pressionado & FROM_LEFT_4TH_BUTTON_PRESSED)?("pressionado"):("nao pressionado"));
				 printf("\n   - Botao mais a direita esta pressionado: %s", (e.mouse.botao_pressionado & RIGHTMOST_BUTTON_PRESSED)?("SIM"):("NAO"));
                 if (nivel_detalhe > 1)
                 {
				     printf("\n\n-> Status das teclas de controle quando o evento ocorreu:");
				     printf("\n   - Caps Lock: %s", (e.mouse.status_teclas_controle & CAPSLOCK_ON)?("ligada"):("desligada"));
				     printf("\n   - ENHANCED KEY (del, ins, pg up, pg down, setas de direcao, etc): %s", (e.mouse.status_teclas_controle & ENHANCED_KEY)?("ligada"):("desligada"));
				     printf("\n   - ALT esquerdo: %s", (e.mouse.status_teclas_controle & LEFT_ALT_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - CTRL esquerdo: %s", (e.mouse.status_teclas_controle & LEFT_CTRL_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - Num Lock: %s", (e.mouse.status_teclas_controle & NUMLOCK_ON)?("ligado"):("desligado"));
				     printf("\n   - ALT direito: %s", (e.mouse.status_teclas_controle & RIGHT_ALT_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - CTRL direito: %s", (e.mouse.status_teclas_controle & RIGHT_CTRL_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - Scroll Lock: %s", (e.mouse.status_teclas_controle & SCROLLLOCK_ON)?("ligado"):("desligado"));
				     printf("\n   - SHIFT: %s", (e.mouse.status_teclas_controle & SHIFT_PRESSED)?("pressionada"):("nao pressionada"));
                     printf("\n     * SHIFT ESQUERDO: %s", (e.mouse.status_teclas_controle & SHIFT_ESQUERDO_PRESSIONADO)?("pressionada"):("nao pressionada"));
                     printf("\n     * SHIFT DIREITO: %s", (e.mouse.status_teclas_controle & SHIFT_DIREITO_PRESSIONADO)?("pressionada"):("nao pressionada"));
                 }

                 printf("\n\n-> Tipo do evento do mouse:");
				 if (e.mouse.flags_qual_evento_mouse == 0)
				 {
				    printf("\n   - Primeiro botao da esquerda para a direita clicou: %s", (e.mouse.botao_clicou & FROM_LEFT_1ST_BUTTON_RELEASED)?("SIM"):("NAO"));
     				printf("\n   - Botao mais a direita clicou: %s", (e.mouse.botao_clicou & RIGHTMOST_BUTTON_RELEASED)?("SIM"):("NAO"));
				 }
				 else
				 {
				     printf("\n   - Double click: %s", (e.mouse.flags_qual_evento_mouse & DOUBLE_CLICK)?("sim"):("nao"));
				     printf("\n   - Mouse foi movimentado: %s", (e.mouse.flags_qual_evento_mouse & MOUSE_MOVED)?("sim"):("nao"));
				     printf("\n   - Scroll do mouse foi movimentado: %s", (e.mouse.flags_qual_evento_mouse & MOUSE_WHEELED)?("sim"):("nao"));
					 if (e.mouse.flags_qual_evento_mouse & MOUSE_WHEELED) /* se foi movimentado o cursor por meio da scroll do mouse */
					 {
					     /* identificar se foi um scroll para frente ou para tr�s */
						 if ((int) e.mouse.botao_pressionado > 0) /* se a high word for positiva ent�o � para frente */
						     printf("\n     * Scroll para frente");
						 else /* caso contr�rio, foi feito um scroll para tr�s (em dire��o ao usu�rio) */
    						 printf("\n     * Scroll para tras");
					 }
                 }
				 printf("\n-------------------");
			     break;

			case KEY_EVENT:
			     printf("\nEvento do teclado");
				 printf("\n-------------------");
				 printf("\n-> Codigo da tecla: %lu", e.teclado.codigo_tecla);
				 if (e.teclado.status_tecla == PRESSIONADA)
				     printf("\n   * Tecla pressionada");
				 else
				     printf("\n   * Tecla liberada");
                 if (nivel_detalhe > 1)
                 {
				     printf("\n\n-> Status das teclas de controle quando o evento ocorreu:");
				     printf("\n   - Caps Lock: %s", (e.teclado.status_teclas_controle & CAPSLOCK_ON)?("ligada"):("desligada"));
				     printf("\n   - ENHANCED KEY (del, ins, pg up, pg down, setas de direcao, etc): %s", (e.teclado.status_teclas_controle & ENHANCED_KEY)?("ligada"):("desligada"));
				     printf("\n   - ALT esquerdo: %s", (e.teclado.status_teclas_controle & LEFT_ALT_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - CTRL esquerdo: %s", (e.teclado.status_teclas_controle & LEFT_CTRL_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - Num Lock: %s", (e.teclado.status_teclas_controle & NUMLOCK_ON)?("ligado"):("desligado"));
				     printf("\n   - ALT direito: %s", (e.teclado.status_teclas_controle & RIGHT_ALT_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - CTRL direito: %s", (e.teclado.status_teclas_controle & RIGHT_CTRL_PRESSED)?("pressionada"):("nao pressionada"));
				     printf("\n   - Scroll Lock: %s", (e.teclado.status_teclas_controle & SCROLLLOCK_ON)?("ligado"):("desligado"));
				     printf("\n   - SHIFT: %s", (e.teclado.status_teclas_controle & SHIFT_PRESSED)?("pressionada"):("nao pressionada"));
                     printf("\n     * SHIFT ESQUERDO: %s", (e.teclado.status_teclas_controle & SHIFT_ESQUERDO_PRESSIONADO)?("pressionada"):("nao pressionada"));
                     printf("\n     * SHIFT DIREITO: %s", (e.teclado.status_teclas_controle & SHIFT_DIREITO_PRESSIONADO)?("pressionada"):("nao pressionada"));
				 }
				 printf("\n-------------------");
                 break;
	 }
}

/* imprime uma mensagem de erro no fluxo de erro e encerra o programa */
void ErrorExit (LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);
    ExitProcess(0);
}
