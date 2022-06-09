/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Code generator helper functions interface.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

// Generate new line
#define CODE_NEWLINE()      printf("\n")
// Generate comment segment
#define CODE_COMMENT(str)   printf("# %s\n", str)
// Generate frame
#define CODE_FRAME()        printf("CREATEFRAME\nPUSHFRAME\n")
// Generate inline code
#define CODE(str)           printf("%s", str)
// Generate code with new line at the end
#define CODE_NL(str)        printf("%s\n", str)
// Generate inline code (int)
#define CODE_INT(i)       	printf("%d", i)
// Generate integer exp code with new line at the end
#define CODE_LINT_NL(i)      printf("%ld\n", i)
// Generate double exp code with new line at the end
#define CODE_DBL_NL(d)      printf("%a\n", d)
// Generate LABEL
#define CODE_LABEL(label)   printf("LABEL $%s\n", label)
// Generate CALL
#define CODE_CALL(label)    printf("CALL $%s\n", label)
// Generate EXIT int@0
#define CODE_EXIT()         printf("EXIT int@0")

#endif
