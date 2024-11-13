#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/helpful.h"
#include "../../Stack/stack.h"

enum TreeErrorAkin TreeCtorAkin (node_t* const root, const tree_elem_t data)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | data = %p |\n",
                root, data);

    strcpy (root->data, data);

    root->left = root->right = NULL;

    strcpy (root->answer, "");

    return kDoneTreeAkin;
}

enum TreeErrorAkin TreeDtorAkin (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p |\n",
                root);

    if (root->left != NULL)
    {
        TreeDtorAkin (root->left);
        FREE_AND_NULL (root->left);
    }

    if (root->right != NULL)
    {
        TreeDtorAkin (root->right);
        FREE_AND_NULL (root->right);
    }

    return kDoneTreeAkin;
}

enum TreeErrorAkin PushNodeAkin (node_t* const root, const tree_elem_t element, const bool right)
{
    ASSERT (root    != NULL, "Invalid argument root = %p\n", root);
    ASSERT (element != NULL, "Invalid argument element = %p\n", element);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | element = \"%s\" |\n",
                root, element);

    node_t* new_node = (node_t*) calloc (1, sizeof (node_t));
    if (new_node == NULL)
    {
        return kCantPushTreeAkin;
    }

    new_node->left = new_node->right = NULL;

    strcpy (new_node->data, element);

    if (right)
    {
        root->right = new_node;
    }
    else
    {
        root->left = new_node;
    }

    return kDoneTreeAkin;
}
