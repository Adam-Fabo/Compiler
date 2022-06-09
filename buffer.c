/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Buffer
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>




Buffer create_buffer(uint64_t size){

	Buffer b = (Buffer)malloc(sizeof(struct buffer));

	if(b == NULL)
		return NULL;

	b->ptr = (char *)malloc(sizeof(char) * size);

	if(b->ptr == NULL){
		free(b);
		return NULL;
	}

	b->size = size;
	b->index = 0;

	for(unsigned int i = 0; i < size; i++)		//initializing - for valgrind sakes
		b->ptr[i] = 0;

	return b;
}


//functions dont control correctness of pointers
char * get_string(Buffer b){	return b->ptr;}
uint64_t get_size(Buffer b){   	return b->size;}
uint64_t get_index(Buffer b){  	return b->index;}



int buffer_append(Buffer b, char c){

	if(b->index >= b->size-2){

		char * tmp;
		//no more space for char c + '0'
		tmp = (char * )realloc((void *) b->ptr, b->size * 2);

		if(tmp == NULL) {
			free(b->ptr);
			return 0;
		}else{
			b->ptr = tmp;
		}


		for(unsigned int i = b->size; i<b->size*2; i++)	//initializing - for valgrind sakes
			b->ptr[i] = 0;

		b->size *= 2;

	}

	b->ptr[b->index] = c;
	b->ptr[b->index + 1] = 0;
	b->index++;

	return 1;

}



char * delete_buffer(Buffer b){
	char * tmp = b->ptr;
	free(b);
	return tmp;
}



void delete_buffer_and_data(Buffer b){

	free(b->ptr);
	free(b);

}


