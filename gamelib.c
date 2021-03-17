#include "gamelib.h"

/* Questo file contiene le definizioni di tutte le funzioni da implementare.
   Quelle visibili in main crea_mappa(), gioca(), termina_gioco() hanno linkage esterno.
   Tutte le funzioni non visibili in main.c e definite qui, devono avere linkage interno.
   Tutte le variabili globali definite in questo file devono avere linkage interno.
   Per esempio, la matrice con zone/probabilita' */
static const int matrice_ogg[6][6]={{30,20,40,00,00,10},
									{20,10,10,30,00,30},
									{20,10,30,00,30,10},
									{80,00,10,00,10,00},
									{70,00,10,00,20,00},
									{90,00,10,00,00,00}};
static void ins_zona();
static void canc_zona();
static void stampa_mappa();
static void chiudi_mappa();
static void avanza();
static void mostra_oggetto();
static void prendi_oggetto();
static void cura();
static void usa_adrenalina();
static void apparizione_gieson();
static void situazione();
static void strategia_benzina_giacomo();
static void strategia_benzina_marzia();
static void caso_benzina();
static void game_over();

static Zona *nuova_zona = NULL;
static Zona *prima_zona = NULL;
static Zona *ultima_zona = NULL;
static Zona *pPrev = NULL;
static Zona *pScan = NULL;
static Zona *pCanc = NULL;
static Zona *zona_corrente_giacomo=NULL;
static Zona *zona_corrente_marzia=NULL;
static Zona *zona_oggetto_giacomo=NULL;
static Zona *zona_oggetto_marzia=NULL;

static Giocatore marzia;
static Giocatore giacomo;

static int num_zone=0;
static int zona_scelta=0;
static int turno_casuale=0;
static int turni_giacomo=0;
static int turni_marzia=0;
static int giacomo_ha_finito=0;
static int marzia_ha_finito=0;
static int turni_giacomo_rimanenti=1;
static int turni_marzia_rimanenti=1;
static int usaadrenalina=0;
static int turni=1;
static int probabilita_gieson=0;
static int probabilita=0;
static int usa_benzina=0;

void crea_mappa (void) 
{
	int azione=0;
	printf("il numero di zone è: %d\n\n",num_zone);
	
		do
		{
			printf("\tInservieni sulla mappa\n");
			printf("\t╔═══════════════════════╗\n");
			printf("\t║ 1.Inserisci una zona  ║\n");
			printf("\t║ 2.Cancella una zona   ║\n");
			printf("\t║ 3.Stampa la mappa     ║\n");
			printf("\t║ 4.Chiudi la mappa     ║\n");
			printf("\t╚═══════════════════════╝\n");
			scanf("%d",&azione);
			scanf("%*[^\n]");
		
				switch(azione)
				{
					case 1:clear(); num_zone++;	scelta_zona();
					break;
					case 2:clear(); num_zone--;	canc_zona();
					break;
					case 3:clear(); printf("\tMappa creata\n");	stampa_mappa();
					break;
					case 4:clear(); chiudi_mappa();
					break;
					default:clear(); printf("Inserisci un numero valido\n\n");
					break;
				}
		}
		while(!(azione >0 && azione <5));
}

void scelta_zona()
{
		do
		{
		printf("\tInserisci la zona\n");
		printf("\t╔═════════════════════╗\n");
		printf("\t║ 1.cucina            ║\n");
		printf("\t║ 2.soggiorno         ║\n");
		printf("\t║ 3.rimessa           ║\n");
		printf("\t║ 4.strada            ║\n");
		printf("\t║ 5.lungo_lago        ║\n");
		printf("\t║ 6.uscita_campeggio  ║\n");
		printf("\t╚═════════════════════╝\n");
		scanf("%d",&zona_scelta);
		scanf("%*[^\n]");
		
			switch(zona_scelta)
					{
						case 1:clear(); ins_zona();
						break;
						case 2:clear(); ins_zona();
						break;
						case 3:clear(); ins_zona();
						break;
						case 4:clear(); ins_zona();
						break;
						case 5:clear(); ins_zona();
						break;
						case 6:clear(); ins_zona();
						break;
						default:clear(); printf("Inserisci un numero valido\n");
						break;
					}
		}
		while(!(zona_scelta >0 && zona_scelta <7));
}

void ins_zona()
{	
		nuova_zona = (Zona*) malloc(sizeof(Zona));
		nuova_zona->zona_successiva=NULL;
		
		if(prima_zona == NULL)
		{
			prima_zona = nuova_zona;
			ultima_zona = prima_zona;
			ultima_zona->zona=zona_scelta-1;
			f_random();
		}
		else
		{
			ultima_zona->zona_successiva=(Zona*)nuova_zona;
			ultima_zona=nuova_zona;
			ultima_zona->zona=zona_scelta-1;
			f_random();
		}
		crea_mappa();
}

void canc_zona()
{
	if(prima_zona==NULL)
		{
		printf("Non hai ancora creato zone\n");
		num_zone=0;
		crea_mappa();
		}	
	else
	{
		pPrev= NULL;
		pScan= prima_zona;
	
		if(pScan->zona_successiva==NULL)
		{
			free(pScan);
			prima_zona=NULL;
			printf("unica zona creata eliminata\n");
		}
		else
		{
			printf("zona eliminata\n");
			do
			{
				if(((Zona*)pScan->zona_successiva)==ultima_zona)
				{
					pPrev=pScan;
					break;
				}
				else
					pScan=(Zona*)pScan->zona_successiva;
			}
			while((pScan->zona_successiva)!=NULL);
		
			free(pPrev->zona_successiva);
			pPrev->zona_successiva = NULL;
			ultima_zona= pPrev;
		}
		
		crea_mappa ();
	}
}

void stampa_mappa()
{
	if(prima_zona==NULL)
	{
		printf("Non hai inserito nessuna zona\n");
		crea_mappa();
	}
	else
	{
		pScan=prima_zona;
	
		printf("\t╔═════════════════════════════════════════════════════════════════╗\n");
		printf("\t║         ZONA        ║       OGGETTO       ║ IND.ZONA SUCCESSIVA ║\n");
		printf("\t╚═════════════════════════════════════════════════════════════════╝\n");
		
		do
		{
		
			if(pScan->zona==0)	{printf("\t║        Cucina       "); visualizza_oggetto();}
			if(pScan->zona==1)	{printf("\t║       Soggiorno     "); visualizza_oggetto();}
			if(pScan->zona==2)	{printf("\t║        Rimessa      "); visualizza_oggetto();}
			if(pScan->zona==3)	{printf("\t║        Strada       "); visualizza_oggetto();}
			if(pScan->zona==4)	{printf("\t║      Lungo_lago     "); visualizza_oggetto();}
			if(pScan->zona==5)	{printf("\t║   Uscita_campeggio  "); visualizza_oggetto();}
				
			if(pScan->zona_successiva==NULL) 	{printf("\t%p             ║\n",pScan->zona_successiva);}
			else 								{printf("\t%p         ║\n",pScan->zona_successiva);}
		
			pScan=(Zona*)pScan->zona_successiva;
		}
		while(pScan!=NULL);
		
		printf("\t╚═════════════════════════════════════════════════════════════════╝\n");
		
		int si_no;
		
		do
		{
			printf("\n\ttornare alla schermata precedente?");
			printf("\n\t╔══════╗\n");
			printf("\t║ 1.SI ║\n");
			printf("\t║ 2.NO ║\n");
			printf("\t╚══════╝\n");
			scanf("%d",&si_no);
			scanf("%*[^\n]");
		
			switch(si_no)
					{
						case 1:clear(); crea_mappa();
						break;
						case 2:clear(); stampa_mappa();
						break;
						default:clear(); printf("Inserisci un numero valido\n"); stampa_mappa();
						break;
					}
		}
		while(!(si_no ==1 || si_no==2));
	}
}

void visualizza_oggetto()
{
	if(pScan->oggetto==0) {printf("║    Cianfrusaglia    ║");}	
	if(pScan->oggetto==1) {printf("║        Bende        ║");}
	if(pScan->oggetto==2) {printf("║      Coltello       ║");}
	if(pScan->oggetto==3) {printf("║       Pistola       ║");}
	if(pScan->oggetto==4) {printf("║       Benzina       ║");}
	if(pScan->oggetto==5) {printf("║      Adrenalina     ║");}
}

void chiudi_mappa()
{
	if(num_zone<8)
	{
		printf("Inserire almeno 8 zone\n");
		crea_mappa();
	}
	else if (ultima_zona->zona!=5)
	{
		printf("Per chiudere la mappa come ultima scelta deve essere uscita_campeggio\n");
		crea_mappa();
		
	}
	else
	{
		printf("Creazione mappa riuscita\n");
		main();
	}
}

void f_random()
{
	int num_casuale;
	srand(time(NULL));
	num_casuale=rand()%100;
	enumeratore_oggetti(num_casuale);
}

void enumeratore_oggetti(int num_casuale)
{		
	int ind_z,ind_o;
	ind_z=ultima_zona->zona;
	
		for(ind_o=0; ind_o<6; ind_o++)
		{
			if (num_casuale < matrice_ogg[ind_z][ind_o])
				{
					ultima_zona->oggetto=ind_o;
					break;
				}
			else if (matrice_ogg[ind_z][ind_o]!=0)
				{
					num_casuale=num_casuale - matrice_ogg[ind_z][ind_o];
				}
		}
}

int turno_random()
{
	int numero_random;
	srand(time(NULL));
	numero_random=rand()%100;
	return numero_random;
}

void gioca()
{

	if(num_zone<8)
	{
		printf("Inserire almeno 8 zone\n");
		main();
	}
	else if (usaadrenalina>0)
		{
			usaadrenalina--;
			
			if(turno_casuale<50)
			{
				//clear();
				printf("TURNO DI GIACOMO\n");
				apparizione_gieson();
				mosse();
			}
			else 
			{
				//clear();
				printf("TURNO DI MARZIA\n");
				apparizione_gieson();
				mosse();
			}
		}
	else if(giacomo.posizione==NULL && marzia.posizione==NULL)
		{
			turno_casuale=turno_random();
			inizializzazione();
		
			if(turno_casuale<50)
			{
				giacomo.posizione=prima_zona;
				//clear();
				printf("TURNO DI GIACOMO\n");
				mosse();
			}
			else 
			{
				marzia.posizione=prima_zona;
				//clear();
				printf("TURNO DI MARZIA\n");
				mosse();
			}
		}
	else if(giacomo.posizione==NULL)
		{
			giacomo.posizione=prima_zona;
			//clear();
			printf("TURNO DI GIACOMO\n");
			turno_casuale=10;
			mosse();
		
		}
		else if(marzia.posizione==NULL)
			{
				marzia.posizione=prima_zona;
				//clear();
				printf("TURNO DI MARZIA\n");
				turno_casuale=99;
				mosse();
			}
			
			apparizione_gieson();
			
		if(giacomo.stato==morto && marzia.stato==morto)
		{
			//clear();
			printf("\nHai perso\n");
			main();
		}
		else if(giacomo.stato==morto || giacomo_ha_finito==1)
			{
				//clear();
				printf("TURNO DI MARZIA\n");
				turno_casuale=99;
				mosse();
			}
			else if(marzia.stato==morto || marzia_ha_finito==1)
			{
				//clear();
				printf("TURNO DI GIACOMO\n");
				turno_casuale=10;
				mosse();
			}	
			else if(turni_giacomo_rimanenti==turni_marzia_rimanenti)
				{
					turno_casuale=turno_random();
		
					if(turno_casuale<50)
					{
						//clear();
						printf("TURNO DI GIACOMO\n");
						turni_giacomo_rimanenti--;
						mosse();
					}
					else 
					{
						//clear();
						printf("TURNO DI MARZIA\n");
						turni_marzia_rimanenti--;
						mosse();
					}
				}
				else if(turni_giacomo_rimanenti==0 && turni_marzia_rimanenti==1)
					{
						printf("TURNO DI MARZIA\n");
						turni_giacomo_rimanenti=1;
						turni_marzia_rimanenti=1;
						turno_casuale=99;
						mosse();
					}
					else if(turni_marzia_rimanenti==0 && turni_giacomo_rimanenti==1)
						{
							printf("TURNO DI GIACOMO\n");
							turni_giacomo_rimanenti=1;
							turni_marzia_rimanenti=1;
							turno_casuale=10;
							mosse();
						}
						else if(turni_giacomo_rimanenti==0 && turni_marzia_rimanenti==2)
							{
								printf("TURNO DI MARZIA\n");
								turni_giacomo_rimanenti=0;
								turni_marzia_rimanenti=2;
								turno_casuale=99;
								mosse();
							}
							else if(turni_marzia_rimanenti==0 && turni_giacomo_rimanenti==2)
								{
									printf("TURNO DI GIACOMO\n");
									turni_giacomo_rimanenti=2;
									turni_marzia_rimanenti=0;
									turno_casuale=10;
									mosse();
								}
}

void mosse()
{
	int mossa;
	situazione();
		do
		{	
			printf("\tChe mossa vuoi fare?\n");
			printf("\t╔═════════════════════╗\n");
			printf("\t║ 1.avanza            ║\n");
			printf("\t║ 2.mostra oggetto    ║\n");
			printf("\t║ 3.prendi oggetto    ║\n");
			printf("\t║ 4.cura              ║\n");
			printf("\t║ 5.usa adrenalina    ║\n");
			printf("\t╚═════════════════════╝\n");
			scanf("%d",&mossa);
			scanf("%*[^\n]");
			
				switch(mossa)
				{
					case 1: clear(); turni++; avanza();
					break;
					case 2: clear(); turni++; mostra_oggetto();
					break;
					case 3:	clear(); turni++; prendi_oggetto();
					break;
					case 4: clear(); turni++; cura();
					break;
					case 5: clear(); turni++; usa_adrenalina();
					break;
					default:printf("Inserisci valore valido\n"); situazione();
					break;
				}
		}
		while(!(mossa>0 && mossa<6));
}


static void avanza()
{
	if(turno_casuale<50)
	{
		if(giacomo.posizione->zona_successiva==NULL && marzia.posizione->zona_successiva==NULL)
		{	
			printf("Hai vinto \n");
			main();
		}
		else if(giacomo.posizione->zona_successiva==NULL)
			{	
				giacomo_ha_finito=1;
				turni_giacomo_rimanenti=0;
				turni_marzia_rimanenti=2;
				printf("Giacomo è riuscito ad uscire\n");
					
				if(marzia.stato==morto || marzia_ha_finito==1)
				{
					printf("Hai vinto\n");
					main();
				}
				else
				{
					turno_casuale=99;
				}
					
				printf("TURNO DI MARZIA\n");
				mosse();
			}
			else if(giacomo_ha_finito==0)
				{
					zona_corrente_giacomo=giacomo.posizione;
					giacomo.posizione=giacomo.posizione->zona_successiva;
					//clear();
					printf("Giacomo ha avanzato\n");
					gioca();
				}
				else
				{
					turno_casuale=99;
					turni_giacomo--;
					turni_marzia++;
					printf("TURNO DI MARZIA\n");
					mosse();
				}
	}
	else
	{
		if(marzia.posizione->zona_successiva==NULL && giacomo.posizione->zona_successiva==NULL)
		{
			printf("Hai vinto \n");
			main();
		}
		else if(marzia.posizione->zona_successiva==NULL)
			{	
				marzia_ha_finito=1;
				turni_giacomo_rimanenti=2;
				turni_marzia_rimanenti=0;
				printf("Marzia è riuscita ad uscire\n");
				
				if(giacomo.stato==morto || giacomo_ha_finito==1)
				{
					printf("Hai vinto\n");
					main();
				}
				else
				{
					turno_casuale=10;
				}
				
				printf("TURNO DI GIACOMO\n");
				mosse();
			}
			else if(marzia_ha_finito==0)
				{
					zona_corrente_marzia=marzia.posizione;
					marzia.posizione=marzia.posizione->zona_successiva;
					//clear();
					printf("Marzia ha avanzato\n");
					gioca();
				}
				else
				{
					turno_casuale=10;
					printf("TURNO DI GIACOMO\n");
					mosse();
				}
	}
	
	
}

static void mostra_oggetto()
{
	if(turno_casuale<50)
	{	
		zona_oggetto_giacomo=giacomo.posizione;
		printf("Hai mostrato l'oggetto nascosto nella zona\n");
		gioca();
	}
	else 
	{		
		zona_oggetto_marzia=marzia.posizione;
		printf("Hai mostrato l'oggetto nascosto nella zona\n");
		gioca();
	}
	
}

static void prendi_oggetto()
{
	if(turno_casuale<50)
	{
		if(zona_oggetto_giacomo==giacomo.posizione)
		{
			giacomo.zaino[giacomo.posizione->oggetto]++;
			giacomo.posizione->oggetto=0;
			turni_giacomo++;
			gioca();
		}
		else 
		{
			printf("Devi prima mostrare l'oggetto\n");
			turni--;
			mosse();
		}
	}
	else if(zona_oggetto_marzia==marzia.posizione)
		{
			marzia.zaino[marzia.posizione->oggetto]++;
			marzia.posizione->oggetto=0;
			turni_marzia++;
			gioca();			
		}
		else 
		{
			printf("Devi prima mostrare l'oggetto\n");
			turni--;
			mosse();
		}
}

static void usa_adrenalina()
{
	if(turno_casuale<50)
	{
		if(giacomo.zaino[adrenalina]>0)
		{	
			giacomo.zaino[adrenalina]--;
			usaadrenalina=2;
			printf("Giacomo ha utilizzato adrenalina\n");
			gioca();
		}
		else
		{
			printf("Non hai a disposizione nessuna unità di adrenalina\n");
			turni--;
			mosse();
		}
	}
	else
	{
		if(marzia.zaino[adrenalina]>0)
		{	
			marzia.zaino[adrenalina]--;
			usaadrenalina=2;
			printf("Marzia ha utilizzato adrenalina\n");
			gioca();
		}
		else
		{
			printf("Non hai a disposizione nessuna unità di adrenalina\n");
			turni--;
			mosse();
		}
	
	}
}

int numero_random_gieson()
{
	int numero_random_gieson;
	numero_random_gieson=rand()%100;
	return numero_random_gieson;
}

static void apparizione_gieson()
{
	if(usa_benzina>0)
	{
		usa_benzina--;
	}
	else
	{
		
		if(marzia.stato==morto || marzia_ha_finito==1)
		{
			probabilita_gieson=numero_random_gieson();
				
			probabilita=50;
			
			if(probabilita_gieson<probabilita)
			{	
				printf("Gieson è apparso\n");
										
				if(giacomo.zaino[pistola]>0 && giacomo.zaino[benzina]>0)
				{
					strategia_benzina_giacomo();						
				}
				else 
				{
					if(giacomo.zaino[pistola]>0)
					{	
						giacomo.zaino[pistola]--;
						printf("Gieson è apparso, giacomo aveva la pistola e l'ha respinto\n");
					}
					else if(giacomo.zaino[benzina]>0)
						{	
							giacomo.zaino[benzina]--;
							if(turni%2==0)
							usa_benzina=2;
							else
							usa_benzina=3;
							printf("Giacomo ha usato benzina\n");
						}
						else if(giacomo.zaino[coltello]>0)
							{	
								if(giacomo.stato==ferito)
								{
									printf("Giacomo è morto\n");
									giacomo.stato=morto;
								}
								else if (giacomo.stato==vivo)
									{	
										giacomo.stato=ferito;
										giacomo.zaino[coltello]--;
										printf("Gieson è apparso,giacomo aveva il coltello e l'ha respinto\n");
									}	
							}
							else
							{
								giacomo.stato=morto;
								printf("Giacomo è morto\n");
							}
				}
			}
			else
			{
				printf("Gieson non è apparso, è andata bene\n");
			}		
		}
		else if(giacomo.stato==morto || giacomo_ha_finito)
			{
				probabilita_gieson=numero_random_gieson();
		
				probabilita=50;
					
				if(probabilita_gieson<probabilita)
				{	
					printf("Gieson è apparso\n");				
				
					if(marzia.zaino[pistola]>0 && marzia.zaino[benzina]>0)
					{
						strategia_benzina_marzia();						
					}
					else
					{
						if(marzia.zaino[pistola]>0)
						{	
							marzia.zaino[pistola]--;
							printf("Gieson è apparso, marzia aveva la pistola e l'ha respinto\n");
						}
						else if(marzia.zaino[benzina]>0)
							{	
								marzia.zaino[benzina]--;
								if(turni%2==0)
								usa_benzina=2;
								else
								usa_benzina=4;
								printf("Marzia ha usato benzina\n");
							}
							else if(marzia.zaino[coltello]>0)
								{	
									if(marzia.stato==ferito)
									{
										printf("Marzia è morta\n");
										marzia.stato=morto;
									}
									else if (marzia.stato==vivo)
									{	
										marzia.stato=ferito;
										marzia.zaino[coltello]--;
										printf("Gieson è apparso,marzia aveva il coltello e l'ha respinto, ma è rimasta ferita\n");
									}	
								}
								else
								{
								marzia.stato=morto;
								printf("Marzia è morta\n");
								}
					}	
				}
				else
				{
					printf("Gieson non è apparso, è andata bene\n");
				}
					
			}
			else if(turni%2==1 && turni!=1)
				{
					probabilita_gieson=numero_random_gieson();
			
					if(turno_casuale<50)
					{	
						if(giacomo.posizione->zona!=5)
						{
							probabilita=30;
						}
						else
						{
							probabilita=75;
						}
						if(probabilita_gieson<probabilita)
						{	
							printf("Gieson è apparso\n");				
				
							if (usa_benzina>0)
							{
								printf("Gieson non può entrare\n");
							}
							else
							{
							
								if(giacomo.zaino[pistola]>0 && giacomo.zaino[benzina]>0)
								{
									printf("Giacomo scegli");
									strategia_benzina_giacomo();
								}
								else
								{
							
								 	if(giacomo.zaino[pistola]>0)
									{	
										giacomo.zaino[pistola]--;
										printf("Gieson è apparso, giacomo aveva la pistola e l'ha respinto\n");
									}
									else if(giacomo.zaino[benzina]>0)
										{	
											giacomo.zaino[benzina]--;
											if(turni%2==0)
											usa_benzina=2;
											else
											usa_benzina=3;
											printf("Giacomo ha usato benzina\n");
										}
									else if(giacomo.zaino[coltello]>0)
										{	
											if(giacomo.stato==ferito)
											{
												printf("Giacomo è morto\n");
												giacomo.stato=morto;
											}
											else if (giacomo.stato==vivo)
												{	
													giacomo.stato=ferito;
													giacomo.zaino[coltello]--;
													printf("Gieson è apparso,giacomo aveva il coltello e l'ha respinto\n");
												}	
										}
										else 
										{
											giacomo.stato=morto;
											printf("Giacomo è morto\n");
										}
								}
							}
							
								if (usa_benzina>0)
								{
									printf("Gieson non può entrare, dentro\n");
								}
								else	
								{	
									if(marzia.zaino[pistola]>0 && marzia.zaino[benzina]>0)
									{
										printf("Marzia scegli");
										strategia_benzina_marzia();						
									}
									else 
									{
							
										if(marzia.zaino[pistola]>0)
										{	
											marzia.zaino[pistola]--;
											printf("Gieson è apparso, marzia aveva la pistola e l'ha respinto\n");
										}
										else if(marzia.zaino[benzina]>0)
											{	
												marzia.zaino[benzina]--;
												if(turni%2==0)
												usa_benzina=2;
												else
												usa_benzina=3;
												printf("Marzia ha usato benzina\n");
											}	
											else if(marzia.zaino[coltello]>0)
												{	
													if(marzia.stato==ferito)
													{
														printf("Marzia è morta\n");
														marzia.stato=morto;
													}
													else if (marzia.stato==vivo)
														{	
															marzia.stato=ferito;
															marzia.zaino[coltello]--;
															printf("Gieson è apparso, marzia aveva il coltello e l'ha respinto\n");
														}	
												}
												else 
												{
													marzia.stato=morto;
													printf("Marzia è morta\n");
												}
						
									}
								}
						}
						else
						{
							printf("Gieson non è apparso, è andata bene\n");
						}
					}
					else
					{
						if(marzia.posizione->zona!=5)
						{
							probabilita=30;
						}
						else
						{
							probabilita=75;
						}
			
							if(probabilita_gieson<probabilita)
							{	
								printf("Gieson è apparso\n");				
							
								if (usa_benzina>0)
								{
									printf("Gieson non può entrare\n");
								}
								else		
								{
									if(giacomo.zaino[pistola]>0 && giacomo.zaino[benzina]>0)
									{
										printf("Giacomo scegli");
										strategia_benzina_giacomo();						
									}
									else 
									{
							
										if(giacomo.zaino[pistola]>0)
										{	
											giacomo.zaino[pistola]--;
											printf("Gieson è apparso, giacomo aveva la pistola e l'ha respinto\n");
										}
										else if(giacomo.zaino[benzina]>0)
											{	
												giacomo.zaino[benzina]--;
												if(turni%2==0)
												usa_benzina=2;
												else
												usa_benzina=3;
												printf("Giacomo ha usato benzina\n");
											}
											else if(giacomo.zaino[coltello]>0)
												{	
													if(giacomo.stato==ferito)
													{
														printf("Giacomo è morto\n");
														giacomo.stato=morto;
													}
													else if (giacomo.stato==vivo)
														{
															giacomo.stato=ferito;
															giacomo.zaino[coltello]--;
															printf("Gieson è apparso,giacomo aveva il coltello e l'ha respinto\n");
														}	
												}
												else
												{
													giacomo.stato=morto;
													printf("Giacomo è morto\n");
												}
									}
								}
							
								if (usa_benzina>0)
								{
									printf("Gieson non può entrare\n");
								}
								else
								{	
									if(marzia.zaino[pistola]>0 && marzia.zaino[benzina]>0)
									{
										printf("Marzia scegli");
										strategia_benzina_marzia();						
									}
									else
									{
										if(marzia.zaino[pistola]>0)
										{	
											marzia.zaino[pistola]--;
											printf("Gieson è apparso, marzia aveva la pistola e l'ha respinto\n");
										}
										else if(marzia.zaino[benzina]>0)
											{	
												marzia.zaino[benzina]--;
												if(turni%2==0)
												usa_benzina=2;
												else
												usa_benzina=3;
												printf("Marzia ha usato benzina\n");
											}
											else if(marzia.zaino[coltello]>0)
												{	
													if(marzia.stato==ferito)
													{
														printf("Marzia è morta\n");
														marzia.stato=morto;
													}
													else if (marzia.stato==vivo)
														{	
															marzia.stato=ferito;
															marzia.zaino[coltello]--;
															printf("Gieson è apparso, marzia aveva il coltello e l'ha respinto\n");
														}	
												}
												else
												{
													marzia.stato=morto;
													printf("Marzia è morta\n");
												}
									}
								}
							}
							else
							{
								printf("Gieson non è apparso, è andata bene\n");
							}
		
					}	
				}
			
		
	}
}

static void strategia_benzina_giacomo()
{
	int scelta_strategia;
	situazione();
			do
			{
				printf("\n\tGiacomo quale oggetto vuoi scegliere?");
				printf("\n\t\t╔═══════════╗\n");
				printf("\t\t║ 1.Benzina ║\n");
				printf("\t\t║ 2.Pistola ║\n");
				printf("\t\t╚═══════════╝\n");
				scanf("%d",&scelta_strategia);
				scanf("%*[^\n]");
		
				switch(scelta_strategia)
						{
							case 1: caso_benzina();
							break;
							case 2: giacomo.zaino[pistola]--;
									clear();
									printf("Gieson è apparso, giacomo aveva la pistola e l'ha respinto dopo averla scelta\n");	
							break;
							default: printf("Inserisci un valore valido\n"); strategia_benzina_giacomo();
							break;
						}
			}
			while(!(scelta_strategia==1 || scelta_strategia==2));

}

static void strategia_benzina_marzia()
{
	int scelta_strategia;
	situazione();
			do
			{
				printf("\n\tMarzia quale oggetto vuoi scegliere?");
				printf("\n\t\t╔═══════════╗\n");
				printf("\t\t║ 1.Benzina ║\n");
				printf("\t\t║ 2.Pistola ║\n");
				printf("\t\t╚═══════════╝\n");
				scanf("%d",&scelta_strategia);
				scanf("%*[^\n]");
		
				switch(scelta_strategia)
						{
							case 1: caso_benzina();
							break;
							case 2: marzia.zaino[pistola]--;
									clear();
									printf("Gieson è apparso, marzia aveva la pistola e l'ha respinto dopo averla scelta\n");
							break;
							default: printf("Inserisci un valore valido\n"); strategia_benzina_marzia();
							break;
						}
			}
			while(!(scelta_strategia==1 || scelta_strategia==2));

}

static void caso_benzina()
{
	if(turno_casuale<50)
	{
		if(giacomo.zaino[benzina]>0)
		{	
			giacomo.zaino[benzina]--;
			
			if(turni%2==0)
			usa_benzina=2;
			else
			usa_benzina=3;
			turni_giacomo++;
			clear();
			printf("Giacomo ha usato benzina\n");
			gioca();
			
		}
	}
	else
	{
		if(marzia.zaino[benzina]>0)
		{	
			marzia.zaino[benzina]--;
			if(turni%2==0)
			usa_benzina=2;
			else
			usa_benzina=3;
			turni_marzia++;
			clear();
			printf("Marzia ha usato benzina\n");
			gioca();
		}
	}
}


static void cura()
{

	if(turno_casuale<50)
	{
		if(giacomo.zaino[bende]>0)
		{	
			giacomo.zaino[bende]--;
			giacomo.stato=vivo;
			turni_giacomo++;
			printf("Giacomo ha utilizzato una benda ed è stato curato\n");
			gioca();
		}
		else
		{	
			turni--;
			printf("Giacomo non ha bende\n");
			mosse();
		}
	}
	else
	{
		if(marzia.zaino[bende]>0)
		{	
			marzia.zaino[bende]--;
			marzia.stato=vivo;
			turni_marzia++;
			printf("Marzia ha utilizzato una benda ed è stato curato\n");
			gioca();
		}
		else
		{	
			turni--;
			printf("Marzia non ha bende\n");
			mosse();
		}
	}
}

void termina_gioco()
{
	if(prima_zona !=NULL)
	{
	
		do
		{
			pCanc=prima_zona->zona_successiva;
			free(prima_zona);
			prima_zona=pCanc;
		}
		while(prima_zona !=NULL);
		
		game_over();
	}
	else
	{
		printf("Vuoi già terminare la partita prima di iniziarla? :)\n");
		main();
	}

}

static void game_over()
{
	clear();

	printf("     _______  _______  _______  _______    _______           _______  _______    \n"
		   "    |  ____ ||  ___  ||       ||  ____ |  |  ___  |||     |||  ____ ||  ____ |   \n"
		   "    | |    ||| |   | || || || || |    ||  | |   | || |   | || |    ||| |    ||   \n"
		   "    | |      | |___) || || || || |__      | |   | || |   | || |__    | |____||   \n"
		   "    | | ____ |  ___  || ||_|| ||  __|     | |   | || |   | ||  __|   |     __|   \n"
		   "    | | |_  || |   | || |   | || |        | |   | | | |_| | | |      | || |      \n"
		   "    | |___| || |   | || |   | || |____||  | |___| |  |   |  | |____||| || |      \n"
		   "    |_______|||     ||||     |||_______|  |_______|   |_|   |_______|| || |__|	 \n");
}

void inizializzazione()
{
		giacomo.stato=vivo;
		giacomo.zaino[cianfrusaglia]=0;
		giacomo.zaino[bende]=0;
		giacomo.zaino[coltello]=1;
		giacomo.zaino[pistola]=0;
		giacomo.zaino[benzina]=0;
		giacomo.zaino[adrenalina]=0;
		
		marzia.stato=vivo;
		marzia.zaino[cianfrusaglia]=0;
		marzia.zaino[bende]=0;
		marzia.zaino[coltello]=0;
		marzia.zaino[pistola]=0;
		marzia.zaino[benzina]=0;
		marzia.zaino[adrenalina]=2;
}

static void situazione()
{
	
	int do_situazione=0;
	printf("\tTurno numero %d\n",turni);
	printf("\t╔════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
	printf("\t║    GIOCATORE     ║       STATO      ║       ZONA       ║       OGGETTO    ║      ZAINO         ║\n");
	printf("\t╚════════════════════════════════════════════════════════════════════════════════════════════════╝\n");

	if(turno_casuale<50)
	{
		printf("\t║     GIACOMO      ║");
		if 		(giacomo.stato==2) {printf("       VIVO       ║");}
		else if (giacomo.stato==1) {printf("      FERITO      ║");}
		else if	(giacomo.stato==0) {printf("      MORTO       ║");}
		if		(giacomo.posizione -> zona ==0) {printf("     CUCINA       ║");}
		else if	(giacomo.posizione -> zona ==1) {printf("     SOGGIORNO    ║");}
		else if	(giacomo.posizione -> zona ==2) {printf("     RIMESSA      ║");}
		else if	(giacomo.posizione -> zona ==3) {printf("     STRADA       ║");}
		else if	(giacomo.posizione -> zona ==4) {printf("     LUNGO LAGO   ║");}
		else if	(giacomo.posizione -> zona ==5) {printf(" USCITA CAMPEGGIO ║");}
		
		
		if(zona_oggetto_giacomo==giacomo.posizione)
		{
			     if	(giacomo.posizione->oggetto==0) {printf("  CIANFRUSAGLIA   ║");}
			else if	(giacomo.posizione->oggetto==1) {printf("     BENDE        ║");}
			else if	(giacomo.posizione->oggetto==2) {printf("     COLTELLO     ║");}
			else if	(giacomo.posizione->oggetto==3) {printf("     PISTOLA      ║");}
			else if	(giacomo.posizione->oggetto==4) {printf("     BENZINA      ║");}
			else if	(giacomo.posizione->oggetto==5) {printf("     ADRENALINA   ║");}
		}
		else
		{
			printf("     NASCOSTO     ║");
		}
		
		printf(" [%d] CIANFRUSAGLIA  ║\n",giacomo.zaino[0]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] BENDE          ║\n",giacomo.zaino[1]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] COLTELLO       ║\n",giacomo.zaino[2]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] PISTOLA        ║\n",giacomo.zaino[3]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] BENZINA        ║\n",giacomo.zaino[4]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] ADRENALINA     ║\n",giacomo.zaino[5]);
		
		printf("\t╚════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
		
	}	
	else	
	{
	
		printf("\t║      MARZIA      ║");
		if		(marzia.stato==2) {printf("      VIVO        ║");}
		else if	(marzia.stato==1) {printf("     FERITO       ║");}
		else if	(marzia.stato==0) {printf("      MORTO       ║");}
		if		(marzia.posizione -> zona ==0) {printf("     CUCINA       ║");}
		else if	(marzia.posizione -> zona ==1) {printf("     SOGGIORNO    ║");}
		else if	(marzia.posizione -> zona ==2) {printf("     RIMESSA      ║");}
		else if	(marzia.posizione -> zona ==3) {printf("     STRADA       ║");}
		else if	(marzia.posizione -> zona ==4) {printf("    LUNGO LAGO    ║");}
		else if	(marzia.posizione -> zona ==5) {printf(" USCITA CAMPEGGIO ║");}
	
	
		if(zona_oggetto_marzia==marzia.posizione)
		{
			     if	(marzia.posizione->oggetto==0) {printf("  CIANFRUSAGLIA   ║");}
			else if	(marzia.posizione->oggetto==1) {printf("     BENDE        ║");}
			else if	(marzia.posizione->oggetto==2) {printf("    COLTELLO      ║");}
			else if	(marzia.posizione->oggetto==3) {printf("     PISTOLA      ║");}
			else if	(marzia.posizione->oggetto==4) {printf("     BENZINA      ║");}
			else if	(marzia.posizione->oggetto==5) {printf("   ADRENALINA     ║");}
	
		}	
		else
		{
			printf("     NASCOSTO     ║");
		}
		
		printf(" [%d] CIANFRUSAGLIA  ║\n",marzia.zaino[0]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] BENDE          ║\n",marzia.zaino[1]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] COLTELLO       ║\n",marzia.zaino[2]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] PISTOLA        ║\n",marzia.zaino[3]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] BENZINA        ║\n",marzia.zaino[4]);
		printf("        ║                  ║                  ║                  ║                  ║");
		printf(" [%d] ADRENALINA     ║\n",marzia.zaino[5]);
		
		printf("\t╚════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
		
	}

}
