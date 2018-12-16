/*
NOURRIT GABRIEL
NOEL JULES
*/

#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "table.h"
#include "global.h"
#include "processus.h"
#include "utilitaire.h"

int main(){
    int i;

    etat();
    
    /*création des pipes*/
    for (i =0; i <= NBNOEUD ; i++ )
        pipe(tabPipe[i]);

    /*interception des signaux SIGUSR1 par le gestionnaire reveil*/
    if(signal(SIGUSR1,reveil) == SIG_ERR){
        perror("signal");
        exit(-1);
    }
	
    creer_fils();

    /*fermeture des pipes inutiles */
    close(tabPipe[NBNOEUD][ECRIRE]);/*on ferme que le gros en écriture*/
    fermer_pipes(LIRE,NBNOEUD);/*on les fermes tous en lecture sauf le gros*/

    etat();
    
    puts("");
    puts("************************************************************");
    puts("************************************************************");
    puts("***     Bonjour et bienvenue dans notre dictionnaire     ***");
    puts("***    Dans ce dictionnaire vous avez le choix entre :   ***");
    puts("***  (1) Set : Ajouter un mot et sa clé au dictionnaire  ***");
    puts("***  (2) Lookup : Rechercher un mot grâce à sa clé       ***");
    puts("***  (3) Dump : Afficher le dictionnaire                 ***");
    puts("************************************************************");
    puts("************************************************************");
    puts("");
    /* Gestion de l'envoi des différentes commandes */
    controller();
    return 0;    
}




