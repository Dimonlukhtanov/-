#ifndef SEMAN_H
#define SEMAN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AST.h"
#include "tabl.h"
#include "lexer.h"

int Count_Error;

void Init_semantic(struct AST *root);
void Tree_traversal(struct Id_Table *table, struct AST *node);
void print_table(struct AST *node);
void Redecl_Message(struct AST *node);
void Not_Declar_Message(struct AST *node);
void CheckArdList();

#endif