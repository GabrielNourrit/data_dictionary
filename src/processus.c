/*
NOURRIT GABRIEL
NOEL JULES
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "table.h"
#include "processus.h"
#include "utilitaire.h"
#include "global.h"

int fils[NBNOEUD];
PTable_entry Dico=NULL;
/*ce dictionnaire vide sera copié dans l'environnement de tout les fils*/


/**
 * fonction qui crée tous les fils
 * @param    void
 * @return   void
 */
void creer_fils(){
    int i,pid;
    /* pid nous permet de repertorier le pid des fils pour leur envoyé des signaux*/
    
    /*création des fils*/
    for (i=0;i<NBNOEUD;i++){
        switch(pid=fork()) {
        case -1:
            /*Erreur*/
            perror("fork");
            exit(-1);
        case 0:
            if (i==0)
                premierfils(); /*ce fils va lire dans le dernier pipe*/
            else
            	/*tout les autres lise dans le pipe d'avant et écrive dans le leur*/
				/*ils récupérent également leur numero pour savoir qui ils sont*/
                autrefils(i);
			
			/*on endort tout les fils une fois crée*/	
            pause();
            
            break;
        default:
            fils[i]=pid;/*on répertorie les pid de chaque fils*/
        }
    }
}


/**
 * fonction du controller 
 * @param    void
 * @return   void
 */
void controller(){
    int id,key,i;
    char word[MOTMAX], cmd[CMDMAX];
    /* id key word sont les parametre d'une commande et cmd la commande en question*/
    
    do{

        id=lire_id();
            
        switch(id){
        case 0: 
        	/*Cas d'un exit*/
        	
            /*tue tout les processus fils*/
            for(i=0;i<NBNOEUD;i++)
                kill(fils[i],SIGINT);
                
            /* On vérifie que tous les fils sont morts */
            while(wait(NULL)!=-1); 
       
            /* on ferme les pipes */
            fermer_pipes(ECRIRE,NBNOEUD+1);
            fermer_pipes(LIRE,NBNOEUD+1);
            
            puts("");
		    puts("**************************************************");
		    puts("***********           Bye Bye          ***********");
		    puts("**************************************************");
		    puts("");
                    etat();
            exit(0);
            break;
        case 1:
            /*cas d'un set*/
            key=lire_key();
            puts("-> Entrez le mot à ajouter (carac UTF8 sans le carac '?' (fin de chaine))");
            printf(">> ");
            scanf("%s",word);
            puts("");
            sprintf(cmd,"%d %d %s?",id,key,word);
            /*on envoie la commande dans le 1er tube, celui ou lit fils 0*/
            write(tabPipe[NBNOEUD-1][ECRIRE],cmd,strlen(cmd));

            break;
        case 2:
            /*cas d'un lookup*/
            key=lire_key();
            puts("");
            sprintf(cmd,"%d %d 0?",id,key);
            write(tabPipe[NBNOEUD-1][ECRIRE],cmd,strlen(cmd)); 
            /*on envoie la commande dans le 1er tube, celui ou lit fils 0*/
            break;
        case 3:
            /*cas d'un dump*/
            puts("");
            sprintf(cmd,"%d 0 0?",id);
            /*on écrit dans tout les tubes pour que tous les fils display leurs tables*/
            for(i=0;i<NBNOEUD;i++){
                write(tabPipe[i][ECRIRE],cmd,strlen(cmd));
            }
            break;
        }
        
        /*on envoie une batterie de signaux pour reveiller tout le monde*/
        for(i=0;i<NBNOEUD;i++){
            if (i==0)
                kill(fils[0],SIGUSR1);
            else{
                kill(fils[i],SIGUSR1);
            }
        }
        
        /*on va faire attendre un peu le père qui tourne en parrallele que ses fils ai le temps de vivre*/
        sleep(0.8);/*on garde une base de 0.5 sec pour 4 proc çà marchait bien*/
        /*le pere récupere des messages d'informations*/
        read(tabPipe[NBNOEUD][LIRE], cmd, strlen(cmd));
        propre(cmd);
        /*puis on les affiches*/
        printf ("Ecriture de PERE message reçu: %s \n\n", cmd);
        
        /*et on recommence tant que l'utilisateur en a envie !*/
    }while(1);
}

/**
 * Cette fonction permet de fermer tout les pipes dans un mode sauf 1
 * @param    mode, le mode LIRE/ECRIRE
 * @param    save, le pipe à garder
 * @return   void
 */
void fermer_pipes(int mode,int save){
    int j;
    for (j=0; j<save; j++)
        close(tabPipe[j][mode]);
    j++;/*on saute save*/
    for (; j<NBNOEUD; j++)
        close(tabPipe[j][mode]);
}

/**
 * fonction pour le premier fils
 * @param    void
 * @return   void
 */
void premierfils() {
    int id,key;
    char cmd[CMDMAX],word[MOTMAX],*s;
    
    /* On ferme les descripteurs*/
    /* le premier fils lit dans le tube qui le lie au dernier frère */
    /* donc on ferme les autres en lecture */
    fermer_pipes(LIRE,NBNOEUD-1);
    close(tabPipe[NBNOEUD][LIRE]);
    /* on écrit dans le premier tube et on ferme les autres sauf le père*/
    fermer_pipes(ECRIRE,0);
    
    /*endormissement apres creation*/
    pause();
    /*réveil lors de la reception du signal SIGUSR1*/

    while(1){
        etat();
        /*on recupere la commande envoyé du père */
        read(tabPipe[NBNOEUD-1][LIRE], cmd, CMDMAX);
        propre(cmd);
        /* on la parse et la stock dans des variables pratique*/
        sscanf(cmd,"%d %d %s",&id,&key,word);

        /*traitement de nos cas*/
        if(id==1 || id==2){
            if((key%NBNOEUD)==0){
				/*si la clé respecte les condition de la table i on la stock (set),ou on y accede (lookup)*/
                if(id==1)
                    store(&Dico,key,word);
                else
                    ((s=lookup(Dico,key))==NULL)? ERRKEY : printf("mot recherché : %s\n",s);
                /*si tout c'est bien passé on va écrire au père ou cela c'est produit*/
                sprintf(cmd,"f0?\n");
                write (tabPipe[NBNOEUD][ECRIRE], cmd, strlen(cmd));
                /*puis on va dire aux freres que c'est bon, on a traiter la demande*/
                sprintf(cmd,"0 0 0?");
                write (tabPipe[0][ECRIRE], cmd, strlen(cmd));
            }else{
                /* sinon si on ne peu rien faire on transmet la commande aux freres*/
                sprintf(cmd,"%s?",cmd);    
                write (tabPipe[0][ECRIRE], cmd, strlen(cmd));
            }
        }
        /*cas du dump*/
        if(id==3){
            /*on écrit notre table*/
            display(Dico);
            /*on écrit au père qu'on a bien reçu l'information et qu'on va la traiter*/
            sprintf(cmd,"dump?");
            write (tabPipe[NBNOEUD][ECRIRE], cmd, strlen(cmd));          
        }
        /*on se rendort*/
        pause();
        /*on recommence si on est reveillé*/
    }
}

/**
 * fonction pour tous les fils sauf le premier
 * @param    numéro, numéro de création du fils
 * @return   void
 */
void autrefils(int numero) {
    int id,key;
    char cmd[CMDMAX],word[MOTMAX],*s;
    
    /*fermeture des pipes*/
    fermer_n_pipes(numero);
    close(tabPipe[NBNOEUD][LIRE]);
    
    
    pause();
    
    while(1){
        etat();
        /*lecture de la cmd + parse*/
        read(tabPipe[numero-1][LIRE], cmd, CMDMAX);
        propre(cmd);
        sscanf(cmd,"%d %d %s",&id,&key,word);

        /*on gere nos cas ici */
        if(id==1 || id==2){
            if((key%NBNOEUD)==numero){
                if(id==2){
                    ((s=lookup(Dico,key))==NULL)? ERRKEY : printf("mot recherché : %s\n",s);
                }
                else
                store(&Dico,key,word);           
                sprintf(cmd,"f%d?\n",numero);
                write (tabPipe[NBNOEUD][ECRIRE], cmd, strlen(cmd));
                sprintf(cmd,"0 0 0?");
                write (tabPipe[numero][ECRIRE], cmd, strlen(cmd));
            }else{
                sprintf(cmd,"%s?",cmd);    
                write (tabPipe[numero][ECRIRE], cmd, strlen(cmd));
            }
        }
        
        if(id==3)
            display(Dico);
        
        pause();
    }
}

/**
 * fonction handler associé à un signal personnalisé
 * il va nous permettre de simplement réveillé les processus fils qu'on a endormit préalablement
 * @param    sig, le signal qu'il va traiter
 * @return   void
 */
void reveil(int sig) {
    ;
}

/**
 * Cette fonction permet de fermer tout les pipes en lecture et écriture qui sont inutile au processus i
 * @param    numéro, le numero du processus en question
 * @return   void
 */
void fermer_n_pipes(int numero){
	int j;
	/*On ferme tous les pipes sauf celui d'indice numéro en ECRITURE 
	et celui de numéro-1 en LECTURE*/
    for (j=0; j<NBNOEUD; j++) {
        if(j==numero)
            close (tabPipe[j][LIRE]);
        else if(j==numero-1)
            close (tabPipe[j][ECRIRE]);
        else{
            close (tabPipe[j][LIRE]);
            close (tabPipe[j][ECRIRE]);
        }
    }
}
