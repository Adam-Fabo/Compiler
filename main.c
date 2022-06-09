/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 * @author Adam Fabo <xfaboa00(at)stud.fit.vutbr.cz>
 * @author Albert Groma <xgroma00(at)stud.fit.vutbr.cz>
 * @author Stanislav Gabriš <xgabri18(at)stud.fit.vutbr.cz>
 */



#include <stdio.h>
#include <stdlib.h>

#include "symtable.h"
#include "lexiA.h"
#include "test_lexical.h"
#include "buffer.h"
#include "parser.h"
#include "initialization.h"
#include "symtable_stack.h"


int main(){

	int val = init();
	if (val == MALLOC_ERROR)
		return 99;

	val = parser();

	deinit();
	return val;

}
