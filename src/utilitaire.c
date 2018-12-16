/*
NOURRIT GABRIEL
NOEL JULES
*/

#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "utilitaire.h"

/**
 * fonction qui permet de nettoyer la string_commande qu'on se passe via les pipes
 * elle enlève les caractères indésirables qui sont lu dans nos read
 * @param    message, le message que l'on veut nettoyer
 * @return   void
 */
void propre(char*s){
    char*it=s;
    while(*it!='?')/*on utilise ? comme caractere de fin de cmd*/
        it++;
    *it='\0';
}

/**
 * fonction qui permet de vider le Buffer de l'entrée standard
 * elle va permettre de gerer les entrées clavier indésirablement trop longue
 * par exemple si un int est attendu et que l'user donne une chaine de caractere
 * @param    void
 * @return   void
 */
void viderBuffer(){
    int c = 0;
    
    while (c != '\n' && c != EOF)
        c = getchar();
}


/**
 * fonction qui permet de récupérer de manière sûre
 * l'identifiant de la commande que l'on va traiter
 * @param    void
 * @return   le numero d'identifiant (0:exit,1:set,2:lookup,3:dump)
 */
int lire_id(){
    int res;
    do{
        res=0;
        puts("");
        puts("**************************************************");
        puts("***       Selectionnez l'action souhaitée      ***");
        puts("***  (1) set, (2) lookup, (3) dump ou (4) exit ***");
        puts("**************************************************");
        puts("");
        printf(">> ");
        if(scanf("%d",&res)!=1){
            getchar();
            viderBuffer();//on vide le buffer des choses que scanf n'aurais pas reussi à lire
        }
    }while((res<1) || (res>4));

    if(res==4)
        res=0;
    
    return res;
}



/**
 * fonction qui permet de récupérer de manière sûre
 * la clé du mot que l'on va traiter
 * @param    void
 * @return   la clé du mot
 */
int lire_key(){
    int res;
    do{
        res=0;
        puts("-> Entrez la clef du mot (dans [1;+oo[)");
        printf(">> ");
        if(scanf("%d",&res)!=1){
            getchar();
            viderBuffer();
        }
    }while(res<1);
    return res;
}

/**
 * fonction qui permet de regarder
 * à chaque instant crucial du programme
 * l'activité des procesus traité
 * au cas ou le mode DEBUG est activé
 * @param    void
 * @return   void
 */
void etat(){
    #ifdef DEBUG
    puts("debug : pause ; pressez une touche sur le terminal de debug pour continuer");
    system("bin/debug.sh");
    #endif
}