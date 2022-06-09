/**
 * Compiler implementation of imperative language IFJ20.
 *
 * String adjustment
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */

#include "adjust_constant.h"

Buffer adjust_string(Buffer buff){
    Buffer tmp = create_buffer(buff->size);
    if(tmp == NULL) return NULL;

    int id = 0;
    char current = buff->ptr[id];

    while(current != '\0'){
        if(current == HASH_SYMBOL){
            for(int i = 0; i < 4; i++){
                buffer_append(tmp,H_HASH_SYMBOL[i]);
            }
        }else if(current == SPACE){
            for(int i = 0; i < 4; i++){
                buffer_append(tmp,H_SPACE[i]);
            }
        }else if(current == BACKSLASH){
            current = buff->ptr[++id];
            if(current == 'n'){
                for(int i = 0; i < 4; i++){
                    buffer_append(tmp,H_NEWLINE[i]);
                }
            }else if(current == 't'){
                for(int i = 0; i < 4; i++){
                    buffer_append(tmp,H_TAB[i]);
                }
            }else if(current == BACKSLASH) {
                for (int i = 0; i < 4; i++) {
                    buffer_append(tmp, H_BACKSLASH[i]);
                }
            }else{
                buffer_append(tmp, current);
            }
        }else{
            buffer_append(tmp, current);
        }


        current = buff->ptr[++id];
    }

    return tmp;
}