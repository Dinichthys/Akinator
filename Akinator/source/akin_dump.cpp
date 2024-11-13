#include "../akinator.h"

#include <stdio.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"

static enum TreeErrorAkin PrintNodeInfoAkin  (const node_t* const node, FILE* const dump_file);
static enum TreeErrorAkin PrintTreeInfoAkin  (const node_t* const node, FILE* const dump_file);
static enum TreeErrorAkin PrintEdgesTreeAkin (const node_t* const root, FILE* const dump_file);

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
        FCLOSE (html_dump_file);
        return kCantDumpTreeAkin;
    }

    fprintf (html_dump_file, "<img src = \"Dump_%lu_.svg\" width = 2450>\n<HR>\n", counter_dump);

    FCLOSE (html_dump_file);

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
                            "{ <f0> Оно %s? |"
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
