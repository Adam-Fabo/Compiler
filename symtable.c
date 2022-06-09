/**
 * Compiler implementation of imperative language IFJ20.
 *
 * @brief Symbol table implementation using Binary Tree.
 * @author Roman Országh <xorsza01(at)stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

/**
 * Initialize binary tree.
 *
 * @return NULL
 */
Symtable bst_init()
{
    return NULL;
}

/**
 * Destroy all nodes inside binary tree.
 *
 * @param root Root node
 */
void bst_destroy(Node *root)
{
    // Stop recursion if node is NULL
    if (*root == NULL) return;

    // Recursive destroy all nodes
    bst_destroy(&(*root)->left);
    bst_destroy(&(*root)->right);

    if((*root)->type == TYPE_FUNC)
    	delete_func((*root)->function);

    free(*root);
    *root = NULL;
}

/**
 * Add identifier into binary tree.
 *
 * @param root Root node
 * @param id_name Identifier name
 * @param id_type Type of identifier
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int bst_add_id(Node *root, char *id_name, ID_Type id_type)
{
    // If root is empty create node
    if (*root == NULL) {
        // Allocate new node
        *root = (Node)malloc(sizeof(struct Node));

        // Check for allocation
        if (!*root) return MALLOC_ERROR;

        // Set data
        (*root)->type = TYPE_ID;
        (*root)->id_name = id_name;
        (*root)->id_type = id_type;
        (*root)->left = (*root)->right = NULL;

        return NO_ERROR;
    }

    // Rewrite if key exists
    if (strcmp((*root)->id_name, id_name) == 0) {
        (*root)->id_name = id_name;
        return NO_ERROR;
    }

    // Key is smaller than root's key go left
    if (strcmp((*root)->id_name, id_name) > 0) {
        return bst_add_id(&(*root)->left, id_name, id_type);
    }

    // Key is greater than root's key go right
    return bst_add_id(&(*root)->right, id_name, id_type);
}

/**
 * Add function into binary tree.
 *
 * @param root Root node
 * @param function Function to add into binary tree
 *
 * @return MALLOC_ERROR if allocation fails, otherwise NO_ERROR
 */
int bst_add_func(Node *root, Func function)
{
    // If root is empty create node
    if (*root == NULL) {
        // Allocate new node
        *root = (Node)malloc(sizeof(struct Node));

        // Check for allocation
        if (!*root) return MALLOC_ERROR;

        // Set data
        (*root)->type = TYPE_FUNC;
        (*root)->function = function;
        (*root)->left = (*root)->right = NULL;

        return NO_ERROR;
    }

    // Rewrite if key exists
    if (strcmp((*root)->function->name, function->name) == 0) {
        (*root)->function->name = function->name;
        return NO_ERROR;
    }

    // Key is smaller than root's key go left
    if (strcmp((*root)->function->name, function->name) > 0) {
        return bst_add_func(&(*root)->left, function);
    }

    // Key is greater than root's key go right
    return bst_add_func(&(*root)->right, function);
}


/**
 * Find identifier using binary tree.
 *
 * @param root Root node
 * @param name Name of identifier/function
 * @param type Type of symbol
 *
 * @return Node we found or NULL if not found.
 */
Node bst_find(Node root, char *name, Type type)
{
    // Node is NULL
    if (root == NULL) return NULL;

    // Set default finding name as identifier
    char *id = root->id_name;

    // Find name of function
    if (type == TYPE_FUNC) id = root->function->name;

    // Key found
    if (strcmp(id, name) == 0) {
        return root;
    }

    // Key is smaller than root's key go left
    if (strcmp(id, name) > 0) {
        return bst_find(root->left, name, type);
    }

    // Key is greater than root's key go right
    return bst_find(root->right, name, type);
}

void print_preorder_id(Node n, char * prefix, int number){

	//printf("idem printovat\n");
	if(n != NULL){
		//printf("69\n");
		printf("%s$%d%s\n",prefix,number,n->id_name);


		print_preorder_id(n->left,prefix,number);
		print_preorder_id(n->right,prefix,number);


	}
	//printf("vyprintoval som\n");

}


void print_postorder_id(Node n, char * prefix, int number){
	if(n != NULL){

		print_postorder_id(n->right,prefix,number);
		print_postorder_id(n->left,prefix,number);

		printf("%s$%d%s\n",prefix,number,n->id_name);

	}
}