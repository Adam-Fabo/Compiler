/**
 * (Header file) Compiler implementation of imperative language IFJ20.
 *
 * @brief stack_of_int interface.
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * Created on 25. 11. 2020.
 */

/**
 * Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_macros.h"
#define INT_S_SIZE 16

#ifndef IFJ_PROJEKT_STACK_OF_INT_H
#define IFJ_PROJEKT_STACK_OF_INT_H

/**
 * Structures
 */
typedef struct int_stack{
	
	int *ptr;
	unsigned size;
	int top;

	unsigned int counter;

} * Int_stack;

/**
 * Functions
 */
Int_stack int_s_init(int size);
int int_s_push(Int_stack s, int num);
int int_s_push_counter(Int_stack s);

int int_s_top_pop(Int_stack s);
bool int_s_empty(Int_stack s);
int int_s_top(Int_stack s);
bool int_s_pop(Int_stack s);
void int_s_delete(Int_stack s);

void inc_counter(Int_stack s);
int get_counter(Int_stack s);

#endif //IFJ_PROJEKT_STACK_OF_INT_H