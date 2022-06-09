/**
 * Compiler implementation of imperative language IFJ20.
 *
 *-----------Syntax analysis-------------
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#include "parser.h"
#include "token_buffer.h"
#include "test_lexical.h"
#include "error_macros.h"
#include "semantic.h"
#include "exp.h"


int parser(){

	return program() - 90;
}

int program(){

	CHECK(sem_start());

	CHECK( prolog() );

	CHECK( func_list() );

	CHECK( sem_end() );

	CHECK( match(SCANEOF,EMPTY) );

	return NO_ERROR;
}

int prolog(){  // package main \n

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case KEYWORD:
			CHECK( match(KEYWORD,KEY_PACKAGE) );
			CHECK( match(IDENTIFIER,EMPTY)    );

			CHECK(sem_prolog());

			CHECK( match(NEWLINE,EMPTY)       );

			return NO_ERROR;

		case NEWLINE:
			CHECK( match(NEWLINE,EMPTY) );
			CHECK(prolog());
			return NO_ERROR;

		default:
			return SYNTAX_ERROR;
	}

}

int func_list(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case KEYWORD:
			CHECK( match(KEYWORD,KEY_FUNC) );
			CHECK( func() );
			CHECK( func_list() );
			return NO_ERROR;

		case NEWLINE:
			CHECK( match(NEWLINE,EMPTY) );
			CHECK( func_list() );
			return NO_ERROR;

		case SCANEOF:
			return NO_ERROR;

		default:
			return SYNTAX_ERROR;
	}
}

int func(){

	CHECK( func_head() );

	CHECK ( statement_list() );

	CHECK( match(RCURL,EMPTY) );

    CHECK(sem_check_return() );

    CHECK(sem_func_end());


	return NO_ERROR;

}

int func_head(){

	CHECK( match(IDENTIFIER,EMPTY) );
	CHECK(sem_func_head());


	CHECK( param_list() );
	CHECK( return_types() );

    CHECK(sem_func_head_end());

	CHECK( match(LCURL,EMPTY) );
	CHECK( match(NEWLINE,EMPTY) );


	return NO_ERROR;
}

int param_list(){

	CHECK( match(LPAREN,EMPTY) );
	CHECK( parameters() );
	CHECK( match(RPAREN,EMPTY) );

	return NO_ERROR;
}

int parameters(){


	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case IDENTIFIER:
			CHECK( match(IDENTIFIER,EMPTY) );
			CHECK( my_type() );

			CHECK( sem_func_param() );

			CHECK( param_tail() );

			return NO_ERROR;

		default:
			return NO_ERROR;
	}
}

int param_tail(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case COMMA:
			CHECK( match(COMMA,EMPTY) );
			CHECK( match(IDENTIFIER,EMPTY) );
			CHECK( my_type() );

            CHECK(sem_func_param());

			CHECK( param_tail() );

			return NO_ERROR;

		default:
			return NO_ERROR;
	}
}

int return_types(){


	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case LPAREN:
			CHECK( match(LPAREN,EMPTY) );

			CHECK( return_list() );

			CHECK( match(RPAREN,EMPTY) );

			return NO_ERROR;
		default:
			return NO_ERROR;
	}

}

int return_list(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case KEYWORD:
			
			CHECK( my_type() );

    		CHECK(sem_func_param_ret());

			CHECK( ret_list_tail() );

			break;
	}
	return NO_ERROR;
}

int ret_list_tail(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case COMMA:
			CHECK( match(COMMA,EMPTY) );

			CHECK( my_type() );

            CHECK(sem_func_param_ret());

			CHECK( ret_list_tail() );
			return NO_ERROR;

		default:
			return NO_ERROR;

	}
}

int my_type(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		
		case KEYWORD:
			switch ( next_token_key_type() ){
		
				case KEY_INT:
					CHECK( match(KEYWORD,KEY_INT) );
					return NO_ERROR;
		
				case KEY_FLOAT64:
					CHECK( match(KEYWORD,KEY_FLOAT64) );
					return NO_ERROR;
		
				case KEY_STRING:
					CHECK( match(KEYWORD,KEY_STRING) );
					return NO_ERROR;

				default:	//if not expected keyword
					return SYNTAX_ERROR;
			}

		default:			//if not keyword
			return SYNTAX_ERROR;
	}
}

int statement_list(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case IDENTIFIER:
			CHECK( match(IDENTIFIER,EMPTY) );
			CHECK( id_type() );
			CHECK( match(NEWLINE,EMPTY) );
			break;

		case KEYWORD:
			switch ( next_token_key_type() ){
				case KEY_IF:
					CHECK( match(KEYWORD,KEY_IF) );
					CHECK( if_statement() );
					break;
				case KEY_FOR:
					CHECK( match(KEYWORD,KEY_FOR) );
					CHECK( for_cycle() );
					break;
				case KEY_RETURN:
					CHECK( match(KEYWORD,KEY_RETURN) );
                    CHECK( sem_begin_equal() ); //resets type stack
					CHECK( my_return() );
                    CHECK( sem_ret() );

					break;
				default:
					return SYNTAX_ERROR;
			}
			break;
		case NEWLINE:
			CHECK( match(NEWLINE,EMPTY) );
			break;

		default:
			return NO_ERROR;
	}

	CHECK( statement_list() );
	
	return NO_ERROR;
}

int id_type(){

    CHECK(sem_func_id());

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case LPAREN:
			CHECK( match(LPAREN,EMPTY) );
			CHECK( func_call() );
			CHECK( sem_func_call());
			break;
				
		default:
		    CHECK( sem_id() );
			CHECK( id_tail() );
			CHECK( def() );
			break;
	}
	return NO_ERROR;
}

int id_tail(){

	CHECK_NEXT_TOKEN( next_token() );
	if( var_next_token == COMMA ){

		CHECK( match(COMMA,EMPTY) );
		
		CHECK_NEXT_TOKEN( next_token() );
		switch ( var_next_token  ){
			
			case IDENTIFIER:
				CHECK( match(IDENTIFIER,EMPTY) );
				break;

			default:
				return SYNTAX_ERROR;
		}
        CHECK( sem_id_next() );
        CHECK( id_tail() );
	}
	return NO_ERROR;
}

int def(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		
		case ASSIGNMENT:
			CHECK( match(ASSIGNMENT,EMPTY) );
			CHECK( expression() );
			CHECK( sem_declaration() );
			break;

		case EQUAL:
			CHECK( match(EQUAL,EMPTY) );
            CHECK( sem_begin_equal() );
			CHECK( exp_equal() );
			break;

		default:
			return SYNTAX_ERROR;
	}

	return NO_ERROR;
}

int exp_equal(){
	
	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token ){

		case IDENTIFIER:
			CHECK( match(IDENTIFIER,EMPTY) );
			CHECK( func_or_exp() );
			break;

		default:
			CHECK( expression() );
			CHECK( exp_main_tail() );
            CHECK( sem_assignment() );
			break;
	}

	return NO_ERROR;	
}

int func_or_exp(){

    CHECK(sem_func_id());
	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token ){

		case LPAREN:
			CHECK( match(LPAREN,EMPTY) );
			CHECK( func_call() );
            CHECK( sem_func_call());
            CHECK( sem_func_assignment() );
			break;

		default:
			
			CHECK( expression() );
			CHECK( exp_main_tail() );
            CHECK( sem_assignment() );
			break;
	}

	return NO_ERROR;
}
			
int exp_main_tail(){

	CHECK_NEXT_TOKEN( next_token() );
	if( var_next_token == COMMA ){

		CHECK( match(COMMA,EMPTY) );
		CHECK( expression() );
		CHECK( exp_main_tail() );
	}
	
	return NO_ERROR;
}

int my_return(){

	CHECK_NEXT_TOKEN( next_token() );
	if( var_next_token != NEWLINE ) {

		CHECK(expression());
		CHECK(exp_main_tail());
		return NO_ERROR;

	} else{
		return NO_ERROR;
	}
}

int func_call(){

	CHECK_NEXT_TOKEN( next_token() );
	if ( var_next_token != RPAREN ){
		CHECK( sem_term_start());
		CHECK( term() );
		CHECK( term_tail() );
		//CHECK( expression() );
		//CHECK( exp_main_tail() );
	}
	CHECK( match(RPAREN,EMPTY) );
	return NO_ERROR;	
}

int term(){
	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case INT_LIT:
			CHECK( match(INT_LIT,EMPTY) );
			break;

		case FLOAT_LIT:
			CHECK( match(FLOAT_LIT,EMPTY) );
			break;

		case STRING_LIT:
			CHECK( match(STRING_LIT,EMPTY) );
			break;

		case IDENTIFIER:
			CHECK( match(IDENTIFIER,EMPTY) );
			break;

		default:
			return SYNTAX_ERROR;
	}

	CHECK( sem_term() );

	return NO_ERROR;
}

int term_tail(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case COMMA:
			CHECK( match(COMMA,EMPTY) );

			CHECK( term() );
			CHECK( term_tail());

			return NO_ERROR;

		default:
			return NO_ERROR;
	}
}

int if_statement(){

	CHECK( sem_if_init());

	CHECK( if_head() );
	CHECK( match(LCURL,EMPTY) );
	CHECK( match(NEWLINE,EMPTY) );

	CHECK( statement_list() );

	CHECK( match(RCURL,EMPTY) );
	CHECK( if_else() );

	return NO_ERROR;
}

int if_head(){

	CHECK( expression() );

	CHECK( sem_if_jmp() );

	return NO_ERROR;
}

int if_else(){

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){

		case KEY_ELSE:

			CHECK( sem_if_else());

			CHECK( match(KEYWORD,KEY_ELSE) );
			CHECK( match(LCURL,EMPTY) );
			CHECK( match(NEWLINE,EMPTY) );
			CHECK( statement_list() );
			CHECK( match(RCURL,EMPTY) );

			CHECK( sem_else_end());

			return NO_ERROR;

		default:
			//CHECK( sem_if_end());
			return SYNTAX_ERROR;
	}
}

int for_cycle(){

	CHECK( sem_for_init() );

	CHECK( for_head() );
	CHECK( match(LCURL,EMPTY) );
	CHECK( match(NEWLINE,EMPTY) );

	CHECK( sem_for_body_start() );

	CHECK( statement_list() );
	CHECK( match(RCURL,EMPTY) );

	CHECK( sem_for_body_end());
	CHECK( sem_for_end() );


	return NO_ERROR;
}

int for_head(){
	CHECK( for1() );
	CHECK( match(SEMICOLON,EMPTY) );

	CHECK( sem_for2_start());

	CHECK( for2() );

	CHECK( sem_for2_end());

	CHECK( match(SEMICOLON,EMPTY) );

	CHECK( sem_for3_start());

	CHECK( for3() );

	CHECK( sem_for3_end());

	return NO_ERROR;
}

int for1(){
	CHECK_NEXT_TOKEN( next_token() );
	if ( var_next_token == IDENTIFIER){
		
		CHECK( match(IDENTIFIER,EMPTY) );
        CHECK( sem_id() );
		CHECK( match(ASSIGNMENT,EMPTY) );
		CHECK( expression() );

        CHECK( sem_declaration() );
	}

	return NO_ERROR;
}

int for2(){

	CHECK( expression() );

	return NO_ERROR;
}

int for3(){
	CHECK_NEXT_TOKEN( next_token() );
	if ( var_next_token == IDENTIFIER){
		CHECK( match(IDENTIFIER,EMPTY) );
        CHECK( sem_id() );
		CHECK( id_tail() );
		CHECK( match(EQUAL,EMPTY) );

        CHECK( sem_begin_equal() );
		CHECK( exp_equal() );

	}

	return NO_ERROR;
}
/**
 * Function checks token type, doesnt control boundaries
 */
int next_token_key_type() {

	return tok_arr->array[tok_arr->num_of_tokens-1]->data.kw;
}

/**
 * Function reads new token but does not match it, if is called multiple times in a row,
 * doesnt load multiple tokents (loads just one)
 */
int next_token(){
	if( (unsigned)(tok_arr->current +1) == tok_arr->num_of_tokens) {
		Token t = (Token) malloc(sizeof(struct token));


		if (t == NULL) {
			PRINT_AND_RET(MALLOC_ERROR);
		}
		t->type = 0;
		t->data.i = 0;

		int val = lexical_analysis(t);

		#ifdef MY_DEBUG
			print_token_type(t);
		#endif


		if (val != NO_ERROR) {
			free(t);
			PRINT_AND_RET(val);
		}
		token_arr_append(tok_arr, t);
		return t->type;

	}else{
		return tok_arr->array[tok_arr->num_of_tokens-1]->type;

	}
}

/**
 * Function reads new token if one is not pre loaded (by calling next_token() )
 * and checks if token is given type
 */
int match(int type, int type2){

	if( tok_arr->num_of_tokens > (unsigned)(tok_arr->current +1 )) {
		tok_arr->current++;
	}else {

		int val = next_token();
		if ( val == LEXICAL_ERROR || val == MALLOC_ERROR )
			return val;

		tok_arr->current++;
	}

	#ifdef MY_DEBUG
		//fprintf(stderr,"Match: \t\t");
		print_token_type(tok_arr->array[tok_arr->current]);
	#endif

	if(type == KEYWORD) {
		if (tok_arr->array[tok_arr->current]->type == (unsigned)type && tok_arr->array[tok_arr->current]->data.kw == (unsigned)type2)
			return NO_ERROR;
	} else if(type == ARITHMOP) {
		if (tok_arr->array[tok_arr->current]->type == (unsigned)type && tok_arr->array[tok_arr->current]->data.ao == (unsigned)type2)
			return NO_ERROR;
	} else if(type == RELATIONOP) {
		if (tok_arr->array[tok_arr->current]->type == (unsigned)type && tok_arr->array[tok_arr->current]->data.ro == (unsigned)type2)
			return NO_ERROR;
	} else {
		if (tok_arr->array[tok_arr->current]->type == (unsigned)type)
			return NO_ERROR;
	}

	return SYNTAX_ERROR;
}