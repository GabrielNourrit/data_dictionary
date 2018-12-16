/*
NOURRIT GABRIEL
NOEL JULES
*/

#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "global.h"
int tabPipe[NBNOEUD+1][2]; /*on a 1 descripteur de fichier de + que de fils -> c'est celui qui permet aux fils de parler au père*/

/**
 * Cette fonction permet de fermer tout les pipes dans un mode sauf 1
 * @param    mode, le mode LIRE/ECRIRE
 * @param    save, le pipe à garder
 * @return   void
 */
void fermer_pipes(int mode,int save);

/**
 * Cette fonction permet de fermer tout les pipes en lecture et écriture qui sont inutile au processus i
 * @param    numéro, le numero du processus en question
 * @return   void
 */
void fermer_n_pipes(int numero);

/**
 * fonction handler associé à un signal personnalisé
 * il va nous permettre de simplement réveillé les processus fils qu'on a endormit préalablement
 * @param    sig, le signal qu'il va traiter
 * @return   void
 */
void reveil(int sig);

/**
 * fonction du controller 
 * @param    void
 * @return   void
 */
void controller();

/**
 * fonction qui crée tous les fils
 * @param    void
 * @return   void
 */
void creer_fils();

/**
 * fonction pour le premier fils
 * @param    void
 * @return   void
 */
void premierfils();

/**
 * fonction pour tous les fils sauf le premier
 * @param    numéro, numéro de création du fils
 * @return   void
 */
void autrefils(int numero);

#endif
