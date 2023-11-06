#pragma once

enum {
	ID
	// keywords
	, TYPE_INT
	// delimiters
	, COMMA, FINISH
	// operators
	, ASSIGN, EQUAL
	// other
	, OPEN_PAREN, CLOSE_PAREN, COLON, LESS_THAN , SEMICOLON, INT_LITERAL, REAL_LITERAL, STRING_LITERAL, ADD,
	IF,ELSE, FUNCTION
};

#define MAX_STR		127

typedef struct {
	int code;		// ID, TYPE_INT, ...
	int line;		// the line from the input file
	union {
		char text[MAX_STR + 1];		// the chars for ID, STR
		int i;		// the value for INT
		double r;		// the value for REAL
	};
}Token;

#define MAX_TOKENS		4096
extern Token tokens[];
extern int nTokens;

void tokenize(const char* pch);
void showTokens();
