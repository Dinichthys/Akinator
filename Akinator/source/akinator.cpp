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
                     " \"%s\" - чтобы Акинатор отгадал твоё СуЩеСтВо\n"
                     " \"%s\"    - чтобы создать дерево вариантов ответов по базе данных\n"
                     " \"%s\"- чтобы дать определение вашего сУщЕсТвА\n"
                     " \"%s\"  - чтобы сравнить два ваших СУЩЕСТВА\n"
                     " \"%s\"     - чтобы записать информацию о дереве в файл\n"
                     " \"%s\"      - чтобы выйти из программы\n",
                     kRunFlag, kCreateFlag, kDefinitionFlag, kComparisonFlag, kWriteFlag, kExitFlag);

    while (true)
    {
        if (fscanf (stdin, "%[^ ^\n^\t^\r]", flag) == EOF)
        {
            return kInvalidFlagAkin;
        }

        fgetc (stdin);

        LOG (DEBUG, "Read flag = \"%s\"", flag);

        if (strcmp (flag, kRunFlag) == 0)
        {
            result = RunAkinatorFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, kCreateFlag) == 0)
        {
            result = CreateTreeFromDataBaseFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, kDefinitionFlag) == 0)
        {
            result = DefinitionFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, kComparisonFlag) == 0)
        {
            result = CompareSubjects (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, kWriteFlag) == 0)
        {
            result = WriteTreeDataBase (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            return ReplayAkinator (root);
        }

        if (strcmp (flag, kExitFlag) == 0)
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
