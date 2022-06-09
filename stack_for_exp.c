/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Stacks used in expressions, declarations, assignment, ...
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */

#include "stack_for_exp.h"

Buff_stack create_buff_stack(uint64_t size){
    Buff_stack st;
    if((st = (Buff_stack) malloc(sizeof(*st))) == NULL){
        return NULL;
    }

    if((st->variables = (Buffer*) malloc(size * sizeof(Buffer))) == NULL){
        return NULL;
    }

    st->size = size;
    st->top = 0;
    return st;
}


int push_to_buff_stack(Buff_stack bstack, Buffer variable){
    Buffer* tmp;
    if(bstack->top == bstack->size){ //get more space for more variables
        bstack->size = bstack->size * 2;
        tmp = (Buffer*) realloc(bstack->variables,bstack->size * sizeof(Buffer));
        if(tmp == NULL){
            return MALLOC_ERROR;
        }
        bstack->variables = tmp;
    }

    bstack->variables[bstack->top] = variable;
    bstack->top++;
    return NO_ERROR;
}

Buffer pop_from_buff_stack(Buff_stack bstack){
    if(bstack->top == 0){
        return NULL;
    }

    bstack->top--;
    return bstack->variables[bstack->top];
}

void delete_buff_stack(Buff_stack bstack){
    free(bstack->variables);
    free(bstack);
}

/////////////////////////////////////////////////////////////

Type_stack create_type_stack(uint64_t size){
    Type_stack st;
    if((st = (Type_stack) malloc(sizeof(*st))) == NULL){
        return NULL;
    }

    if((st->types = (ID_Type*) malloc(size * sizeof(ID_Type))) == NULL){
        return NULL;
    }

    st->size = size;
    st->top = 0;
    return st;
}

int push_to_type_stack(Type_stack tstack, ID_Type type){
    ID_Type * tmp;
    if(tstack->top == tstack->size){ //get more space for more variables
        tstack->size = tstack->size * 2;
        tmp = (ID_Type*) realloc(tstack->types,tstack->size * sizeof(ID_Type));
        if(tmp == NULL){
            return MALLOC_ERROR;
        }
        tstack->types = tmp;
    }

    tstack->types[tstack->top] = type;
    tstack->top++;
    return NO_ERROR;
}

ID_Type pop_from_type_stack(Type_stack tstack){

    tstack->top--;
    return tstack->types[tstack->top];
}

void reset_type_stack(Type_stack tstack){

	tstack->top = 0;
}

void delete_type_stack(Type_stack tstack){
    free(tstack->types);
    free(tstack);
}

