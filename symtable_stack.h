/**
 * Compiler implementation of imperative language IFJ20.
 *
 * (Header file) Symtable stack
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include "symtable.h"

#ifndef IFJ_PROJEKT_SYMTABLE_STACK_H
#define IFJ_PROJEKT_SYMTABLE_STACK_H



typedef  Symtable *  Symtable_ptr;

typedef struct sym_stack_of_trees_s{
	unsigned top;
	unsigned size;

	Symtable_ptr sym_ptr;

} * sym_SOT_t;			//stack of trees


sym_SOT_t create_SOT(unsigned size);

void delete_SOT(sym_SOT_t s);

int push_SOT(sym_SOT_t s, Symtable tree);

void pop_SOT(sym_SOT_t s);

Symtable * get_last_tree(sym_SOT_t s);

Node find_in_sot(sym_SOT_t s, char *name ,Type type, int * index);

int add_to_last_tree_id(sym_SOT_t s, char *id_name, ID_Type id_type);
int add_to_last_tree_func(sym_SOT_t s, Func function);

void print_sot_preorder_id(sym_SOT_t s, char * prefix);
void print_sot_postorder_id(sym_SOT_t s, char * prefix);






#endif //IFJ_PROJEKT_SYMTABLE_STACK_H
