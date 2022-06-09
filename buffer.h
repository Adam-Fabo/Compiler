/**
 * Compiler implementation of imperative language IFJ20.
 *
 * (Header file) Buffer
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */
#include <stdint.h>

#ifndef IFJ_PROJEKT_BUFFER_H
#define IFJ_PROJEKT_BUFFER_H

typedef struct buffer{
	uint64_t size;
	uint64_t index;
	char * ptr;
} * Buffer;



/*
 * Creates the buffer of size=size
 * - returns NULL if MALLOC_ERROR
 * - returns pointer to it otherwise
 */
Buffer create_buffer(uint64_t size); //
char * get_string(Buffer b);
/*
 * Appends char into Buffer
 * - returns 0 if something went wrong
 * - returns 1 otherwise
 */
int buffer_append(Buffer b, char c);
/*
 * Frees only the Buffer and returns char * ptr
 */
char * delete_buffer(Buffer b);
/*
 * Frees all the data
 */
void delete_buffer_and_data(Buffer);

char * get_string(Buffer b);
uint64_t get_size(Buffer b);
uint64_t get_index(Buffer b);




#endif //IFJ_PROJEKT_BUFFER_H
