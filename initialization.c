/**
 * Compiler implementation of imperative language IFJ20.
 *
 * Initialization
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "token_buffer.h"
#include "error_macros.h"
#include "initialization.h"
#include "lexiA.h"
#include "parser.h"
#include "symtable.h"
#include "semantic.h"
#include "exp.h"

#include "symtable_stack.h"


int init(){
	int tmp;

	tok_arr = create_token_arr(SOME_NUMBER);
	if(tok_arr == NULL) 				goto END0;

	symtable_functions = bst_init();		//No allocation happening
	function = NULL;

	exp_buf = create_buf(SOME_NUMBER);
	if(exp_buf == NULL) 				goto END1;

	in_expr_types = create_type_stack(SOME_NUMBER);
	if(in_expr_types == NULL)			goto END2;

	left_to_expr_names = create_buff_stack(SOME_NUMBER);
	if(left_to_expr_names == NULL)  	goto END3;

	tmp = builtin_init();
	if(tmp == MALLOC_ERROR)				goto END4;

	stack_label_if = int_s_init(20);
	if(stack_label_if == NULL)			goto END5;

	stack_label_else = int_s_init(20);
	if(stack_label_else == NULL)		goto END6;

	stack_label_for = int_s_init(20);
	if(stack_label_for == NULL)			goto END7;

	func_stack = create_func_stack(20);
	if (func_stack == NULL)             goto END8;




	stack_sot = NULL;




	exp_type = ID_TYPE_BOOLEAN;

	ret_flag = 0;



	return NO_ERROR;

    END8:
        int_s_delete(stack_label_for);
	END7:
		int_s_delete(stack_label_else);
	END6:
		int_s_delete(stack_label_if);
	END5:
		builtin_destroy();
	END4:
		delete_buff_stack(left_to_expr_names);
	END3:
		delete_type_stack(in_expr_types);
	END2:
		delete_buf_and_data(exp_buf);
	END1:
		delete_token_arr(tok_arr);
	END0:
		return MALLOC_ERROR;
}




void deinit(){


	delete_SOT(stack_sot);

	delete_func_stack(func_stack);

	int_s_delete(stack_label_for);
	int_s_delete(stack_label_else);
	int_s_delete(stack_label_if);

	builtin_destroy();


	delete_buff_stack(left_to_expr_names);

	delete_type_stack(in_expr_types);


	delete_buf_and_data(exp_buf);

	if(function != NULL)
		delete_func(function);

	bst_destroy(&symtable_functions);
	delete_token_arr(tok_arr);

}



