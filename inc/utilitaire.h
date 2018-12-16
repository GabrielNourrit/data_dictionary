/*
NOURRIT GABRIEL
NOEL JULES
*/

#ifndef UTILITAIRE_H
#define UTILITAIRE_H


/**
 * fonction qui permet de nettoyer la string_commande qu'on se passe via les pipes
 * elle enlève les caractères indésirables qui sont lu dans nos read
 * @param    message, le message que l'on veut nettoyer
 * @return   void
 */
void propre(char*s);

/**
 * fonction qui permet de vider le Buffer de l'entrée standard
 * elle va permettre de gerer les entrées clavier indésirablement trop longue
 * par exemple si un int est attendu et que l'user donne une chaine de caractere
 * @param    void
 * @return   void
 */
void viderBuffer();

/**
 * fonction qui permet de récupérer de manière sûre
 * l'identifiant de la commande que l'on va traiter
 * @param    void
 * @return   le numero d'identifiant (0:exit,1:set,2:lookup,3:dump)
 */
int lire_id();


/**
 * fonction qui permet de récupérer de manière sûre
 * la clé du mot que l'on va traiter
 * @param    void
 * @return   la clé du mot
 */
int lire_key();


/**
 * fonction qui permet de regarder
 * à chaque instant crucial du programme
 * l'activité des procesus traité
 * au cas ou le mode DEBUG est activé
 * @param    void
 * @return   void
 */
void etat();
    
#endif
