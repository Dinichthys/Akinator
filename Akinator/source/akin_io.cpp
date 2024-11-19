#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"
#include "../../My_lib/helpful.h"

static enum TreeErrorAkin CreateTreeFromDataBaseFlag (node_t* const root);
static enum TreeErrorAkin WriteTreeDataBase          (const node_t* const root);
static enum TreeErrorAkin ReadTreeDataBase           (node_t* const node, char** const input_buffer);
static enum TreeErrorAkin WriteTreeDataBaseInFile    (const node_t* const node,
                                                      const size_t depth, FILE* const output);

enum TreeErrorAkin CreateFlag (node_t* const root)
{
    ASSERT (root != NULL, "invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    while (result == kDoneTreeAkin)
    {
        result = CreateTreeFromDataBaseFlag (root);

        if (result != kDoneTreeAkin)
        {
            if ((result == kCantOpenDataBase) || (result == kCantReadAnswerAddElem))
            {
                fprintf (stdout, "\nНевозможно открыть файл. Попробуй снова.\n");

                result = kDoneTreeAkin;
                continue;
            }

            if ((result == kCantReadDataBaseAkin) || (result == kCantCallocInputBuffer))
            {
                fprintf (stdout, "\nНевозможно считать из файла. Возможно, он слишком большой.\n\n"
                                    "Выход из режима\n\n");

                break;
            }

            return result;
        }

        fprintf (stdout, "Готово!\n");

        break;
    }

    return result;
}

enum TreeErrorAkin WriteFlag (const node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    while (result == kDoneTreeAkin)
    {
        result = WriteTreeDataBase (root);

        if (result != kDoneTreeAkin)
        {
            if (result == kCantReadNameOfFileAkin)
            {
                fprintf (stdout, "\nНе удалось считать имя файла. Попробуй снова\n");

                result = kDoneTreeAkin;
                continue;
            }

            if (result == kCantOpenDataBase)
            {
                fprintf (stdout, "\nНе удалось открыть файл. Попробуй снова.\n");

                result = kDoneTreeAkin;
                continue;
            }

            return result;
        }

        fprintf (stdout, "Готово!\n");

        break;
    }

    return result;
}

static enum TreeErrorAkin CreateTreeFromDataBaseFlag (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    FILE* input_file = NULL;
    char file_name [kLenFileName] = "";

    fprintf (stdout, "А теперь введи имя файла, где находится твоя база данных\n");
    if (fgets (file_name, kLenFileName, stdin) == NULL)
    {
        return kCantReadNameOfFileAkin;
    }

    if (strchr (file_name, '\n') != NULL)
    {
        *(strchr (file_name, '\n')) = '\0';
    }

    FOPEN (input_file, file_name, "r");

    if (input_file == NULL)
    {
        return kCantOpenDataBase;
    }

    size_t size_input = size_of_file (input_file);

    char* input_buffer = (char*) calloc (size_input, sizeof (char));
    if (input_buffer == NULL)
    {
        return kCantCallocInputBuffer;
    }

    if (fread (input_buffer, sizeof (char), size_input, input_file) != size_input)
    {
        FREE_AND_NULL (input_buffer);
        return kCantReadDataBaseAkin;
    }

    char* input_buffer_for_function = input_buffer;

    result = ReadTreeDataBase (root, &input_buffer_for_function);

    FREE_AND_NULL (input_buffer);

    return result;
}

static enum TreeErrorAkin WriteTreeDataBase (const node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    fprintf (stdout, "Тогда напиши имя выходного файла\n");

    enum TreeErrorAkin result = kDoneTreeAkin;

    char file_name [kLenFileName] = {'\0'};
    if (fgets (file_name, kLenFileName, stdin) == NULL)
    {
        return kCantReadNameOfFileAkin;
    }

    if (strchr (file_name, '\n') != NULL)
    {
        *(strchr (file_name, '\n')) = '\0';
    }

    FILE* output = NULL;

    FOPEN (output, file_name, "w");
    if (output == NULL)
    {
        return kCantOpenDataBase;
    }

    result = WriteTreeDataBaseInFile (root, 0, output);

    FCLOSE (output);

    return result;
}

static enum TreeErrorAkin ReadTreeDataBase (node_t* const node, char** const input_buffer)
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

static enum TreeErrorAkin WriteTreeDataBaseInFile (const node_t* const node, const size_t depth, FILE* const output)
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
        WriteTreeDataBaseInFile (node->left, depth + 1, output);
        WriteTreeDataBaseInFile (node->right, depth + 1, output);
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

        CASE_ENUM_ (kCantPrintDefinition);
        CASE_ENUM_ (kCantMakeDefinition);
        CASE_ENUM_ (kCantCtorStackDefinition);
        CASE_ENUM_ (kCantDtorStackDefinition);

        CASE_ENUM_ (kCantPrintComparison);
        CASE_ENUM_ (kCantMakeComparison);
        CASE_ENUM_ (kCantCtorStackComparison);
        CASE_ENUM_ (kCantDtorStackComparison);

        CASE_ENUM_ (kNoAnswerInStack);
        CASE_ENUM_ (kNoDataInStack);

        CASE_ENUM_ (kInvalidFlagAkin);

        default:
            return "Invalid enum element";
    }

    #undef CASE_ENUM_
}
