#include "lexer.h"
#include "AST.h"
#include "parser.h"

int main(int argc, char const *argv[]) {
		tokens* Tokens = ident(argc, argv[2]);
	if (strcmp(argv[1], "--dump-tokens") == 0) {
    	printtok(Tokens);
    }

	if (strcmp(argv[1], "--dump-ast") == 0) {
		struct AST* root = initParse(Tokens);
    	createTree(root);
    }

    return 0;
}