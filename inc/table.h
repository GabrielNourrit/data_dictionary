/*
NOURRIT GABRIEL
NOEL JULES
*/

#ifndef TABLE_H
#define TABLE_H

typedef struct _Table_entry {
	int key;
	char* val;
	struct _Table_entry *next;
} Table_entry;

typedef Table_entry *PTable_entry;


char* lookup(PTable_entry table, int k);
// Retourne la valeur se trouvant dans table sous la cle k ou NULL si la
// clé n'existe pas

void store(PTable_entry *table, int k, char v[]);
// Stocke dans table la valeur v sous la cle k

void display(PTable_entry table);
//Affiche le contenu d'une table

#endif
