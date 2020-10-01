#include "codegen.h"

void code_gen(struct AST *root)
{
	if ((asmb = fopen("main.asm", "w")) == NULL) {
		printf ("Cannot open file.\n");
		exit(1); 
	}
	
	for (int i = 0; i < 6; i++) {
		regValArg[i] = 0;
		regValAri[i] = 0;
	}
	countJmp = 5;

	fprintf(asmb, "section .text\n");
	fprintf(asmb, "\tglobal\tmain\n");
	fprintf(asmb, "main:\n");
	Push("rbp");
	Mov("rbp", "rsp");

	int sizeTab = root->table->sizeTable * 2;
	char *size = (char*) calloc(getSizeNumber(sizeTab), sizeof(char));
	sprintf(size, "%d", sizeTab);
	Sub("rsp", size);

	treversal(root);

	Add("rsp", size);
	free(size);

	Pop("rbp");
	
	Mov("rax", "1");
	Xor("rbx", "rbx");
	fprintf(asmb, "\tint\t\t0x80\n\n");

	fprintf(asmb, "\textern\tprintf\n");
	fprintf(asmb, "\textern\tscanf\n\n");

	fprintf(asmb, "section .data\n");
	fprintf(asmb, "\tformString db \"%s\", 10, 0\n", "\%s");
	fprintf(asmb, "\tformInt db \"%s\", 10, 0\n", "\%d");
	fprintf(asmb, "\tformInInt db \"%s\", 0\n", "\%d");
	fprintf(asmb, "\tbufInInt dq 0\n");

	fclose(asmb);
}

void treversal(struct AST *node)
{
	struct Id_Table *table = currTable;
	if (node->table != NULL)
		currTable = node->table;
	if (strcmp(node->string, "assign") == 0) {
		struct child *child1 = node->children;
		struct child *child2 = child1->next;

		char *name = child1->Node->Token->lexeme;
		struct listnode *elemOfTab = Find_in_all_table(currTable, name);
		int offset = elemOfTab->offset;

		int sizeNumb = getSizeNumber(offset);
		char* str1 = (char*) calloc(7 + sizeNumb, sizeof(char));
		sprintf(str1, "[rbp-%d]", offset);

		char* str2 = rightPart(child2->Node);

		fprintf(asmb, "\tmov\t\t%s, %s\n", str1, str2);

		free(str1);
		freeRegValAri(str2);
		free(str2);
	} else if (strcmp(node->string, "if") == 0) {
		currTable = node->table;

		createIf(node);

		currTable = table;

		return;
	} else if (strcmp(node->string, "while") == 0) {
		currTable = node->table;
		
		createWhile(node);
		
		currTable = table;

		return;
	} else if (strcmp(node->string, "input") == 0) {

	} else if (strcmp(node->string, "print") == 0) {
		createPrint(node->children->Node);
	}

	struct child* Children = node->children;
	while (Children != NULL) {
		treversal(Children->Node);
		Children = Children->next;
	}
}


void createIf(struct AST *node)
{
	struct AST *exprNode = node->children->Node;
	struct AST *thenNode = node->children->next->Node;
	struct AST *lastChild = NULL;
	if (node->children->next->next != NULL)
		lastChild = node->children->next->next->Node;

	int currJmp = countJmp;
	countJmp += 2;

	Loop(currJmp);

	char* jmp = createExpr(exprNode);
	
	Jmp(jmp, currJmp + 100);

	free(jmp);

	treversal(thenNode);

	if (node->children->next->next != NULL)
		Jmp("jmp", currJmp + 200);

	Loop(currJmp + 100);

	if (lastChild != NULL && strcmp(lastChild->string, "bodyIf") == 0) {
		// stmtListNode(lastChild->getLastChild());
		treversal(lastChild);
	}

	if (node->children->next->next != NULL)
		Loop(currJmp + 200);
}

void createWhile(struct AST *node)
{
	struct AST *exprNode = node->children->Node;
	struct AST *thenNode = node->children->next->Node;

	int currJmp = countJmp;
	countJmp += 2;

	Loop(currJmp);

	char* jmp = createExpr(exprNode);
	
	Jmp(jmp, currJmp + 100);

	free(jmp);

	treversal(thenNode);

	Jmp("jmp", currJmp);
	Loop(currJmp + 100);
}

void createPrint(struct AST *node)
{
	fprintf(asmb, "\n");
	Push("rdi");
	Push("rax");
	Push("rsi");
	fprintf(asmb, "\n");

	if (node->Token != NULL) {
		char *lexeme = node->Token->lexeme;
		// if (node->getToken()->getTokenName() == literal) {
		// 	msg.push_back("\tmsg"+to_string(countLiteral)+" db "+lexeme+", 0");

		// 	operMov("rdi", "formString");
		// 	operMov("rax", "msg" + to_string(countLiteral));

		// 	countLiteral++;
		// 	flagOutput = 1;
		// 	flagFormString = 1;
		// } else
		if (strcmp(node->Token->token, "id") == 0) {
			struct listnode *hashtab = Find_in_all_table(currTable, lexeme);
			if (hashtab->type == 1) {
				Mov("rdi", "formInt");
				int offset = hashtab->offset;
				int sizeNum = getSizeNumber(offset);
				char *str2 = (char*) calloc(6 + sizeNum, sizeof(char));
				sprintf(str2, "[rbp-%d]", offset);

				// Mov("rax", "[rbp-" + getOffset(node) + "]");
				Mov("rax", str2);
				free(str2);

				// flagFormInt = 1;
			}
			// else if (node->getReturnType() == "char") {
			// 	// IdAttributes *idAttrib = currId_Table->searchAttrib(lexeme);

			// 	operMov("rdi", "formChar");
			// 	operMov("rax", "[rbp-" + getOffset(node) + "]");

			// 	flagFormChar = 1;
			// }
			// flagOutput = 1;
		}
	}

	Mov("rsi", "rax");
	Xor("rax", "rax");
	fprintf(asmb, "\tcall\tprintf");

	fprintf(asmb, "\n");
	Pop("rsi");
	Pop("rax");
	Pop("rdi");
	fprintf(asmb, "\n");
}

void createInput(struct AST *node)
{

}

char* rightPart(struct AST *node)
{
	if (strcmp(node->string, "numeric10") == 0) {
		char *reg = regAriNumToStr(getRegForAri());

		Mov(reg, node->Token->lexeme);

		return reg;
	} else if (strcmp(node->string, "var") == 0) {
		char *reg = regAriNumToStr(getRegForAri());
		
		char* offset = getOffset(node);

		char *addr = (char*) calloc(6 + strlen(offset), sizeof(char));
		sprintf(addr, "[rbp-%s]", offset);
		Mov(reg, addr);
		
		free(offset);
		free(addr);

		return reg;
	}

	char *str1 = rightPart(node->children->Node);
	char *str2 = rightPart(node->children->next->Node);

	if (strcmp(node->string, "plus") == 0) {
		Add(str1, str2);
		freeRegValAri(str2);
		free(str2);
	} else if (strcmp(node->string, "minus") == 0) {
		Sub(str1, str2);
		freeRegValAri(str2);
		free(str2);
	} else if (strcmp(node->string, "multiply") == 0) {
		Imul(str1, str2);
		freeRegValAri(str2);
		free(str2);
	} else if (strcmp(node->string, "divide") == 0) {
		Mov("rax", str1);

		char *buf = NULL;
		if (strcmp(str2, "rdx") == 0) {
			buf = regAriNumToStr(getRegForAri());
			Mov(buf, "rdx");
		}

		Xor("rdx", "rdx");
		if (buf != NULL)
			Idiv(buf);
		else
			Idiv(str2);

		setRegForAri(regAriStrToReg("rax"), 1);
		freeRegValAri("rdx");
		freeRegValAri(str1);
		free(str1);
		freeRegValAri(str2);
		free(str2);
		if (buf != NULL)
			free(buf);

		char *reg = (char*) calloc(3, sizeof(char));
		sprintf(reg, "rax");
		return reg;
	}

	return str1;
}

int getRegForAri()
{
	for (int i = 1; i < 6; i++) {
		if (regValAri[i] == 0) {
			setRegForAri(i, 1);
			return i;
		}
	}
	return -1;
}

void setRegForAri(int numReg, int value)
{
	regValAri[numReg] = value;
}

char* regAriNumToStr(int numReg)
{
	char* reg = (char*) calloc(3, sizeof(char));
	switch(numReg)
	{
		case 0:
			strcpy(reg, "rax");
			break;
		case 1:
			strcpy(reg, "rdx");
			break;
		case 2:
			strcpy(reg, "rcx");
			break;
		case 3:
			strcpy(reg, "rsi");
			break;
		case 4:
			strcpy(reg, "r8");
			break;
		case 5:
			strcpy(reg, "r9");
			break;
		default:
			strcpy(reg, "000");
			break;
	}
	return reg;
}

int regAriStrToReg(char* strReg)
{
	int intReg = -1;
	
	if (strcmp(strReg, "rax") == 0) {
		intReg = 0;
	} else if (strcmp(strReg, "rdx") == 0) {
		intReg = 1;
	} else if (strcmp(strReg, "rcx") == 0) {
		intReg = 2;
	} else if (strcmp(strReg, "rsi") == 0) {
		intReg = 3;
	} else if (strcmp(strReg, "r8") == 0) {
		intReg = 4;
	} else if (strcmp(strReg, "r9") == 0) {
		intReg = 5;
	}
	return intReg;
}

// void printRegValAri()
// {
// 	for (int i = 0; i < 6; i++)
// 		cout << regAriNumToStr(i) + ": " + to_string(getValRegAri(i)) << endl;
// }

void freeRegValAri(char* strReg)
{
	setRegForAri(regAriStrToReg(strReg), 0);
}

int getValRegAri(int numReg)
{
	return regValAri[numReg];
}

int getSizeNumber(int num)
{
	int size = 1;
	int mult = 10;
	while (mult < num) {
		mult *= 10;
		size++;
	}
	return size;
}

void Mov(char *str1, char *str2)
{
	fprintf(asmb, "\tmov\t\t%s, %s\n", str1, str2);
}

void Add(char *str1, char *str2)//+
{
	fprintf(asmb, "\tadd\t\t%s, %s\n", str1, str2);
}

void Sub(char *str1, char *str2)//-
{
	fprintf(asmb, "\tsub\t\t%s, %s\n", str1, str2);
}

void Imul(char *str1, char *str2)//
{
	fprintf(asmb, "\timul\t%s, %s\n", str1, str2);
}

void Mul(char *str1, char *str2)//*
{
	fprintf(asmb, "\tmul\t\t%s, %s\n", str1, str2);
}

void Idiv(char *str1)
{
	fprintf(asmb, "\tidiv\t%s\n", str1);
}

void Xor(char *str1, char *str2)
{
	fprintf(asmb, "\txor\t\t%s, %s\n", str1, str2);
}

void Or(char *str1, char *str2)
{
	fprintf(asmb, "\tor\t\t%s, %s\n", str1, str2);
}

void And(char *str1, char *str2)
{
	fprintf(asmb, "\tand\t\t%s, %s\n", str1, str2);
}

void Cmp(char *str1, char *str2)
{
	fprintf(asmb, "\tcmp\t\t%s, %s\n", str1, str2);
}

void Loop(int jmp)
{
	fprintf(asmb, ".L%d:\n", jmp);
}

void Jmp(char *str1, int x)
{
	fprintf(asmb, "\t%s\t\t.L%d\n", str1, x);
}

void Push(char *reg)
{
	fprintf(asmb, "\tpush\t%s\n", reg);
}

void Pop(char *reg)
{
	fprintf(asmb, "\tpop\t\t%s\n", reg);
}

char* getOffset(struct AST *node)
{
	char* nodeLexem = node->Token->lexeme;
	struct listnode *hashtab = Find_in_all_table(currTable, nodeLexem);
	int numOffset = hashtab->offset;
	char *offset = NULL;

	#if 0
	if (hashtab->baseType == 2) {
		struct AST *child = node->children->Node;
		
		char* lexeme = child->Token->lexeme;
		if (strcmp(child->Token->token, "numeric") == 0) {
			int intdex = atoi(lexeme);
			int sizeData = hashtab->size;
			
			numOffset -= intdex * sizeData;
			offset = (char*) calloc(getSizeNumber(numOffset), sizeof(char));
			sprintf(offset, "%d", numOffset);
		} else if (strcmp(child->Token->token, "id") == 0) {
			struct listnode *tabChild = Find_in_all_table(currTable, lexeme);
			

			operMov("rax", to_string(idAttribChlid->getSize()));
			Mov("rax", );
			operImul("rax", "[rbp-" + getOffset(child) + "]");
			offset += "+rax";
		}
	} else {
		offset = (char*) calloc(getSizeNumber(numOffset), sizeof(char));
		sprintf(offset, "%d", numOffset);
	}
	#endif

	offset = (char*) calloc(getSizeNumber(numOffset), sizeof(char));
	sprintf(offset, "%d", numOffset);

	return offset;
}

char* createExpr(struct AST *node)
{
	struct AST *condNode = node->children->Node;

	char* str1 = rightPart(condNode->children->Node);
	char* str2 = NULL;
	if (condNode->children->next != NULL)
		str2 = rightPart(condNode->children->next->Node);

	char* jump = NULL;
	if (str2 == NULL) {

	} else if (str2 != NULL) {
		Cmp(str1, str2);

		if (strcmp(condNode->string, "less") == 0) {//<
			jump = (char*) calloc(3, sizeof(char));
			sprintf(jump, "jge");
		} else if (strcmp(condNode->string, "less_or_equal") == 0) {//<=
			jump = (char*) calloc(2, sizeof(char));
			sprintf(jump, "jg");
		} else if (strcmp(condNode->string, "more") == 0) {//>
			jump = (char*) calloc(3, sizeof(char));
			sprintf(jump, "jle");
		} else if (strcmp(condNode->string, "more_or_equal") == 0) {//>=
			jump = (char*) calloc(2, sizeof(char));
			sprintf(jump, "jl");
		} else if (strcmp(condNode->string, "double_equal") == 0) {//==
			jump = (char*) calloc(3, sizeof(char));
			sprintf(jump, "jne");
		} else if (strcmp(condNode->string, "not_equal") == 0) {//!= <>
			jump = (char*) calloc(2, sizeof(char));
			sprintf(jump, "je");
		}
	}
	
	freeRegValAri(str1);
	free(str1);
	freeRegValAri(str2);
	free(str2);
	return jump;
}