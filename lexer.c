#include "lexer.h"

tokens *init() {
	tokens *tok;
	tok = (tokens*)malloc(sizeof(tokens));
	tok->row = -1;
	tok->next = NULL;
	return tok;
}

tokens *addlexeme(tokens *spisok, char *token, char *lexeme, int row, int column) {
	if (spisok->row == -1) {
		strcpy(spisok->lexeme, lexeme);
		strcpy(spisok->token, token);
		spisok->row = row;
		spisok->column = column;
		return spisok;
	} else {
		tokens *spisok2;
		spisok2 = (tokens*)malloc(sizeof(tokens));
		spisok->next = spisok2;
		strcpy(spisok2->lexeme, lexeme);
		strcpy(spisok2->token, token);
		spisok2->row = row;
		spisok2->column = column;
		spisok2->next = NULL;
		return spisok2;
	}
	return NULL;
}

char *dict(char *lexeme) {
	if (strcmp(lexeme, "readline") == 0)
		return "input";
	if ((strcmp(lexeme, "print") == 0) || (strcmp(lexeme, "echo") == 0))
		return "print";
	if (strcmp(lexeme, "+") == 0)
		return "plus";
	if (strcmp(lexeme, "-") == 0)
		return "minus";
	if (strcmp(lexeme, "*") == 0)
		return "multiply";
	if (strcmp(lexeme, "/") == 0)
		return "divide";
	if (strcmp(lexeme, "**") == 0)
		return "pow";
	if (strcmp(lexeme, "%") == 0)
		return "mod";
	if (strcmp(lexeme, "=") == 0)
		return "equal";
	if (strcmp(lexeme, "==") == 0)
		return "double_equal";
	if ((strcmp(lexeme, "<>") == 0 || strcmp(lexeme, "!=") == 0))
		return "not_equal";
	if (strcmp(lexeme, ">") == 0)
		return "more";
	if (strcmp(lexeme, "<") == 0)
		return "less";
	if (strcmp(lexeme, ">=") == 0)
		return "more_or_equal";
	if (strcmp(lexeme, "<=") == 0)
		return "less_or_equal";
	if (strcmp(lexeme, "and") == 0)
		return "and";
	if (strcmp(lexeme, "&&") == 0)
		return "and&&";
	if (strcmp(lexeme, "or") == 0)
		return "or";
	if (strcmp(lexeme, "||") == 0)
		return "or||";
	if (strcmp(lexeme, "xor") == 0)
		return "xor";
	if (strcmp(lexeme, "!") == 0)
		return "negation";
	if (strcmp(lexeme, "if") == 0)
		return "if";
	if (strcmp(lexeme, "then") == 0)
		return "then";
	if (strcmp(lexeme, "else") == 0)
		return "else";
	if (strcmp(lexeme, "{") == 0)
		return "open";
	if (strcmp(lexeme, "}") == 0)
		return "close";
	/*if (strcmp(lexeme, "do") == 0)
		return "do";*/
	if (strcmp(lexeme, "while") == 0)
		return "while";
	if (strcmp(lexeme, "$") == 0)
		return "var";
	if (strcmp(lexeme, "array") == 0)
		return "array";
	if (strcmp(lexeme, "<?php") == 0)
		return "start";
	if (strcmp(lexeme, "?>") == 0)
		return "end";
	if (strcmp(lexeme, "true") == 0)
		return "true";
	if (strcmp(lexeme, "false") == 0)
		return "false";
	if (strcmp(lexeme, "null") == 0)
		return "null";
	if (strcmp(lexeme, "(") == 0)
		return "l_paren";
	if (strcmp(lexeme, ")") == 0)
		return "r_paren";
	if (strcmp(lexeme, "[") == 0)
		return "sq_l_paren";
	if (strcmp(lexeme, "]") == 0)
		return "sq_r_paren";
	if (strcmp(lexeme, ",") == 0)
		return "comma";
	if (strcmp(lexeme, ";") == 0)
		return "semi";
	if (strcmp(lexeme, ".") == 0)
		return "semi";
	if (lexeme[0] == '"' || lexeme[0] == '\'') {
		if (lexeme[strlen(lexeme) - 1] == '"' || lexeme[strlen(lexeme) - 1] == '\'') ///
			return "stringliteral";
		else
			return "unknown";
	}
	if (lexeme[0] == '0' && lexeme[1] == 'x') {//16
		for (int i = 2; i < strlen(lexeme); i++) {
			if (!(((lexeme[i] >= '0' && lexeme[i] <= '9')) || (lexeme[i] >= 'a' && lexeme[i] <= 'z') || (lexeme[i] >= 'A' && lexeme[i] <= 'Z')))
				return "unknown";
		}
		return "numeric16";
	}
	if ((lexeme[0] >= 'A' && lexeme[0] <= 'Z') ||
	        (lexeme[0] >= 'a' && lexeme[0] <= 'z') || (lexeme[0] == '_')) {

		for (int i = 1; i < strlen(lexeme); i++) {
			if (!((lexeme[i] >= 'A' && lexeme[i] <= 'Z') ||
			        (lexeme[i] >= 'a' && lexeme[i] <= 'z') || (lexeme[i] == '_') ||
			        (lexeme[i] >= '0' && lexeme[i] <= '9')))
				return "unknown";
		}
		return "id";
	}

	if (lexeme[0] >= '1' && lexeme[0] <= '9') {//10
		for (int i = 1; i < strlen(lexeme); i++) {
			if (!(lexeme[i] >= '0' && lexeme[i] <= '9'))
				return "unknown";
		}
		return "numeric10";
	}

	if (lexeme[0] == '0' && (lexeme[1] >= '0' && lexeme[1] <= '7')) {//8
		for (int i = 2; i < strlen(lexeme); i++) {
			if (!(lexeme[i] >= '0' && lexeme[i] <= '7'))
				return "unknown";
		}
		return "numeric8";
	}

	if (lexeme[0] == '0' && lexeme[1] == 'b') {//2
		for (int i = 2; i < strlen(lexeme); i++) {
			if (!(lexeme[i] >= '0' && lexeme[i] <= '1'))
				return "unknown";
		}
		return "numeric2";
	}

	if (lexeme[0] == '0')
		return "numeric10";

	return "unknown";
}

tokens *ident(int argc, char const *argv) {
	char buf[150];
	FILE *file;
	file = fopen (argv, "r");
	if (file == NULL) {
		printf("error\n");
		return NULL;
	}

	tokens *spisok = init();
	tokens *head = spisok;

	char sign;
	int i = 0;
	int row = 1, column = 1;
	char *token = (char *) malloc(sizeof(char) * 80);

	while ((sign = fgetc(file)) != EOF) {
		//литералы
		if (sign == '"') {
			buf[i] = sign;
			i++;
			column++;
			while ((sign = fgetc(file)) != '"' && sign != '\n') {
				buf[i] = sign;
				i++;
				column++;
			}
			if (sign == '"') {
				buf[i] = sign;
				i++;
				column++;
			}
			buf[i] = '\0';

			token = dict(buf);
			spisok = addlexeme(spisok, token, buf, row, column - i);
			if (strcmp(token, "unknown") == 0) {
				row++;
				column = 1;
			}
			i = 0;
			continue;
		}
		if (sign == '\'') {
			buf[i] = sign;
			i++;
			column++;
			while ((sign = fgetc(file)) != '\'' && sign != '\n') {
				buf[i] = sign;
				i++;
				column++;
			}
			if (sign == '\'') {
				buf[i] = sign;
				i++;
				column++;
			}
			buf[i] = '\0';

			token = dict(buf);
			spisok = addlexeme(spisok, token, buf, row, column - i);
			if (strcmp(token, "unknown") == 0) {
				row++;
				column = 1;
			}
			i = 0;
			continue;
		}
		//игнор пробелов
		else if (sign == ' ') {
			if (i == 0) {
				column++;
				continue;
			}

			buf[i] = '\0';

			token = dict(buf);
			spisok = addlexeme(spisok, token, buf, row, column - i);

			i = 0;
			column++;
			continue;
		}
		//новая строка
		else if (sign == '\n') {
			if (i == 0) {
				column = 1;
				row++;
				continue;
			}

			buf[i] = '\0';

			token = dict(buf);
			spisok = addlexeme(spisok, token, buf, row, column - i);

			column = 1;
			row++;
			i = 0;
			continue;
		}
		//обработка комментов
		else if (sign == '/') {
			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}

			buf[i] = sign;
			i++;
			column++;
			if ((sign = fgetc(file)) == '/') {
				while ((sign = fgetc(file)) != '\n')
					continue;
				i = 0;
				row++;
				column = 1;
				continue;
			}
			else if (sign == '*') {
				buf[0] = '/';
				buf[1] = '*';
				i = 2;
				int n = 0;
				while (n != 1) {
					sign = fgetc(file);
					if (sign == '*') {
						buf[i] = sign;
						i++;
						column++;
						if ((sign = fgetc(file)) == '/') {
							buf[i] = '\0';
							i = 0;
							n = 1;
						}
						else {
							buf[i] = sign;
							i++;
							column++;
						}
					} else {
						buf[i] = sign;
						i++;
						column++;
					}

					if (sign == '\n') {
						row++;
					}
					if (feof (file) != 0)
					{

						buf[i - 1] = '\0';
						token = "unknown";
						spisok = addlexeme(spisok, token, buf, row, column - i);
						i = 0;
						column = 1;
						n = 1;
					}
				}
				continue;
			} else if (sign == '\n') {
				if (i == 0) {
					column = 1;
					row++;
					continue;
				}

				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				column = 1;
				row++;
				i = 0;
				continue;
			} else {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;

				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}
		//
		else if (sign == ',' || sign == ';' || sign == '+' || sign == '-' ||
		         /* sign == '=' ||*/ sign == '/' || sign == '%' ||
		         sign == '(' || sign == ')' || sign == '[' || sign == ']' || sign == '$') {

			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}

			buf[i] = sign;
			i++;
			column++;
			buf[i] = '\0';

			token = dict(buf);
			spisok = addlexeme(spisok, token, buf, row, column - i);

			i = 0;
			continue;
		}
		else if (sign == '?') {
			buf[i] = sign;
			i++;
			column++;
			if ((sign = fgetc(file)) == '>') {
				buf[i] = sign;
				i++;
				column++;
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				//row++;
				i = 0;
				continue;
			} else {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;

				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}

		else if (sign == '*') {
			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}

			buf[i] = sign;
			i++;
			column++;

			if ((sign = fgetc(file)) == '*') {
				buf[i] = sign;
				i++;
				column++;
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				//row++;
				i = 0;
				continue;
			} else if (sign == '\n') {
				if (i == 0) {
					column = 1;
					row++;
					continue;
				}

				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				column = 1;
				row++;
				i = 0;
				continue;
			} else {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;

				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}

		else if (sign == '=') {
			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}

			buf[i] = sign;
			i++;
			column++;

			if ((sign = fgetc(file)) == '=') {
				buf[i] = sign;
				i++;
				column++;
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				//row++;
				i = 0;
				continue;
			} else if (sign == '\n') {
				if (i == 0) {
					column = 1;
					row++;
					continue;
				}

				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				column = 1;
				row++;
				i = 0;
				continue;
			} else {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;

				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}


		else if (sign == '>' || sign == '!') {
			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}

			buf[i] = sign;
			i++;
			column++;

			if ((sign = fgetc(file)) == '=') {
				buf[i] = sign;
				i++;
				column++;
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
				continue;
			} else {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;

				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}

		else if (sign == '<') {
			if (i != 0) {
				buf[i] = '\0';

				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);

				i = 0;
			}
			buf[i] = sign;
			i++;
			column++;

			sign = fgetc(file);

			if ((sign == '=') || (sign == '>')) {
				buf[i] = sign;
				i++;
				column++;
				buf[i] = '\0';
				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);
				i = 0;
				continue;
			} else if ((sign == '?')) {
				buf[i] = sign;
				i++;
				column++;
				while ((sign = fgetc(file)) != '\n') {
					if (sign == ' ')
						continue;
					buf[i] = sign;
					i++;
					column++;
					continue;

				}
				buf[i] = '\0';
				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);
				row++;
				i = 0;
				continue;
			} else {
				buf[i] = '\0';
				token = dict(buf);
				spisok = addlexeme(spisok, token, buf, row, column - i);
				i = 0;
				if (sign == ' ') {
					column++;
					continue;
				}
			}
		}
		buf[i] = sign;
		i++;
		column++;
	}
	if (i != 0) {
		buf[i] = '\0';
		token = dict(buf);
		spisok = addlexeme(spisok, token, buf, row, column - i);
		i = 0;
	}
	spisok = addlexeme(spisok, "eof", "", row, column - i);
	fclose (file);
	return head;
	return NULL;
}

void printtok(tokens *head) {
	tokens *print;
	print = head;
	while (print != NULL) {
		if (strcmp(print->token, "unknown") == 0)
			printf("\x1b[31;1mLoc=<%d:%d> %s '%s'\x1b[0m\n", print->row, print->column, print->token, print->lexeme);
		else
			printf("Loc=<%d:%d> %s '%s'\n", print->row, print->column, print->token, print->lexeme);
		print = print->next;
	}
}