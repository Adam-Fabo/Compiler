/**
 * Compiler implementation of imperative language IFJ20.
 *
 * ------------------Header file for Scanner---------------------
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdint.h>
#include "buffer.h"
#include "error_macros.h"

#ifndef LEXIA_H
#define LEXIA_H


#define STR_IF 			"if"
#define STR_ELSE 		"else"
#define STR_FLOAT64 	"float64"
#define STR_FOR 		"for"
#define STR_FUNC 		"func"
#define STR_INT 		"int"
#define STR_PACKAGE 	"package"
#define STR_RETURN 		"return"
#define STR_STRING 		"string"

#define DECADIC 10

/*IDENTIFIER 	- e.g.  my_int
 * KEYWORD		- else, float64, for, func, if, int, package, return, string
 * INT_LIT		- 1324
 * FLOAT_LIT	- 1.25   1e6  1.25e-5
 * STRING_LIT   - "abc"
 * LBRACKET     - [
 * RBRACKET     - ]
 * LPAREN		- (
 * RPAREN		- )
 * LCURL		- {
 * RCURL		- }
 * COMMA		- ,
 * SEMICOLON	- ;
 * ASSIGNMENT	- :=
 * EQUAL		- =
 * ARITHMOP		- + - * /
 * RELATIONOP 	- < <= > >= == !=
 * NEWLINE		- '\n'
 * SCANEOF		- says that scanning hit EOF
 * */
typedef enum token_types{
    IDENTIFIER, KEYWORD, INT_LIT, FLOAT_LIT, STRING_LIT,LBRACKET, RBRACKET,LPAREN, RPAREN, LCURL, RCURL,
    ASSIGNMENT, EQUAL, COMMA, SEMICOLON, ARITHMOP, RELATIONOP, NEWLINE, SCANEOF
}Token_type;


/* ADD	-  +
 * SUB 	-  -
 * MUL  -  *
 * DIV  -  /
 * */
typedef enum arithmetic_operators{
	ADD, SUB, MUL, DIV
}Arithmetic_operators;


/* GR	-  >
 * GRE 	-  >=
 * LS   -  <
 * LSE  - <=
 * EE   - ==
 * NE	- !=
 * */
typedef enum relation_operators{
	GR, GRE , LS, LSE, EE, NE
}Relation_operators;


typedef enum keywords{
	NOT_VALID,KEY_ELSE, KEY_FLOAT64, KEY_FOR, KEY_FUNC, KEY_IF, KEY_INT, KEY_PACKAGE, KEY_RETURN, KEY_STRING
}Keywords;

/**
 * @enum Symbol type
 */
typedef enum type {
	TYPE_ID, TYPE_FUNC
} Type;

/**
 * @enum Identifier type
 */
typedef enum id_type {
	ID_TYPE_STRING, ID_TYPE_INT, ID_TYPE_FLOAT, ID_TYPE_BOOLEAN, ID_NOT_VALID, ID_TYPE_UNDERSCORE
} ID_Type;

/*
 * Represents the data read by Scanner
 */
typedef union data{

    int64_t i;
    double  d;
    Buffer b;     //string pointer (contains string_lit or name of identifier)


	Keywords kw;
	Relation_operators ro;
	Arithmetic_operators ao;


}Data;


typedef struct token{

    Token_type type;
    Data data;

} *Token;

/*
 * Scanner
 * - returns LEXICAL_ERROR if the scanning action couldn't be finished due to the FSM
 * - returns MALLOC_ERROR if malloc failed
 * - returns NO_ERROR otherwise
 */
int lexical_analysis(Token token);


/*
 * Matches Keywords
 * - returns enum of found keyword, if not found returns NOT_VALID
 */
int detect_keyword(Buffer b);

/*
 * Part of finite state machine for float
 * - returns LEXICAL_ERROR if there is a lexical error
 * - returns NO_ERROR otherwise
 */
int FST_float(Token token,int ch);

/*
 * Part of finite state machine for comment
 * - *newline carries information about presence of \n in comment
 * - returns LEXICAL_ERROR if there is a lexical error
 * - returns NO_ERROR otherwise
 */
int FST_comment(Token token, int ch, int* newline);

/*
 * Part of finite state machine for string literal
 * - returns LEXICAL_ERROR if there is a lexical error
 * - returns MALLOC_ERROR if malloc/realloc failed
 * - returns NO_ERROR otherwise
 */
int FST_string_lit(Token token,int ch);
#endif


