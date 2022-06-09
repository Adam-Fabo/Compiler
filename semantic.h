/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Semantic interface.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include "symtable.h"
#include "token_buffer.h"
#include "param_buffer.h"
#include "stack_for_exp.h"
#include "builtin_functions.h"
#include "symtable_stack.h"

#ifndef SEMANTIC_H
#define SEMANTIC_H

/**
 * Token Array.
 */
extern Token_arr tok_arr;

/**
 * Function variables.
 */
extern Func function;
char *func_name; //holds the function name (current function in code) in function head
char *current_func_name; //same as func_name except that it holds the name until new function
extern Func_stack func_stack;
extern int ret_flag;
/**
 * Binary tree for functions.
 */
extern Symtable symtable_functions;

/**
 * Binary tree for built-in functions.
 */
extern Symtable builtin_functions;

/**
 * Int Buffer for special prints.
 */
extern Int_Buffer int_buffer;

extern sym_SOT_t stack_sot;

/**
 * Expression main variables.
 */
extern Type_stack in_expr_types;
extern Buff_stack left_to_expr_names;

/**
 * Semantic main functions.
 */
int sem_start();
int sem_end();
int sem_prolog();

/**
 * Functions
 */
int sem_func_id();
int sem_func_call();
int sem_func_head();
int sem_func_param();
int sem_func_param_ret();
int sem_func_head_end();
int sem_func_end();

int sem_ret(); //processes return
int sem_check_return(); //checks if the function had a return

/**
 * If
 */
int sem_if_init();
int sem_if_jmp();
int sem_if_else();
int sem_if_end();
int sem_else_end();

/**
 * Term
 */

int sem_term_start();
int sem_term();

/**
 * Declaration and assignment
 */
int sem_id(); //first id
int sem_id_next(); //id after ,
int sem_check_id(); //can the id be declared?
int sem_declaration();
int sem_begin_equal(); //preparation for processing assignment
int sem_assignment();
int sem_func_assignment(); //assignment calling function

/**
* for
*/

int sem_for_init();
int sem_for_end();
int sem_for_body_start();
int sem_for_body_end();
int sem_for2_start();
int sem_for2_end();
int sem_for3_start();
int sem_for3_end();

/**
 * Utility functions
 */
static int sem_check_main_function();
static int sem_check_functions();

#endif
