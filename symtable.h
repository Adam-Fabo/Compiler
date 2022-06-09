/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Symbol table interface.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include <stdint.h>
#include "param_buffer.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H

/**
 * @struct Node structure.
 */
typedef struct Node {
    Type type;

    // Symbol is a Identifier
    char *id_name;
    ID_Type id_type;

    // Symbol is a Function
    Func function;

    struct Node *left;
    struct Node *right;
} *Node;

/**
 * @typedef Symtable Pointer on Nodes
 */
typedef Node Symtable;

/**
 * Initialize binary tree.
 *
 * @return NULL
 */
Symtable bst_init();

/**
 * Destroy all nodes inside binary tree.
 *
 * @param root Root node
 */
void bst_destroy(Node *root);

/**
 * Add identifier into binary tree.
 *
 * @param root Root node
 * @param id_name Identifier name
 * @param id_type Type of identifier
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int bst_add_id(Node *root, char *id_name, ID_Type id_type);

/**
 * Add function into binary tree.
 *
 * @param root Root node
 * @param function Function to add into binary tree
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int bst_add_func(Node *root, Func function);

/**
 * Find identifier using binary tree.
 *
 * @param root Root node
 * @param name Name of identifier/function
 * @param type Type of symbol
 *
 * @return Node we found or NULL if not found.
 */
Node bst_find(Node root, char *name, Type type);


void print_preorder_id(Node n, char * prefix, int number);
void print_postorder_id(Node n, char * prefix, int number);


#endif