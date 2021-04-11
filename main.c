#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

void stampa_menuPrimaVolta(void);
void stampa_menuPricipale(void);

int flag01 = 0;

int main(void) {
  srand(time(0));
  int scelta01 = 0;

  /* ASCII Art - "Trannoi" */
  {
    printf("\t    _____                            _ \n");
    printf("\t   |_   _| __ __ _ _ __  _ __   ___ (_)\n");
    printf("\t     | || '__/ _` | '_ \\| '_ \\ / _ \\| |\n");
    printf("\t     | || | | (_| | | | | | | | (_) | |\n");
    printf("\t     |_||_|  \\__,_|_| |_|_| |_|\\___/|_|\n");
  }

  /* ASCII Art - Astronauta */
  {
    printf("\t       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("\t       ⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀⠀⠀⠀⠀⠀\n");
    printf("\t       ⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀⠀⠀\n");
    printf("\t    ⠀ ⠀  ⠀⠀⠀⠀⠀⣸⣿⡏⠀⠀⠀⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄⠀⠀⠀\n");
    printf("\t   ⠀ ⠀ ⠀⠀  ⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⡄⠀\n");
    printf("\t      ⠀ ⠀⣀⣤⣴⣶⣶⣿⡟⠀⠀⠀⢸⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀\n");
    printf("\t       ⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿⠀\n");
    printf("\t      ⠀ ⢸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀\n");
    printf("\t       ⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇⠀⠀\n");
    printf("\t       ⠀⣿⣿⠁⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⠀⠀\n");
    printf("\t       ⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n");
    printf("\t       ⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n");
    printf("\t   ⠀    ⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀\n");
    printf("\t       ⠀⠸⣿⣧⡀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀\n");
    printf("\t       ⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀⠀⠀\n");
    printf("\t⠀⠀⠀       ⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⣽⣿⡏⠁⠀⠀⢸⣿⡇⠀⠀⠀ \n");
    printf("\t⠀⠀    ⠀   ⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀⠀⠀\n");
    printf("\t⠀⠀    ⠀⠀⠀   ⠀⠀⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁⠀⠈⠻⣿⣿⣿⣿⡿⠏⠀⠀⠀⠀\n");
    printf("\t⠀⠀⠀    ⠀⠀⠀ ⠀  ⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("* ASCII art tratto da https://www.twitchquotes.com/copypastas/3822 *\n");
  }
  while (getchar() != '\n');    // si aspetta che il giocatore prema INVIO

  printf("\n");

  /* La storia */
  {
    printf("Un gruppo di astronauti si trova in viaggio sull’astronave Skelt.\n");
    printf("Il loro obiettivo è riuscire a completare tutte le attività previste (le quest) per il mantenimento della nave, arrivando così a destinazione.\n");
    printf("Tra di loro si celano però anche degli impostori, il cui scopo è eliminare di nascosto gli astronauti senza farsi scoprire da essi.\n");
    printf("Riusciranno ad arrivare a destinazione prima di essere decimati?\n");
    printf("* Trannoi è liberamente ispirato ad un gioco esistente. *\n");
  }
  while (getchar() != '\n');

  printf("\n\n");

  do {
    if (flag01 == 0) {
      stampa_menuPrimaVolta();
    }
    else if (flag01 == 1) {
      stampa_menuPricipale();
    }
    scanf("%d", &scelta01);
    while(fgetc(stdin) != '\n');

    switch (scelta01) {
      case 1:
        imposta_gioco();
        break;
      case 2:
        gioca();
        break;
      case 3:
        termina_gioco();
        break;
      default:
        printf("\n\n");
        printf("Il valore inserito non è valido.\nRiprova inserendo una delle opzioni disponibili.\nInserisci quindi un intero tra 1 e 3.\n\n");
        break;
    }
  } while (scelta01 != 3);

  return 0;
}

void stampa_menuPrimaVolta(void) {
  /* ASCII Art - Menu Principale */
  {
    printf("\t  __  __             __     ____       _            _             _      \n");
    printf("\t |  \\/  | ___ _ __  _\\_\\_  |  _ \\ _ __(_)_ __   ___(_)_ __   __ _| | ___ \n");
    printf("\t | |\\/| |/ _ \\ '_ \\| | | | | |_) | '__| | '_ \\ / __| | '_ \\ / _` | |/ _ \\ \n");
    printf("\t | |  | |  __/ | | | |_| | |  __/| |  | | | | | (__| | |_) | (_| | |  __/\n");
    printf("\t |_|  |_|\\___|_| |_|\\__,_| |_|   |_|  |_|_| |_|\\___|_| .__/ \\__,_|_|\\___|\n");
    printf("\t                                                     |_|                 \n");
  }
  printf("\t\t\t\tPremere INVIO per iniziare\n");
  while (getchar() != '\n');

  printf("Inserisci una opzione per decidere cosa fare:\n");
  printf("1. Imposta Gioco\n");
  printf("2. Gioca\n");
  printf("3. Termina Gioco\n");

  flag01++;
}

void stampa_menuPricipale(void) {
  /* ASCII Art - Menu Principale */
  {
    printf("\t  __  __             __     ____       _            _             _      \n");
    printf("\t |  \\/  | ___ _ __  _\\_\\_  |  _ \\ _ __(_)_ __   ___(_)_ __   __ _| | ___ \n");
    printf("\t | |\\/| |/ _ \\ '_ \\| | | | | |_) | '__| | '_ \\ / __| | '_ \\ / _` | |/ _ \\ \n");
    printf("\t | |  | |  __/ | | | |_| | |  __/| |  | | | | | (__| | |_) | (_| | |  __/\n");
    printf("\t |_|  |_|\\___|_| |_|\\__,_| |_|   |_|  |_|_| |_|\\___|_| .__/ \\__,_|_|\\___|\n");
    printf("\t                                                     |_|                 \n");
  }

  printf("Inserisci una opzione per decidere cosa fare:\n");
  printf("1. Imposta Gioco\n");
  printf("2. Gioca\n");
  printf("3. Termina Gioco\n");
}




