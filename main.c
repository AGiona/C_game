#include "gamelib.h"

/* Questo file contiene solo la stampa del menu principale e poi richiama una delle
 tre funzioni possibili: crea_mappa(), gioca(), termina_gioco(). */

void main () 
{
	int azione=0;
	do
	{
		
		printf("\nInserisci 1,2 o 3 a seconda dell'azione desiderata\n\n");
		printf("\t╔══════════════════╗\n");
		printf("\t║ 1.Crea mappa     ║\n");
		printf("\t║ 2.Gioca          ║\n");
		printf("\t║ 3.Termina gioco  ║\n");
		printf("\t╚══════════════════╝\n");
		scanf("%d",&azione);
		scanf("%*[^\n]");
		
			switch(azione)
			{
				case 1:clear(); printf("Per iniziare la partita inserire almeno 8 zone.\n"); crea_mappa();
				break;
				case 2:clear(); printf("2)Gioca\n"); gioca();
				break;
				case 3:clear(); printf("3)Termina gioco\n"); termina_gioco();
				break;
				default:clear(); printf("Inserisci un numero valido\n");
				break;
			}
	}
	while(!(azione >0 && azione <4));

}
