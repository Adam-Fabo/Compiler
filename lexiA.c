/**
 * Compiler implementation of imperative language IFJ20.
 *
 * ------------------Scanner---------------------
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexiA.h"





int lexical_analysis(Token token){
    int in_char;
    int lex_err = NO_ERROR;  //stores return values of functions
    int newline = 0;

    if((in_char = getchar()) == EOF){
        token->type = SCANEOF;
        return NO_ERROR;
    }else{
        ungetc(in_char,stdin);
    }



	//beginning of lexema
	while((in_char = getchar()) != EOF){


        if(in_char == '\n'){     //case '\n'
            token->type = NEWLINE;
            break;

        }else if( isspace(in_char)){
            if((in_char = getchar()) == EOF){ //is next char EOF?
                token->type = SCANEOF;
                return NO_ERROR;
            }else{
                ungetc(in_char,stdin);
            }
			continue;



		}else if( isalpha(in_char) || in_char == '_'){	//identifier or keyword (allocates buffer)
            Keywords keyw;

            if((token->data.b = create_buffer(20))== NULL){
                return MALLOC_ERROR;
            }
            if((buffer_append(token->data.b,(char)in_char)) == 0){ //insert 1. char to array
                return MALLOC_ERROR;
            }
            token->type = IDENTIFIER;

            while((in_char = getchar()) != EOF){ //continue reading
                if(in_char == '_' || isalpha(in_char) || isdigit(in_char)){
                    if((buffer_append(token->data.b, (char)in_char)) == 0){
                        return MALLOC_ERROR;
                    }
                }else{
                    break; //end of id
                }

            }
            ungetc(in_char,stdin); //return the undesired char back
            if((keyw = detect_keyword(token->data.b)) != NOT_VALID){ //if it is keyword
                delete_buffer_and_data(token->data.b);
                token->data.kw = keyw;
                token->type = KEYWORD;
            }
            break;


		}else if( isdigit(in_char)){	//number
            token->type = INT_LIT;
            token->data.i = in_char - '0';

            if(token->data.i == 0){  //first number is 0
                lex_err = FST_float(token,in_char);
            }else{
                while(isdigit(in_char = getchar())){
                    token->data.i = (token->data.i * DECADIC) + (in_char - '0');
                }
                ungetc(in_char,stdin); //return the non-digit number
                lex_err = FST_float(token,in_char);
            }

            if(lex_err == LEXICAL_ERROR){
                return LEXICAL_ERROR;
            }

            break;



		}else if( in_char == '[') {
            token->type = LBRACKET;
            break;

        }else if( in_char == ']') {
            token->type = RBRACKET;
            break;

        }else if( in_char == '(') {
            token->type = LPAREN;
            break;

        }else if( in_char == ')') {
            token->type = RPAREN;
            break;

        }else if( in_char == '{') {
            token->type = LCURL;
            break;

        }else if( in_char == '}') {
            token->type = RCURL;
            break;

        }else if( in_char == ',') {
            token->type = COMMA;
            break;

        }else if( in_char == ';') {
            token->type = SEMICOLON;
            break;

        }else if( in_char == '+') {
            token->type = ARITHMOP;
            token->data.ao = ADD;
            break;

        }else if( in_char == '-') {
            token->type = ARITHMOP;
            token->data.ao = SUB;
            break;

        }else if( in_char == '*') {
            token->type = ARITHMOP;
            token->data.ao = MUL;
            break;

        }else if( in_char == '/') { //div or comment
            token->type = ARITHMOP;
            token->data.ao = DIV;
            if (((in_char = getchar()) == '/') || in_char == '*'){ //comment
                lex_err = FST_comment(token, in_char, &newline);
                if(lex_err == NO_ERROR){
                    if(newline == 1){ //there was newline in comment so we check what is after comment
                        while((in_char = getchar()) == ' ' || in_char == '\t');
                        ungetc(in_char, stdin);
                        if(in_char != '\n') {
                            token->type = NEWLINE;
                            return NO_ERROR;
                        }
                    }
                    continue;
                }else{ //lexical error encountered
                    return LEXICAL_ERROR;
                }
            }
            ungetc(in_char,stdin);
            break;

        }else if( in_char == ':') { //:=
            token->type = ASSIGNMENT;
            if((in_char = getchar()) != '='){
                return LEXICAL_ERROR;
            }
            break;

        }else if( in_char == '!') { //!=
            token->type = RELATIONOP;
            token->data.ro = NE;
            if((in_char = getchar()) != '='){
                return LEXICAL_ERROR;
            }
            break;

        }else if( in_char == '=') { //= ==
            token->type = EQUAL;
            if((in_char = getchar()) == '='){ //==
                token->type = RELATIONOP;
                token->data.ro = EE;
                return NO_ERROR;
            }
            ungetc(in_char,stdin);
            break;

        }else if( in_char == '>') { //> >=
            token->type = RELATIONOP;
            token->data.ro = GR;
            if ((in_char = getchar()) == '=') { //>=
                token->data.ro = GRE;
                return NO_ERROR;
            }
            ungetc(in_char, stdin);
            break;

        }else if( in_char == '<') { //< <=
            token->type = RELATIONOP;
            token->data.ro = LS;
            if ((in_char = getchar()) == '=') { //<=
                token->data.ro = LSE;
                return NO_ERROR;
            }
            ungetc(in_char, stdin);
            break;

        }else if(in_char == '\"'){ // "abc"
            token->type = STRING_LIT;
            if((token->data.b = create_buffer(20))== NULL){
                return MALLOC_ERROR;
            }



            /*
            if((buffer_append(token->data.b,(char)in_char)) == 0){
                return MALLOC_ERROR;
            }
            */
            lex_err = FST_string_lit(token,in_char);
            if(lex_err == LEXICAL_ERROR || lex_err == MALLOC_ERROR){
                delete_buffer_and_data(token->data.b);
                return lex_err;
            }

            return NO_ERROR;

        }else{ //not an allowed character - default
            return LEXICAL_ERROR;
        }





	}




    return NO_ERROR;
}









int FST_float(Token token,int ch){
    int negative = 0;
    int num = 0;
    int power;
    double tmp = (double)token->data.i;



    ch = getchar();
    if(ch == '.' || ch == 'e' || ch == 'E'){
        token->data.d = tmp;
        token->type = FLOAT_LIT;


        if(ch == '.'){              //2.
            while(isdigit(ch = getchar())){ // sequence of numbers after .          //3.
                num++;
                tmp = (double)(ch - '0');
                for(int i = 0; i < num; i++){
                    tmp = tmp/DECADIC;
                }
                token->data.d += tmp ;

            }

            if(num == 0){ //there is no number after '.'
                return LEXICAL_ERROR;
            }else{ //float is ok
                if (ch != 'E' && ch != 'e'){ //float x.x
                    ungetc(ch,stdin);
                    return NO_ERROR;
                }
            }
        }

        //---e/E    4.
        if(isdigit(ch = getchar()) || ch == '+' || ch == '-'){
            if(ch == '-' || ch == '+'){     //5.
                if(ch == '-'){
                    negative = -1;
                }
                if(!isdigit(ch = getchar())){ //+/- not followed by number
                    return LEXICAL_ERROR;
                }
            }

            power = ch - '0';
            while(isdigit(ch = getchar())){  // 6.
                power = (power*DECADIC) + (ch - '0'); //power is exponent
            }

            if(negative < 0){ //E-x
                power = -(power);
            }

            for(int i = 0; i < power;i++) { //E+x
                token->data.d = token->data.d * DECADIC; //float x.xEx
            }

            for(int i = 0; i > power;i--) {  //E-x
                token->data.d = token->data.d * (1.0/DECADIC); //float x.xEx
            }




        }else{ //e/E not followed by number
            return LEXICAL_ERROR;
        }



    }

    ungetc(ch,stdin);

    return NO_ERROR;
}





int FST_comment(Token token ,int ch, int* newline){
    if(ch == '/'){ //one line comment
        while(((ch = getchar()) != '\n') && ch != EOF);
        if(ch == EOF){
            token->type = SCANEOF;
        }
        ungetc(ch,stdin); //return \n into stream
        return NO_ERROR;

    }else if(ch == '*'){ //comment /**/
        while(1) {
            while (((ch = getchar()) != '*') && ch != EOF){
                if(ch == '\n'){ //if the comment contains newline
                    *newline = 1;
                }
            }

            if (ch == '*') {
                if ((ch = getchar()) == '/') { //end of comment
                    token->type = SCANEOF;
                    return NO_ERROR;
                }
                ungetc(ch,stdin);
            }
            if(ch == EOF) { //err comment not finished
                token->type = SCANEOF;
                return LEXICAL_ERROR;
            }
        }
    }

    return LEXICAL_ERROR; //unreachable code
}



int FST_string_lit(Token token,int ch){
    while(1){
        while(((ch = getchar()) > 31) && ch != '\\' && ch != '\"' && ch != EOF) {//chars below 31 are not allowed
            if((buffer_append(token->data.b,(char)ch)) == 0){
                return MALLOC_ERROR;
            }
        }
        if(ch == '\"'){ //end of literal
            /*
            if((buffer_append(token->data.b,(char)ch)) == 0){
                return MALLOC_ERROR;
            }
            */
            break;

        }else if(ch == '\\'){ //escape sequence
            if((buffer_append(token->data.b,(char)ch)) == 0){
                return MALLOC_ERROR;
            }
            ch = getchar();

            if(ch == '\"' || ch == 'n' || ch == 't' || ch == '\\'){
                if((buffer_append(token->data.b,(char)ch)) == 0){
                    return MALLOC_ERROR;
                }

            }else if(ch == 'x'){ //hexadecimal number
                if(((ch = getchar()) >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')){
                    if((buffer_append(token->data.b,(char)ch)) == 0){
                        return MALLOC_ERROR;
                    }

                    if(((ch = getchar()) >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')) {
                        if((buffer_append(token->data.b,(char)ch)) == 0){
                            return MALLOC_ERROR;
                        }
                    }else{
                        return LEXICAL_ERROR;
                    }

                }else{
                    return LEXICAL_ERROR;
                }

            }else{ //no valid character after backslash
                return LEXICAL_ERROR;
            }


        }else{ //EOF or char with value less then 31
            return LEXICAL_ERROR;
        }
    }

    return NO_ERROR;
}




int detect_keyword(Buffer b){

	//if equal strcmp returns 0
	if     ( !strcmp(b->ptr,STR_IF)      ) return KEY_IF;
	else if( !strcmp(b->ptr,STR_ELSE)    ) return KEY_ELSE;
	else if( !strcmp(b->ptr,STR_FLOAT64) ) return KEY_FLOAT64;
	else if( !strcmp(b->ptr,STR_FOR)     ) return KEY_FOR;
	else if( !strcmp(b->ptr,STR_FUNC)    ) return KEY_FUNC;
	else if( !strcmp(b->ptr,STR_INT)     ) return KEY_INT;
	else if( !strcmp(b->ptr,STR_PACKAGE) ) return KEY_PACKAGE;
	else if( !strcmp(b->ptr,STR_RETURN)  ) return KEY_RETURN;
	else if( !strcmp(b->ptr,STR_STRING)  ) return KEY_STRING;
	else return NOT_VALID;

}
