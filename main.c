#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "tabl.h"
#include "semant.h"
#include "codegen.h"

int main(int argc, char const *argv[]) {
		tokens* Tokens = ident(argc, argv[2]);
		//tokens* Tokens = ident(argc, argv[1]);
	if (strcmp(argv[1], "--dump-tokens") == 0) {
    	printtok(Tokens);
    }

	if (strcmp(argv[1], "--dump-ast") == 0) {
		struct AST* root = initParse(Tokens);
    	createTree(root);
    }


	if (strcmp(argv[1], "--dump-asm") == 0) {
		printtok(Tokens);
		struct AST* root = initParse(Tokens);
		createTree(root);
		Init_semantic(root);
		print_table(root);
		code_gen(root);
    }

    /*struct AST* root = initParse(Tokens);
    createTree(root);
    Init_semantic(root);
    print_table(root);
	code_gen(root);*/
    return 0;
}