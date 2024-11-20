#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"
#include "../../My_lib/helpful.h"

static void PrintMenu (void);
static bool ReplayAkinator (node_t* const root);
static enum TreeErrorAkin EndAkinator (const node_t* const root);

enum TreeErrorAkin ParseFlags (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    char flag [kLenFlag] = "";

    enum TreeErrorAkin result = kDoneTreeAkin;

    while (true)
    {
        PrintMenu ();

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

            if (ReplayAkinator (root))
            {
                continue;
            }
            else
            {
                return EndAkinator (root);
            }
        }

        if (strcmp (flag, kCreateFlag) == 0)
        {
            result = CreateFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            if (ReplayAkinator (root))
            {
                continue;
            }
            else
            {
                return EndAkinator (root);
            }
        }

        if (strcmp (flag, kDefinitionFlag) == 0)
        {
            result = DefinitionFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            if (ReplayAkinator (root))
            {
                continue;
            }
            else
            {
                return EndAkinator (root);
            }
        }

        if (strcmp (flag, kComparisonFlag) == 0)
        {
            result = ComparisonFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            if (ReplayAkinator (root))
            {
                continue;
            }
            else
            {
                return EndAkinator (root);
            }
        }

        if (strcmp (flag, kWriteFlag) == 0)
        {
            result = WriteFlag (root);

            if (result != kDoneTreeAkin)
            {
                return result;
            }

            if (ReplayAkinator (root))
            {
                continue;
            }
            else
            {
                return EndAkinator (root);
            }
        }

        if (strcmp (flag, kExitFlag) == 0)
        {
            return EndAkinator (root);
        }

        fprintf (stdout, "Ответ не распознан.\nПерепроверь правописание и повтори запрос.\n");
    }

    return kInvalidFlagAkin;
}

static void PrintMenu (void)
{
    fprintf (stdout, "Это Великий и Ужасный Акинатор!\n"
                     "Для продолжения напиши:\n"
                     " \"%s\" - чтобы Акинатор отгадал твоё СуЩеСтВо\n"
                     " \"%s\"    - чтобы создать дерево вариантов ответов по базе данных\n"
                     " \"%s\"- чтобы дать определение вашего сУщЕсТвА\n"
                     " \"%s\"  - чтобы сравнить два ваших СУЩЕСТВА\n"
                     " \"%s\"     - чтобы записать информацию о дереве в файл\n"
                     " \"%s\"      - чтобы выйти из программы\n",
                     kRunFlag, kCreateFlag, kDefinitionFlag, kComparisonFlag, kWriteFlag, kExitFlag);
}

static bool ReplayAkinator (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    fprintf (stdout, "Хочешь ещё раз?)\n"
                     "Тогда напиши \"Да\"\n");

    sleep (2);

    fprintf (stdout, "Сосал? \n");

    if (GetAnswer () == kTrueResult)
    {
        fputc ('\n', stdout);
        return true;
    }

    return false;
}

static enum TreeErrorAkin EndAkinator (const node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    fprintf (stdout, "\nХочешь записать дерево в файл?\n");

    enum TreeResult answer = GetAnswer ();

    if (answer == kTrueResult)
    {
        result = WriteFlag (root);
    }

    return result;
}
