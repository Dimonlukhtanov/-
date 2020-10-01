#include "semant.h"

void Init_semantic(struct AST *root){
	root->table = Id_Table_Init(0);
	Tree_traversal(root->table, root);
	Count_Error= 0;
}
void Tree_traversal(struct Id_Table *table, struct AST *node)
{
	struct Id_Table* currTable = table;
	if(strcmp(node->string, "var") == 0 || strcmp(node->string, "input") == 0 || strcmp(node->string, "array") == 0)
	{
		if(node->type == 1 || node->type == 3 || node->type == 2 )
		{
			struct listnode *Found_Node = hashtab_lookup(currTable->hashtab, node->Token->lexeme);
			if (Found_Node == NULL)
			{
				char* name = node->Token->lexeme;
				int value = hashtab_hash(name);
				int base_type;
				if(strcmp(node->string, "var") == 0)
				{
					base_type = 1;
				}else if(strcmp(node->string, "array") == 0)
				{
					base_type = 2;
				} else if (strcmp(node->string, "input") == 0)
				{
					base_type = 1;
				}
				hashtab_add(currTable->hashtab, name, value, node->type, base_type);
				Add_Size_Table(currTable);
			    hashtab_setOffset(currTable->hashtab, name, currTable->sizeTable);
			}
		} else {
			char* name = node->Token->lexeme;
			struct listnode *Found_Node = Find_in_all_table(currTable, name);
			if (Found_Node == NULL)
			{
				Not_Declar_Message(node);
			}
		}
		return;
	}else if (strcmp(node->string, "if") == 0 ||
			strcmp(node->string, "while") == 0)	{
		int lvl = currTable->level + 1;
		node->table = Id_Table_Init(lvl);

		node->table->next = currTable;
		currTable = node->table;
	} 
	struct child* Children = node->children;
	while (Children != NULL)
	{
		Tree_traversal(currTable,Children->Node);
		Children = Children->next;
	}
}
void Redecl_Message(struct AST *node)
{
	Count_Error++;
	printf("%d:", node->Token->row);
	printf("%d:", node->Token->column);
	printf("ERROR: REDECLORTE\t");
	printf("%s\n", node->Token->lexeme);
}
void Not_Declar_Message(struct AST *node)
{
	Count_Error++;
	printf("%d:", node->Token->row);
	printf("%d:\t", node->Token->column);
	printf("ERROR: undeclared (first use in this function)\t ");
	printf("%s\n", node->Token->lexeme);
}

void print_table(struct AST *node)
{
	if (node->table != NULL)
	{
		printf("\tlevel\t=  %d\n", node->table->level);
		for (int i = 0; i < HASHTAB_SIZE; i++)
		{
			struct listnode *hash_node = node->table->hashtab[i] ;
			if (hash_node != NULL)
			{
				printf("id = %s  ", hash_node->key);
				printf("Base Type = %d\t", hash_node->base_type);
				printf("Type = %d\t", hash_node->type);
				printf("offset = %d\n", hash_node->offset);
			}
		}
		printf("SIZE TABLE: %d\n", node->table->sizeTable);
		
	}
	struct child* Children = node->children;
	while (Children != NULL)
	{
		print_table(Children->Node);
		Children = Children->next;
	}
	//printf("!!!!! \n");
}