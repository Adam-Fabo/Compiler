/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Built-in functions interface.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include "symtable.h"
#include "code_generator.h"
#include "lexiA.h"
#include "exp.h"

#ifndef BUILTIN_FUNC_H
#define BUILTIN_FUNC_H

#define CHECK_PARAM(buff, param) do {           \
        buff = generate_param(param);           \
        if (buff == NULL) return MALLOC_ERROR;  \
    } while(0)

#define CLEAR_PARAM(buff) do {      \
    delete_buffer_and_data(buff);   \
} while(0)


/**
 * @struct Stack for ints
 */
typedef struct Int_Buffer {
    int *ptr;
    unsigned size;
    unsigned top;
} *Int_Buffer;

/**
 * Built-in functions tree.
 */
Symtable builtin_functions;

/**
 * Global variable of parameters identifier.
 */
Buffer tmp_param;

/**
 * Stack of integers for special prints.
 */
Int_Buffer int_buffer;

/**
 * Initialize built-in functions.
 *
 * @return NO_ERROR|MALLOC_ERROR
 */
int builtin_init();

/**
 * Destroys initialized tree.
 *
 * @return MALLOC_ERROR if allocation error occurs otherwise NO_ERROR.
 */
int builtin_destroy();

/**
 * Check if function is a built-in function.
 *
 * @param name Name of function
 *
 * @return TRUE if function found inside tree, otherwise FALSE.
 */
int is_builtin_function(char *name);

/**
 * Generates tree with built-in functions.
 *
 * @return MALLOC_ERROR if allocation error occurs otherwise NO_ERROR.
 */
int generate_tree();

/**
 * Generates code with built-in functions.
 */
void generate_builtin_functions();

/**
 * Utility function for generating parameters for function.
 *
 * @param buffer Buffer for character
 * @param size Size of buffer
 * @param c Character to insert into buffer
 *
 * @return Parameter for function
 */
Buffer generate_param(char c);

/**
 * Initialize Int Buffer.
 *
 * @return Int_Buffer or NULL if fails.
 */
Int_Buffer int_buffer_init();

/**
 * Destroy Int Buffer.
 *
 * @param stack Stack of numbers
 */
void int_buffer_destroy(Int_Buffer stack);

/**
 * Appends int into Int Buffer (if that number does not exists)
 *
 * @param stack Stack of numbers
 * @param number Number to add into stack
 *
 * @return MALLOC_ERROR if allocation error, otherwise NO_ERROR
 */
int int_buffer_append(Int_Buffer stack, int number);

/**
 * Built-in function inputs.
 *
 * @brief Read one line string from standard input.
 */
void inputs();

/**
 * Built-in function inputi.
 *
 * @brief Read one line integer from standard input.
 */
void inputi();

/**
 * Built-in function inputf.
 *
 * @brief Read one line float from standard input.
 */
void inputf();

/**
 * Generate prints for x number of terms.
 */
void generate_prints();

/**
 * Built-in function print.
 *
 * @brief Write on standard output.
 */
void generate_print(int parameters_count);

/**
 * Built-in function int2float.
 *
 * @brief Converts int into float.
 */
void int2float();

/**
 * Built-in function float2int.
 *
 * @brief Converts float into int.
 */
void float2int();

/**
 * Built-in function len.
 *
 * @brief Get the length of a string.
 */
void len();

/**
 * Built-in function substr.
 *
 * @brief Get the sub string of a string.
 */
void substr();

/**
 * Built-in function ord.
 *
 * @brief Get ASCII value of char from string on selected position.
 */
void ord();

/**
 * Built-in function chr.
 *
 * @brief Get ASCII symbol based on integer value.
 */
void chr();

#endif //BUILTIN_FUNC_H
