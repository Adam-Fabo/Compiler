/**
 * Compiler implementation of imperative language IFJ20.
 *
 * (Header file) Token buffer
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexiA.h"

#ifndef IFJ_PROJEKT_TOKEN_BUFFER_H
#define IFJ_PROJEKT_TOKEN_BUFFER_H

typedef struct token_arr{

	unsigned size;
	unsigned num_of_tokens;
	int current;

	Token * array;

} *Token_arr;

/*
 * Creates a Token_arr struct and array for tokens
 * - returns NULL if MALLOC_ERROR
 * - returns Token_arr otherwise
 */
Token_arr create_token_arr(unsigned size);
/*
 * Appends token into array
 */
int token_arr_append(Token_arr tarr,Token t);

int get_last_index(Token_arr t);
Token get_last_matched(Token_arr t);

Token get_token(Token_arr t, unsigned i);
/*
 * Deletes the struct and its contents
 */
void delete_token_arr(Token_arr t);




#endif //IFJ_PROJEKT_TOKEN_BUFFER_H
