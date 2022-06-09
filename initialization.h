/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Initialization (Header file)
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include "symtable.h"
#include "param_buffer.h"
#include "exp.h"
#include "symtable_stack.h"
#include "stack_for_exp.h"
#include "stack_of_int.h"
#ifndef IFJ_PROJEKT_INITIALIZATION_H
#define IFJ_PROJEKT_INITIALIZATION_H


Token_arr tok_arr;
Symtable symtable_functions;
Func function;
Buf exp_buf;
Type_stack in_expr_types;
Buff_stack left_to_expr_names;



Int_stack stack_label_if;
Int_stack stack_label_else;
Int_stack stack_label_for;

ID_Type exp_type;

Func_stack func_stack;


int ret_flag;

sym_SOT_t stack_sot;

int init();

/*
 * Deinitializing after program finishes or an error is encountered
 */
void deinit();

#endif //IFJ_PROJEKT_INITIALIZATION_H
