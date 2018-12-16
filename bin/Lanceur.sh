#!/bin/bash

a="init"
err=0

while [ $a != "vide" ]
do
    if [ $err -eq '1' ]
    then
        echo "ERR: Veuillez saisir un chiffre s'il vous plait !"
    fi
    echo "Combien voulez vous de noeud ?"
    read a
    nombre=$a
    a=$( echo vide$a | tr -d "[0-9]" )
    err=1
done

#si on est ici l'user à entré un chiffre

#maintenant on va voir si l'utilisateur veux le mode debug ou pas

a="init"
err=0

while [ $a != "vide" ]
do
    if [ $err -eq '1' ]
    then
        echo -e "ERR: Veuillez saisir 1 (pour oui) ou 0 s'il vous plait !\n>>"
    fi
    echo "Voulez vous que le mode debug soit activé ? (1:oui 0:non)"
    read a
    debug=$a
    a=$( echo vide$a | tr -d "[01]" )
    err=1
done

#on traite la demande de l'utilisateur
make DEBUG=$debug NBNOEUD=$nombre