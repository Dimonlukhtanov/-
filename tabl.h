#ifndef TABL_H
#define TABL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"

#define HASHTAB_SIZE 71
#define HASHTAB_MUL  21//31

struct listnode
{
    char *key;//наименование id 
    int value; //хэш-ключ
    //enum Base_Type base_type;
    int base_type;//1var 2arr 3func///1int 2string 3unknown
    //enum Type type;
    int type;//1int 2string/// 1var 2array 3func 
    int offset; //смещение в стеке
    int size; 
    int count_el; //кол-во эл-в. для массива
    struct listnode *hashkoll; //для решения коллизий
    struct listnode *next;  //переход к след узлу
};

struct Id_Table
{
    struct listnode *hashtab[HASHTAB_SIZE];
    struct Id_Table *next;
    int level;
    int sizeTable;
};

int hashtab_hash(char *key);

void hashtab_init(struct listnode **hashtab);
void hashtab_add(struct listnode **hashtab,char *key, int value, int base_type, int type);
void hashtab_delete(struct listnode **hashtab,char *key);
void hashtab_setOffset(struct listnode **hashtab, char *key, int offset);
void addSizeTable(struct Id_Table *table, char *key);

struct listnode *hashtab_lookup(struct listnode **hashtab, char *key);
struct Id_Table *Id_Table_Init (int level);
struct listnode *Find_in_all_table(struct Id_Table *Table, char *key);

void Add_Size_Table(struct Id_Table *table);

#endif