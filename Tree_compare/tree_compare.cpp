#include "tree_compare.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../My_lib/Assert/my_assert.h"
#include "../My_lib/Logger/logging.h"
#include "../My_lib/My_stdio/my_stdio.h"
#include "../My_lib/helpful.h"

static enum TreeError PrintNodeInfo  (const node_t* const node, FILE* const dump_file,
                                      void PrintData (void* const data, FILE* const dump_file));
static enum TreeError PrintTreeInfo  (const node_t* const node, FILE* const dump_file,
                                      void PrintData (void* const data, FILE* const dump_file));
static enum TreeError PrintEdgesTree (const node_t* const root, FILE* const dump_file);

enum TreeError TreeCtor (node_t* const root, void* const data, const size_t size_elem)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | data = %p | size_elem = %lu |\n",
                root, data, size_elem);

    root->more = NULL;
    root->less_or_equal = NULL;

    root->data = calloc (1, size_elem);
    if (data == NULL)
    {
        return kCantCtorTree;
    }

    memcpy (root->data, data, size_elem);

    root->size = size_elem;

    return kDoneTree;
}

enum TreeError TreeDtor (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p |\n",
                root);

    if (root->less_or_equal != NULL)
    {
        TreeDtor (root->less_or_equal);
        FREE_AND_NULL (root->less_or_equal);
    }

    if (root->more != NULL)
    {
        TreeDtor (root->more);
        FREE_AND_NULL (root->more);
    }

    if ((root->more == NULL) && (root->less_or_equal == NULL))
    {
        FREE_AND_NULL (root->data);
    }

    return kDoneTree;
}

enum TreeError PushNode (node_t* const root, void* const element, int Compare (void* first, void* second))
{
    ASSERT (root    != NULL, "Invalid argument root = %p\n", root);
    ASSERT (element != NULL, "Invalid argument element = %p\n", element);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | element = %p | Compare = %p |\n",
                root, element, Compare);

    const bool more = (Compare (element, root->data) > 0);

    if (((root->less_or_equal == NULL) && (!more)) || ((root->more == NULL) && (more)))
    {
        node_t* new_node = (node_t*) calloc (1, sizeof (node_t));
        if (new_node == NULL)
        {
            return kCantPushTree;
        }

        new_node->data = calloc (1, root->size);
        if (new_node->data == NULL)
        {
            FREE_AND_NULL (new_node);
            return kCantPushTree;
        }

        new_node->more = new_node->less_or_equal = NULL;
        new_node->size = root->size;

        memcpy (new_node->data, element, new_node->size);

        if (more)
        {
            root->more = new_node;
        }
        else
        {
            root->less_or_equal = new_node;
        }

        return kDoneTree;
    }

    enum TreeError result = kDoneTree;

    if (more)
    {
        result = PushNode (root->more, element, Compare);
    }
    else
    {
        result = PushNode (root->less_or_equal, element, Compare);
    }

    return result;
}

enum TreeError DumpTree (const node_t* const root, void PrintData (void* const data, FILE* const dump_file))
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
        return kCantDumpTree;
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
        return kCantDumpTree;
    }

    fprintf (dump_file, "digraph\n{\n"
                        "\tfontname = \"Helvetica,Arial,sans-serif\";\n"
                        "\tnode [fontname = \"Helvetica,Arial,sans-serif\"];\n"
                        "\tgraph [rankdir = \"TB\"];\n"
                        "\tranksep = 1.5;\n\n");

    PrintTreeInfo (root, dump_file, PrintData);

    PrintEdgesTree (root, dump_file);

    fputc ('}', dump_file);

    FCLOSE (dump_file);

    counter_dump++;

    char command [kCommandTerminalDumpLen] = "";

    sprintf (command, "dot -Tsvg Dump_Files/Dump.dot -o Dump_Files/Dump_%lu_.svg\n", counter_dump);

    if (system (command) == -1)
    {
        CLOSE_AND_NULL (html_dump_file);
        return kCantDumpTree;
    }

    fprintf (html_dump_file, "<img src = \"Dump_%lu_.svg\" width = 2450>\n<HR>\n", counter_dump);

    CLOSE_AND_NULL (html_dump_file);

    return kDoneTree;
}

static enum TreeError PrintNodeInfo (const node_t* const node, FILE* const dump_file,
                                     void PrintData (void* const data, FILE* const dump_file))
{
    ASSERT (node      != NULL, "Invalid argument node = %p\n", node);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);
    ASSERT (PrintData != NULL, "Invalid argument PrintData = %p\n", PrintData);

    LOG (DEBUG, "Function got arguments:\n"
                "| node = %p | dump_file = %p | PrintData = %p |\n",
                node, dump_file, PrintData);

    fprintf (dump_file, "\t\"node%p\"\n\t[\n"
                        "\t\tlabel = \""
                        "{ <f0> data = ",
                        node);

    PrintData (node->data, dump_file);

    fprintf (dump_file, "| { <f1> less_or_equal = %p| "
                        "<f2> more = %p } }\"\n"
                        "\t\tshape = \"record\"\n"
                        "\t];\n\n",
                        node->less_or_equal, node->more);

    return kDoneTree;
}

static enum TreeError PrintTreeInfo (const node_t* const root, FILE* const dump_file,
                                     void PrintData (void* const data, FILE* const dump_file))
{
    ASSERT (root      != NULL, "Invalid argument root = %p\n", root);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);
    ASSERT (PrintData != NULL, "Invalid argument PrintData = %p\n", PrintData);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | dump_file = %p | PrintData = %p |\n",
                root, dump_file, PrintData);

    PrintNodeInfo (root, dump_file, PrintData);

    if (root->less_or_equal != NULL)
    {
        PrintTreeInfo (root->less_or_equal, dump_file, PrintData);
    }

    if (root->more != NULL)
    {
        PrintTreeInfo (root->more, dump_file, PrintData);
    }

    return kDoneTree;
}

static enum TreeError PrintEdgesTree (const node_t* const root, FILE* const dump_file)
{
    ASSERT (root      != NULL, "Invalid argument root = %p\n", root);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);

    LOG (DEBUG, "Function got arguments:\n"
                "| root = %p | dump_file = %p |\n",
                root, dump_file);

    if (root->less_or_equal != NULL)
    {
        fprintf (dump_file, "\t\"node%p\":f1 -> \"node%p\":f0 "
                            "[color = \"black\"];\n\n",
                            root, root->less_or_equal);
        PrintEdgesTree (root->less_or_equal, dump_file);
    }

    if (root->more != NULL)
    {
        fprintf (dump_file, "\t\"node%p\":f2 -> \"node%p\":f0 "
                            "[color = \"black\"];\n\n",
                            root, root->more);
        PrintEdgesTree (root->more, dump_file);
    }

    return kDoneTree;
}

