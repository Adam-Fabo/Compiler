/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief stack_of_int interface.
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * Created on 17. 11. 2020.
 */

#include "stack_of_int.h"

Int_stack int_s_init(int size){

	Int_stack s = (Int_stack) malloc(sizeof(struct int_stack));

	if(s == NULL)
		return NULL;

	int *ptr = (int *) calloc( size, sizeof(int));

	if(ptr == NULL){
		free(s);
		return NULL;
	}

	s->ptr = ptr;
	s->size = INT_S_SIZE;
	s->top = -1;
	s->counter = 0;

	return s;
}

int int_s_push(Int_stack s, int num){
	
	if (s->top + 1 == s->size){
		
		s->ptr = (int *) realloc( (void *) s->ptr, sizeof(int) * s->size * 2);
		if(s->ptr == NULL)
			return MALLOC_ERROR;

		for(int i = s->size; i < s->size * 2; i++)
			s->ptr[i] = 0;

		s->size *= 2;
	}
	s->top++;
	s->ptr[s->top] = num;

	return MALLOC_ERROR;
}


int int_s_push_counter(Int_stack s){

	return int_s_push(s, s->counter++ );


}

int int_s_top_pop(Int_stack s){

	if (s->top == -1)
		return -1;

	return (s->ptr[s->top--]);
}

bool int_s_empty(Int_stack s){
	return(s->top == -1);
}

int int_s_top(Int_stack s){
	return (s->ptr[s->top]);
}

bool int_s_pop(Int_stack s){
	if (s->top == -1)
		return false;

	s->top--;
	return true;
}
void int_s_delete(Int_stack s){

	free(s->ptr);
	free(s);
}

void inc_counter(Int_stack s){

	s->counter++;
	return;
}
int get_counter(Int_stack s){
	
	return s->counter;
}



