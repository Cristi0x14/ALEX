#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

Token tokens[MAX_TOKENS];
int nTokens;

int line = 1;		// the current line in the input file

// adds a token to the end of the tokens list and returns it
// sets its code and line
Token* addTk(int code) {
	if (nTokens == MAX_TOKENS)err("too many tokens");
	Token* tk = &tokens[nTokens];
	tk->code = code;
	tk->line = line;
	nTokens++;
	return tk;
}

// copy in the dst buffer the string between [begin,end)
char* copyn(char* dst, const char* begin, const char* end) {
	char* p = dst;
	if (end - begin > MAX_STR)err("string too long");
	while (begin != end)*p++ = *begin++;
	*p = '\0';
	return dst;
}

void tokenize(const char* pch) {
	const char* start;
	Token* tk;
	char buf[MAX_STR + 1];
	for (;;) {
		switch (*pch) {
		case ' ':
			pch++;
			break;
		case '\t':
			pch++; 
			break;
		case '\r':
			if (pch[1] == '\n')
				pch++;
		case '\n':
			line++;
			pch++;
			break;
		case '\0':
			addTk(FINISH);
			return;
		case ',':
			addTk(COMMA);
			pch++; 
			break;
		case '=':
			if (pch[1] == '=') {
				addTk(EQUAL);
				pch += 2;
			}
			else {
				addTk(ASSIGN);
				pch++;
			}
			break;
		case '#':
			while (*pch != '\0' && *pch != '\n' && *pch != '\r') {
				pch++;
			}
			break;
		case '(':
			addTk(OPEN_PAREN); 
			pch++;
			break;
		case ':':
			addTk(COLON); 
			pch++;
			break;
		case ')':
			addTk(CLOSE_PAREN); 
			pch++;
			break;
		case '<':
			addTk(LESS_THAN);
			pch++;
			break;
		case ';':
			addTk(SEMICOLON);
			pch++;
			break;
		case '.':
			if (isdigit(*(pch--))) {
				char* end;
				double realVal = strtod(pch, &end);
				
				if (end > pch) {
					tk = addTk(REAL_LITERAL);
					tk->r = realVal;
					pch = end;
				}
				else {
					err("invalid real number: %s", pch);
				}
			}
			break;
		case '"':
			pch++; 
			start = pch;
			while (*pch && *pch != '"') {
				pch++;
			}
			if (*pch == '"') {
				tk = addTk(STRING_LITERAL);
				copyn(tk->text, start, pch);
				pch++;
			}
			else {
				err("Unterminated string");
			}
			break;
		default:
			if (isalpha(*pch) || *pch == '_') {
				for (start = pch++; isalnum(*pch) || *pch == '_'; pch++) {}
				char* text = copyn(buf, start, pch);
				if (strcmp(text, "int") == 0)addTk(TYPE_INT);
				if (strcmp(text, "if") == 0)addTk(IF);
				if (strcmp(text, "else") == 0)addTk(ELSE);
				if (strcmp(text, "function") == 0)addTk(FUNCTION);
				else {
					tk = addTk(ID);
					strcpy(tk->text, text);
				}
			}
			if (isdigit(*pch)) {
				char* end;
				int intVal = strtol(pch, &end, 10);
				if (end > pch) {
					if (*(pch + 1) == '.')
					{
						end++;
						double realVal = strtod(pch, &end);
						realVal = (float)intVal;
						tk = addTk(REAL_LITERAL);
						tk->r =(float) intVal;
						int intVal1 = strtol(pch+2, &end, 10);
						int pow = 10;
						while (pow < intVal1) {
							pow = pow * 10;
						}
						realVal += (float) intVal1 / (pow);
						/*printf("\nnr int gasit %d\n", intVal);
						printf("\nnr int1 gasit %d\n", intVal1);
						printf("\nnr real gasit %lf\n", realVal);*/
						tk->r = realVal;
						pch = end;
					}
					else
					{
						tk = addTk(INT_LITERAL);
						tk->i = intVal;
						pch = end;
					}
				}
				else {
					err("invalid number: %s", pch);
				}
			}
			
			if (*pch == '.') {
				pch++;
				break;
			}
			if (*pch == 32) {
				pch++;
				break;
			}
			if (*pch == 40) {
				addTk(OPEN_PAREN);
				pch++;
				break;
			}
			if (*pch == 41) {
				addTk(CLOSE_PAREN);
				pch++;
				break;
			}
			if (*pch == 58) {
				addTk(COLON);
				pch++;
				break;
			}
			if (*pch == 44) {
				addTk(COMMA);
				pch++;
				break;
			}
			if (*pch == 13) {
				pch++;
				break;
			}
			if (*pch == 59) {
				addTk(SEMICOLON);
				pch++;
				break;
			}
			if (*pch == 60) {
				addTk(LESS_THAN);
				pch++;
				break;
			}
			if (*pch == 61) {
				addTk(ASSIGN);
				pch++;
				break;
			}
			if (*pch == '+') {
				addTk(ADD);
				pch++;
				break;
			}
			else err("invalid char: %c (%d)", *pch, *pch);
		}
	}
}

void showTokens() {
	for (int i = 0; i < nTokens; i++) {
		Token* tk = &tokens[i];
		printf("%d ", tk->line);
		switch (tk->code) {
		case ID: printf("ID: %s\n", tk->text); break;
		case TYPE_INT: printf("TYPE_INT\n"); break;
		case COMMA: printf("COMMA\n"); break;
		case COLON: printf("COLON\n"); break;
		case FINISH: printf("FINISH\n"); break;
		case ASSIGN: printf("ASSIGN\n"); break;
		case EQUAL: printf("EQUAL\n"); break;
		case OPEN_PAREN: printf("LPAR\n"); break;
		case CLOSE_PAREN: printf("RPAR\n"); break;
		case LESS_THAN: printf("LESS\n"); break;
		case SEMICOLON: printf("SEMICOLON\n"); break;	
		case INT_LITERAL: printf("INT:%d\n", tk->i); break;
		case REAL_LITERAL: printf("REAL:%g\n", tk->r); break;
		case ADD: printf("ADD\n"); break;
		case IF: printf("IF\n"); break;
		case ELSE: printf("ELSE\n"); break;
		case STRING_LITERAL: printf("STR:%s\n", tk->text); break;
		case FUNCTION: printf("FUNCTION\n"); break;
			
		}
	}
}
