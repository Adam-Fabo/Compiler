/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Structures used for functions
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 * @author Roman Orzságh <xorsza01(at)stud.fit.vutbr.cz>
 */
#include "param_buffer.h"
#include <stdio.h>


Param create_param_buff(uint64_t size){
    Param p;
    if((p = (Param) malloc(sizeof(*p))) == NULL){
        return NULL;
    }

    if((p->parameters = (Single_Param*) malloc(size * sizeof(Single_Param))) == NULL){
        return NULL;
    }

    p->size = size;
    p->top = 0;
    return p;
}


int append_param(Param param, ID_Type type, Buffer buff){
    Single_Param* tmp;
    if(param->top == param->size){ //get more space for more parameters
        param->size = param->size * 2;
        tmp = (Single_Param*) realloc(param->parameters,param->size * sizeof(Single_Param));
        if(tmp == NULL){
            return MALLOC_ERROR;
        }
        param->parameters = tmp;
    }

    Single_Param tmp2;
    if((tmp2 = (Single_Param) malloc(sizeof(*tmp2))) == NULL){ //new parameter
        return MALLOC_ERROR;
    }

    tmp2->type = type;
    tmp2->buffer = buff;

    param->parameters[param->top] = tmp2;
    param->top++;
    return NO_ERROR;
}

Single_Param get_last_param(Param param){
    if(param->top == 0){
        return NULL;
    }
    param->top--;
    return (param->parameters[param->top]);
}


void delete_one_param(Single_Param p){
    /*
    if(p->buffer != NULL) {
       ;// free(p->buffer);
    }
     */
    free(p);
}


void delete_param(Param param){
    while(param->top != 0){
        param->top--;
        delete_one_param(param->parameters[param->top]);
    }
    free(param->parameters);
    free(param);
}




///////////////////////////////////////////////////
///////////////////////////////////////////////////


Func create_func(){
    Func f;
    if((f = (Func) malloc(sizeof(*f))) == NULL){
        return NULL;
    }

    f->par = create_param_buff(20);
    f->ret = create_param_buff(20);

    if(f->par == NULL || f->ret == NULL ) {
        return NULL;
    }

    return f;
}

void delete_func(Func func){
    delete_param(func->par);
    delete_param(func->ret);
    free(func);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 * Creates new function stack.
 *
 * @param size Size of stack
 *
 * @return NULL if allocation fails, otherwise pointer on Func_stack
 */
Func_stack create_func_stack(uint64_t size)
{
    Func_stack stack;
    if ((stack = (Func_stack) malloc(sizeof(*stack))) == NULL) return NULL;

    if ((stack->functions = (Func*) malloc(size * sizeof(Func))) == NULL) return NULL;

    stack->top = 0;
    stack->size = size;

    return stack;
}

/**
 * Append function into function stack.
 *
 * @param stack Stack of functions
 * @param func Function to append
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int append_func_into_stack(Func_stack stack, Func func)
{
    // Check if stack is big enough
    if(stack->top == stack->size) {
        Func* tmp;

        stack->size *= 2;

        tmp = (Func*) realloc(stack->functions, stack->size * sizeof(Func));

        if(tmp == NULL) return MALLOC_ERROR;

        stack->functions = tmp;
    }

    stack->functions[stack->top] = func;
    stack->top++;

    return NO_ERROR;
}

/**
 * Delete the whole function stack.
 *
 * @param stack Stack to be deleted
 */
void delete_func_stack(Func_stack stack)
{
    while(stack->top != 0) {
        stack->top--;
        delete_func(stack->functions[stack->top]);
    }

    free(stack->functions);
    free(stack);
}

/**
 * Utility function for converting Keyword to ID_Type
 *
 * @param keyword Keyword to be converted
 *
 * @return ID_Type equivalent to ID_Type
 */
ID_Type convert_keyword_to_type(Keywords keyword)
{
    if (keyword == KEY_INT)          return ID_TYPE_INT;
    else if (keyword == KEY_STRING)  return ID_TYPE_STRING;
    else if (keyword == KEY_FLOAT64) return ID_TYPE_FLOAT;

    return ID_NOT_VALID;
}

int parameters_match(Func func1, Func func2, unsigned index)
{
    if (func1 == NULL || func2 == NULL) return 0;

    // Check if parameter exists at index
    if (func1->par->parameters[index] == NULL) return 0;
    if (func2->par->parameters[index] == NULL) return 0;

    if (func1->par->parameters[index]->type == func2->par->parameters[index]->type)
        return 1;

    return 0;
}