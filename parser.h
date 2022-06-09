/**
 * Compiler implementation of imperative language IFJ20.
 *
 *-----------Header file for Syntax analysis-------------
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "lexiA.h"
#include "token_buffer.h"
#include "test_lexical.h"
#include "error_macros.h"
#include "semantic.h"

#ifndef IFJ_PROJEKT_PARSER_H
#define IFJ_PROJEKT_PARSER_H
extern Token_arr tok_arr;

#define SOME_NUMBER 32

int parser();
int program();
int prolog();

int func_list();
int func();
int func_body();
int func_head();

int param_list();
int parameters();
int param_tail();

int return_types();
int return_list();
int ret_list_tail();


int my_type();

int statement_list();
int id_type();
int id_tail();
int def();
int exp_main();
int exp_equal();

int func_or_exp();
int exp_main_tail();
int my_return();
int func_call();
int expression();
int exp_next();
int exp_tail();

int term();
int term_tail();

int aritmetic_op();
int relation_op();

int if_statement();
int if_else();
int if_head();

int for_cycle();
int for_head();
int for1();
int for2();
int for3();

int next_token_key_type();

/**
 * Function reads new token but does not match it, if is called multiple times in a row,
 * doesnt load multiple tokents (loads just one)
 */
int next_token();

/**
 * Function reads new token if one is not pre loaded (by calling next_token() )
 * and checks if token is given type
 */
int match(int type,int type2);

#endif //IFJ_PROJEKT_PARSER_H
