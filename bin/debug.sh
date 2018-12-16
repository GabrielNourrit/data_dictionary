#!/bin/bash

aff=$(ps -ax |grep "dictionnaire")
echo $aff>./debug.log
lignes=$(ps -ax |grep dictionnaire |wc -l)
lignes=$(( lignes - 1 ))
i=1

echo "situation actuel des processus">>./debug.log
echo "">>./debug.log
echo "on va analyser les fils et les pipes de tout ces processus">>./debug.log
echo "">>./debug.log

while [ $i -le $lignes ]
do
    
    pid=$(ps -ax |grep dictionnaire | cut -f1 -d" "|head -$i |tail -1)
    #xubuntu
    tpid="vide"$pid
    if [ $tpid == "vide" ]
    then
        pid=$(ps -ax |grep dictionnaire | cut -f2 -d" "|head -$i |tail -1)
        #ubuntu
    fi
    echo "je suis le processus $pid">>./debug.log
    echo "analyse de mes fils ...">>./debug.log
    echo "le fils supplementaire correspond à la console de debug !">>./debug.log
    aff=$(pgrep -P $pid)
    echo $aff>>./debug.log
    echo "press key...">>./debug.log
    echo "">>./debug.log
    echo "analyse de mes pipes ...">>./debug.log
    aff=$(lsof -p $pid |grep pipe)
    echo $aff>>./debug.log
    echo "press key...">>./debug.log
    echo "">>./debug.log
    i=$(( $i+ 1 ))
done

echo "">>./debug.log
echo "inscription terminé dans debug.log !"
read a

