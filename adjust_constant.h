/**
 * Compiler implementation of imperative language IFJ20.
 *
 * String adjustment (Header file)
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */


#include "buffer.h"
#include <string.h>
#include "error_macros.h"

#ifndef IFJ_PROJEKT_TODO_H
#define IFJ_PROJEKT_TODO_H



#define H_HASH_SYMBOL "\\035"
#define H_BACKSLASH "\\092"
#define H_NEWLINE "\\010"
#define H_SPACE "\\032"
#define H_TAB "\\009"
#define HASH_SYMBOL '#'
#define BACKSLASH '\\'
#define SPACE ' '

/**
 * Returns adjusted string which can be processed by IFJ20 at string@...
 * Returns NULL if MALLOC_ERROR
 */
Buffer adjust_string(Buffer buff);


#endif //IFJ_PROJEKT_TODO_H
