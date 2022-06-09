/**
 *  Compiler implementation of imperative language IFJ20.
 *
 * Symtable stack
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>


#include "error_macros.h"
#include "symtable_stack.h"

//vytvori novu struct na zasobnik stromov
sym_SOT_t create_SOT(unsigned size){

	sym_SOT_t s = (sym_SOT_t) malloc( sizeof( struct sym_stack_of_trees_s) );
	if(s == NULL)
		return NULL;

	s->top  = 0;
	s->size = size;

	s->sym_ptr = (Symtable_ptr) malloc( size * sizeof(Symtable)  );
	if(s->sym_ptr == NULL){
		free(s);
		return NULL;
	}
	for(unsigned i = 0; i<size; i++)
		s->sym_ptr[i] = NULL;


	return s;

}


void delete_SOT(sym_SOT_t s){
	if(s != NULL) {
		for (unsigned i = 0; i < s->size; i++) {
			if (s->sym_ptr[i] != NULL) {
				bst_destroy(&(s->sym_ptr[i]));
			}
		}
		free(s->sym_ptr);
		free(s);
	}

}

int push_SOT(sym_SOT_t s, Symtable tree){

	if(s->top == s->size){	//ak to je na konci
		Symtable_ptr tmp = (Symtable_ptr) realloc(s->sym_ptr, s->size * 2 * sizeof(Symtable));
		if(tmp == NULL)
			return MALLOC_ERROR;

		s->sym_ptr = tmp;
		s->size *=2;

		for(int i = s->top; i< s->size; i++)
			s->sym_ptr[i] = NULL;
	}

	s->sym_ptr[s->top] = tree;
	s->top++;
	return NO_ERROR;


}

Symtable * get_last_tree(sym_SOT_t s){

	if(s->top !=0)
		return & ( s->sym_ptr[s->top - 1] );

	return NULL;
}


int add_to_last_tree_id(sym_SOT_t s, char *id_name, ID_Type id_type){

	return bst_add_id(get_last_tree(s) , id_name, id_type);

}

int add_to_last_tree_func(sym_SOT_t s, Func function){

	return bst_add_func(get_last_tree(s) ,function);

}


Node find_in_sot(sym_SOT_t s, char *name ,Type type, int * index){
	Node tmp;

	for(int i = s->top; i>0; i--){
		tmp = bst_find( s->sym_ptr[i-1] , name,type);
		if(tmp != NULL){

			*index = i-1;
			return tmp;
		}
	}

	return NULL;
}



void pop_SOT(sym_SOT_t s){

	if(s->top != 0) {

			bst_destroy(&(s->sym_ptr[s->top - 1]));
			s->sym_ptr[s->top - 1] = NULL;
			s->top--;


	}
}

void print_sot_preorder_id(sym_SOT_t s, char * prefix){

	for(unsigned i = 0; i<s->top;i++){
		print_preorder_id(s->sym_ptr[i], prefix, i);

	}
}



void print_sot_postorder_id(sym_SOT_t s, char * prefix){

	for(int i = s->top-1; i>=0;i--){
		print_postorder_id(s->sym_ptr[i], prefix, i);

	}
}