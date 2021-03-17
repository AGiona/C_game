#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define clear() printf("\033[H\033[J");
/* Questo file contiene le dichiarazioni delle funzioni crea_mappa(), gioca(), termina_gioco().
 Continene anche le definizioni dei tipi utilizzati in gamelib.c:
 struct Giocatore, struct Zona, enum Stato_giocatore, enum Tipo_zona, enum Tipo_oggetto. */

void main();
void crea_mappa();
void gioca();
void termina_gioco();

void f_random(void);
void scelta_zona(void);
void enumeratore_oggetti(int num_casuale);
void visualizza_oggetto(void);
void inizializzazione(void);
int turno_random();
void mosse();
int numero_random_gieson();


enum Stato_giocatore {morto, ferito, vivo};
enum Tipo_zona {cucina, soggiorno, rimessa, strada, lungo_lago, uscita_campeggio};
enum Tipo_oggetto {cianfrusaglia, bende, coltello, pistola, benzina, adrenalina};

typedef struct giocatore
{
	enum Stato_giocatore stato;
	struct zona *posizione;
	unsigned short zaino[6];
}Giocatore;

typedef struct zona
{
	enum Tipo_zona zona;
	enum Tipo_oggetto oggetto;
	struct zona *zona_successiva;
}Zona;
