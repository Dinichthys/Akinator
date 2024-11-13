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
                     " \"Отгадывать\" - чтобы Акинатор отгадал твоё СуЩеСтВо\n"
                     " \"Создать\"    - чтобы создать дерево вариантов ответов по базе данных\n"
                     " \"Определение\"- чтобы дать определение вашего сУщЕсТвА\n"
                     " \"Сравнение\"  - чтобы сравнить два ваших СУЩЕСТВА\n"
                     " \"Запись\"     - чтобы записать информацию о дереве в файл\n"
                     " \"Выход\"      - чтобы выйти из программы\n");

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
            result = RunAkinatorFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Создать") == 0)
        {
            result = CreateTreeFromDataBaseFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Определение") == 0)
        {
            result = DefinitionFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Сравнение") == 0)
        {
            result = CompareSubjects (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, "Запись") == 0)
        {
            result = WriteTreeDataBase (root);

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

        fprintf (stdout, "Ответ не распознан.\nПерепроверь правописание и повтори запрос.\n");
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

    enum TreeResult answer = GetAnswer ();

    if (answer == kTrueResult)
    {
        result = WriteTreeDataBase (root);
    }

    return result;
}
