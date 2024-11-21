#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"

static enum TreeResult    PrintResultAkin    (const tree_elem_t result);
static enum TreeResult    PrintQuestionAkin  (const tree_elem_t result);
static enum TreeErrorAkin AddNewElemAkin     (node_t* const node);

enum TreeErrorAkin RunAkinatorFlag (node_t* root)
{
    ASSERT (root != NULL, "Invalid argument: root = %p\n", root);

    LOG (DEBUG, "The question is \"%s?\"\n"
                "Yes - \"%p\"\n"
                "No - \"%p\"\n",
                root->data, root->right, root->left);

    enum TreeResult result = kTrueResult;

    while ((root->left != NULL) && (root->right != NULL))
    {
        result = PrintQuestionAkin (root->data);

        if ((result == kCantFindAnswerForResult) || (result == kInvalAnswer))
        {
            return kRunTimeErrorAkin;
        }

        root = (result == kTrueResult) ? root->right
                                       : root->left;
    }

    result = PrintResultAkin (root->data);

    if ((result == kCantFindAnswerForResult) || (result == kInvalAnswer))
    {
        return kRunTimeErrorAkin;
    }

    if (result == kTrueResult)
    {
        fprintf (stdout, "ХА! Так и знал, что у меня всё получится😎)))\n\n");

        return kDoneTreeAkin;
    }

    if (AddNewElemAkin (root) != kDoneTreeAkin)
    {
        return kRunTimeErrorAkin;
    }

    return kDoneTreeAkin;
}

static enum TreeResult PrintResultAkin (const tree_elem_t result)
{
    ASSERT (result != NULL, "Invalid argument result = %p\n", result);

    fprintf (stdout, "Вы загадывали %s?\n", result);

    return GetAnswer ();
}

static enum TreeResult PrintQuestionAkin (const tree_elem_t result)
{
    ASSERT (result != NULL, "Invalid argument result = %p\n", result);

    fprintf (stdout, "Оно %s?\n", result);

    return GetAnswer ();
}

static enum TreeErrorAkin AddNewElemAkin (node_t* const node)
{
    ASSERT (node != NULL, "Invalid argument node = %p\n", node);

    tree_elem_t element = "";
    tree_elem_t question = "";

    fprintf (stdout, "А что это было?\n");

    if (fgets (element, kLenElement, stdin) == NULL)
    {
        return kCantReadAnswerAddElem;
    }

    if (strchr (element, '\n') != NULL)
    {
        *(strchr (element, '\n')) = '\0';
    }

    fprintf (stdout, "А, точно... Я тупанул че-то\n");

    fprintf (stdout, "А чем %s отличается от %s?\n", element, node->data);

    if (fgets (question, kLenElement, stdin) == NULL)
    {
        return kCantReadAnswerAddElem;
    }

    if (strchr (question, '\n') != NULL)
    {
        *(strchr (question, '\n')) = '\0';
    }

    char temp [strlen ("не ") + 1] = "";

    const char* no_ = "не ";

    memcpy (temp, question, (size_t) (strchr (no_, '\0') - no_));

    LOG (DEBUG, "temp = %s\n", temp);

    if (strcmp (temp, "не ") == 0)
    {
        PushNodeAkin (node, element, false);
        PushNodeAkin (node, node->data, true);
        strcpy (node->data, question + strlen ("не "));
    }
    else
    {
        PushNodeAkin (node, element, true);
        PushNodeAkin (node, node->data, false);
        strcpy (node->data, question);
    }

    fputc ('\n', stdout);

    return kDoneTreeAkin;
}
