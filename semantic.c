/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Semantic implementation.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */

#include "semantic.h"
#include <strings.h>
#include "lexiA.h"
#include "parser.h"
#include "code_generator.h"
#include "adjust_constant.h"
#include "builtin_functions.h"
#include "stack_of_int.h"

extern Int_stack stack_label_if;
extern Int_stack stack_label_else;
extern Int_stack stack_label_for;

int sem_start()
{
	CODE_NL(".IFJcode20");

    CODE("DEFVAR "); CODE_NL("GF@$$forunderscore");
	CODE("DEFVAR "); CODE_NL("GF@$$varstr");
	CODE("DEFVAR "); CODE_NL("GF@$$symb1str");
	CODE("DEFVAR "); CODE_NL("GF@$$symb2str");

    CODE_NL("JUMP $$main");

    CODE_NEWLINE();

	return NO_ERROR;
}

int sem_end()
{
    if (sem_check_main_function() != NO_ERROR) return sem_check_main_function();

    if (sem_check_functions() != NO_ERROR) return sem_check_functions();

	generate_builtin_functions();

    CODE_LABEL("$main");
    CODE_CALL("main");
    CODE_EXIT();

    return NO_ERROR;
}

int sem_prolog()
{
    // Check if main is after package
    if(strcmp(get_string(get_last_matched(tok_arr)->data.b), "main") != 0) return SYNTAX_ERROR;

    return NO_ERROR;
}

int sem_func_head()
{
	char *name = get_string(get_last_matched(tok_arr)->data.b);

	// Check if function already exists in built-in functions
    if (bst_find(builtin_functions, name, TYPE_FUNC) != NULL) return ALREADY_DECLARED;
	// Check if function already exists
	if (bst_find(symtable_functions, name, TYPE_FUNC) != NULL) return ALREADY_DECLARED;

	function = create_func();
	function->name = name;
	current_func_name = name;

	stack_sot = create_SOT(20);		//creates new tree stack
	if(stack_sot == NULL)
		return MALLOC_ERROR;


	Symtable tree = bst_init();			    //creates new tree and adds it to stack
	push_SOT(stack_sot,tree);

	CODE_LABEL(name);
	CODE_NL("CREATEFRAME");
	CODE_NL("PUSHFRAME");

	return NO_ERROR;
}

int sem_func_param()
{
	ID_Type id_type = convert_keyword_to_type(get_token(tok_arr,get_last_index(tok_arr))->data.kw);
	Buffer param = get_token(tok_arr,get_last_index(tok_arr)-1)->data.b;

	// Check if param exists
    for (unsigned i = 0; i < function->par->top; i++) {
        if (strcmp(function->par->parameters[i]->buffer->ptr, param->ptr) == 0) return ALREADY_DECLARED;
    }

	CHECK(append_param(function->par, id_type, param));

	char *var = get_string(param);



	if(id_type == ID_TYPE_STRING)
        return bst_add_id(get_last_tree(stack_sot), var, ID_TYPE_STRING);
	else if(id_type == ID_TYPE_INT)
        return bst_add_id(get_last_tree(stack_sot), var, ID_TYPE_INT);
	else if(id_type == ID_TYPE_FLOAT)
        return bst_add_id(get_last_tree(stack_sot), var, ID_TYPE_FLOAT);
}

int sem_func_param_ret()
{
	ID_Type id_type = convert_keyword_to_type(get_token(tok_arr, get_last_index(tok_arr))->data.kw);

	CHECK(append_param(function->ret, id_type, NULL));

	return NO_ERROR;
}

int sem_func_head_end()
{
    if (strcmp(function->name, "main") == 0) {
        if(function->par->top != 0 || function->ret->top != 0 ) return PARAMS_ERR;
    }

	bst_add_func(&symtable_functions, function);

	for (unsigned i = function->par->top; i > 0; i--) {
	    char *var = get_string(function->par->parameters[i - 1]->buffer);
	    int tmp = 0;
		find_in_sot(stack_sot,var,TYPE_ID, &tmp);
	    CODE("DEFVAR LF@$"); CODE_INT(tmp); CODE_NL(var);
	    CODE("POPS LF@$"); CODE_INT(tmp); CODE_NL(var);
//		printf("DEFVAR LF@$%d%s\n",tmp,var);
//	    printf("POPS LF@$%d%s\n",tmp,var);
	}

	function = NULL;

	return NO_ERROR;
}

int sem_func_end()
{
	CODE_NL("POPFRAME");
	CODE_NL("RETURN");
	CODE_NEWLINE();

	/*Symtable tree = bst_init();

	//bst_add_id(&tree, "aa", 9);
	push_SOT(stack_sot,tree);
	add_to_last_tree_id(stack_sot, "aa",9);

	int hej;
	Node tmp = find_in_sot(stack_sot,"aa", TYPE_ID, &hej);
	if(tmp != NULL){
		printf("nasiel som na %d\n",hej);
	}*/

	delete_SOT(stack_sot);
	stack_sot = NULL;

	return NO_ERROR;
}

int sem_func_id()
{
    func_name = get_string(get_last_matched(tok_arr)->data.b);

    // Clear expression stack
    if (in_expr_types != NULL) delete_type_stack(in_expr_types);

    // Initialize new stack
    if ((in_expr_types = create_type_stack(SOME_NUMBER)) == NULL) return MALLOC_ERROR;

    return NO_ERROR;
}

int sem_func_call()
{
    //Control for overshadowed function
    int dpt = 0;
    Node oversh = find_in_sot(stack_sot, func_name, TYPE_ID, &dpt);
    if(oversh != NULL){
        return UNDEFINED_FUNC;
    }

    if (strcmp(func_name, "print") == 0) {
        int parameters_count = in_expr_types->top;
        CHECK(int_buffer_append(int_buffer, parameters_count));

        // Check if there are no return parameters
        if (left_to_expr_names->top > 0) return PARAMS_ERR;

        CODE("CALL $$print_"); CODE_INT(parameters_count); CODE_NEWLINE();

        return NO_ERROR;
    }

    // Append function into stack to check if declared at the end
    Func tmpFunc = create_func();
    tmpFunc->name = func_name;

    // Append function parameters
    for (int i = 0; i < in_expr_types->top; i++) {
        CHECK(append_param(tmpFunc->par, in_expr_types->types[i], NULL));
    }

    // Append function return parameters
    int depth = 0;
    for (int i = 0; i < left_to_expr_names->top; i++) {
        Node var = find_in_sot(stack_sot, left_to_expr_names->variables[i]->ptr, TYPE_ID, &depth);
        // If var is NULL -> _
        if(var == NULL){
            CHECK(append_param(tmpFunc->ret, ID_TYPE_UNDERSCORE, NULL));
            continue;
        }
        CHECK(append_param(tmpFunc->ret, var->id_type, left_to_expr_names->variables[i]));
    }

    append_func_into_stack(func_stack, tmpFunc);

    if (is_builtin_function(func_name)) {
        CODE("CALL $$"); CODE(func_name); CODE_NEWLINE();
    } else {
        CODE_CALL(func_name);
    }

	return NO_ERROR;
}


int sem_term_start(){
    //reset type stack
    reset_type_stack(in_expr_types);
	return NO_ERROR;
}


int sem_term(){

	Buffer b = NULL;
	int i = 0;
	Node n;

	switch (get_last_matched(tok_arr)->type){
		case INT_LIT:
			printf("PUSHS int@%ld\n",get_last_matched(tok_arr)->data.i);

			push_to_type_stack(in_expr_types,ID_TYPE_INT);
			break;

		case FLOAT_LIT:
			// if float then transform
			printf("PUSHS float@%a\n", get_last_matched(tok_arr)->data.d);
			push_to_type_stack(in_expr_types,ID_TYPE_FLOAT);
			break;

		case STRING_LIT:
			// if string then transform

			b = adjust_string(get_last_matched(tok_arr)->data.b);
			if(b == NULL)
				return MALLOC_ERROR;

			printf("PUSHS string@%s\n",b->ptr);
			delete_buffer_and_data(b);
			push_to_type_stack(in_expr_types,ID_TYPE_STRING);
			break;

		case IDENTIFIER:

			// if id find out if exists and push
			n = find_in_sot(stack_sot, get_last_matched(tok_arr)->data.b->ptr,TYPE_ID,&i);
			if(n == NULL)
				return UNDEFINED_FUNC;

			printf("PUSHS LF@$%d%s\n",i,get_last_matched(tok_arr)->data.b->ptr);
			push_to_type_stack(in_expr_types,n->id_type);

			break;
	}


	return NO_ERROR;
}


int sem_if_init(){

	reset_type_stack(in_expr_types);
	print_sot_preorder_id(stack_sot,"PUSHS LF@");
	return NO_ERROR;

}



int sem_if_jmp(){

	if( pop_from_type_stack(in_expr_types) != ID_TYPE_BOOLEAN)
		return TYPE_COMPATIBILITY;
	//if(pop_from_type_stack(exp_s) != bool)
	//	return error




	Symtable tree = bst_init();
	push_SOT(stack_sot,tree);

	CODE_NL("PUSHS bool@true");
//	CODE("JUMPIFNEQS $"); CODE_INT(stack_label_if->counter); CODE_NL("IF_LABEL");
     printf("JUMPIFNEQS $%dIF_LABEL\n", stack_label_if->counter);

	CODE_NL("CREATEFRAME");
	CODE_NL("PUSHFRAME");

	print_sot_postorder_id(stack_sot,"DEFVAR LF@");
	print_sot_postorder_id(stack_sot,"POPS LF@");



	int_s_push_counter(stack_label_if);


	return NO_ERROR;

}

int sem_if_else(){

	pop_SOT(stack_sot);

	print_sot_preorder_id(stack_sot,"PUSHS LF@");
	CODE_NL("POPFRAME");
//	CODE("JUMP $"); CODE_INT(stack_label_else->counter); CODE_NL("ELSE_LABEL");
	printf("JUMP $%dELSE_LABEL\n", stack_label_else->counter);
	int_s_push_counter(stack_label_else);



	Symtable tree = bst_init();
	push_SOT(stack_sot,tree);
//    CODE("LABEL $"); CODE_INT(int_s_pop(stack_label_if)); CODE_NL("IF_LABEL");
	printf("LABEL $%dIF_LABEL\n" , int_s_top_pop(stack_label_if));
	CODE_NL("CREATEFRAME");
	CODE_NL("PUSHFRAME");

	print_sot_postorder_id(stack_sot,"DEFVAR LF@");
	print_sot_postorder_id(stack_sot,"POPS LF@");


	return NO_ERROR;
}


int sem_if_end(){

	pop_SOT(stack_sot);

	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("POPFRAME");
//    CODE("LABEL $"); CODE_INT(int_s_pop(stack_label_if)); CODE_NL("IF_LABEL");
	printf("LABEL $%dIF_LABEL\n", int_s_top_pop(stack_label_if));

	print_sot_postorder_id(stack_sot,"POPS LF@");

	return NO_ERROR;
}

int sem_else_end(){

	pop_SOT(stack_sot);

	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("POPFRAME");
//    CODE("LABEL $"); CODE_INT(int_s_pop(stack_label_else)); CODE_NL("ELSE_LABEL");
	printf("LABEL $%dELSE_LABEL\n", int_s_top_pop(stack_label_else));

	print_sot_postorder_id(stack_sot,"POPS LF@");


	return NO_ERROR;
}



int sem_id(){
    Token tmp = get_last_matched(tok_arr);
    if((push_to_buff_stack(left_to_expr_names,tmp->data.b)) == MALLOC_ERROR)    return MALLOC_ERROR;
    return NO_ERROR;
}

int sem_id_next(){ //will not be declaration so we can check if variables are declared
    Token tmp = get_last_matched(tok_arr);
    if((sem_check_id()) == 0)   return UNDEFINED_VAR; //variable not declared yet
    /*
    for(int i = 0; i < left_to_expr_names->top;i++){
         //check matching var ()
        if(strcmp(left_to_expr_names->variables[i]->ptr, tmp->data.b->ptr) == 0){
            if(strcmp(tmp->data.b->ptr,"_") != 0){
                return SEMANTIC_ERR;
            }
        }

    }
    */
    if((push_to_buff_stack(left_to_expr_names,tmp->data.b)) == MALLOC_ERROR)    return MALLOC_ERROR;
    if((sem_check_id()) == 0)   return UNDEFINED_VAR; //variable not declared yet
    return NO_ERROR;
}

int sem_check_id(){
    Node tmp;
    int i = 0;
    char* tmpid = left_to_expr_names->variables[left_to_expr_names->top-1]->ptr;
    if(strcmp(tmpid,"_") == 0)  return 1;
    tmp = find_in_sot(stack_sot, tmpid, TYPE_ID, &i);
    if(tmp == NULL) return 0;
    return 1;

    //checks if var on top of stack is already declared
}

int sem_declaration(){
    ID_Type tmpk;
    Node tmp;
    int i = 0;
    if(in_expr_types->top != 0)
    	tmpk = pop_from_type_stack(in_expr_types);

    char* tmpid = pop_from_buff_stack(left_to_expr_names)->ptr;
    if(strcmp(tmpid,"_") == 0){ // _:=anything is invalid
        return SEMANTIC_ERR;
    }

    tmp = find_in_sot(stack_sot, tmpid, TYPE_ID, &i);
    if(tmp == NULL || stack_sot->top-1 != i){ //has not yet been declared in this scope
        if(tmpk == ID_TYPE_BOOLEAN)		return INVALID_DATATYPE;
        if(add_to_last_tree_id(stack_sot, tmpid, tmpk) == MALLOC_ERROR)     return MALLOC_ERROR;

    }else{
        return ALREADY_DECLARED;
    }
    //          DEFVAR                LF@$i(scope)tmpid(name)
    CODE("DEFVAR LF@$");   CODE_INT(stack_sot->top-1);     CODE_NL(tmpid);
    //          POPS                  LF@$i(scope)tmpid(name)
    CODE("POPS LF@$");   CODE_INT(stack_sot->top-1);     CODE_NL(tmpid);
    return NO_ERROR;
}

int sem_begin_equal(){ //makes sure the stack is empty before new assignment
    reset_type_stack(in_expr_types);
    return NO_ERROR;
}

int sem_assignment(){
    if(left_to_expr_names->top != in_expr_types->top){ //not the same number of , on both side of =
        return SEMANTIC_ERR;
    }
    if((sem_check_id()) == 0)   return UNDEFINED_VAR;
    Node tmp;
    Buffer tmpb;
    ID_Type tmpk;
    int i = 0;
    int pocet = left_to_expr_names->top;
    int flg; //if one dont print POPS LF -> used in case more variables of the same name are used (a,a=...)

    while(left_to_expr_names->top != 0){
        flg = 0;
    	tmpb = pop_from_buff_stack(left_to_expr_names); //current var
    	tmpk = pop_from_type_stack(in_expr_types);  //current type

    	if(tmpk == ID_TYPE_BOOLEAN){
    		return INVALID_DATATYPE;
    	}
    	if((strcmp(tmpb->ptr,"_")) == 0){
            CODE("POPS GF@$$forunderscore\n");
    		continue;
    	}
    	tmp = find_in_sot(stack_sot, tmpb->ptr, TYPE_ID, &i);
    	if(tmpk != tmp->id_type)		return  TYPE_COMPATIBILITY;

    	for(unsigned j = left_to_expr_names->top+1; j < pocet; j++) {
            if (strcmp(tmpb->ptr, left_to_expr_names->variables[j]->ptr) == 0) { //was this variable already used?
                CODE("POPS GF@$$forunderscore\n"); //in for eg. a,a = 1,2 -> dont pop 1 into a let it be 2
                flg = 1;
                break; //no need to go further
            }
        }
    	if(flg == 1)    continue;

    	CODE("POPS LF@$");   CODE_INT(i);     CODE_NL(tmpb->ptr);
    }
    return NO_ERROR;
}

int sem_func_assignment(){ //something like a = f()
    // difference between this and sem_assignment is that we do not control number of variables and type match(done
    // after the whole program has been read)
    Buffer tmpb;
    int i = 0;
    int pocet = left_to_expr_names->top;
    int flg = 0; //if one dont print POPS LF

    if((sem_check_id()) == 0)   return UNDEFINED_VAR;
    while(left_to_expr_names->top > 0) {
        tmpb = pop_from_buff_stack(left_to_expr_names);
        if((strcmp(tmpb->ptr,"_")) == 0){
            CODE("POPS GF@$$forunderscore\n");
            continue;
        }
        find_in_sot(stack_sot, tmpb->ptr, TYPE_ID, &i);

        for(unsigned j = left_to_expr_names->top+1; j < pocet; j++) {
            if (strcmp(tmpb->ptr, left_to_expr_names->variables[j]->ptr) == 0) { //was this variable already used?
                CODE("POPS GF@$$forunderscore\n"); //in for eg. a,a = 1,2 -> dont pop 1 into a let it be 2
                flg = 1;
                break; //no need to go further
            }
        }
    	if(flg == 1)    continue;

        CODE("POPS LF@$");   CODE_INT(i);     CODE_NL(tmpb->ptr);
    }
    return NO_ERROR;
};

int sem_ret(){
    ret_flag = 1; //return present
    Node tmp = bst_find(symtable_functions, current_func_name,TYPE_FUNC);
    if( tmp->function->ret->top != in_expr_types->top)    return PARAMS_ERR;
    for(int i = 0; i < tmp->function->ret->top;i++){
        if(tmp->function->ret->parameters[i]->type != in_expr_types->types[i])   return PARAMS_ERR;
    }

    unsigned i = stack_sot->top;
    for(; i > 0; i--){
        CODE_NL("POPFRAME");
        //pop_SOT(stack_sot);
    }
    CODE_NL("RETURN");
    return NO_ERROR;
}

int sem_check_return(){
    Node tmp = bst_find(symtable_functions, current_func_name,TYPE_FUNC);
    if(tmp->function->ret->top != 0){ //function with return types needs to have return
        if(ret_flag == 0){ //no return in function
            return PARAMS_ERR;
        }
    }
    ret_flag = 0;
    return NO_ERROR;
}


int sem_for_init(){

	Symtable tree = bst_init();
	push_SOT(stack_sot,tree);

	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("CREATEFRAME");
	CODE_NL("PUSHFRAME");

	print_sot_postorder_id(stack_sot,"DEFVAR LF@");
	print_sot_postorder_id(stack_sot,"POPS LF@");

	int_s_push_counter(stack_label_for);


	return NO_ERROR;

}


int sem_for_end(){


	pop_SOT(stack_sot);

    //CODE("LABEL $"); CODE_INT(int_s_pop(stack_label_for)); CODE_NL("FOR_END");
	printf("LABEL $%dFOR_END\n",int_s_top_pop(stack_label_for));

	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("POPFRAME");

	print_sot_postorder_id(stack_sot,"POPS LF@");

	return NO_ERROR;

}



int sem_for2_start(){
//    CODE("LABEL $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_IF");
	printf("LABEL $%dFOR_IF\n",stack_label_for->counter-1);


	return NO_ERROR;
}


int sem_for2_end(){



	if( pop_from_type_stack(in_expr_types) != ID_TYPE_BOOLEAN)
		return TYPE_COMPATIBILITY;

	CODE_NL("PUSHS bool@true");
//	CODE("JUMPIFNEQS $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_END");
	printf("JUMPIFNEQS $%dFOR_END\n", stack_label_for->counter-1);

//	CODE("JUMP $"); CODE_INT(stack_label_for->counter-1); CODE("FOR_CODE");
	printf("JUMP $%dFOR_CODE\n", stack_label_for->counter-1);	//skip incrementation

	return NO_ERROR;
}


int sem_for3_start(){
//    CODE("LABEL $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_ASS");
	printf("LABEL $%dFOR_ASS\n", stack_label_for->counter-1);

	return NO_ERROR;
}


int sem_for3_end(){
//    CODE("JUMP $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_IF");
	printf("JUMP $%dFOR_IF\n", stack_label_for->counter-1);

	return NO_ERROR;
}

int sem_for_body_start(){
//    CODE("LABEL $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_CODE");
	printf("LABEL $%dFOR_CODE\n", stack_label_for->counter-1);


	Symtable tree = bst_init();
	push_SOT(stack_sot,tree);

	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("CREATEFRAME");
	CODE_NL("PUSHFRAME");

	print_sot_postorder_id(stack_sot,"DEFVAR LF@");
	print_sot_postorder_id(stack_sot,"POPS LF@");


	return NO_ERROR;
}


int sem_for_body_end(){


	pop_SOT(stack_sot);


	print_sot_preorder_id(stack_sot,"PUSHS LF@");

	CODE_NL("POPFRAME");

	print_sot_postorder_id(stack_sot,"POPS LF@");

//    CODE("JUMP $"); CODE_INT(stack_label_for->counter-1); CODE_NL("FOR_ASS");
	printf("JUMP $%dFOR_ASS\n", stack_label_for->counter-1);

	return NO_ERROR;
}

static int sem_check_main_function()
{
    // Check if main function found
    Node found = bst_find(symtable_functions, "main", TYPE_FUNC);

    if(found == NULL) return UNDEFINED_FUNC;

    // Check if there are no parameters
    if(found->function->par->top != 0 || found->function->ret->top != 0 ) return PARAMS_ERR;

    return NO_ERROR;
}

static int sem_check_functions()
{
    // Check if all called functions are declared
    for (unsigned i = 0; i < func_stack->top; i++) {
        Node found;
        // Find function in all declared functions and built-in functions
        if (is_builtin_function(func_stack->functions[i]->name)) {
            found = bst_find(builtin_functions, func_stack->functions[i]->name, TYPE_FUNC);
        } else {
            found = bst_find(symtable_functions, func_stack->functions[i]->name, TYPE_FUNC);
        }

        // Check if found
        if (found == NULL) {
            // Function not found -> called function is not declared
            return UNDEFINED_FUNC;
        } else {
            // Function found
            Func defined_func = found->function;
            Func called_func = func_stack->functions[i];

            // Check if number of function parameters match
            if (called_func->par->top != defined_func->par->top) return PARAMS_ERR;

            // Check if parameters type match
            for (unsigned j = 0; j < func_stack->functions[i]->par->top; j++) {
                if (called_func->par->parameters[j]->type != defined_func->par->parameters[j]->type) return PARAMS_ERR;
            }

            // Check if all return params returned
            if (called_func->ret->top != defined_func->ret->top) return PARAMS_ERR;

            // Check if return parameters type match
            for (int j = 0; j < called_func->ret->top; j++) {
                if (called_func->ret->parameters[j]->type == ID_TYPE_UNDERSCORE) continue;
                if (defined_func->ret->parameters[j]->type != called_func->ret->parameters[j]->type) {
                    return PARAMS_ERR;
                }
            }
        }
    }

    return NO_ERROR;
}