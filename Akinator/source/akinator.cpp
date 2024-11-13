#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"
#include "../../My_lib/helpful.h"

static enum TreeErrorAkin ReplayAkinator     (node_t* const root);

enum TreeErrorAkin ParseFlags (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    char flag [kLenFlag] = "";

    enum TreeErrorAkin result = kDoneTreeAkin;

    fprintf (stdout, "Это Великий и Ужасный Акинатор!\n"
                     "Для продолжения напиши:\n"
                     " \"Отгадывать\", чтобы Акинатор отгадал твоё СуЩеСтВо\n"
                     " \"Создать\", чтобы создать дерево вариантов ответов по базе данных\n"
                     " \"Определение\", чтобы дать определение вашего сУщЕсТвА\n"
                     " \"Сравнение\", чтобы сравнить два ваших СУЩЕСТВА\n"
                     " \"Выход\", чтобы выйти из программы\n");

    while (true)
    {
        if (fscanf (stdin, "%[^ ^\n^\t^\r]", flag) == EOF)
        {
            return kInvalidFlagAkin;
        }

        fgetc (stdin);

        LOG (DEBUG, "Read flag = \"%s\"", flag);

        if (strcmp (flag, "Отгадывать") == 0)
        {
            result = RunAkinator (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Создать") == 0)
        {
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

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            fprintf (stdout, "Готово!\n");

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Определение") == 0)
        {
            fprintf (stdout, "А теперь введи имя существа, определение которого ты хочешь получить\n");

            stack_t stack = 0;

            if (STACK_INIT_SHORT (stack, kDepthStack) == CANT_CREATE)
            {
                return kCantCtorStackDefinition;
            }

            tree_elem_t subject = "";

            if (fgets (subject, kLenElement, stdin) == NULL)
            {
                return kCantReadSubjectAkin;
            }

            if (strchr (subject, '\n') != NULL)
            {
                *(strchr (subject, '\n')) = '\0';
            }

            LOG (DEBUG, "Subject = \"%s\"\n", subject);

            result = MakeDefinition (root, subject, stack);

            if (STACK_DTOR_SHORT (stack) == CANT_DESTROY)
            {
                return kCantDtorStackDefinition;
            }

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Сравнение") == 0)
        {
            fprintf (stdout, "А теперь введи имена двух существ, определения которых ты хочешь сравнить\n");

            stack_t stack_first = 0;
            stack_t stack_second = 0;

            if (STACK_INIT_SHORT (stack_first, kDepthStack) == CANT_CREATE)
            {
                return kCantCtorStackComparison;
            }

            if (STACK_INIT_SHORT (stack_second, kDepthStack) == CANT_CREATE)
            {
                return kCantCtorStackComparison;
            }

            tree_elem_t subject_first = "";

            if (fgets (subject_first, kLenElement, stdin) == NULL)
            {
                return kCantReadSubjectAkin;
            }

            if (strchr (subject_first, '\n') != NULL)
            {
                *(strchr (subject_first, '\n')) = '\0';
            }

            LOG (DEBUG, "Subject = \"%s\"\n", subject_first);

            result = MakeDefinition (root, subject_first, stack_first);
            if (result != kDoneTreeAkin)
            {
                return result;
            }

            tree_elem_t subject_second = "";

            if (fgets (subject_second, kLenElement, stdin) == NULL)
            {
                return kCantReadSubjectAkin;
            }

            if (strchr (subject_second, '\n') != NULL)
            {
                *(strchr (subject_second, '\n')) = '\0';
            }

            LOG (DEBUG, "Subject = \"%s\"\n", subject_second);

            result = MakeDefinition (root, subject_second, stack_second);

            if (STACK_DTOR_SHORT (stack_first) == CANT_DESTROY)
            {
                return kCantDtorStackComparison;
            }

            if (STACK_DTOR_SHORT (stack_second) == CANT_DESTROY)
            {
                return kCantDtorStackComparison;
            }

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Выход") == 0)
        {
            return kDoneTreeAkin;
        }

        fprintf (stdout, "Ответ не распознан.\nПерепроверь правописание и повтори запрос\n");
    }
    return kInvalidFlagAkin;
}

static enum TreeErrorAkin ReplayAkinator (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    fprintf (stdout, "Хочешь ещё раз?)\n"
                     "Тогда напиши \"Да\"\n");

    sleep (2);

    fprintf (stdout, "Сосал? \n");

    if (GetAnswer () == kTrueResult)
    {
        return ParseFlags (root);
    }

    fprintf (stdout, "Хочешь записать дерево в файл?\n");

    if (GetAnswer () == kTrueResult)
    {
        fprintf (stdout, "Тогда напиши имя выходного файла\n");

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

        WriteTreeDataBase (root, 0, output);

        FCLOSE (output);
    }

    return result;
}
