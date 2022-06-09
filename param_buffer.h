/**
 * Compiler implementation of imperative language IFJ20.
 *
 * (Header file) Structures used for functions
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 * @author Roman Orzságh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include "lexiA.h"

#ifndef IFJ_PROJEKT_PARAM_BUFFER_H
#define IFJ_PROJEKT_PARAM_BUFFER_H
/*
 * Describes single parameter
 * type   - what type of parameter(string,int,...)
 * buffer - name of parameter (can be NULL = empty)
 */
typedef struct single_param{
    ID_Type type;
    Buffer buffer;
}*Single_Param;

/*
 * Stores parameters (works like a stack)
 * top        - top of stack
 * size       - allocated size
 * parameters - stack with parameters
 */
typedef struct param{
    unsigned top;
    uint64_t size;
    Single_Param* parameters;
}*Param;

/*
 * Struct containing information about function
 * name - name of function
 * par  - parameters of function
 * ret  - return types of function
 */
typedef struct func{
    char* name;
    Param par;
    Param ret;
}*Func;

/*
 * Stores functions
 * top        - top of stack
 * size       - allocated size
 * functions  - stack with functions
 */
typedef struct func_stack {
    unsigned top;
    uint64_t size;
    Func *functions;
}*Func_stack;

/*
 * Creates and initializes stack
 * - returns NULL if MALLOC_ERROR
 * - returns allocated param_buffer otherwise
 */
Param create_param_buff(uint64_t size);

/*
 * Adds new parameter into stack
 * - returns MALLOC_ERROR if malloc failed
 * - returns NO_ERROR otherwise
 */
int append_param(Param param, ID_Type type, Buffer buff);

/*
 * Pops the value of last parameter out
 * (does not deallocate!!!)
 * - returns NULL if stack is empty
 */
Single_Param get_last_param(Param param);

/*
 * Frees desired parameter with all its contents
 */
void delete_one_param(Single_Param p);

/*
 * Frees the whole stack
 */
void delete_param(Param param);

///////////////////////////////////////////////////
///////////////////////////////////////////////////

/*
 * Creates the struct
 */
Func create_func();

/*
 * Deletes the whole struct with all the data save for buffers
 */
void delete_func(Func func);

///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 * Creates new function stack.
 *
 * @param size Size of stack
 *
 * @return NULL if allocation fails, otherwise pointer on Func_stack
 */
Func_stack create_func_stack(uint64_t size);

/**
 * Append function into function stack.
 *
 * @param stack Stack of functions
 * @param func Function to append
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int append_func_into_stack(Func_stack stack, Func func);

/**
 * Pops the function from the top of the stack.
 *
 * @warning Does not deallocate! Use delete_one_func after popping.
 *
 * @param stack Stack of functions
 *
 * @return Popped function or NULL if stack's empty
 */
Func pop_func_from_stack(Func_stack stack);

/**
 * Delete the whole function stack.
 *
 * @param stack Stack to be deleted
 */
void delete_func_stack(Func_stack stack);

/**
 * Utility function for converting Keyword to ID_Type
 *
 * @param keyword Keyword to be converted
 *
 * @return ID_Type equivalent to ID_Type
 */
ID_Type convert_keyword_to_type(Keywords keyword);
int parameters_match(Func func1, Func func2, unsigned index);

#endif //IFJ_PROJEKT_PARAM_BUFFER_H
