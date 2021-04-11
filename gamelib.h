#include <stdio.h>
#include <stdlib.h>

/* Definisco tutti gli enum */
enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore {nero, rosso, verde, giallo, blu, magenta, ciano, bianco, arancione, viola};
enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola};

/* Definisco tutte le struct */
struct Giocatore {
  struct Stanza *posizione;
  enum Stato_giocatore stato;
  enum Nome_giocatore nome;
};

struct Stanza {
  struct Stanza *avanti;
  struct Stanza *sinistra;
  struct Stanza *destra;
  struct Stanza *stanza_precedente;
  enum Tipo_stanza tipo;
  int emergenza_chiamata;
};

/* Definisco le funzioni */
void imposta_gioco();
void gioca();
void termina_gioco();
