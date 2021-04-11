/* Alcuni commenti sono tratti direttamente dal testo fornito dal professore */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

/* Definisco tutte le variabili globali. Le variabili sono "static" perché modificabili solo all'interno di gamelib.c */

static struct Giocatore *giocatori = NULL;   // puntatore all'array di "struct Giocatore" che viene creato dinamicamente in base a quanti giocatori si vogliono nel gioco
static unsigned short quest_da_finire;    // numero di quest da finire per vincere il gioco da parte degli astronauti
static struct Stanza *stanza_inizio = NULL;    // puntatore in cui viene memorizzata la prima stanza, dalla quale iniziano a giocare tutti i giocatori presenti
static struct Stanza *lista_stanze = NULL;   // puntatore che contiene il puntatore alla stanza di inizio, e che rappresenta la lista di tutte le "struct Stanze" create durante il gioco, inserite in ordine di creazione

static int n;   //numero di giocatori
static struct Giocatore *scorri_giocatori = NULL;    // puntatore che scorre l'array dei giocatori attivi
static int numero_impostori;   // numero di impostori
static int numero_stanze;   // numero di stanze
static int impostore1;
static int impostore2;
static int impostore3;
static int astronauti;    // numero di astronauti
static int turno;    // turno corrente, inizializzato a 1 per il primo
static int giro;    // numero di volte in cui tutti i giocatori hanno giocato
static struct Stanza *ultima_stanza_creata = NULL;
static int quest_completate;
static int quest_rimaste;

static int flag_impostato = 0;    // flag che viene azionato quando vengono inseriti i giocatori, e di conseguenza creata la stanza. Vale 1 quando sono state create
static int flag_pronto = 0;   // flag che serve per determinare se si può iniziare a giocare
static int flag_fineGioco = 0;    // flag che indica la fine del gioco. Vale 1 quando qualcuno vince
static int flag_compagnia = 0;    // flag che indica se nella stanza c'è un altro giocatore oppure no. Vale 1 se c'è qualcuno
static int flag_morto = 0;    // flag che indica se nella stanza sono presenti degli astronauti morti. Vale 1 se c'è un cadavere
static int flag_azioniAstronauta = 0;   // flag che serve a capire quale azioni stampare e in quale ordine per gli astronauti
static int flag_azioniImpostore = 0;    // flag che serve a capire quale azioni stampare e in quale ordine per gli impostori
static int flag_giocato = 0;    // flag che serve a capire se un giocatore ha giocato nel proprio turno
static int flag_defenestrato = 0;   // flag che indica se un giocatore è stato defenestrato. Vale 1 se lo è già stato


/* Definisco tutte le funzioni che verranno usate */

void imposta_gioco(void);    // funzione che serve per richiedere i parametri per impostare il gioco
void gioca(void);   // funzione da cui si possono chiamare tutte le azioni per giocare
void termina_gioco(void);   // funzione che serve per terminare il gioco e uscire

/* Azioni */
static struct Stanza* avanza(struct Stanza *stanza);    // funzione che fa scegliere al giocatore in quale direzione procedere rispetto alla stanza in cui si trova, oppure rimanere fermo
static void esegui_quest(struct Stanza *stanza);    // funzione che fa eseguire una quest se nella stanza ce n'è una presente
static void chiamata_emergenza(struct Stanza *stanza);    // funzione che defenestra uno e uno solo tra i presenti nella stanza al momento della chiamata
static void uccidi_astronauta(struct Giocatore *impostore);    // funzione che permette ad un impostore di uccidere uno degli astronauti presenti nella stessa stanza a scelta
static void usa_botola(struct Giocatore *impostore);    // funzione che permette ad un impostore di muoversi ad un'altra stanza a caso con una botola, oppure, se non ce ne sono, in una stanza a caso
static void sabotaggio(struct Stanza *stanza);    // funzione che permette ad un impostore di cambiare lo stato di una stanza da "quest_semplice" o "quest_complicata" a "vuota"

void stampa_menuPrimaVolta(void);
void stampa_menuPricipale(void);

static void stampa_menuImpostaGioco(void);
static void num_giocatori(void);    // funzione che richiede il numero di giocatori e crea l'array di "n" giocatori
static void shuffle(int array[], int n);    // funzione che mescola tra di loro tutti i valori di un array
static void swap(int *a, int *b);   // funzione utile a scambiare il valore tra due interi
static void tipo_stanza(struct Stanza *stanza);   // funzione che assegna in modo casuale lo stato della stanza
static void assegna_nome(int nome[], int n, struct Giocatore *giocatori, struct Stanza *inizio);   // funzione che assegna i nomi ai giocatori, e li colloca nella stanza iniziale
static int num_quest(void);   // funzione che chiede e assegna il numero di quest che ci saranno nella partita
static void stampa_giocatori(void);   // funzione che stampa tutte le informazioni relative ad ogni giocatore
static void stampa_statoGiocatore(struct Giocatore *stato);   // funzione che determina lo stato del giocatore. Fatta da una funzione per convenienza, dovesse servire in futuro
static void stampa_nomeGiocatore(struct Giocatore *nome);   // funzione che determina il nome del giocatore. Fatta da una funzione per convenienza, dovesse servire in futuro
static int inizia_gioco(void);    // funzione che controlla tutte le impostazioni e rimanda al menù principale, pronto per giocare

static void stampa_menuTurno(struct Giocatore *giocatore_attuale);
static void stampa_tipoStanza(struct Giocatore *tipo_stanza);
static void stampa_giocatoriPresenti(struct Giocatore *giocatore_presente);
static void stampa_azioni(struct Giocatore *giocatore_diTurno);
static void stampa_azioniAstronauta(struct Giocatore *astronauta);
static void scelta_astronauta(int scelta, struct Giocatore *astronauta);
static void stampa_azioniImpostore(struct Giocatore *impostore);
static void scelta_impostore(int scelta, struct Giocatore *impostore);
static void stampa_vittoriaAstronauti(void);    // funzione che viene chiamata quando gli astronauti eseguono tutte le quest, o defenestrano tutti gli impostori
static void stampa_vittoriaImpostori(void);    // funzione che viene chiamata quando gli impostori si liberano di tutti gli astronauti
static struct Stanza *cancella_stanza(struct Stanza *stanza);   // funzione ricorsiva che serve a cancellare le stanze
static struct Stanza *stanza_successiva(struct Stanza *stanza);   // funzione che permettere di visitare tutte le stanze per cancellarle


/* FUNZIONI */

void imposta_gioco(void) {
  if (flag_impostato == 1) {   // se le impostazioni sono state già messe, se si richiama "imposta_gioco()" viene prima deallocata tutta la memoria, tramite "termina_gioco()"
    termina_gioco();
  }

  int scelta_impostaGioco;
  int flag_impostaGioco = 1;

  printf("\n\nAdesso dovremo inserire i parametri per impostare il gioco.\n");

  do {
    stampa_menuImpostaGioco();
    scanf("%d", &scelta_impostaGioco);
    while(fgetc(stdin) != '\n');
    switch (scelta_impostaGioco) {
      case 1:   // num_giocatori
        if (flag_impostato == 1) {   // se i giocatori sono stati già inseriti, si cancellano poi si possono inserire di nuovo
          free(giocatori);

          num_giocatori();
        }
        else {
          num_giocatori();
        }
        break;
      case 2:  // num_quest
        quest_da_finire = num_quest();
        printf("Le quest da completare per vincere il gioco sono %hd.\n\n", quest_da_finire);
        break;
      case 3:   // stampa_giocatori
        stampa_giocatori();
        break;
      case 4:   // inizia_gioco
        flag_impostaGioco = inizia_gioco();
        break;
      default:
        printf("Il valore inserito non è valido.\nRiprova inserendo una delle opzioni disponibili.\nInserisci quindi un intero tra 1 e 4.\n\n");
        break;
    }
  } while (flag_impostaGioco != 0);
}

static void stampa_menuImpostaGioco(void) {
  /* ASCII Art - Imposta Gioco */
  {
    printf("\t  ___                           _           ____ _                 \n");
    printf("\t |_ _|_ __ ___  _ __   ___  ___| |_ __ _   / ___(_) ___   ___ ___  \n");
    printf("\t  | || '_ ` _ \\| '_ \\ / _ \\/ __| __/ _` | | |  _| |/ _ \\ / __/ _ \\ \n");
    printf("\t  | || | | | | | |_) | (_) \\__ \\ || (_| | | |_| | | (_) | (_| (_) |\n");
    printf("\t |___|_| |_| |_| .__/ \\___/|___/\\__\\__,_|  \\____|_|\\___/ \\___\\___/ \n");
    printf("\t               |_|                                                 \n");
    printf("\n");
  }

  printf("1. Inserisci il numero di giocatori\n");
  printf("2. Inserisci il numero di quest da finire per vincere il gioco da parte degli astronauti\n");
  printf("3. Stampa tutte le informazioni relative ad ogni giocatore\n");
  printf("4. Salva le impostazioni e inizia a giocare.\n");
}

static void num_giocatori(void) {
  n = 0;

  /* Richiedo il numero di giocatori (da n=4 a n=10) */
  do {
    printf("\n\n");
    printf("Inserisci il numero di giocatori.\nDa un minimo di 4, a un massimo di 10.\n");
    scanf("%d", &n);
    while(fgetc(stdin) != '\n');

    if (n < 4) {
      printf("\n");
      printf("Sono stati inseriti troppi pochi giocatori.\nIl minimo è di 4 giocatori.\n\n");
    }
    if (n > 10) {
      printf("\n");
      printf("Sono stati inseriti troppi giocatori.\nIl massimo è di 10 giocatori.\n\n");
    }
  } while (n < 4 || n > 10);

  giocatori = (struct Giocatore*) calloc(n, sizeof(struct Giocatore));
  scorri_giocatori = giocatori;    // la zona di memoria dinamica l'ho assegnata al puntatore che scorre l'array dei giocatori attivi

  int nome[n];

  for (int i = 0; i < n; i++) {
    nome[i] = i;
  }
  shuffle(nome, n);   // l'array di nomi è ora casuale

  /* Creo ora la prima stanza.
     Così, con il prossimo ciclo, ogni giocatore viene anche inserito nella prima stanza */
  stanza_inizio = (struct Stanza*) malloc(sizeof(struct Stanza));
  lista_stanze = (struct Stanza*) malloc(sizeof(struct Stanza));

  // i puntatori di direzione della prima stanza sono tutti inizializzati a NULL
  stanza_inizio -> avanti = NULL;
  stanza_inizio -> sinistra = NULL;
  stanza_inizio -> destra = NULL;
  stanza_inizio -> stanza_precedente = NULL;

  tipo_stanza(stanza_inizio);   // funzione che assegna in modo casuale il tipo della stanza
  stanza_inizio -> emergenza_chiamata = 0;
  lista_stanze = stanza_inizio;   // inizializzo la lista delle stanza alla sola stanza iniziale

  assegna_nome(nome, n, scorri_giocatori, stanza_inizio);

  /*
   Decisione su "impostore" o "astronauta": ogni giocatore ha una probabilità di essere un impostore, e ci possono essere
                                            da 1 a 3 di loro per gioco (su n giocatori). All'aumentare dei giocatori (n)
                                            è più probabile che aumentino gli impostori

   imposto una probabilità casuale, tra 1 e 100
   Criteri di decisione
   n <= 6:  probabilità <= 70: 1 impostore
            71 <= probabilità <= 90: 2 impostori
            91 <= probabilità <= 100: 3 impostori
   7 <= n <= 8: probabilità <= 35: 1 impostore
                36 <= probabilità <= 80: 2 impostori
                81 <= probabilità <= 100: 3 impostori
   9 <= n <= 10: probabilità <= 15: 1 impostore
                 16 <= probabilità <= 55: 2 impostori
                 56 <= probabilità <= 100: 3 impostori

   n <= 6: 70 - 20 - 10
   7 <= n <= 8: 35 - 45 - 20
   9 <= n <= 10: 15 - 40 - 45
  */

  int probabilita_impostore = 1 + rand() % 100;
  scorri_giocatori = giocatori;

  if (n <= 6) {   // analiziamo il primo caso: un massimo di 6 giocatori
    if (probabilita_impostore <= 70) {
      numero_impostori = 1;
      printf("\nCi sarà %d solo Impostore.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      impostore1 = rand() % n;   // assegnamo un "indice" casuale tra il numero dei giocatori presenti

      for (int i = 0; i < n; i++) {
        if (i == impostore1) {    // se l'indice casuale corrisponde a l'indice di scorrimento dei giocatori presenti...
          scorri_giocatori -> stato = 1;    // ... lo stato del giocatore verrà impostato a "impostore" (confrontare gamelib.h)
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 71 && probabilita_impostore <= 90) {
      numero_impostori = 2;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;    // assegnamo un "indice" casuale tra il numero dei giocatori presenti
        impostore2 = rand() % n;

        if (impostore1 != impostore2) {
          flag_impostori = 0;   // dato che i due numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i due numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2){
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 91 && probabilita_impostore <= 100) {
      numero_impostori = 3;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;
        impostore2 = rand() % n;
        impostore3 = rand() % n;

        if (impostore1 != impostore2 && impostore2 != impostore3 && impostore3 != impostore1) {
          flag_impostori = 0;   // dato che i tre numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i tre numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2 || i == impostore3) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2 && i != impostore3) {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
  }

  else if (n == 7 || n == 8) {    // analiziamo il secondo caso: 7 <= n <= 8
    if (probabilita_impostore <= 35) {
      numero_impostori = 1;
      printf("\nCi sarà %d solo Impostore.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti\n\n", astronauti);

      impostore1 = rand() % n;    // assegnamo un "indice" casuale tra il numero dei giocatori presenti

      for (int i = 0; i < n; i++) {
        if (i == impostore1) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 36 && probabilita_impostore <= 80) {
      numero_impostori = 2;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;    // assegnamo un "indice" casuale tra il numero dei giocatori presenti
        impostore2 = rand() % n;

        if (impostore1 != impostore2) {
          flag_impostori = 0;   // dato che i due numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i due numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2){
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 81 && probabilita_impostore <= 100 ) {
      numero_impostori = 3;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;
        impostore2 = rand() % n;
        impostore3 = rand() % n;

        if (impostore1 != impostore2 && impostore2 != impostore3 && impostore3 != impostore1) {
          flag_impostori = 0;   // dato che i tre numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i tre numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2 || i == impostore3) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2 && i != impostore3) {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
  }

  else if (n == 9 || n == 10) {   // analiziamo il terzo caso: 9 <= n <= 10
    if (probabilita_impostore <= 15) {
      printf("\nCi sarà un solo Impostore.\n");

      numero_impostori = 1;
      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti\n\n", astronauti);

      impostore1 = rand() % n;    // assegnamo un "indice" casuale tra il numero dei giocatori presenti

      for (int i = 0; i < n; i++) {
        if (i == impostore1) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 16 && probabilita_impostore <= 55) {
      numero_impostori = 2;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;    // assegnamo un "indice" casuale tra il numero dei giocatori presenti
        impostore2 = rand() % n;

        if (impostore1 != impostore2) {
          flag_impostori = 0;   // dato che i due numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i due numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2){
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
    else if (probabilita_impostore >= 56 && probabilita_impostore <= 100 ) {
      numero_impostori = 3;
      printf("\nCi saranno %d Impostori.\n", numero_impostori);

      astronauti = n - numero_impostori;
      printf("Ci saranno %d Astronauti.\n\n", astronauti);

      int flag_impostori = 1;

      do {
        impostore1 = rand() % n;
        impostore2 = rand() % n;
        impostore3 = rand() % n;

        if (impostore1 != impostore2 && impostore2 != impostore3 && impostore3 != impostore1) {
          flag_impostori = 0;   // dato che i tre numeri sono diversi, possiamo uscire dal ciclo
        }
      } while (flag_impostori);   // il ciclo continuerà finchè i tre numeri generati casualmente non saranno diversi

      for (int i = 0; i < n; i++) {
        if (i == impostore1 || i == impostore2 || i == impostore3) {
          scorri_giocatori -> stato = 1;    // lo stato del giocatore verrà impostato a "impostore"
          scorri_giocatori += sizeof(struct Giocatore);
        }
        else if (i != impostore1 && i != impostore2 && i != impostore3) {
          scorri_giocatori -> stato = 0;    // lo stato del giocatore verrà impostato a "astronauta"
          scorri_giocatori += sizeof(struct Giocatore);
        }
      }
    }
  }

  flag_impostato = 1;   // indico che le impostazioni sono state inserite. Il flag servirà se si reimpostano le impostazioni, al che verrà chiamata "termina_gioco()" che libererà tutta la memoria allocata dinamicamente
}

static void shuffle(int array[], int n) {
  srand (time(NULL));   //  usiamo un seed value diverso così non otteniamo sempre gli stessi risultati

  for (int i = n - 1; i >= 0; i--) {   /* iniziamo da l'ultimo elemento e li scambiamo uno ad uno
                                         Non dobbiamo eseguirlo per il primo elemento, per questo "i > 0" */
    int j = rand() % (i + 1);    // scegliamo un indice casuale tra 0 e i
    swap(&array[i], &array[j]);   // scambio il valore di "array[i]" con l'elemento a l'indice casuale j
  }
}

static void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

static void tipo_stanza(struct Stanza *stanza) {
  int percentuale = 0;

  percentuale = 1 + (rand() % 100);   // assegna un numero tra 1 e 100, che indicherà la percentuale

  if (percentuale >= 1 && percentuale <= 30) {    // probabilità che la stanza sia vuota: 30%
    stanza -> tipo = 0;
  }
  else if (percentuale >= 31 && percentuale <= 60) {    // probabilità che la stanza abbia una quest_semplice: 30%
    stanza -> tipo = 1;
  }
  else if (percentuale >= 61 && percentuale <= 75) {    // probabilità che la stanza abbia una quest_complicata: 15%
    stanza -> tipo = 2;
  }
  else if (percentuale >= 76 && percentuale <= 100) {    // probabilità che la stanza abbia una botola: 25%
    stanza -> tipo = 3;
  }
}

static void assegna_nome(int nome[], int n, struct Giocatore *giocatori, struct Stanza *inizio) {
  for (int i = 0; i < n; i++) {   // ogni indice però ha un nome (colore) fisso
    switch (nome[i]) {
      case 0:
        giocatori -> nome = (char)nero;
        break;
      case 1:
        giocatori -> nome = (char)rosso;
        break;
      case 2:
        giocatori -> nome = (char)verde;
        break;
      case 3:
        giocatori -> nome = (char)giallo;
        break;
      case 4:
        giocatori -> nome = (char)blu;
        break;
      case 5:
        giocatori -> nome = (char)magenta;
        break;
      case 6:
        giocatori -> nome = (char)ciano;
        break;
      case 7:
        giocatori -> nome = (char)bianco;
        break;
      case 8:
        giocatori -> nome = (char)arancione;
        break;
      case 9:
        giocatori -> nome = (char)viola;
        break;
    }

    giocatori -> posizione = inizio;    //ogni giocatore viene inserito nella prima stanza
    giocatori += sizeof(struct Giocatore);
  }
}

static int num_quest(void) {
  short q;

  printf("\n\n");
  printf("Inserisci il numero di quest da finire. Quando questo numero viene raggiunto, gli Impostori hanno automaticamente perso, e vincono gli Astronauti.\n");
  scanf("%hd", &q);
  while(fgetc(stdin) != '\n');

  return q;
}

static void stampa_giocatori(void) {
  while (giocatori == NULL) {
    num_giocatori();
  }
  struct Giocatore *lista_giocatori = giocatori;

  printf("\n\n");

  int count = 1;

  // per ogni giocatore stampo la posizione, lo stato e il nome: i tre campi della "struct Giocatore"
  for (int i = 0; i < n; i++) {
    printf("Posizione: %p\n", lista_giocatori -> posizione);

    printf("Stato: ");
    stampa_statoGiocatore(lista_giocatori);

    printf("Giocatore %d: ", count);
    stampa_nomeGiocatore(lista_giocatori);
    printf("\n");

    printf("---------------------------\n");

    lista_giocatori += sizeof(struct Giocatore);

    count++;
  }

  printf("\n\n");
}

static void stampa_statoGiocatore(struct Giocatore *stato) {
  switch (stato -> stato) {
    case 0:
      printf("Astronauta\n");
      break;
    case 1:
      printf("Impostore\n");
      break;
    case 2:
      printf("Assassinato\n");
      break;
    case 3:
      printf("Defenestrato\n");
      break;
  }
}

static void stampa_nomeGiocatore(struct Giocatore *nome) {

    switch (nome -> nome) {
      case 0:
        printf("Nero");
        break;
      case 1:
        printf("Rosso");
        break;
      case 2:
        printf("Verde");
        break;
      case 3:
        printf("Giallo");
        break;
      case 4:
        printf("Blu");
        break;
      case 5:
        printf("Magenta");
        break;
      case 6:
        printf("Ciano");
        break;
      case 7:
        printf("Bianco");
        break;
      case 8:
        printf("Arancione");
        break;
      case 9:
        printf("Viola");
        break;
  }
}

static int inizia_gioco(void) {
  if (giocatori != NULL && quest_da_finire != 0) {
    flag_pronto = 1;    // i giocatori e le quest sono state inserite, quindi le impostazioni sono pronte

    printf("\n");
    printf("Impostazioni salvate.\n");
    printf("Verrai mandato al Menù Pricipale, e da lì potrai iniziare a giocare.\n");
  }
  if (giocatori == NULL) {
    flag_pronto = 0;
    printf("Devi inserire i giocatori.\n");
    num_giocatori();

    return 1;
  }
  if (quest_da_finire == 0) {
    flag_pronto = 0;
    printf("Devi inserire le quest.\n");

    quest_da_finire = num_quest();
    printf("Le quest da completare per vincere il gioco sono %hd.\n\n", quest_da_finire);

    return 1;
  }
  return 0;
}

void gioca(void) {

    if (flag_pronto == 1) {   // si può iniziare a giocare solo se prima sono state inserite le impostazioni
      printf("\n\n");
      printf("Sta per iniziare il gioco.\n");
      printf("Preparatevi astronauti (..e impostori).\n");
      printf("\n");

      turno = 1;    // turno corrente, inizializzato a 1 per il primo
      giro = 1;
      int turni[n];   // array di turni per ciascun giocatore

      for (int i = 0; i < n; i++) {
        turni[i] = i;   /* inizializzo l'array dei turni di ciascun giocatore:
                           quindi il primo giocatore giocherà nel primo turno; il secondo nel secondo ecc... */
      }

      if (flag_fineGioco == 0) {
        while (flag_fineGioco == 0) {
          printf("\n\n");
          printf("Giro %d.\n", giro);

          /* All'inizio del proprio turno viene mostrato al giocatore il tipo della stanza, i giocatori nella stanza,
             e le possibili azioni */
          for (int i = 0; i < n; i++) {
            printf("\n");
            printf("------------------------------------------------------------------------------------------\n");
            printf("Turno %d.\n", turno);
            printf("\n");

            scorri_giocatori = giocatori;   // parto da l'inizio dell'array
            scorri_giocatori += turni[i]*sizeof(struct Giocatore);

            printf("E' il turno di ");
            stampa_nomeGiocatore(scorri_giocatori);
            printf(".\n");

            stampa_menuTurno(scorri_giocatori);

            turno++;
          }
          giro++;
        }
      }
    }
    else if (flag_pronto == 0) {    // se le impostazioni non sono state inserite non si può iniziare a giocare
      printf("\n");
      printf("Prima di iniziare a giocare devi impostare i parametri di gioco.\n");
      printf("Verrai mandato ora al Menù delle Impostazioni.\n");

      imposta_gioco();
    }
}

static void stampa_menuTurno(struct Giocatore *giocatore_attuale) {
  if (flag_fineGioco == 0) {
    printf("La stanza in cui ti trovi ");
    stampa_tipoStanza(giocatore_attuale);   // mostra il tipo della stanza

    stampa_giocatoriPresenti(giocatore_attuale);    // mostra i giocatori presenti nella stessa stanza
    stampa_azioni(giocatore_attuale);   // mostra le azioni disponibile al giocatore attuale
  }
}

static void stampa_tipoStanza(struct Giocatore *tipo_stanza) {
  switch (tipo_stanza -> posizione -> tipo) {
    case 0:   // vuota
      printf("è una stanza vuota.\n");
      break;
    case 1:   // quest_semplice
      printf("presenta una quest semplice.\n");
      break;
    case 2:   // quest_complicata
      printf("presenta una quest complicata.\n");
      break;
    case 3:   // botola solo per imostori
      if (tipo_stanza -> stato == 1) {    // controllo lo stato del giocatore, così solo gli Imostori possono usarla
        printf("ha una botola.\n");
        printf("Usandola andrai in un'altra stanza.\n");
      }
      else if (tipo_stanza -> stato == 0) {
        printf("è una stanza vuota, ma un po' sospetta...\n");
      }
      break;
  }

  printf("La posizione della stanza è: %p .\n", tipo_stanza -> posizione);
}

static void stampa_giocatoriPresenti(struct Giocatore *giocatore_presente) {
  flag_compagnia = 0;
  flag_morto = 0;

  struct Giocatore *compagnia = giocatori;
  printf("Giocatori presenti nella stessa stanza: \n");
  for (int i = 0; i < n; i++) {
    if (giocatore_presente -> nome != compagnia -> nome && giocatore_presente -> posizione == compagnia -> posizione && (compagnia -> stato == astronauta || compagnia -> stato == impostore)) {   // stampa i nomi (diversi tra loro) dei giocatori presenti nella stessa stanza
      printf("-");
      stampa_nomeGiocatore(compagnia);
      printf("\n");

      if (compagnia -> stato == assassinato) {
        flag_morto = 1;   // segnala che c'è almeno un morto
      }
      if (compagnia -> stato == astronauta /*|| compagnia -> stato == impostore*/) {
        flag_compagnia = 1;   // segnala che ci sono altri giocatori presenti
      }
    }
    compagnia += sizeof(struct Giocatore);
  }
}

static void stampa_azioni(struct Giocatore *giocatore_diTurno) {
  int scelta02;

  switch (giocatore_diTurno -> stato) {
    case 0:   // astronauta
      printf("\n");
      printf("Ecco le tue possibili azioni, astronauta:\n");
      stampa_azioniAstronauta(giocatore_diTurno);
      scanf("%d", &scelta02);
      while(fgetc(stdin) != '\n');
      scelta_astronauta(scelta02, giocatore_diTurno);
      break;
    case 1:   // impostore
      printf("\n");
      printf("Ecco le tue possibili azioni, impostore:\n");
      stampa_azioniImpostore(giocatore_diTurno);
      scanf("%d", &scelta02);
      while(fgetc(stdin) != '\n');
      scelta_impostore(scelta02, giocatore_diTurno);
      break;
    case 2:   // assassinato
      printf("Sei stato già assassinato da un impostore, dovevi stare più attento quando ancora potevi..\n");
      // printf("L'unica cosa che puoi fare.. è premere INVIO per continuare.\n");
      // while (getchar() != '\n');
      break;
    case 3:   // defenestrato
      printf("Sei stato già defenestrato, goditi il freddo vuoto spaziale..\n");
      // printf("L'unica cosa che puoi fare.. è premere INVIO per continuare.\n");
      // while (getchar() != '\n');
      break;
  }
}

static void stampa_azioniAstronauta(struct Giocatore *astronauta) {
  printf("1. Cambia stanza.\n");

  if (astronauta -> posizione -> tipo == quest_semplice || astronauta -> posizione -> tipo == quest_complicata) {
    flag_azioniAstronauta = 1;
    printf("2. C'è una quest da finire! Ricorda, finendo le quest avrete più probabilità di vincere il gioco.\n");

    if (flag_morto == 1 && astronauta -> posizione -> emergenza_chiamata == 0) {
      flag_azioniAstronauta = 2;
      printf("3. Attenzione! Hai trovato un morto! Effetua una chiamata di emergenza finchè sei in tempo.\n");
    }
  }
  else if (flag_morto == 1 && astronauta -> posizione -> emergenza_chiamata == 0 ) {
    flag_azioniAstronauta = 3;
    printf("2. Attenzione! Hai trovato un morto! Effetua una chiamata di emergenza finchè sei in tempo.\n");
  }

  if (flag_morto == 1 && astronauta -> posizione -> emergenza_chiamata == 1) {
    printf("   Attenzione! Hai trovato un morto! Purtroppo però da questa stanza è stata già effettuata una chiamata di emergenza.\nTi conviene cambiare stanza.\n");
  }

  printf("\n");
  printf("   Ricorda: puoi effettuare una sola azione per turno.\nScegli con attenzione.. potresti essere il prossimo astronauta ad attivare il \"flag_morto\"\n");
}

static void scelta_astronauta(int scelta, struct Giocatore *astronauta) {
  switch (flag_azioniAstronauta) {
    case 0:
      switch (scelta) {
        case 1:   // avanza
          astronauta -> posizione = (struct Stanza*) avanza(astronauta -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(astronauta);
          break;
      }
      break;
    case 1:
      switch (scelta) {
        case 1:   // avanza
          astronauta -> posizione = (struct Stanza*) avanza(astronauta -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(astronauta);
          break;
        case 2:   // esegui_quest
          esegui_quest(astronauta -> posizione);

          if (quest_rimaste == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
      }
      break;
    case 2:
      switch (scelta) {
        case 1:   // avanza
          astronauta -> posizione = (struct Stanza*) avanza(astronauta -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(astronauta);
          break;
        case 2:   // esegui_quest
          esegui_quest(astronauta -> posizione);
          if (quest_rimaste == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 3:   // chiamata_emergenza
          chiamata_emergenza(astronauta -> posizione);
          if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          else if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          break;
      }
      break;
    case 3:
      switch (scelta) {
        case 1:   // avanza
          astronauta -> posizione = (struct Stanza*) avanza(astronauta -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(astronauta);
          break;
        case 2:   // chiamata_emergenza
          chiamata_emergenza(astronauta -> posizione);
          if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          else if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          break;
      }
      break;
  }
}

static void stampa_azioniImpostore(struct Giocatore *impostore) {

  printf("1. Cambia stanza.\n");

  if (flag_morto == 1 && impostore -> posizione -> emergenza_chiamata == 0) {
    flag_azioniImpostore = 1;
    printf("2. C'è un corpo! Prova a defenestrare qualcuno.\n");

    if (flag_compagnia == 1) {
      flag_azioniImpostore = 2;
      printf("3. C'è un altro astronauta in questa stanza! Sporcati le mani e toglilo di mezzo.\n");
      if (giro == 1) {
        printf("   Al primo turno in cui giochi è sconsigliato.");
      }
      printf("\n");

      if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
        flag_azioniImpostore = 3;
        printf("4. C'è una quest da finire! Dovresti sabotarla, complicando la vita agli astronauti.\n");
      }
      else if (impostore -> posizione -> tipo == botola) {
        flag_azioniImpostore = 3;
        printf("4. C'è una botola! Usala per cambiare velocemente stanza.\n");
      }
    }
    else if (flag_compagnia == 0) {
      flag_azioniImpostore = 4;
      if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
        printf("3. C'è una quest da finire! Dovresti sabotarla, complicando la vita agli astronauti.\n");
      }
      else if (impostore -> posizione -> tipo == botola) {
        printf("3. C'è una botola! Usala per cambiare velocemente stanza.\n");
      }
    }
  }
  else if (flag_compagnia == 1) {
    flag_azioniImpostore = 5;
    printf("2. C'è un altro astronauta in questa stanza! Sporcati le mani e toglilo di mezzo.\n");
    if (giro == 1) {
      printf("   Al primo turno in cui giochi è sconsigliato.");
    }
    printf("\n");

    if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
      flag_azioniImpostore = 6;
      printf("3. C'è una quest da finire! Dovresti sabotarla, complicando la vita agli astronauti.\n");
    }
    else if (impostore -> posizione -> tipo == botola) {
      flag_azioniImpostore = 6;
      printf("3. C'è una botola! Usala per cambiare velocemente stanza.\n");
    }
  }

  else if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
    flag_azioniImpostore = 7;
    printf("2. C'è una quest da finire! Dovresti sabotarla, complicando la vita agli astronauti.\n");
  }
  else if (impostore -> posizione -> tipo == botola) {
    flag_azioniImpostore = 7;
    printf("2. C'è una botola! Usala per cambiare velocemente stanza.\n");
  }

  if (flag_morto == 1 && impostore -> posizione -> emergenza_chiamata == 1) {
    printf("   C'è un corpo! Non puoi effettuare di nuovo una chiamata di emergenza da questa stanza però.\nSarebbe una ottima stanza per continuare a assassinare gli astronauti.\n");
  }

  printf("\n");
  printf("   Ricorda: puoi effettuare una sola azione per turno.\n");
}

static void scelta_impostore(int scelta, struct Giocatore *impostore) {
  switch (flag_azioniImpostore) {
    case 0:
      switch (scelta) {
        case 1:   // avanza
        impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
        printf("La stanza in cui ti trovi ");
        stampa_tipoStanza(impostore);
        break;
      }
      break;
    case 1:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // chiamata_emergenza
          chiamata_emergenza(impostore -> posizione);

          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
      }
      break;
    case 2:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // chiamata_emergenza
          chiamata_emergenza(impostore -> posizione);

          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 3:   // uccidi_astronauta
          uccidi_astronauta(impostore);
          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
      }
      break;
    case 3:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // chiamata_emergenza
          chiamata_emergenza(impostore -> posizione);

          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 3:   // uccidi_astronauta
          uccidi_astronauta(impostore);
          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 4:   // usa_botola || sabotaggio
          if (impostore -> posizione -> tipo == botola) {
            printf("Stai per lasciare la stanza %p tramite la botola presente.\n.", impostore -> posizione);
            usa_botola(impostore);
            printf("Hai raggiunto la stanza %p.\n", impostore -> posizione);
          }
          else if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
            sabotaggio(impostore -> posizione);
          }
          break;
      }
      break;
    case 4:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // chiamata_emergenza
          chiamata_emergenza(impostore -> posizione);

          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 3:   // usa_botola || sabotaggio
          if (impostore -> posizione -> tipo == botola) {
            printf("Stai per lasciare la stanza %p tramite la botola presente.\n.", impostore -> posizione);
            usa_botola(impostore);
            printf("Hai raggiunto la stanza %p.\n", impostore -> posizione);
          }
          else if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
            sabotaggio(impostore -> posizione);
          }
          break;
      }
      break;
    case 5:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // uccidi_astronauta
          uccidi_astronauta(impostore);
          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
      }
      break;
    case 6:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // uccidi_astronauta
          uccidi_astronauta(impostore);
          if (astronauti == 0) {
            stampa_vittoriaImpostori();
          }
          else if (numero_impostori == 0) {
            stampa_vittoriaAstronauti();
          }
          break;
        case 3:   // usa_botola || sabotaggio
          if (impostore -> posizione -> tipo == botola) {
            printf("Stai per lasciare la stanza %p tramite la botola presente.\n.", impostore -> posizione);
            usa_botola(impostore);
            printf("Hai raggiunto la stanza %p.\n", impostore -> posizione);
          }
          else if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
            sabotaggio(impostore -> posizione);
          }
          break;
      }
      break;
    case 7:
      switch (scelta) {
        case 1:   // avanza
          impostore -> posizione = (struct Stanza*) avanza(impostore -> posizione);
          printf("La stanza in cui ti trovi ");
          stampa_tipoStanza(impostore);
          break;
        case 2:   // usa_botola || sabotaggio
          if (impostore -> posizione -> tipo == botola) {
            printf("Stai per lasciare la stanza %p tramite la botola presente.\n", impostore -> posizione);
            usa_botola(impostore);
            printf("Hai raggiunto la stanza %p.\n", impostore -> posizione);
          }
          else if (impostore -> posizione -> tipo == quest_semplice || impostore -> posizione -> tipo == quest_complicata) {
            sabotaggio(impostore -> posizione);
          }
          break;
        }
      break;
  }
}

static struct Stanza* avanza(struct Stanza *stanza) {
  int scelta03;
  struct Stanza *nuova_stanza = NULL;   // inizializzo la nuova stanza

  do{
    printf("\n");
    printf("Stai per cambiare stanza.\nTi puoi muovere in tre direzioni, o rimanere fermo:\n");
    printf("Rimanendo fermo sprecherai il tuo turno però.\n");
    printf("1. Avanti\n2. Sinistra\n3. Destra\n4. Rimani nella stanza corrente\n");
    scanf("%d", &scelta03);
    while(fgetc(stdin) != '\n');

    switch (scelta03) {
      case 1:   // avanti
        // se la direzione "avanti" ha un puntatore NULL, viene creata una nuova stanza collegando a questa attraverso il puntatore corrispondente
        if (stanza -> avanti == NULL) {
          nuova_stanza = malloc(sizeof(struct Stanza));
          numero_stanze++;
          stanza -> avanti = nuova_stanza;
          nuova_stanza -> stanza_precedente = stanza;
          nuova_stanza -> emergenza_chiamata = 0;   // assegno a 1 il flag emergenza_chiamata perchè essendo una stanza nuova, appena creata non è stata effettuata la stanza
          stanza = nuova_stanza;
          tipo_stanza(nuova_stanza);   // assegno casualmente il tipo della stanza

          // lista delle stanze
          struct Stanza *nuova = (struct Stanza*) malloc(sizeof(struct Stanza));
          nuova = nuova_stanza;

          // controlliamo se stiamo lasciando la prima stanza, cioè che la lista_stanze contenga solo una stanza
          if (lista_stanze -> stanza_precedente == NULL) {
            lista_stanze -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          else if (ultima_stanza_creata -> stanza_precedente == NULL) {
            ultima_stanza_creata -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          return stanza;
        }
        else {
          stanza = stanza -> avanti;
          return stanza;
        }
      break;
      case 2:   // sinistra
        // se la direzione "sinistra" ha un puntatore NULL, viene creata una nuova stanza collegando a questa attraverso il puntatore corrispondente
        if (stanza -> sinistra == NULL) {
          nuova_stanza = malloc(sizeof(struct Stanza));
          numero_stanze++;
          stanza -> sinistra = nuova_stanza;
          nuova_stanza -> stanza_precedente = stanza;
          nuova_stanza -> emergenza_chiamata = 0;   // assegno a 1 il flag emergenza_chiamata perchè essendo una stanza nuova, appena creata non è stata effettuata la stanza
          stanza = nuova_stanza;
          tipo_stanza(nuova_stanza);    // assegno casualmente il tipo della stanza

          // lista delle stanze
          struct Stanza *nuova = (struct Stanza*) malloc(sizeof(struct Stanza));
          nuova = nuova_stanza;

          // controlliamo se stiamo lasciando la prima stanza, cioè che la lista_stanze contenga solo una stanza
          if (lista_stanze -> stanza_precedente == NULL) {
            lista_stanze -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          else if (ultima_stanza_creata -> stanza_precedente == NULL) {
            ultima_stanza_creata -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          return stanza;
        }
        else {
          stanza = stanza -> sinistra;
          return stanza;
        }
      break;
      case 3:   // destra
        // se la direzione "destra" ha un puntatore NULL, viene creata una nuova stanza collegando a questa attraverso il puntatore corrispondente
        if (stanza -> destra == NULL) {
          nuova_stanza = malloc(sizeof(struct Stanza));
          numero_stanze++;
          stanza -> destra = nuova_stanza;
          nuova_stanza -> stanza_precedente = stanza;
          nuova_stanza -> emergenza_chiamata = 0;   // assegno a 1 il flag emergenza_chiamata perchè essendo una stanza nuova, appena creata non è stata effettuata la stanza
          stanza = nuova_stanza;
          tipo_stanza(nuova_stanza);    // assegno casualmente il tipo della stanza

          // lista delle stanze
          struct Stanza *nuova = (struct Stanza*) malloc(sizeof(struct Stanza));
          nuova = nuova_stanza;

          // controlliamo se stiamo lasciando la prima stanza, cioè che la lista_stanze contenga solo una stanza
          if (lista_stanze -> stanza_precedente == NULL) {
            lista_stanze -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          else if (ultima_stanza_creata -> stanza_precedente == NULL) {
            ultima_stanza_creata -> stanza_precedente = nuova;
            ultima_stanza_creata = nuova;
            ultima_stanza_creata -> stanza_precedente = NULL;
          }
          return stanza;
        }
        else {
          stanza = stanza -> sinistra;
          return stanza;
        }
      break;
      case 4:   // rimani fermo
        printf("Hai scelto di rimanere fermo, non ti sposterai da questa stanza.\nIl tuo turno è finito.\n");
        return stanza;

        break;
      default:
        printf("Opzione non disponibile.\nPerciò hai scelto di rimanere fermo.\n");
    }
  } while (scelta03 != 1 && scelta03 != 2 && scelta03 != 3 && scelta03 != 4);

  return 0;
}

static void esegui_quest(struct Stanza *stanza) {
  if (stanza -> tipo == quest_semplice) {
    quest_completate++;    // se quest_semplice il contatore delle quest_completate viene incrementato di 1
  }
  else if (stanza -> tipo == quest_complicata) {
    quest_completate = quest_completate + 2;    // se quest_complicata il contatore delle quest_completate viene incrementato di 2
  }

  stanza -> tipo = 0;   // una volta eseguita, la stanza diventa "vuota"

  quest_rimaste = quest_da_finire - quest_completate;
  if (quest_rimaste < 0) {
    quest_rimaste = 0;    // se dovesse andare sotto zero, lo imposto a 0
  }

  printf("Astronauti, avete completato %d quest su %d.\n", quest_completate, quest_da_finire);
  printf("Vi mancano %d quest rimaste.\n", quest_rimaste);
}

static void stampa_vittoriaAstronauti(void) {
  /* ASCII Art */
  {
    printf("\n");
    printf("     _        _                               _   _   \n");
    printf("    / \\   ___| |_ _ __ ___  _ __   __ _ _   _| |_(_)  \n");
    printf("   / _ \\ / __| __| '__/ _ \\| '_ \\ / _` | | | | __| |  \n");
    printf("  / ___ \\\\__ \\ |_| | | (_) | | | | (_| | |_| | |_| |_ \n");
    printf(" /_/   \\_\\___/\\__|_|  \\___/|_| |_|\\__,_|\\__,_|\\__|_( )\n");
    printf("                  _               _       _        |/ \n");
    printf("   __ ___   _____| |_ ___  __   _(_)_ __ | |_ ___     \n");
    printf("  / _` \\ \\ / / _ \\ __/ _ \\ \\ \\ / / | '_ \\| __/ _ \\    \n");
    printf(" | (_| |\\ V /  __/ ||  __/  \\ V /| | | | | || (_) |   \n");
    printf("  \\__,_| \\_/ \\___|\\__\\___|   \\_/ |_|_| |_|\\__\\___(_)  \n");
  }

  printf("\n\n");
  printf("Complimenti Astronauti, avete vinto la partita! Potete continuare il vostro viaggio con tranquillità.\n");
  printf("\n\n");

  flag_fineGioco = 1;
  flag_giocato = 0;

  termina_gioco();
}

static void chiamata_emergenza (struct Stanza *stanza) {
  struct Giocatore *scorri_impostori = NULL;
  struct Giocatore *scorri_astronauti = NULL;
  struct Giocatore *scorri_giocatori01 = NULL;

  int num_impostori;
  int num_astronauti;
  int defenestra_impostore = 30;    // probabilità che venga defenestrato un impostore, parte da 30%
  int defenestra_astronauta = 30;   // probabilità che venga defenestrato un astronauta, parte da 30%
  int probabilita_defenestrato;

  scorri_impostori = giocatori;
  for (int i = 0; i < n; i++) {   // determino il numero di impostori presenti in quella stanza
    if (scorri_impostori -> posizione == stanza && scorri_impostori -> stato == impostore) {
      num_impostori++;
    }
    scorri_impostori += sizeof(struct Giocatore);
  }

  scorri_astronauti = giocatori;
  for (int i = 0; i < n; i++) {   // determino il numero di astronauti presenti in quella stanza
    if (scorri_astronauti -> posizione == stanza && scorri_astronauti -> stato == astronauta) {
      num_astronauti++;
    }
    scorri_astronauti += sizeof(struct Giocatore);
  }

  /* Probabilità di essere defenestrato:
     - impostore: 30% + 20% per ogni altro astronauta nella stessa stanza, - 30% per ogni altro impostore nella stessa stanza
     - astronauta: 30% + 20% per ogni altro impostore nella stessa stanza, - 30% per ogni altro astronauta nella stessa stanza */

  defenestra_impostore = defenestra_impostore + (20 * num_astronauti) - (30 * (num_impostori - 1));    // tolgo 1 da num_impostori cosiì da non contare se stesso
  // pongo i limiti tra 0 e 100
  if (defenestra_impostore < 0) {
    defenestra_impostore = 0;
  }
  if (defenestra_impostore > 100) {
    defenestra_impostore = 100;
  }

  defenestra_astronauta = defenestra_astronauta + (20 * num_impostori) - (30 * (num_astronauti - 1));
  // pongo i limiti tra 0 e 100
  if (defenestra_astronauta < 0) {
    defenestra_astronauta = 0;
  }
  if (defenestra_astronauta > 100) {
    defenestra_astronauta = 100;
  }

  scorri_giocatori01 = giocatori;
  probabilita_defenestrato = rand() % 100;

  for (int i = 0; i < n; i++) {
    if (scorri_giocatori01 -> posizione == stanza && scorri_giocatori01 -> stato != assassinato && scorri_giocatori01 -> stato != defenestrato) {
      if (scorri_giocatori01 -> stato == impostore) {   // controllo se un impostore verrà defenestrato
        if (defenestra_impostore >= probabilita_defenestrato) {
          scorri_giocatori01 -> stato = defenestrato;   // cambio lo stato del giocatore a "defenestrato"
          flag_defenestrato = 1;
          numero_impostori--;

          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" è stato defenestrato.\n");
          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" era un impostore.\n");
          printf("%d impostori rimasti.\n", numero_impostori);
          printf("%d astronauti rimasti.\n", astronauti);

          break;
        }
        else {
          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" non è stato defenestrato.\n");

          break;
        }
      }

      if (scorri_giocatori01 -> stato == astronauta) {    // contollo se un astronauta verrà defenestrato
      if (defenestra_astronauta >= probabilita_defenestrato) {
          scorri_giocatori01 -> stato = defenestrato;   // cambio lo stato del giocatore a "defenestrato"
          flag_defenestrato = 1;
          astronauti--;

          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" è stato defenestrato.\n");
          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" non era un impostore.\n");
          printf("%d impostori rimasti.\n", numero_impostori);
          printf("%d astronauti rimasti.\n", astronauti);

          break;
        }
        else {
          stampa_nomeGiocatore(scorri_giocatori01);
          printf(" non è stato defenestrato.\n");

          break;
        }
      }
    }

    scorri_giocatori01 += sizeof(struct Giocatore);
  }

  if (flag_defenestrato == 0) {
    printf("Non è stato defenestrato nessuno questa volta.\n");
  }

  stanza -> emergenza_chiamata = 1;   // imposto il flag a 1, così in questa stanza non verrà chiamata una seconda volta
}

static void stampa_vittoriaImpostori(void) {
  /* ASCII Art */
  {
    printf("\n");
    printf("  ___                           _             _     \n");
    printf(" |_ _|_ __ ___  _ __   ___  ___| |_ ___  _ __(_)    \n");
    printf("  | || '_ ` _ \\| '_ \\ / _ \\/ __| __/ _ \\| '__| |    \n");
    printf("  | || | | | | | |_) | (_) \\__ \\ || (_) | |  | |_   \n");
    printf(" |___|_| |_| |_| .__/ \\___/|___/\\__\\___/|_|  |_( )  \n");
    printf("               |_|_               _       _    |/   \n");
    printf("   __ ___   _____| |_ ___  __   _(_)_ __ | |_ ___   \n");
    printf("  / _` \\ \\ / / _ \\ __/ _ \\ \\ \\ / / | '_ \\| __/ _ \\  \n");
    printf(" | (_| |\\ V /  __/ ||  __/  \\ V /| | | | | || (_) | \n");
    printf("  \\__,_| \\_/ \\___|\\__\\___|   \\_/ |_|_| |_|\\__\\___(_)\n");
  }
  printf("\n\n");
  printf("Complimenti Impostori, avete vinto la partita! Non ci sono più astronauti presenti su questa astronave.\n");
  printf("\n\n");

  flag_fineGioco = 1;
  flag_giocato = 0;

  termina_gioco();
}

static void uccidi_astronauta(struct Giocatore *impostore) {
  int vittima;
  int giocatori_stanzaAttuale = 0;
  int giocatori_stanzaPrecedente = 0;
  int scoperta;   // indica la probabilità che un impostore venga scoperto dopo aver compiuto una uccisione
  int probabilita_scoperta;
  struct Giocatore *scorri_giocatori03 = giocatori;
  struct Stanza *stanza_precedente = impostore -> posizione -> stanza_precedente;   // puntatore alla stanza precedente
  int flag = 0;

  for (int i = 0; i < n; i++) {
    if (impostore -> posizione == scorri_giocatori03 -> posizione && scorri_giocatori03 -> stato == astronauta) {   // mi permette di stampare tutti gli astronauti nella stessa stanza in cui si trova il giocatore (impostore) attuale
        flag = 1;
        printf("Per assassinare ");
        stampa_nomeGiocatore(scorri_giocatori03);   // stampo il nome
        printf(" inserire il suo ID: %d \n", scorri_giocatori03 -> nome);
        giocatori_stanzaAttuale++;    // incremento il numero di giocatori nella stanza attuale
    }
    scorri_giocatori03 += sizeof(struct Giocatore);
  }
  if (flag == 1) {

  }
  scanf("%d", &vittima);
  while(fgetc(stdin) != '\n');
  printf("\n\n");

  scorri_giocatori03 = giocatori;   // re-inizializzo
  while (scorri_giocatori03 -> nome != vittima) {
    scorri_giocatori03 += sizeof(struct Giocatore);
  }
  scorri_giocatori03 -> stato = assassinato;
  printf("Hai assassinato ");
  stampa_nomeGiocatore(scorri_giocatori03);
  printf(".\n");
  giocatori_stanzaAttuale--;
  astronauti--;
  printf("%d astronauti rimasti.\n", astronauti);

  scorri_giocatori03 = giocatori;   // re-inizializzo
  for (int i = 0; i < n; i++) {
    if (stanza_precedente == scorri_giocatori03 -> posizione && scorri_giocatori03 -> stato == astronauta) {
      giocatori_stanzaPrecedente++;
    }
    scorri_giocatori03 += sizeof(struct Giocatore);
  }

  switch (giocatori_stanzaAttuale) {    // assegno la probabilità e verifico se l'impostore verrà scoperto
    case 0:   // senza giocatori nella stanza attuale
      if (giocatori_stanzaPrecedente != 0) {    // verifico se ci siano giocatori nella stanza precedente
        scoperta = giocatori_stanzaPrecedente * 20;   // la probabilità aumenta del 20% per ogni giocatore nella stanza precedende

        if (scoperta > 100) {   // pongo il limite di 100
          scoperta = 100;
        }
      }

      probabilita_scoperta = rand() % 100;

      if (scoperta >= probabilita_scoperta) {
        impostore -> stato = defenestrato;
        numero_impostori--;

        printf("Ti è andata male, impostore.\nTi hanno scoperto e defenestrato.\nRimangono %d impostori.", numero_impostori);
      }
      else {
        printf("Te la sei cavata questa volta, impostore.\n");
      }
      break;

    // caso ci siano dai 1 ai 9 giocatori nella stanza attuale (massimo di 10 giocatori - 1 impostore)
    case 1 ... 9:
      if (giocatori_stanzaPrecedente != 0) {    // verifico se ci siano giocatori nella stanza precedente
        scoperta = giocatori_stanzaPrecedente * 20;   // la probabilità aumenta del 20% per ogni astronauta nella stanza precedende
      }

      scoperta = scoperta + (giocatori_stanzaAttuale * 50);   // la probabilità aumenta del 50% per ogni astronuta nella staza attuale

      if (scoperta > 100) {   // pongo il limite di 100
        scoperta = 100;
      }

      probabilita_scoperta = rand() % 100;

      if (scoperta >= probabilita_scoperta) {
        impostore -> stato = defenestrato;
        numero_impostori--;

        printf("Ti è andata male, impostore.\nTi hanno scoperto e defenestrato.\nRimangono %d impostori.", numero_impostori);
      }
      else {
        printf("Te la sei cavata questa volta, impostore.\n");
      }
      break;
  }
}

static void usa_botola(struct Giocatore *impostore) {
  int flag_stanzaTrovata = 0;
  int flag_cambiaStanza = 0;    // flag che indica se è avvenuto un cambiamento di stanza
  int continua = 1;
  int probabilita_cambio01;
  int probabilita_cambio02;
  struct Stanza *scorri_stanze = lista_stanze;

  do {
    while (scorri_stanze -> stanza_precedente != NULL) {    // questa procedura non può funzionare se ci troviamo nella prima stanza
      if (scorri_stanze -> tipo == botola && scorri_stanze != impostore -> posizione) {   // la stanza che consideriamo deve essere diversa dalla posizione attuale del giocatore, ma sempre di tipo "botola"
        flag_stanzaTrovata = 1;

        probabilita_cambio01 = rand() % 100;

        if (probabilita_cambio01 >= 0 && probabilita_cambio01 < 50) {
          impostore -> posizione = scorri_stanze;
          flag_cambiaStanza = 1;    // segnalo che la stanza è stata cambiata

          if (flag_cambiaStanza == 1) {
            flag_stanzaTrovata = 0;
          }
          break;
        }
      }
      scorri_stanze = scorri_stanze -> stanza_precedente;
    }
  } while (flag_stanzaTrovata == 1);

  while (continua == 1) {
    scorri_stanze = lista_stanze;
    if (flag_cambiaStanza == 0) {
      while (scorri_stanze -> stanza_precedente != NULL) {
        probabilita_cambio02 = rand() % 100;

        if (probabilita_cambio02 >= 0 && probabilita_cambio02 < 50) {
          impostore -> posizione = scorri_stanze;
          continua = 0;
          break;
        }
        scorri_stanze = scorri_stanze -> stanza_precedente;
      }
    }
    else {
      continua = 0;
    }
  }
}

static void sabotaggio(struct Stanza *stanza) {
  if (stanza -> tipo == quest_semplice || stanza -> tipo == quest_complicata) {
    stanza -> tipo = vuota;

    printf("Sabotaggio completato!\nLa stanza è diventata vuota.\n");
    printf("%d quest rimaste.\n", quest_rimaste);
  }
}

void termina_gioco(void) {
  printf("Tutta la memoria allocata dinamicamente sta per essere deallocata dinamicamente.\n");

  free(giocatori);

  cancella_stanza(stanza_inizio);
}

static struct Stanza *cancella_stanza(struct Stanza *stanza) {
  while (stanza_successiva(stanza) != stanza) {
    if ((stanza -> avanti) != NULL) {
      stanza -> avanti = cancella_stanza(stanza -> avanti);
    }

    if ((stanza -> sinistra) != NULL) {
      stanza -> sinistra = cancella_stanza(stanza -> sinistra);
    }

    if ((stanza -> destra) != NULL) {
      stanza -> destra = cancella_stanza(stanza -> destra);
    }
  }

  printf("Libero la memoria riservata alle stanze.\n");

  struct Giocatore *giocatori1 = giocatori;
  for (int i = 0; i < n; i++) {
    struct Giocatore *giocatori2 = giocatori1;
    free(giocatori2);
    giocatori1 += sizeof(struct Giocatore);
  }
  free(stanza);

  return NULL;
}

static struct Stanza *stanza_successiva(struct Stanza *stanza) {
  if (stanza -> avanti != NULL) {
    stanza = stanza -> avanti;    // ci spostiamo in avanti
  }
  else if (stanza -> sinistra != NULL) {
    stanza = stanza -> sinistra;    // ci spostiamo a sinistra
  }
  else if (stanza -> destra != NULL) {
    stanza = stanza -> destra;    // ci spostiamo a destra
  }

  return stanza;    // nel "while (stanza successiva(stanza) != stanza)" di "cancella_stanza" consideriamo la stanza successiva, finchè c'è
}
