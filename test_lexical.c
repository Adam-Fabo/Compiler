//
// Created by freee on 18. 10. 2020.
//

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "symtable.h"
#include "lexiA.h"
#include "test_lexical.h"
#include "token_buffer.h"




Token_arr tarr;

void test_lexical(){



	Token tk;
	if( (tarr = create_token_arr(32)) == NULL){
		fprintf(stderr,"Popici\n");
		return;
	}

	Token_type type;



	do {

		tk = (Token) malloc(sizeof(struct token));

		if(tk == NULL){  fprintf(stderr,"Popici\n"); return;  }


		if ( lexical_analysis(tk) == LEXICAL_ERROR ){ fprintf(stderr,"LEX_error \n"); free(tk); return ; }

		token_arr_append(tarr,tk);



		print_token_type(tk);

		if(tk->type == IDENTIFIER){
			fprintf(stderr,"Data = %s",tk->data.b->ptr);
			//delete_buffer_and_data(tk->data.b);
		}else if(tk->type == KEYWORD){
			switch (tk->data.kw){
				case NOT_VALID: fprintf(stderr,"NOT_VALID "); break;
				case KEY_ELSE: fprintf(stderr,"KEY_ELSE "); break;
				case KEY_FLOAT64: fprintf(stderr,"KEY_FLOAT64 "); break;
				case KEY_FOR: fprintf(stderr,"KEY_FOR "); break;
				case KEY_FUNC: fprintf(stderr,"KEY_FUNC "); break;
				case KEY_IF: fprintf(stderr,"KEY_IF "); break;
				case KEY_INT: fprintf(stderr,"KEY_INT "); break;
				case KEY_PACKAGE: fprintf(stderr,"KEY_PACKAGE "); break;
				case KEY_RETURN: fprintf(stderr,"KEY_RETURN "); break;
				case KEY_STRING: fprintf(stderr,"KEY_STRING "); break;
			}
		}else if(tk->type == INT_LIT){
			fprintf(stderr,"Data = %ld",tk->data.i);
		}else if(tk->type == FLOAT_LIT){
			fprintf(stderr,"Data = %f",tk->data.d);
		}else if(tk->type == STRING_LIT){
			fprintf(stderr,"Data = %s",tk->data.b->ptr);
			//delete_buffer_and_data(tk->data.b);
		}else if(tk->type == LBRACKET){
			fprintf(stderr,"Data = [");
		}else if(tk->type == RBRACKET){
			fprintf(stderr,"Data = ]");
		}else if(tk->type == LPAREN){
			fprintf(stderr,"Data = (");
		}else if(tk->type == RPAREN){
			fprintf(stderr,"Data = )");
		}else if(tk->type == LCURL){
			fprintf(stderr,"Data = {");
		}else if(tk->type == RCURL){
			fprintf(stderr,"Data = }");
		}else if(tk->type == ASSIGNMENT){
			fprintf(stderr,"Data = :=");
		}else if(tk->type == EQUAL){
			fprintf(stderr,"Data = =");
		}else if(tk->type == COMMA){
			fprintf(stderr,"Data = ,");
		}else if(tk->type == SEMICOLON){
			fprintf(stderr,"Data = ;");
		}else if(tk->type == ARITHMOP){
			switch (tk->data.ao){
				case ADD: fprintf(stderr,"ADD "); break;
				case SUB: fprintf(stderr,"SUB "); break;
				case MUL: fprintf(stderr,"MUL "); break;
				case DIV: fprintf(stderr,"DIV "); break;
			}
		}else if(tk->type == RELATIONOP){
			switch (tk->data.ro){
				case GR: fprintf(stderr,"GR > "); break;
				case GRE: fprintf(stderr,"GRE >="); break;
				case LS: fprintf(stderr,"LS <"); break;
				case LSE: fprintf(stderr,"LSE <="); break;
				case EE: fprintf(stderr,"EE =="); break;
				case NE: fprintf(stderr,"NE !="); break;
			}
		}else if(tk->type == NEWLINE){
			fprintf(stderr,"NEWLINE");
		}else if(tk->type == SCANEOF) {
			fprintf(stderr,"SCANEOF");
		}else{
			fprintf(stderr,"Dajaka neznama hodnota");
		}

		fprintf(stderr,"\n");

		type = tk->type;
		//free(tk);

	}while(type != SCANEOF);


	delete_token_arr(tarr);
	//free(tk);



}


void print_token_type(Token  tk){

	fprintf(stderr,"Token ma typ :  ");

	switch (tk->type){
		case IDENTIFIER: fprintf(stderr,"IDENTIFIER "); break;
		case KEYWORD: fprintf(stderr,"KEYWORD "); break;
		case INT_LIT: fprintf(stderr,"INT_LIT "); break;
		case FLOAT_LIT: fprintf(stderr,"FLOAT_LIT "); break;
		case STRING_LIT: fprintf(stderr,"STRING_LIT "); break;
		case LBRACKET: fprintf(stderr,"LBRACKET "); break;
		case RBRACKET: fprintf(stderr,"RBRACKET "); break;
		case LPAREN: fprintf(stderr,"LPAREN "); break;
		case RPAREN: fprintf(stderr,"RPAREN "); break;

		case LCURL: fprintf(stderr,"LCURL "); break;
		case RCURL: fprintf(stderr,"RCURL "); break;
		case ASSIGNMENT: fprintf(stderr,"ASSIGNMENT "); break;
		case EQUAL: fprintf(stderr,"EQUAL "); break;
		case COMMA: fprintf(stderr,"COMMA "); break;
		case SEMICOLON: fprintf(stderr,"SEMICOLON "); break;
		case ARITHMOP: fprintf(stderr,"ARITHMOP ");
			switch (tk->data.ao){
				case ADD: fprintf(stderr,"ADD "); break;
				case SUB: fprintf(stderr,"SUB "); break;
				case MUL: fprintf(stderr,"MUL "); break;
				case DIV: fprintf(stderr,"DIV "); break;
			}
			break;
		case RELATIONOP: fprintf(stderr,"RELATIONOP ");
			switch (tk->data.ro){
				case GR: fprintf(stderr,"GR "); break;
				case GRE: fprintf(stderr,"GRE "); break;
				case LS: fprintf(stderr,"LS "); break;
				case LSE: fprintf(stderr,"LSE "); break;
				case EE: fprintf(stderr,"EE "); break;
				case NE: fprintf(stderr,"NE "); break;
			}
			break;
		case NEWLINE: fprintf(stderr,"NEWLINE "); break;
		case SCANEOF: fprintf(stderr,"SCANEOF "); break;


	}

	fprintf(stderr,"\n");


}


void print_type(int typ){

	fprintf(stderr,"Dostal som typ :  ");

	switch (typ){
		case IDENTIFIER: fprintf(stderr,"IDENTIFIER "); break;
		case KEYWORD: fprintf(stderr,"KEYWORD "); break;
		case INT_LIT: fprintf(stderr,"INT_LIT "); break;
		case FLOAT_LIT: fprintf(stderr,"FLOAT_LIT "); break;
		case STRING_LIT: fprintf(stderr,"STRING_LIT "); break;
		case LBRACKET: fprintf(stderr,"LBRACKET "); break;
		case RBRACKET: fprintf(stderr,"RBRACKET "); break;
		case LPAREN: fprintf(stderr,"LPAREN "); break;
		case RPAREN: fprintf(stderr,"RPAREN "); break;

		case LCURL: fprintf(stderr,"LCURL "); break;
		case RCURL: fprintf(stderr,"RCURL "); break;
		case ASSIGNMENT: fprintf(stderr,"ASSIGNMENT "); break;
		case EQUAL: fprintf(stderr,"EQUAL "); break;
		case COMMA: fprintf(stderr,"COMMA "); break;
		case SEMICOLON: fprintf(stderr,"SEMICOLON "); break;
		case ARITHMOP: fprintf(stderr,"ARITHMOP "); break;
		case RELATIONOP: fprintf(stderr,"RELATIONOP "); break;
		case NEWLINE: fprintf(stderr,"NEWLINE "); break;
		case SCANEOF: fprintf(stderr,"SCANEOF "); break;


		default:fprintf(stderr,"DACO  INE %d ", typ); break;

	}

	fprintf(stderr,"\n");


}

