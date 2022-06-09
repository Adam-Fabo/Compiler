/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Token buffer
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include "token_buffer.h"






Token_arr create_token_arr(unsigned size){

	Token_arr tmp_arr = (Token_arr) malloc(sizeof(struct token_arr));

	if(tmp_arr == NULL){
		return NULL;
	}


	Token * tmp = (Token * ) malloc(sizeof(Token) * size);

	if(tmp == NULL){
		free(tmp_arr);
		return NULL;
	}

	for(unsigned int i = 0; i< size; i++)
		tmp[i] = NULL;


	tmp_arr->size = size;
	tmp_arr->current = -1;
	tmp_arr->num_of_tokens = 0;
	tmp_arr->array = tmp;

	return tmp_arr;

}


int token_arr_append(Token_arr tarr, Token t){

	if(tarr->num_of_tokens == tarr->size){
		tarr->array = (Token *)realloc( (void *) tarr->array, sizeof(Token) * tarr->size * 2);

		if(tarr->array == NULL) {
			return 0;
		}

		for(unsigned int i = tarr->size; i < tarr->size*2; i++) {
			tarr->array[i] = NULL;
		}

		tarr->size *=2;
	}


	tarr->num_of_tokens++;
	tarr->array[tarr->num_of_tokens-1] = t;
	return 1;
}

int get_last_index(Token_arr t){
	return t->current;
}

Token get_last_matched(Token_arr t){
	if(t->current < 0)
		return NULL;
	else
		return t->array[t->current];
}

Token get_token(Token_arr t, unsigned i){

	return t->array[i];
}

void delete_token_arr(Token_arr tarr){


	for(unsigned int i = 0; i< tarr->num_of_tokens; i++){
		if(tarr->array[i]->type == IDENTIFIER || tarr->array[i]->type == STRING_LIT )
			delete_buffer_and_data(tarr->array[i]->data.b);
		free(tarr->array[i]);
	}
	free(tarr->array);
	free(tarr);
}
