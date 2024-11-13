#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"

enum TreeErrorAkin ReadTreeDataBase (node_t* const node, char** const input_buffer)
{
    ASSERT (node         != NULL, "Invalid argument node = %p\n", node);
    ASSERT (input_buffer != NULL, "Invalid argument input_buffer = %p\n", input_buffer);

    if (sscanf (*input_buffer, "{ %[^\n]\n", node->data) == 0)
    {
        return kCantReadAnswerAddElem;
    }

    LOG (DEBUG, "Element = \"%s\"\n"
                "Input buffer = %p\n",
                node->data, *input_buffer);

    char control_symbol = '\0';

    *input_buffer = strchr (*input_buffer, '\n');

    *input_buffer += skip_space_symbols (*input_buffer);

    sscanf (*input_buffer, "%c", &control_symbol);

    LOG (DEBUG, "Control symbol = \"%c\" (%d)\n", control_symbol, '\t');

    if (control_symbol != '}')
    {
        if (PushNodeAkin (node, "", false) != kDoneTreeAkin)
        {
            return kCantReadDataBaseAkin;
        }
        if (ReadTreeDataBase (node->left, input_buffer) != kDoneTreeAkin)
        {
            return kCantReadDataBaseAkin;
        }

        if (PushNodeAkin (node, "", true) != kDoneTreeAkin)
        {
            return kCantReadDataBaseAkin;
        }
        if (ReadTreeDataBase (node->right, input_buffer) != kDoneTreeAkin)
        {
            return kCantReadDataBaseAkin;
        }
    }

    *input_buffer += skip_space_symbols (*input_buffer + 1) + 1;

    return kDoneTreeAkin;
}

enum TreeErrorAkin WriteTreeDataBase (const node_t* const node, const size_t depth, FILE* const output)
{
    ASSERT (node   != NULL, "Invalid argument node = %p\n", node);
    ASSERT (output != NULL, "Invalid argument output = %p\n", output);

    for (size_t counter = 0; counter < depth; counter++)
    {
        fputc ('\t', output);
    }

    fprintf (output, "{ %s\n", node->data);

    if ((node->left != NULL) && (node->right != NULL))
    {
        WriteTreeDataBase (node->left, depth + 1, output);
        WriteTreeDataBase (node->right, depth + 1, output);
    }

    for (size_t counter = 0; counter < depth; counter++)
    {
        fputc ('\t', output);
    }

    fprintf (output, "}\n");

    return kDoneTreeAkin;
}

const char* EnumToStr (const enum TreeErrorAkin error)
{
    #define CASE_ENUM_(error)   \
    case error:                 \
    {                           \
        return #error;          \
    }

    switch (error)
    {
        CASE_ENUM_ (kDoneTreeAkin);

        CASE_ENUM_ (kCantCtorTreeAkin);

        CASE_ENUM_ (kCantReadNameOfFileAkin);

        CASE_ENUM_ (kCantCallocInputBuffer);
        CASE_ENUM_ (kCantOpenDataBase);
        CASE_ENUM_ (kCantReadDataBaseAkin);

        CASE_ENUM_ (kRunTimeErrorAkin);

        CASE_ENUM_ (kCantPushTreeAkin);

        CASE_ENUM_ (kCantDumpTreeAkin);

        CASE_ENUM_ (kCantReadAnswerAddElem);
        CASE_ENUM_ (kCantReadAnswerReplay);

        CASE_ENUM_ (kCantReadSubjectAkin);
        CASE_ENUM_ (kDidntFoundSubject);
        CASE_ENUM_ (kCantMakeDefinition);

        CASE_ENUM_ (kCantCtorStackDefinition);
        CASE_ENUM_ (kCantDtorStackDefinition);

        CASE_ENUM_ (kCantCtorStackComparison);
        CASE_ENUM_ (kCantDtorStackComparison);

        CASE_ENUM_ (kInvalidFlagAkin);

        default:
            return "Invalid enum element";
    }

    #undef CASE_ENUM_
}
