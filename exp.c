/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Exp interface.
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * Created on 17. 11. 2020.
 */

#include "exp.h"
#include "lexiA.h"
#include "token_buffer.h"
#include "code_generator.h"
#include "adjust_constant.h"
#include "parser.h"

extern Type_stack in_expr_types;
extern Token_arr tok_arr;

// PRECEDENCE TABLE
char precedence_table[9][9] = {
// i = operand ( INT_LIT, FLOAT_LIT, STRING_LIT, IDENTIFIER)
// r = relation operators ( >, >=, <, <=, ==, !=)
//  TOP OF                   			NEW SYMBOL
// PUSHDOWN			 +	 -	 *	 /	 (	 )	 i	 r	 $
/*     +  	*/	{	'>','>','<','<','<','>','<','>','>'},
/*     -  	*/	{	'>','>','<','<','<','>','<','>','>'},
/*     *  	*/	{	'>','>','>','>','<','>','<','>','>'},
/*     /  	*/	{	'>','>','>','>','<','>','<','>','>'},
/*     (  	*/	{	'<','<','<','<','<','=','<','<',' '},
/*     )  	*/	{	'>','>','>','>',' ','>',' ','>','>'},
/*     i  	*/	{	'>','>','>','>',' ','>',' ','>','>'},
/*     r  	*/	{	'<','<','<','<','<','>','<','>','>'},
/*     $  	*/	{	'<','<','<','<','<',' ','<','<',' '}
};

char input_symbol(int *open_brackets){

    char locSymbol;

	CHECK_NEXT_TOKEN( next_token() );
	switch ( var_next_token  ){
		case ARITHMOP:
			switch ( next_token_ari_type() ){
				case ADD:
					locSymbol = '+';
					break;
				case SUB:
					locSymbol = '-';
					break;
				case MUL:
					locSymbol = '*';
					break;
				case DIV:
					locSymbol = '/';
					break;
			}
			break;
		case LPAREN:
			(*open_brackets)++;
			locSymbol = '(';
			break;
		case RPAREN:
			(*open_brackets)--;
			if((*open_brackets) < 0){
				locSymbol = '$';
			} else {
				locSymbol = ')';
			}
			break;
		case RELATIONOP:
			switch ( next_token_rel_type() ){
				case GR:
					locSymbol = 'a';
					break;
				case GRE:
					locSymbol = 'b';
					break;
				case LS:
					locSymbol = 'c';
					break;
				case LSE:
					locSymbol = 'd';
					break;
				case EE:
					locSymbol = 'e';
					break;
				case NE:
					locSymbol = 'f';
					break;
			}
			break;
		case INT_LIT:
		case FLOAT_LIT:
		case STRING_LIT:
		case IDENTIFIER:
			locSymbol = 'i';
			break;
		default:
			locSymbol = '$';
			break;
	}

    return locSymbol;
}

int table_index( char symbol){

	switch ( symbol ){
		
		case '+':
			return 0;
		case '-':
			return 1;
		case '*':
			return 2;
		case '/':
			return 3;
		case '(':
			return 4;
		case ')':
			return 5;
		case 'i':
			return 6;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return 7;
		case '$':
			return 8;
		default:
			return 8;
	}
}	

int is_rule( char *rule, Token t, int *set_type){
	if(strcmp(rule, "i")==0){
		CHECK( check_type(t, set_type) );
		CHECK( sem_operand(t) );
	} else if(strcmp(rule, "E+E")==0){
		CHECK( sem_ar_op('+') );
	} else if(strcmp(rule, "E-E")==0){
		CHECK( sem_ar_op('-') );
	} else if(strcmp(rule, "E*E")==0){
		CHECK( sem_ar_op('*') );
	} else if(strcmp(rule, "E/E")==0){
		CHECK( sem_ar_op('/') );
	} else if(strcmp(rule, "EaE")==0){
		CHECK( sem_rel_op('a') );
	} else if(strcmp(rule, "EbE")==0){
		CHECK( sem_rel_op('b') );
	} else if(strcmp(rule, "EcE")==0){
		CHECK( sem_rel_op('c') );
	} else if(strcmp(rule, "EdE")==0){
		CHECK( sem_rel_op('d') );
	} else if(strcmp(rule, "EeE")==0){
		CHECK( sem_rel_op('e') );
	} else if(strcmp(rule, "EfE")==0){
		CHECK( sem_rel_op('f') );
	} else if(strcmp(rule, "(E)")==0){
		return NO_ERROR;
	} else {
		return SYNTAX_ERROR;
	}
	return NO_ERROR;
}

int expression(){

	char buf_sym, input_sym, table_option;
	int idx, input_idx, pom;
	char rule[3];
		// variables to check errors
	int set_type = 1;
	int open_brackets = 0;
		// set first pushdown symbol
	buf_clean(exp_buf);
	CHECK( buf_append(exp_buf, '$') );
		// get first input symbol	
	Token t = get_last_matched(tok_arr);
	if(t->type == IDENTIFIER){
		CHECK( buf_append(exp_buf, 'E') );
		CHECK( is_rule( "i", t, &set_type) );
	}
		// load new input, input_idx = index of input symbol
	input_sym = input_symbol(&open_brackets);
	input_idx = table_index(input_sym);
		//repeat to solve expression
	do {
			// get index of symbol on top of pushdown (exp_buf)
		buf_sym = check_top(exp_buf);
		idx = table_index(buf_sym);
			// check end of expression
		if(get_idx(exp_buf) == 2)
			if( (compare_rule("$E",check_prev(exp_buf,1),check_last(exp_buf),'\0')) && (input_sym == '$') )
				break;
			// check option in precedence_table
		table_option = precedence_table[idx][input_idx];
			// option >
		if(table_option == '>'){
				// get rule from pushdown
			CHECK( get_rule(&rule) );
				// check if rule is valid
			CHECK( is_rule( rule, t,  &set_type) );
			CHECK( buf_append(exp_buf, 'E') );

			// option <
		} else if(table_option == '<'){
				
			CHECK( move_E_to_new() );
				// add to pushdown input_sym and check division
			CHECK( buf_append(exp_buf, input_sym) );
			CHECK( match_exp_check_div() );
				// load new input
			input_sym = input_symbol(&open_brackets);
			input_idx = table_index(input_sym);
				// put to t new matched token
			t = get_last_matched(tok_arr);

			// option =
		} else if(table_option == '='){
				// add to pushdown ')'
			CHECK( buf_append(exp_buf, input_sym) );
			CHECK( match_exp() );
				// load new input
			input_sym = input_symbol(&open_brackets);
			input_idx = table_index(input_sym);
		} else {
			return SYNTAX_ERROR;
		}

	} while (1);
		// give exp_type to definition & assignment
	CHECK( push_to_type_stack(in_expr_types, exp_type) );

	return NO_ERROR;
}

int match_exp_check_div(){
	Token t = get_last_matched(tok_arr);
	if( (t->type == ARITHMOP) && (matched_token_ari_type() == DIV) ){
		match_exp();
		t = get_last_matched(tok_arr);
		if(t->type == INT_LIT){
			if(t->data.i == 0)
				return DIV_BY_ZERO;
		} else if (t->type == FLOAT_LIT) {
			if(t->data.d == 0)
				return DIV_BY_ZERO;
		}
		return NO_ERROR;
	}
	CHECK( match_exp() );
	return NO_ERROR;	
}

int move_E_to_new(){
	if(check_last(exp_buf) == 'E'){
		pop_last(exp_buf);
		CHECK( buf_append(exp_buf, '<') );
		CHECK( buf_append(exp_buf, 'E') );
	} else {
		CHECK( buf_append(exp_buf, '<') );
	}
	return NO_ERROR;
}

bool compare_rule(char *rule, char a, char b, char c){
	char str[3];
	str[0] = a;
	str[1] = b;
	str[2] = c;

	if(strcmp(rule,str)==0)
		return true;

	return false;
}

int get_rule(char (*rule)[3]){
	int diff;
	for(int i = 0; i < 4; i++)
		(*rule)[i] = 0;

	for(int i = get_idx(exp_buf) - 1; i >= 0; i--){
		if(check_symbol(exp_buf, i) == '<'){
			diff = get_idx(exp_buf) - 1 - i;
			if( diff > 3){
				return SYNTAX_ERROR;
			}
			for(int j = diff-1; j >= 0 ; j--){
				(*rule)[j] = get_last(exp_buf);
			}
			pop_last(exp_buf);
			break;
		}
	}
	return NO_ERROR;
}

Buf create_buf(int size){

	Buf b = (Buf)malloc(sizeof(struct buf));

	if(b == NULL)
		return NULL;

	b->ptr = (char *)malloc(sizeof(char) * size);

	if(b->ptr == NULL){
		free(b);
		return NULL;
	}

	b->size = size;
	b->index = 0;

	for(int i = 0; i < size; i++)		//vynulovanie alokovanej pamate - valgrind pindal
		b->ptr[i] = 0;

	return b;
}

int buf_append(Buf b, char c){

	if(b->index >= b->size-2){

		char * tmp;
		//uz by sa tam nezmestil zapis charu c + '0'
		tmp = (char * )realloc((void *) b->ptr, b->size * 2);

		if(tmp == NULL) {
			free(b->ptr);
			return MALLOC_ERROR;
		}else{
			b->ptr = tmp;
		}

		for(int i = b->size; i<b->size*2; i++)	//vynulovanie alokovanej pamate - valgrind pindal
			b->ptr[i] = 0;

		b->size *= 2;
	}

	b->ptr[b->index] = c;
	b->ptr[b->index + 1] = 0;
	b->index++;

	return NO_ERROR;
}

char check_symbol(Buf b, int index){
	// back = 0 -> last char, back = 1 -> previous char
	return b->ptr[index];
}

char check_prev(Buf b, int back){
	// back = 0 -> last char, back = 1 -> previous char
	return b->ptr[b->index - 1 - back];
}

int get_idx(Buf b){
	return b->index;
}

char check_top(Buf b){
	for(int i = b->index - 1; i >= 0; i--)
		if(b->ptr[i] != 'E')
			return b->ptr[i];
	return '$';
}

char check_last(Buf b){

	return b->ptr[b->index - 1];
}

void pop_last(Buf b){

	b->ptr[b->index - 1] = 0;
	b->index--;
	return;
}

char get_last(Buf b){
	char c = b->ptr[b->index - 1];
	b->ptr[b->index - 1] = 0;
	b->index--;
	return c;
}

void buf_clean(Buf b){

	for(int i = 0; i < b->index; i++){
		b->ptr[i] = '\0';
	}
	b->index = 0;
}

void delete_buf_and_data(Buf b){
	free(b->ptr);
	free(b);
}



int check_type( Token t, int *set_type){
	if(*set_type == 1){
		exp_type = get_type(t);

		*set_type = 0;
		
		if(exp_type == ID_TYPE_BOOLEAN){
			// if(strcmp("_",t->data.b->ptr)==0)
			// 	return SEMANTIC_ERR;
			return UNDEFINED_VAR;
		}

	} else {
		// 
		if(exp_type == ID_TYPE_BOOLEAN)
			return TYPE_COMPATIBILITY;

		ID_Type new_type = get_type(t);

		if(new_type == ID_TYPE_BOOLEAN){
			// if(strcmp("_",t->data.b->ptr)==0)
			// 	return SEMANTIC_ERR;
			return UNDEFINED_VAR;
		}

		if(exp_type != new_type)
			return TYPE_COMPATIBILITY;
	}
	return NO_ERROR;
}

// returns ID_TYPE_BOOLEAN if UNDEFINED_VAR
ID_Type get_type( Token t){
	int idx;
	Node id_node;
	
	switch(t->type){

		case INT_LIT:
			return ID_TYPE_INT;
		
		case FLOAT_LIT:
			return ID_TYPE_FLOAT;

		case STRING_LIT:
			return ID_TYPE_STRING;

		case IDENTIFIER:
			id_node = find_in_sot( stack_sot, t->data.b->ptr, TYPE_ID, &idx);

			if(id_node == NULL)
				return ID_TYPE_BOOLEAN;

			switch(id_node->id_type){

				case ID_TYPE_INT:
					return ID_TYPE_INT;
				
				case ID_TYPE_FLOAT:
					return ID_TYPE_FLOAT;
				
				case ID_TYPE_STRING:
					return ID_TYPE_STRING;
			}
	}
	return ID_TYPE_BOOLEAN;
}

int sem_operand(Token t){

	int i;
	if(t->type == IDENTIFIER){
		find_in_sot(stack_sot,t->data.b->ptr,TYPE_ID,&i);
		CODE("PUSHS LF@$"); CODE_INT(i); CODE_NL(t->data.b->ptr);
	} else {
		if(exp_type == ID_TYPE_INT){
			CODE("PUSHS int@"); CODE_LINT_NL(t->data.i);
		} else if(exp_type == ID_TYPE_FLOAT){
			CODE("PUSHS float@"); CODE_DBL_NL(t->data.d);
		} else if(exp_type == ID_TYPE_STRING) {
			Buffer buf = adjust_string(t->data.b);
			if(buf == NULL){
				return MALLOC_ERROR;
			}
			CODE("PUSHS string@"); CODE_NL(get_string(buf));
			delete_buffer_and_data(buf);
		} else {
			return TYPE_COMPATIBILITY;
		}
	}
	return NO_ERROR;
}

int sem_ar_op( char type){
	if(exp_type == ID_TYPE_STRING){
		switch(type){
			case '+':
				CODE("POPS "); CODE_NL("GF@$$symb2str");
				CODE("POPS "); CODE_NL("GF@$$symb1str");
				CODE("CONCAT "); CODE("GF@$$varstr "); CODE("GF@$$symb1str "); CODE_NL("GF@$$symb2str");
				CODE("PUSHS "); CODE_NL("GF@$$varstr");
				break;
			default:
				return TYPE_COMPATIBILITY;
		}
	} else if (exp_type == ID_TYPE_BOOLEAN){
		return TYPE_COMPATIBILITY;
	} else {
		switch(type){
			
			case '+':
				CODE_NL("ADDS ");
				break;

			case '-':
				CODE_NL("SUBS ");
				break;

			case '*':
				CODE_NL("MULS ");
				break;

			case '/':
				if(exp_type == ID_TYPE_INT){
					CODE_NL("IDIVS ");
				} else {
					CODE_NL("DIVS ");
				}
				break;
		}
	}
	return NO_ERROR;
}

int sem_rel_op( char type){

	if (exp_type == ID_TYPE_BOOLEAN)
		return TYPE_COMPATIBILITY;

	switch(type){
		case 'a':
			CODE_NL("GTS ");
			break;
		
		case 'b':
			CODE_NL("LTS ");
			CODE_NL("NOTS ");
			break;
		case 'c':
			CODE_NL("LTS ");
			break;

		case 'd':
			CODE_NL("GTS ");
			CODE_NL("NOTS ");
			break;

		case 'e':
			CODE_NL("EQS ");
			break;
			
		case 'f':
			CODE_NL("EQS ");
			CODE_NL("NOTS ");
			break;
	}
	exp_type = ID_TYPE_BOOLEAN;
	return NO_ERROR;
}

int match_exp(){
	//num of tokens bude vzdy aspon o 1 vacsie, ak je vacsie o viacej to znamena ze mame nacitany token dopredu
	if( tok_arr->num_of_tokens > (unsigned)(tok_arr->current +1 )) {
		tok_arr->current++;
	}else {
		//nacitaj novy token
		int val = next_token();
		if ( val == LEXICAL_ERROR || val == MALLOC_ERROR )
			return val;

		tok_arr->current++;
	}

	#ifdef MY_DEBUG
		fprintf(stderr,"Match: \t\t");
		print_token_type(tok_arr->array[tok_arr->current]);
	#endif

	return NO_ERROR;
}

int next_token_rel_type() {
	return tok_arr->array[tok_arr->num_of_tokens-1]->data.ro;
}

int next_token_ari_type() {
	return tok_arr->array[tok_arr->num_of_tokens-1]->data.ao;
}

int matched_token_ari_type() {
	return tok_arr->array[tok_arr->current]->data.ao;
}