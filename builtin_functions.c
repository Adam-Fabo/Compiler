/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Built-in functions.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include "builtin_functions.h"

/**
 * Initialize built-in functions.
 *
 * @return NO_ERROR|MALLOC_ERROR
 */
int builtin_init()
{
    // Initialize tree
    builtin_functions = bst_init();

    // Create int_buffer
    if ((int_buffer = int_buffer_init()) == NULL) return MALLOC_ERROR;

    // Add built-in functions into tree
    return generate_tree();

   // return NO_ERROR;
}

/**
 * Destroys initialized tree.
 *
 * @return MALLOC_ERROR if allocation error occurs otherwise NO_ERROR.
 */
int builtin_destroy()
{
    // Destroy tree
    bst_destroy(&builtin_functions);

    // Destroy int_buffer
    int_buffer_destroy(int_buffer);

    return NO_ERROR;
}

/**
 * Check if function is a built-in function.
 *
 * @param name Name of function
 *
 * @return TRUE if function found inside tree, otherwise FALSE.
 */
int is_builtin_function(char *name)
{
    return bst_find(builtin_functions, name, TYPE_FUNC) != NULL ? 1 : 0;
}

/**
 * Generates tree with built-in functions.
 *
 * @return MALLOC_ERROR if allocation error occurs otherwise NO_ERROR.
 */
int generate_tree()
{
    Func function;
    Buffer p;

    /**
     * inputs() (string, int)
     */
    function = create_func();
    function->name = "inputs";
    CHECK(append_param(function->ret, ID_TYPE_STRING, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * inputi() (int, int)
     */
    function = create_func();
    function->name = "inputi";
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * inputf() (float64, int)
     */
    function = create_func();
    function->name = "inputf";
    CHECK(append_param(function->ret, ID_TYPE_FLOAT, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * print(term(1),..,term(n))
     */
    function = create_func();
    function->name = "print";
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * int2float(i int) (float64)
     */
    function = create_func();
    function->name = "int2float";
    CHECK_PARAM(p, 'i');
    CHECK(append_param(function->par, ID_TYPE_INT, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_FLOAT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;


    /**
     * float2int(f float64) (int)
     */
    function = create_func();
    function->name = "float2int";
    CHECK_PARAM(p, 'f');
    CHECK(append_param(function->par, ID_TYPE_FLOAT, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * len(s string) (int)
     */
    function = create_func();
    function->name = "len";
    CHECK_PARAM(p, 's');
    CHECK(append_param(function->par, ID_TYPE_STRING, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * substr(s string, i int, n int) (string, int)
     */
    function = create_func();
    function->name = "substr";
    CHECK_PARAM(p, 's');
    CHECK(append_param(function->par, ID_TYPE_STRING, p));
    CLEAR_PARAM(p);
    CHECK_PARAM(p, 'i');
    CHECK(append_param(function->par, ID_TYPE_INT, p));
    CLEAR_PARAM(p);
    CHECK_PARAM(p, 'n');
    CHECK(append_param(function->par, ID_TYPE_INT, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_STRING, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * ord(s string, i int) (int, int)
     */
    function = create_func();
    function->name = "ord";
    CHECK_PARAM(p, 's');
    CHECK(append_param(function->par, ID_TYPE_STRING, p));
    CLEAR_PARAM(p);
    CHECK_PARAM(p, 'i');
    CHECK(append_param(function->par, ID_TYPE_INT, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    /**
     * chr(i int) (string, int)
     */
    function = create_func();
    function->name = "chr";
    CHECK_PARAM(p, 'i');
    CHECK(append_param(function->par, ID_TYPE_INT, p));
    CLEAR_PARAM(p);
    CHECK(append_param(function->ret, ID_TYPE_STRING, NULL));
    CHECK(append_param(function->ret, ID_TYPE_INT, NULL));
    if (bst_add_func(&builtin_functions, function) != NO_ERROR) return MALLOC_ERROR;

    return NO_ERROR;
}

/**
 * Generates code with built-in functions.
 */
void generate_builtin_functions()
{
    inputs();
    inputi();
    inputf();
    generate_prints();
    int2float();
    float2int();
    len();
    substr();
    ord();
    chr();
}

/**
 * Utility function for generating parameters for function.
 *
 * @param c Character to insert into buffer
 *
 * @return Parameter for function
 */
Buffer generate_param(char c)
{
    tmp_param = create_buffer(2);
    if (!tmp_param) return NULL;
    if (!buffer_append(tmp_param, c)) return NULL;
    return tmp_param;
}

/**
 * Initialize Int Buffer.
 *
 * @return Int_Buffer or NULL if fails.
 */
Int_Buffer int_buffer_init()
{
    Int_Buffer stack = (Int_Buffer) malloc(sizeof(struct Int_Buffer));

    if (!stack) return NULL;

    unsigned init_size = 5;
    stack->ptr = (int *) calloc(init_size, sizeof(int));

    if (!stack->ptr) {
        free(stack);
        return NULL;
    }

    stack->top = 0;
    stack->size = init_size;

    return stack;
}

/**
 * Destroy Int Buffer.
 *
 * @param stack Stack of numbers
 */
void int_buffer_destroy(Int_Buffer stack)
{
    free(stack->ptr);
    free(stack);
}

/**
 * Appends int into Int Buffer (if that number does not exists)
 *
 * @param stack Stack of numbers
 * @param number Number to add into stack
 *
 * @return MALLOC_ERROR if allocation error, otherwise NO_ERROR
 */
int int_buffer_append(Int_Buffer stack, int number)
{
    // Check if value is not already inside stack
    for (unsigned i = 0; i < stack->top; i++) {
        if (stack->ptr[i] == number) return NO_ERROR;
    }

    if (stack->top >= stack->size) {
        // Create 5 more elements
        int *tmp = (int *) realloc(stack->ptr, stack->size + (5 * sizeof(int)));

        if (!tmp) {
            free(stack->ptr);
            return MALLOC_ERROR;
        }

        stack->ptr = tmp;

        stack->size += 5 * sizeof(int);
    }

    stack->ptr[stack->top] = number;
    stack->top++;

    return NO_ERROR;
}

/**
 * Built-in function inputs.
 *
 * @brief Read one line string from standard input.
 */
void inputs()
{
    CODE_COMMENT("Built-in function inputs");

    CODE_LABEL("$inputs");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@ret_str");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("DEFVAR LF@tmp");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("READ LF@ret_str string");
    CODE_NL("TYPE LF@tmp LF@ret_str");
    CODE_NL("JUMPIFNEQ $$inputs_err LF@tmp string@string");
    CODE_NL("STRLEN LF@tmp LF@ret_str");
    CODE_NL("JUMPIFNEQ $$inputs_end LF@tmp int@0");

    CODE_LABEL("$inputs_err");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$inputs_end");
    CODE_NL("PUSHS LF@ret_str");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function inputi.
 *
 * @brief Read one line integer from standard input.
 */
void inputi()
{
    CODE_COMMENT("Built-in function inputi");

    CODE_LABEL("$inputi");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@ret_int");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("DEFVAR LF@tmp");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("READ LF@ret_int int");
    CODE_NL("TYPE LF@tmp LF@ret_int");
    CODE_NL("JUMPIFEQ $$inputi_end LF@tmp string@int");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$inputi_end");
    CODE_NL("PUSHS LF@ret_int");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function inputf.
 *
 * @brief Read one line float from standard input.
 */
void inputf()
{
    CODE_COMMENT("Built-in function inputf");

    CODE_LABEL("$inputf");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@ret_float");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("DEFVAR LF@tmp");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("READ LF@ret_float float");
    CODE_NL("TYPE LF@tmp LF@ret_float");
    CODE_NL("JUMPIFEQ $$inputf_end LF@tmp string@float");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$inputf_end");
    CODE_NL("PUSHS LF@ret_float");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Generate prints for x number of terms.
 */
void generate_prints()
{
    for (unsigned i = 0; i < int_buffer->top; i++) {
        generate_print(int_buffer->ptr[i]);
    }
}

/**
 * Built-in function print.
 *
 * @brief Write on standard output.
 */
void generate_print(int parameters_count)
{
    CODE("# Built-in function print with "); CODE_INT(parameters_count); CODE_NL(" parameters");
    CODE("LABEL $$print_"); CODE_INT(parameters_count); CODE_NEWLINE();
    CODE_FRAME();

    for (int i = 1; i <= parameters_count; i++) {
        CODE("DEFVAR LF@$"); CODE_INT(i); CODE_NEWLINE();
    }

    for (int i = parameters_count; i > 0; i--) {
        CODE("POPS LF@$"); CODE_INT(i); CODE_NEWLINE();
    }

    for (int i = 1; i <= parameters_count; i++) {
        CODE("WRITE LF@$"); CODE_INT(i); CODE_NEWLINE();
    }

    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function int2float.
 *
 * @brief Converts int into float.
 */
void int2float()
{
    CODE_COMMENT("Built-in function int2float");
    CODE_LABEL("$int2float");
    CODE_FRAME();
    CODE_NL("INT2FLOATS");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function float2int.
 *
 * @brief Converts float into int.
 */
void float2int()
{
    CODE_COMMENT("Built-in function float2int");
    CODE_LABEL("$float2int");
    CODE_FRAME();
    CODE_NL("FLOAT2INTS");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function len.
 *
 * @brief Get the length of a string.
 */
void len()
{
    CODE_COMMENT("Built-in function len");
    CODE_LABEL("$len");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@s");
    CODE_NL("DEFVAR LF@str_len");
    CODE_NL("POPS LF@s");
    CODE_NL("STRLEN LF@str_len LF@s");
	CODE_NL("PUSHS LF@str_len");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function substr.
 *
 * @brief Get the sub string of a string.
 */
void substr()
{
    CODE_COMMENT("Built-in function substr");
    CODE_LABEL("$substr");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@s");
    CODE_NL("DEFVAR LF@i");
    CODE_NL("DEFVAR LF@n");
    CODE_NL("DEFVAR LF@char");
    CODE_NL("DEFVAR LF@str_len");
    CODE_NL("DEFVAR LF@bool");
    CODE_NL("DEFVAR LF@ret_str");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("POPS LF@n");
    CODE_NL("POPS LF@i");
    CODE_NL("POPS LF@s");
    CODE_NL("MOVE LF@ret_str string@");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("STRLEN LF@str_len LF@s");
    // Check if 0 < i < len(s)
    CODE_NL("LT LF@bool LF@i int@-1");
    CODE_NL("JUMPIFEQ $$substr_err LF@bool bool@true");
    CODE_NL("GT LF@bool LF@i LF@str_len");
    CODE_NL("JUMPIFEQ $$substr_err LF@bool bool@true");
    // Check if n < 0
    CODE_NL("LT LF@bool LF@n int@0");
    CODE_NL("JUMPIFEQ $$substr_err LF@bool bool@true");
    // Check if n > len(s)-i
    CODE_NL("SUB LF@str_len LF@str_len LF@i");
    CODE_NL("GT LF@bool LF@n LF@str_len");
    CODE_NL("JUMPIFEQ $$substr_err LF@bool bool@true");

    CODE_LABEL("$substr_loop");
    CODE_NL("JUMPIFEQ $$substr_end LF@n int@0");
    CODE_NL("SUB LF@n LF@n int@1");
    CODE_NL("GETCHAR LF@char LF@s LF@i");
    CODE_NL("CONCAT LF@ret_str LF@ret_str LF@char");
    CODE_NL("ADD LF@i LF@i int@1");
    CODE_NL("JUMP $$substr_loop");

    CODE_LABEL("$substr_err");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$substr_end");
    CODE_NL("PUSHS LF@ret_str");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function ord.
 *
 * @brief Get ASCII value of char from string on selected position.
 */
void ord()
{
    CODE_COMMENT("Built-in function ord");
    CODE_LABEL("$ord");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@s");
    CODE_NL("DEFVAR LF@i");
    CODE_NL("DEFVAR LF@ret_ascii");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("DEFVAR LF@str_len");
    CODE_NL("DEFVAR LF@bool");
    CODE_NL("POPS LF@i");
    CODE_NL("POPS LF@s");
    CODE_NL("MOVE LF@ret_ascii int@0");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("MOVE LF@str_len int@0");
    CODE_NL("MOVE LF@bool bool@false");

    CODE_NL("STRLEN LF@str_len LF@s");
    CODE_NL("SUB LF@str_len LF@str_len int@1");
    // Check if 0 < i < len(s) - 1
    CODE_NL("LT LF@bool LF@i int@0");
    CODE_NL("JUMPIFEQ $$ord_err LF@bool bool@true");
    CODE_NL("GT LF@bool LF@i LF@str_len");
    CODE_NL("JUMPIFEQ $$ord_err LF@bool bool@true");

    CODE_NL("STRI2INT LF@ret_ascii LF@s LF@i");
    CODE_NL("JUMP $$ord_end");

    CODE_LABEL("$ord_err");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$ord_end");
    CODE_NL("PUSHS LF@ret_ascii");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}

/**
 * Built-in function chr.
 *
 * @brief Get ASCII symbol based on integer value.
 */
void chr()
{
    CODE_COMMENT("Built-in function chr");
    CODE_LABEL("$chr");
    CODE_FRAME();
    CODE_NL("DEFVAR LF@i");
    CODE_NL("DEFVAR LF@ret_str");
    CODE_NL("DEFVAR LF@ret_state");
    CODE_NL("DEFVAR LF@bool");
    CODE_NL("POPS LF@i");
    CODE_NL("MOVE LF@ret_str string@");
    CODE_NL("MOVE LF@ret_state int@0");
    CODE_NL("MOVE LF@bool bool@false");
    // Check if 0 < i < 255
    CODE_NL("LT LF@bool LF@i int@0");
    CODE_NL("JUMPIFEQ $$chr_err LF@bool bool@true");
    CODE_NL("GT LF@bool LF@i int@255");
    CODE_NL("JUMPIFEQ $$chr_err LF@bool bool@true");

    CODE_NL("INT2CHAR LF@ret_str LF@i");
    CODE_NL("JUMP $$chr_end");

    CODE_LABEL("$chr_err");
    CODE_NL("MOVE LF@ret_state int@1");

    CODE_LABEL("$chr_end");
    CODE_NL("PUSHS LF@ret_str");
    CODE_NL("PUSHS LF@ret_state");
    CODE_NL("POPFRAME");
    CODE_NL("RETURN");
    CODE_NEWLINE();
}