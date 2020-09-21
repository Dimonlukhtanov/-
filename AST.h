#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

struct AST {
    char *string;
    tokens* Token;
    struct child* children;
    int idNode;
    int type;
    struct Id_Table* table;
};

struct AST* initASTNode();

struct child {
    struct AST *Node;
    struct child *next;
};

struct child *chinit();
struct child *addChild(struct child *spisok, struct AST *Node);

void add_child(struct AST* Node, struct AST* parent);

void setString(struct AST* node, char *str);
void setToken(struct AST* node, tokens *token);

struct child* searchLastChild(struct AST* node);
struct AST* getLastChild(struct AST* node);
void deleteLastChild(struct AST *node);
void swapChild(struct AST *parent, struct AST *newChild);
void addNewChild(struct AST *parent, struct AST *newChild);

void createTree(struct AST* Node);
void createConnect(struct AST* Node);
void createBox(struct AST* Node);
void toGvNode(struct AST* Node);

#endif
