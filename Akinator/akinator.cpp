#include "akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../My_lib/Assert/my_assert.h"
#include "../My_lib/Logger/logging.h"
#include "../My_lib/My_stdio/my_stdio.h"
#include "../My_lib/helpful.h"

static enum TreeErrorAkin PrintNodeInfoAkin  (const node_t* const node, FILE* const dump_file);
static enum TreeErrorAkin PrintTreeInfoAkin  (const node_t* const node, FILE* const dump_file);
static enum TreeErrorAkin PrintEdgesTreeAkin (const node_t* const root, FILE* const dump_file);
static enum TreeResult PrintResultAkin   (const tree_elem_t result);
static enum TreeResult PrintQuestionAkin (const tree_elem_t result);
static enum TreeErrorAkin AddNewElemAkin (node_t* const node);

enum TreeErrorAkin RunAkinator (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument: root = %p\n", root);

    LOG (DEBUG, "The question is \"%s?\"\n"
                "Yes - \"%p\"\n"
                "No - \"%p\"\n",
                root->data, root->right, root->left);

    enum TreeResult result = kTrueResult;

    if ((root->left == NULL) && (root->right == NULL))
    {
        result = PrintResultAkin (root->data);

        if ((result == kCantFindAnswerForResult) || (result == kInvalAnswer))
        {
            return kRunTimeErrorAkin;
        }

        if (result == kTrueResult)
        {
            return kDoneTreeAkin;
        }

        if (AddNewElemAkin (root) != kDoneTreeAkin)
        {
            return kRunTimeErrorAkin;
        }

        return kDoneTreeAkin;
    }

    result = PrintQuestionAkin (root->data);

    if ((result == kCantFindAnswerForResult) || (result == kInvalAnswer))
    {
        return kRunTimeErrorAkin;
    }

    return (result == kTrueResult) ? RunAkinator (root->right)
                                   : RunAkinator (root->left);
}

enum TreeErrorAkin TreeCtorAkin (node_t* const root, const tree_elem_t data)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | data = %p |\n",
                root, data);

    root->left = root->right = NULL;

    strcpy (root->data, data);

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

enum TreeErrorAkin DumpTreeAkin (const node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument: root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p |\n",
                root);

    static size_t counter_dump = 0;

    FILE* html_dump_file = NULL;
    FOPEN (html_dump_file, "Dump_Files/Dump.html", "a");
    if (html_dump_file == NULL)
    {
        return kCantDumpTreeAkin;
    }

    if (ftell (html_dump_file) == 0)
    {
        fprintf (html_dump_file, "<pre>\n<HR>\n");
    }

    FILE* dump_file = NULL;
    FOPEN (dump_file, "Dump_Files/Dump.dot", "w");
    if (dump_file == NULL)
    {
        FCLOSE (html_dump_file);
        return kCantDumpTreeAkin;
    }

    fprintf (dump_file, "digraph\n{\n"
                        "\tfontname = \"Helvetica,Arial,sans-serif\";\n"
                        "\tnode [fontname = \"Helvetica,Arial,sans-serif\"];\n"
                        "\tgraph [rankdir = \"TB\"];\n"
                        "\tranksep = 1.5;\n\n");

    PrintTreeInfoAkin (root, dump_file);

    PrintEdgesTreeAkin (root, dump_file);

    fputc ('}', dump_file);

    FCLOSE (dump_file);

    counter_dump++;

    char command [kCommandTerminalDumpLen] = "";

    sprintf (command, "dot -Tsvg Dump_Files/Dump.dot -o Dump_Files/Dump_%lu_.svg\n", counter_dump);

    if (system (command) == -1)
    {
        CLOSE_AND_NULL (html_dump_file);
        return kCantDumpTreeAkin;
    }

    fprintf (html_dump_file, "<img src = \"Dump_%lu_.svg\" width = 2450>\n<HR>\n", counter_dump);

    CLOSE_AND_NULL (html_dump_file);

    return kDoneTreeAkin;
}

static enum TreeErrorAkin PrintNodeInfoAkin (const node_t* const node, FILE* const dump_file)
{
    ASSERT (node      != NULL, "Invalid argument node = %p\n", node);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);

    LOG (DEBUG, "Function got arguments:\n"
                "| node = %p | dump_file = %p |\n",
                node, dump_file);

    if ((node->left == NULL) && (node->right == NULL))
    {
        fprintf (dump_file, "\t\"node%p\"\n\t[\n"
                            "\t\tlabel = \""
                            "{ <f0> %s |"
                            "{ <f1> left = %p |"
                            " <f2> right = %p } }\"\n"
                            "\t\tshape = \"record\"\n"
                            "\t];\n\n",
                            node, node->data,
                            node->left, node->right);
    }
    else
    {
        fprintf (dump_file, "\t\"node%p\"\n\t[\n"
                            "\t\tlabel = \""
                            "{ <f0> Оно \t%s? |"
                            "{ <f1> left = %p |"
                            " <f2> right = %p } }\"\n"
                            "\t\tshape = \"record\"\n"
                            "\t];\n\n",
                            node, node->data,
                            node->left, node->right);
    }

    return kDoneTreeAkin;
}

static enum TreeErrorAkin PrintTreeInfoAkin (const node_t* const root, FILE* const dump_file)
{
    ASSERT (root      != NULL, "Invalid argument root = %p\n", root);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | dump_file = %p |\n",
                root, dump_file);

    PrintNodeInfoAkin (root, dump_file);

    if (root->left != NULL)
    {
        PrintTreeInfoAkin (root->left, dump_file);
    }

    if (root->right != NULL)
    {
        PrintTreeInfoAkin (root->right, dump_file);
    }

    return kDoneTreeAkin;
}

static enum TreeErrorAkin PrintEdgesTreeAkin (const node_t* const root, FILE* const dump_file)
{
    ASSERT (root      != NULL, "Invalid argument root = %p\n", root);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | dump_file = %p |\n",
                root, dump_file);

    if (root->left != NULL)
    {
        fprintf (dump_file, "\t\"node%p\":f1 -> \"node%p\":f0 "
                            "[color = \"black\" label = \"Нет\"];\n\n",
                            root, root->left);
        PrintEdgesTreeAkin (root->left, dump_file);
    }

    if (root->right != NULL)
    {
        fprintf (dump_file, "\t\"node%p\":f2 -> \"node%p\":f0 "
                            "[color = \"black\" label = \"Да\"];\n\n",
                            root, root->right);
        PrintEdgesTreeAkin (root->right, dump_file);
    }

    return kDoneTreeAkin;
}

static enum TreeResult PrintResultAkin (const tree_elem_t result)
{
    ASSERT (result != NULL, "Invalid argument result = %p\n", result);

    fprintf (stdout, "Вы загадывали %s?\n", result);

    char answer [kLenAnswer] = {0};

    if (fgets (answer, kLenAnswer, stdin) == NULL)
    {
        return kCantFindAnswerForResult;
    }

    LOG (DEBUG, "Get answer \"%s\"\n", answer);

    if (strchr (answer, '\n') != NULL)
    {
        *(strchr (answer, '\n')) = '\0';
    }

    LOG (DEBUG, "Get answer \"%s\"\n", answer);

    if ((my_strcmp (answer, "Да") == 0)
        || (my_strcmp (answer, "да") == 0)
        || (my_strcmp (answer, "дА") == 0)
        || (my_strcmp (answer, "ДА") == 0))
    {
        fprintf (stdout, "ХА, так и знал\n");
        return kTrueResult;
    }

    if ((my_strcmp (answer, "Нет") == 0)
        || (my_strcmp (answer, "НеТ") == 0)
        || (my_strcmp (answer, "НЕт") == 0)
        || (my_strcmp (answer, "НЕТ") == 0)
        || (my_strcmp (answer, "нет") == 0)
        || (my_strcmp (answer, "неТ") == 0)
        || (my_strcmp (answer, "нЕт") == 0)
        || (my_strcmp (answer, "нЕТ") == 0))
    {
        return kFalseResult;
    }

    return kInvalAnswer;
}

static enum TreeResult PrintQuestionAkin (const tree_elem_t result)
{
    ASSERT (result != NULL, "Invalid argument result = %p\n", result);

    fprintf (stdout, "Оно %s?\n", result);

    char answer [kLenAnswer] = {0};

    if (fgets (answer, kLenAnswer, stdin) == NULL)
    {
        return kCantFindAnswerForResult;
    }

    if (strchr (answer, '\n') != NULL)
    {
        *(strchr (answer, '\n')) = '\0';
    }

    LOG (DEBUG, "Get answer \"%s\"\n", answer);

    if (my_strcmp (answer, "Да") == 0)
    {
        return kTrueResult;
    }

    if (my_strcmp (answer, "Нет") == 0)
    {
        return kFalseResult;
    }

    return kInvalAnswer;
}

static enum TreeErrorAkin AddNewElemAkin (node_t* const node)
{
    ASSERT (node != NULL, "Invalid argument node = %p\n", node);

    tree_elem_t element = "";
    tree_elem_t question = "";

    fprintf (stdout, "А что это было?\n");

    if (fgets (element, kLenElement, stdin) == NULL)
    {
        return kCantReadAnswer;
    }

    if (strchr (element, '\n') != NULL)
    {
        *(strchr (element, '\n')) = '\0';
    }

    fprintf (stdout, "А бля реально тупанул че-то\n");

    fprintf (stdout, "А чем %s отличается от %s\n", element, node->data);

    if (fgets (question, kLenElement, stdin) == NULL)
    {
        return kCantReadAnswer;
    }

    if (strchr (question, '\n') != NULL)
    {
        *(strchr (question, '\n')) = '\0';
    }

    PushNodeAkin (node, element, true);
    PushNodeAkin (node, node->data, false);

    strcpy (node->data, question);

    return kDoneTreeAkin;
}
