/**
 * Compiler implementation of imperative language IFJ20.
 *
 * -----------Macros-----------
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 */

#ifndef IFJ_PROJEKT_ERROR_MACROS_H
#define IFJ_PROJEKT_ERROR_MACROS_H

#define NO_ERROR 			90
#define LEXICAL_ERROR 		91
#define SYNTAX_ERROR 		92
#define UNDEFINED_FUNC      93
#define UNDEFINED_VAR       93
#define ALREADY_DECLARED    93
#define INVALID_DATATYPE    94
#define TYPE_COMPATIBILITY  95
#define PARAMS_ERR          96
#define SEMANTIC_ERR        97
#define DIV_BY_ZERO         99

#define MALLOC_ERROR 189

#define TRUE 51
#define FALSE 50
#define EMPTY -1

//#define MY_DEBUG 0

int var_next_token;

#define PRINT_AND_RET(X) do{\
		switch(X){\
			case(MALLOC_ERROR):\
				fprintf(stderr,"MALLOC_ERROR\n");\
				return MALLOC_ERROR;\
			case(LEXICAL_ERROR):\
				fprintf(stderr,"LEXICAL_ERROR\n");\
				return LEXICAL_ERROR;\
		}\
	}while(0)

#define CHECK(X) do{ \
	int val = X;\
		if( val != NO_ERROR) \
			return val;\
	}while(0)

#define CHECK_ONLY_ERR(X) do{ \
	int val = X;\
		if( val != TRUE && val != FALSE) \
			return val;\
	}while(0)



#define CHECK_NEXT_TOKEN(X) do{ \
	var_next_token = X;\
		if( var_next_token == MALLOC_ERROR || var_next_token == LEXICAL_ERROR)\
			return var_next_token;\
	}while(0)





#endif //IFJ_PROJEKT_ERROR_MACROS_H
