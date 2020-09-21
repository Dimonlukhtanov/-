#include "AST.h"

struct child *chinit() {
    struct child *children;
    children = (struct child*)malloc(sizeof(struct child));
    children->next = NULL;
    children->Node = NULL;
    return children;
}

struct child *addChild(struct child *spisok, struct AST *Node) {
    if (spisok->Node == NULL) {
        spisok->Node = Node;
        return spisok;
    } else {
        struct child *spisok2;
        spisok2 = (struct child*)malloc(sizeof(struct child));
        spisok->next = spisok2;
        spisok->Node = Node;
        spisok2->next = NULL;
        return spisok2;
    }
    return NULL;
}

int countNode = 1;

struct AST* initASTNode() {
	struct AST* ast;
	ast = (struct AST*) calloc(1, sizeof(struct AST));
	ast->string = NULL;
	ast->Token = NULL;
	ast->children = NULL;
	ast->idNode = countNode;
	countNode++;
	ast->type = 0;
	ast->table = NULL;

	return ast;
}

void add_child(struct AST* Node, struct AST* parent) {
	struct child* childr;
	childr = (struct child*) calloc(1, sizeof(struct child));
	childr->next = NULL;
	childr->Node = Node;
	struct child* pnode = parent->children;  
	if (pnode == NULL) {
		parent->children = childr;
	} else {
		while (pnode->next != NULL)
		{
			pnode = pnode->next;
		}
		pnode->next = childr;
	}
}

void setString(struct AST *node, char* str) {
	node->string = (char*) calloc(strlen(str), sizeof(char*));
	strcpy(node->string, str);
}

void setToken(struct AST *node, tokens *token) {
	node->Token = token;
}

struct child* searchLastChild(struct AST* node) {
	struct child* child = node->children;
	while (child->next != NULL)
		child = child->next;
	return child;
}

struct AST* getLastChild(struct AST* node) {
	return searchLastChild(node)->Node;
}

void deleteLastChild(struct AST *node) {
	struct child* child = searchLastChild(node);
	child->Node = NULL;
}

void swapChild(struct AST *parent, struct AST *newChild) {
	struct AST *oldChild = getLastChild(parent);
	deleteLastChild(parent);
	addNewChild(parent, newChild);
	add_child(oldChild, newChild);
}

void addNewChild(struct AST *parent, struct AST *newChild) {
	struct child *child = searchLastChild(parent);
	child->Node = newChild;
}

void createTree(struct AST* Node) {
	printf("AST {\n");
	createConnect(Node);
	printf("}\n");
}

void createConnect(struct AST* Node) {
	createBox(Node);
	struct child* child = Node->children;
	while (child != NULL) {
		printf("\t");
		toGvNode(Node);
		printf(" -> ");
		toGvNode(child->Node);
		printf("\n");
		createConnect(child->Node);
		child = child->next;
	}
}

void createBox(struct AST* Node) {
	printf("\t\"node%d\" [label=\"", Node->idNode);
	if (Node->string != NULL) {
		printf("TypeNode=%s",Node->string);
	} else {
		printf("TypeNode=unknown");
	}

	if (Node->type == 1) {
		printf("\\ntype=integer");
	} else if (Node->type == 2) {
		printf("\\ntype=string");
	}

	if (Node->Token != NULL) {
		printf("\\nlexeme=%s", Node->Token->lexeme);
	}
	
	printf("\"]\n");
}

void toGvNode(struct AST* Node) {
	printf("\"node%d\"", Node->idNode);
}