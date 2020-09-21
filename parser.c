#include "parser.h"

struct AST* initParse(tokens* Tokens) {
	parse = (Parse*) calloc(1, sizeof(Parse));
	parse->Tokens = Tokens;
	parse->count = 0;
	parse->lookahead = Tokens;
	parse->root = NULL;

	Start(parse);
	printf("End parsing\n");
	return parse->root;
}

void Start(Parse* parse) {
	struct AST* StartNode = initASTNode();
	setString(StartNode, "start");
	parse->root = StartNode;
	match("start");
	StList(StartNode);
	match("end");
}

void StList(struct AST* StartNode) {
	if (strcmp(parse->lookahead->token, "print") == 0) {
		struct AST* printNode = initASTNode();
		setString(printNode, "print");
		add_child(printNode, StartNode);

		print(printNode);
		StList(StartNode);
	} else if (strcmp(parse->lookahead->token, "var") == 0) {
		struct AST* varNode = initASTNode();
		setString(varNode, "var");
		add_child(varNode, StartNode);

		initi(varNode);
		StList(StartNode);
	} else if (strcmp(parse->lookahead->token, "if") == 0) {
		ifstmt(StartNode);
		StList(StartNode);
	} else if (strcmp(parse->lookahead->token, "while") == 0) {
		While(StartNode);
		StList(StartNode);
	} /*else if (strcmp(parse->lookahead->token, "do") == 0) {
		dowhile(StartNode);
		StList(StartNode);
	}*/
}

void semiid(struct AST* node) {
	struct AST* varNode = initASTNode();
	setString(varNode, "var");
	setToken(varNode, getLookahead());
	match("var");

	struct AST *idNode = initASTNode();
	setString(idNode, parse->lookahead->token);
	setToken(idNode, getLookahead());
	add_child(idNode, node);

	match ("id");
	match("semi");
}

void print(struct AST *node) {
	match("print");
	if (strcmp(parse->lookahead->token, "var") == 0) {
		match("var");
		struct AST *idNode = initASTNode();
		setString(idNode, "var");
		//setToken(idNode, parse->lookahead);
		add_child(idNode, node);
		id_or_sign_number(node);
		//match ("id");
		match("semi");
	} else if (strcmp(parse->lookahead->token, "stringliteral") == 0) {
		struct AST* literalNode = initASTNode();
		setString(literalNode, "stringliteral");
		add_child(literalNode, node);
		match("stringliteral");
		match("semi");
	} else {
		printf("error: %d:%d: expecting id or literal, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void initi(struct AST* node) {
	match("var");
	struct AST *idNode = initASTNode();
	setString(idNode, parse->lookahead->token);
	setToken(idNode, parse->lookahead);
	add_child(idNode, node);
	match("id");
	if (strcmp(parse->lookahead->token, "sq_l_paren") == 0) {
		match("sq_l_paren");
		sign_number(node);
		match("sq_r_paren");
	} else
		varorarr(node);
}

void varorarr(struct AST* node) {
	int TypeNode;
	if (strcmp(parse->lookahead->token, "equal") == 0) {
		match("equal");
		/*if (strcmp(parse->lookahead->token, "id") == 0) {
			struct AST *child = getLastChild(node);
			setString(child, "id");
		} else */if (strcmp(parse->lookahead->token, "input") == 0) {
			struct AST* inputNode = initASTNode();
			setString(inputNode, "input");
			add_child(inputNode, getLastChild(node));

			match("input");
			input(inputNode);
		} else if (strcmp(parse->lookahead->token, "var") == 0) {

			struct AST* AssignNode = initASTNode();
			setString(AssignNode, "assign");
			add_child(AssignNode, node);		
			match ("var");

			struct AST* varNode = initASTNode();
			setString(varNode, "var");
			//setToken(varNode, parse->lookahead);
			//swapChild(node, AssignNode);
			//setToken(varNode, parse->lookahead);

			add_child(varNode, AssignNode);
		    id_or_sign_number(AssignNode);
			if (strcmp(parse->lookahead->token, "sq_l_paren") == 0) {
				match("sq_l_paren");
				sign_number(node);
				match("sq_r_paren");
			}
			term(AssignNode);

		} else if (strcmp(parse->lookahead->token, "numeric16") == 0) {
			match("numeric16");
			if (strcmp(parse->lookahead->token, "semi") == 0) {
				TypeNode = 1;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
				match("semi");
			} else {
				struct AST* AssignNode = initASTNode();
				setString(AssignNode, "assign");
				swapChild(node, AssignNode);
				term(AssignNode);
				match("semi");
			}
		} else if (strcmp(parse->lookahead->token, "numeric10") == 0) {
			match("numeric10");
			//litornum(node);
			if (strcmp(parse->lookahead->token, "semi") == 0) {
				TypeNode = 1;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
				match("semi");
			} else {
				struct AST* AssignNode = initASTNode();
				setString(AssignNode, "assign");
				swapChild(node, AssignNode);
				term(AssignNode);
				match("semi");
			}
		} else if (strcmp(parse->lookahead->token, "numeric8") == 0) {
			match("numeric8");
			if (strcmp(parse->lookahead->token, "semi") == 0) {
				TypeNode = 1;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
				match("semi");
			} else {
				struct AST* AssignNode = initASTNode();
				setString(AssignNode, "assign");
				swapChild(node, AssignNode);
				term(AssignNode);
				match("semi");
			}
		} else if (strcmp(parse->lookahead->token, "numeric2") == 0) {
			match("numeric2");
			if (strcmp(parse->lookahead->token, "semi") == 0) {
				TypeNode = 1;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
				match("semi");
			} else {
				struct AST* AssignNode = initASTNode();
				setString(AssignNode, "assign");
				swapChild(node, AssignNode);
				term(AssignNode);
				match("semi");
			}
		} else if (strcmp(parse->lookahead->token, "stringliteral") == 0) {
			match("stringliteral");
			TypeNode = 2;
			struct child* Node = node->children;
			while (Node != NULL) {
				Node->Node->type = TypeNode;
				Node = Node->next;
			}
			match("semi");
		} else if (strcmp(parse->lookahead->token, "array") == 0) {
			match("array");
			match("l_paren");
			struct AST* arrayNode = getLastChild(node);
			setString(arrayNode, "array");
			litornum(arrayNode);
			match("r_paren");
			match("semi");
		}
	} else {
		printf("error: %d:%d: expecting id/equal/num/lit or input or array, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void litornum(struct AST* node) {
	int TypeNode;
	/*if (strcmp(parse->lookahead->token, "stringliteral") == 0) {
		struct AST *idNode = initASTNode();
		setString(idNode, "id");
		add_child(idNode, node);
		TypeNode = 2;
		struct child* Node = node->children;
		while (Node != NULL) {
			Node->Node->type = TypeNode;
			Node = Node->next;
		}
		match("stringliteral");
		if (strcmp(parse->lookahead->token, "equal") == 0) {
			match ("equal");
			if (strcmp(parse->lookahead->token, "numeric10") == 0) {
				struct AST *numNode = initASTNode();
				setString(numNode, "numeric10");
				add_child(numNode, node);
				TypeNode = 1;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
				match("numeric10");
			} 
			else if (strcmp(parse->lookahead->token, "stringliteral") == 0) {
				struct AST *literalNode = initASTNode();
				setString(literalNode, "stringliteral");
				add_child(literalNode, node);
				match("stringliteral");
				TypeNode = 2;
				struct child* Node = node->children;
				while (Node != NULL) {
					Node->Node->type = TypeNode;
					Node = Node->next;
				}
			}
		}
	} else 	*/if (strcmp(parse->lookahead->token, "numeric16") == 0) {
		struct AST *numNode = initASTNode();
		setString(numNode, "numeric16");
		add_child(numNode, node);
		TypeNode = 1;
		struct child* Node = node->children;
		while (Node != NULL) {
			Node->Node->type = TypeNode;
			Node = Node->next;
		}
		match("numeric16");
		if (strcmp(parse->lookahead->token, "comma") == 0) {
			match("comma");
			litornum(node);
		}
	} else 	if (strcmp(parse->lookahead->token, "numeric10") == 0) {
		struct AST *numNode = initASTNode();
		setString(numNode, "numeric10");
		add_child(numNode, node);
		TypeNode = 1;
		struct child* Node = node->children;
		while (Node != NULL) {
			Node->Node->type = TypeNode;
			Node = Node->next;
		}
		match("numeric10");
		if (strcmp(parse->lookahead->token, "comma") == 0) {
			match("comma");
			litornum(node);
		}
	} else 	if (strcmp(parse->lookahead->token, "numeric8") == 0) {
		struct AST *numNode = initASTNode();
		setString(numNode, "numeric8");
		add_child(numNode, node);
		TypeNode = 1;
		struct child* Node = node->children;
		while (Node != NULL) {
			Node->Node->type = TypeNode;
			Node = Node->next;
		}
		match("numeric8");
		if (strcmp(parse->lookahead->token, "comma") == 0) {
			match("comma");
			litornum(node);
		}
	} else 	if (strcmp(parse->lookahead->token, "numeric2") == 0) {
		struct AST *numNode = initASTNode();
		setString(numNode, "numeric2");
		add_child(numNode, node);
		TypeNode = 1;
		struct child* Node = node->children;
		while (Node != NULL) {
			Node->Node->type = TypeNode;
			Node = Node->next;
		}
		match("numeric2");
		if (strcmp(parse->lookahead->token, "comma") == 0) {
			match("comma");
			litornum(node);
		}
	} else {
		printf("error: %d:%d: expecting id or numeric, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void input(struct AST * inputNode) {
	match("l_paren");
	if (strcmp(parse->lookahead->token, "stringliteral") == 0) {
		struct AST* printNode = initASTNode();
		setString(printNode, "print_sringliteral");
		add_child(printNode, inputNode);
		/*struct AST* literalNode = initASTNode();
		setString(literalNode, "stringliteral");
		add_child(literalNode, printNode);*/

		match("stringliteral");
	} else if (strcmp(parse->lookahead->token, "r_paren") == 0) {
		match("r_paren");
		match("semi");
	}
	else {
		printf("error: %d:%d: expecting " " or ' ' or literal, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
	match ("r_paren");
	match("semi");
}

void term(struct AST * node) {
	add_prior1(node);
}

void add_prior1(struct AST * node) {
	mult_prior1(node);
	add_prior2(node);
}

void add_prior2(struct AST * node) {////////
	if ((strcmp(parse->lookahead->token, "plus") == 0) ||
	        (strcmp(parse->lookahead->token, "minus") == 0)) {
		struct AST *operNode = initASTNode();
		setString(operNode, parse->lookahead->token);
		swapChild(node, operNode);

		add_prior_oper(node);
		mult_prior1(operNode);
		add_prior2(node);
	}
}

void mult_prior1(struct AST * node) {
	group(node);
	mult_prior2(node);
}

void mult_prior2(struct AST * node) {
	if ((strcmp(parse->lookahead->token, "pow") == 0) ||
		(strcmp(parse->lookahead->token, "multiply") == 0) ||
	        (strcmp(parse->lookahead->token, "divide") == 0) ||
	        (strcmp(parse->lookahead->token, "mod") == 0)) {
		struct AST *operNode = initASTNode();
		setString(operNode, parse->lookahead->token);
		swapChild(node, operNode);

		mult_prior_oper(node);
		group(operNode);
		mult_prior2(node);
	}
	else {
		match("semi");
	}
}

void group(struct AST * node) {
	if (strcmp(parse->lookahead->token, "l_paren") == 0) {
		match ("l_paren");
		term(node);
		match ("r_paren");
	} else if ((strcmp(parse->lookahead->token, "id") == 0) ||
		(strcmp(parse->lookahead->token, "pow") == 0) ||
		(strcmp(parse->lookahead->token, "multiply") == 0) ||
		(strcmp(parse->lookahead->token, "divide") == 0) ||
		(strcmp(parse->lookahead->token, "mod") == 0) ||
		(strcmp(parse->lookahead->token, "plus") == 0) ||
	    (strcmp(parse->lookahead->token, "minus") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric16") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric10") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric8") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric2") == 0) ) {
		id_or_sign_number(node);
	}
}

void mult_prior_oper(struct AST * node) {
	if (strcmp(parse->lookahead->token, "pow")) {
	 	match ("pow");
	} else if (strcmp(parse->lookahead->token, "multiply") == 0) {
		match ("multiply");
	} else if (strcmp(parse->lookahead->token, "divide") == 0) {
		match("divide");
	} else if (strcmp(parse->lookahead->token, "mod") == 0) {
		match ("mod");
	}/* else {
		printf("error: %d:%d: expecting multiply or divide, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}*/
}

void add_prior_oper(struct AST * node) {
	if (strcmp(parse->lookahead->token, "plus") == 0) {
		match ("plus");
	} else if (strcmp(parse->lookahead->token, "minus") == 0) {
		match("minus");
	}/*  else {
		printf("error: %d:%d: expecting plus or minus, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}*/
}

void id_or_sign_number(struct AST * node) {
	if (strcmp(parse->lookahead->token, "id") == 0) {
		struct AST *idNode = initASTNode();
		setString(idNode, parse->lookahead->token);
		setToken(idNode, parse->lookahead);
		add_child(idNode, node);

		match ("id");
		if (strcmp(parse->lookahead->token, "sq_l_paren") == 0) {
			match("sq_l_paren");
			sign_number(node);
			match("sq_r_paren");
		}
	} else if ((strcmp(parse->lookahead->token, "pow") == 0) ||
		(strcmp(parse->lookahead->token, "multiply") == 0) ||
		(strcmp(parse->lookahead->token, "divide") == 0) ||
		(strcmp(parse->lookahead->token, "mod") == 0) ||
		(strcmp(parse->lookahead->token, "plus") == 0) ||
	    (strcmp(parse->lookahead->token, "minus") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric16") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric10") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric8") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric2") == 0)) {
		sign_number(node);
	} else {
		printf("error: %d:%d: expecting id or arithmetic operations or numeric, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void sign_number(struct AST * node) {
	if ((strcmp(parse->lookahead->token, "pow") == 0) ||
		(strcmp(parse->lookahead->token, "multiply") == 0) ||
		(strcmp(parse->lookahead->token, "divide") == 0) ||
		(strcmp(parse->lookahead->token, "mod") == 0) ||
	    (strcmp(parse->lookahead->token, "var") == 0) ||
	    (strcmp(parse->lookahead->token, "plus") == 0) ||
	    (strcmp(parse->lookahead->token, "minus") == 0) ||
	   	(strcmp(parse->lookahead->token, "numeric16") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric10") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric8") == 0) ||
	    (strcmp(parse->lookahead->token, "numeric2") == 0)) {
		sign(node);///////////
		if ((strcmp(parse->lookahead->token, "var") == 0)) {
			match("var");
			struct AST *varNode = initASTNode();
			setString(varNode, "var");
			add_child(varNode, node);
			id_or_sign_number(node);
		} else {
			if (strcmp(parse->lookahead->token, "numeric16") == 0) 
				match("numeric16");
			else if (strcmp(parse->lookahead->token, "numeric10") == 0){
				match("numeric10");
			}
			else if (strcmp(parse->lookahead->token, "numeric8") == 0)
				match("numeric8");
			else if (strcmp(parse->lookahead->token, "numeric2") == 0)
				match("numeric2");
		}
	} else {
		printf("error: %d:%d: expecting arithmetic operations, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void sign(struct AST * node) {
	struct AST *operNode = initASTNode();
	setString(operNode, parse->lookahead->token);
	add_child(operNode, node);
	if (strcmp(parse->lookahead->token, "pow") == 0) {
		match("pow");
	} else if (strcmp(parse->lookahead->token, "multiply") == 0){
		match("multiply");
	} else if (strcmp(parse->lookahead->token, "divide") == 0){
		match("divide");
	} else if (strcmp(parse->lookahead->token, "mod") == 0){
		match("mod");
	} else if (strcmp(parse->lookahead->token, "plus") == 0){
		match("plus");
	} else if (strcmp(parse->lookahead->token, "minus") == 0){
		match("minus");
	}

}

void ifstmt(struct AST * StartNode)
{
	match("if");
	struct AST *ifNode = initASTNode();
	setString(ifNode, "if");
	add_child(ifNode, StartNode);

	struct AST *exprNode = initASTNode();
	setString(exprNode, "expression");
	add_child(exprNode, ifNode);

	expression(exprNode);

	struct AST *thenNode = initASTNode();
	setString(thenNode, "bodyIf");
	add_child(thenNode, ifNode);
	match("open");

	while (strcmp(parse->lookahead->token, "close") != 0) {
		StList(thenNode);
	}
	match("close");
	if (strcmp(parse->lookahead->token, "else") == 0) {
		match("else");
		struct AST *elseNode = initASTNode();
		setString(elseNode, "else");
		add_child(elseNode, ifNode);
		match("open");

		while (strcmp(parse->lookahead->token, "close") != 0) {
			StList(elseNode);
		}
		match("close");
	}
	StList(StartNode);
}

void expression(struct AST * node) {
	if ((strcmp(parse->lookahead->token, "l_paren") == 0)) {
		match("l_paren");
		if ((strcmp(parse->lookahead->token, "var") == 0) ||
		    (strcmp(parse->lookahead->token, "numeric16") == 0) ||
			(strcmp(parse->lookahead->token, "numeric10") == 0) ||
			(strcmp(parse->lookahead->token, "numeric8") == 0) || 
			(strcmp(parse->lookahead->token, "numeric2") == 0)){
			//(strcmp(parse->lookahead->token, "r_paren") == 0))
			first_prior_expr1(node);
		} else {
			printf("error: %d:%d: expecting var or arithmetic operations or numeric, find %s\n",
			       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
			exit(1);
		}
	} else {
		printf("error: %d:%d: expecting l_paren; find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
	if ((strcmp(parse->lookahead->token, "stringliteral") == 0)){
		match("stringliteral");
		struct AST *strNode = initASTNode();
		setString(strNode, "stringliteral");
		add_child(strNode, node);
	}
	if ((strcmp(parse->lookahead->token, "r_paren") == 0))
		match("r_paren");
	else {
		printf("error: %d:%d: expecting r_paren; find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}

void first_prior_expr1(struct AST * node) {
	group_expr(node);
	first_prior_expr2(node);
}

void first_prior_expr2(struct AST * node) {
	if ((strcmp(parse->lookahead->token, "less") == 0) ||
		(strcmp(parse->lookahead->token, "more") == 0) ||
		(strcmp(parse->lookahead->token, "double_equal") == 0) ||
		(strcmp(parse->lookahead->token, "negation") == 0) ||
		(strcmp(parse->lookahead->token, "equal") == 0) ||
	    (strcmp(parse->lookahead->token, "not_equal") == 0) ||
	    (strcmp(parse->lookahead->token, "less_or_equal") == 0) ||
	    (strcmp(parse->lookahead->token, "more_or_equal") == 0) ||
		(strcmp(parse->lookahead->token, "and&&") == 0) ||
		(strcmp(parse->lookahead->token, "or||") == 0) ||
		(strcmp(parse->lookahead->token, "and") == 0) ||
		(strcmp(parse->lookahead->token, "xor") == 0) ||
		(strcmp(parse->lookahead->token, "or") == 0)) {
		struct AST *conditionNode = initASTNode();
		setString(conditionNode, parse->lookahead->token);
		swapChild(node, conditionNode);

		condition(node);
		group_expr(conditionNode);
		first_prior_expr2(node);
	}
}

void group_expr (struct AST * node) {
	if (strcmp(parse->lookahead->token, "var") == 0) {
		match("var");
	struct AST *varNode = initASTNode();
	setString(varNode, "var");
	add_child(varNode, node);
	id_or_sign_number(node);
	} else if (strcmp(parse->lookahead->token, "numeric16") == 0 ||
		(strcmp(parse->lookahead->token, "numeric10") == 0) ||
		(strcmp(parse->lookahead->token, "numeric8") == 0) ||
		(strcmp(parse->lookahead->token, "numeric2") == 0)) {
		sign_number(node);
	} else if (strcmp(parse->lookahead->token, "stringliteral")) {
		match("stringliteral");//////
		struct AST *strNode = initASTNode();
		setString(strNode, "stringliteral");
		add_child(strNode, node);
	} else if (strcmp(parse->lookahead->token, "r_paren") == 0) {
		match("r_paren");
	}
}

void condition(struct AST * node) {
	if (strcmp(parse->lookahead->token, "less") == 0) 
		match("less");
	else if (strcmp(parse->lookahead->token, "more") == 0) 
		match("more");
	else if (strcmp(parse->lookahead->token, "double_equal") == 0) 
		match("double_equal");
	else if (strcmp(parse->lookahead->token, "negation") == 0) 
		match("negation");
	else if (strcmp(parse->lookahead->token, "equal") == 0) 
		match("equal");
	else if (strcmp(parse->lookahead->token, "not_equal") == 0) 
		match("not_equal");
	else if (strcmp(parse->lookahead->token, "less_or_equal") == 0) 
		match("less_or_equal");
	else if (strcmp(parse->lookahead->token, "more_or_equal") == 0) 
		match("more_or_equal");
	else if (strcmp(parse->lookahead->token, "and&&") == 0) 
		match("and&&");
	else if (strcmp(parse->lookahead->token, "or||") == 0)
		match("or||");
	else if (strcmp(parse->lookahead->token, "xor") == 0)
		match("xor");
	else if (strcmp(parse->lookahead->token, "and") == 0)
		match("and");
	else if (strcmp(parse->lookahead->token, "or") == 0)
		match("or");
	 /*else {
		printf("error: %d:%d: expecting condition, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}*/
}

void While(struct AST * StartNode) {
	match("while");

	struct AST *whileNode = initASTNode();
	setString(whileNode, "while");
	add_child(whileNode, StartNode);

	struct AST *exprNode = initASTNode();
	setString(exprNode, "expression");
	add_child(exprNode, whileNode);

	expression(exprNode);

	struct AST *thenWhile = initASTNode();
	setString(thenWhile, "bodyWhile");
	add_child(thenWhile, whileNode);
	match("open");
	while (strcmp(parse->lookahead->token, "close") != 0) {
		StList(thenWhile);
	}
	match("close");
	StList(StartNode);
}

/*void dowhile(struct AST* StartNode) {
	if (strcmp(parse->lookahead->token, "do") == 0) {
		struct AST *doNode = initASTNode();
		setString(doNode, "do");
		add_child(doNode, StartNode);
		match("do");

		struct AST *stmNode = initASTNode();
		setString(stmNode, "bodyDo");
		add_child(stmNode, doNode);

		StList(stmNode);

		struct AST *whileNode = initASTNode();
		setString(whileNode, "while");
		add_child(whileNode, doNode);
		match("while");

		struct AST *exprNode = initASTNode();
		setString(exprNode, "expr");
		add_child(exprNode, whileNode);

		expression(exprNode);
		match("semi");
	} else {
		printf("error: %d:%d: expecting do, find %s\n",
		       parse->lookahead->row, parse->lookahead->column, parse->lookahead->token);
		exit(1);
	}
}*/

void match(char* x) {
	if (strcmp(parse->lookahead->token, x) == 0) {
		consume();
	} else {
		printErrorMessage(parse->lookahead->row, parse->lookahead->column, x);
		exit(1);
	}
}

void consume() {
	parse->lookahead = nextToken();
}

tokens* nextToken() {
	parse->count++;
	return parse->lookahead->next;
}

tokens* getLookahead()
{
	return parse->lookahead;
}

void printErrorMessage(int row, int column, char *x) {
	printf("%d:%d: error: expecting %s; find %s\n", row, column, x, parse->lookahead->token);
}