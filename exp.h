/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Exp interface.
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * Created on 17. 11. 2020.
 */

/**
 * Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "token_buffer.h"
#include "symtable_stack.h"
#include "buffer.h"
#include "stack_for_exp.h"

#ifndef IFJ_PROJEKT_EXP_H
#define IFJ_PROJEKT_EXP_H

/**
 * Buf structure
 */
typedef struct buf{
	int size;
	int index;
	char * ptr;
} * Buf;

/**
 * Expression main variables.
 */
extern Buf exp_buf;
extern ID_Type exp_type;
extern sym_SOT_t stack_sot;

/**
 * Expression functions
 */
char input_symbol(int *open_brackets);
int table_index( char symbol);
int is_rule( char *rule, Token t, int *set_type);
int expression();
int match_exp_check_div();
int move_E_to_new();
bool compare_rule(char *rule, char a, char b, char c);
int get_rule(char (*rule)[3]);

/**
 * Buf functions
 */
Buf create_buf(int size); 
int buf_append(Buf b, char c);
char check_symbol(Buf b, int index);
char check_prev(Buf b, int back);
int get_idx(Buf b);
char check_top(Buf b);
char check_last(Buf b);
void pop_last(Buf b);
char get_last(Buf b);
void buf_clean(Buf b);
void delete_buf_and_data(Buf b);

/**
 * Type functions
 */
int check_type( Token t, int *set_type);
ID_Type get_type( Token t);

/**
 * Semantic functions
 */
int sem_operand(Token t);
int sem_ar_op( char type);
int sem_rel_op( char type);

/**
 * Token functions
 */
int match_exp();
int next_token_rel_type();
int next_token_ari_type();
int matched_token_ari_type();

#endif //IFJ_PROJEKT_EXP_H