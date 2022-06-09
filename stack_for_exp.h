/**
 * Compiler implementation of imperative language IFJ20.
 *
 * (Header file) Stacks used in expressions, declarations, assignment, ...
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */
#include "lexiA.h"
#include <stdbool.h>
#include <stdlib.h>
#include "symtable.h"
#ifndef IFJ_PROJEKT_STACK_FOR_EXP_H
#define IFJ_PROJEKT_STACK_FOR_EXP_H

/*
 * Stack for variables
 * top       - stack top
 * size      - size of buffer
 * variables - individual variables
 */
typedef struct buff_stack{
    uint64_t top;
    uint64_t size;
    Buffer* variables;
}*Buff_stack;

/*
 * Stack for types
 * top       - stack top
 * size      - size of buffer
 * variables - individual types
 */
typedef struct type_stack{
    uint64_t top;
    uint64_t size;
    ID_Type* types;
}*Type_stack;

//  Following are the functions for manipulating the stacks

Buff_stack create_buff_stack(uint64_t size);
int push_to_buff_stack(Buff_stack bstack, Buffer variable);
Buffer pop_from_buff_stack(Buff_stack bstack);
void delete_buff_stack(Buff_stack bstack);

//////////////////////////////////////////////////////////

Type_stack create_type_stack(uint64_t size);
int push_to_type_stack(Type_stack tstack, ID_Type type);
ID_Type pop_from_type_stack(Type_stack tstack);
void reset_type_stack(Type_stack tstack);
void delete_type_stack(Type_stack tstack);


#endif //IFJ_PROJEKT_STACK_FOR_EXP_H
