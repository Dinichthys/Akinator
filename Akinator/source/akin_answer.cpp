#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"

static bool StrCmpYes (char* const answer);
static bool StrCmpNo  (char* const answer);

enum TreeResult GetAnswer (void)
{
    char answer [kLenAnswer] = {'\0'};

    if (fgets (answer, kLenAnswer, stdin) == NULL)
    {
        return kCantFindAnswerForResult;
    }

    if (strchr (answer, '\n') != NULL)
    {
        *(strchr (answer, '\n')) = '\0';
    }

    LOG (DEBUG, "Answer = \"%s\"", answer);

    if (StrCmpYes (answer))
    {
        return kTrueResult;
    }

    if (StrCmpNo (answer))
    {
        return kFalseResult;
    }

    fprintf (stdout, "Ответ не распознан. Повторите, пожалуйста.\n");

    return GetAnswer ();
}

static bool StrCmpYes (char* const answer)
{
    ASSERT (answer != NULL, "Invalid argument answer = %p\n", answer);

    return ((my_strcmp (answer, "Да") == 0)
            || (my_strcmp (answer, "да") == 0)
            || (my_strcmp (answer, "дА") == 0)
            || (my_strcmp (answer, "ДА") == 0));
}

static bool StrCmpNo (char* const answer)
{
    ASSERT (answer != NULL, "Invalid argument answer = %p\n", answer);

    return ((my_strcmp (answer, "Нет") == 0)
            || (my_strcmp (answer, "НеТ") == 0)
            || (my_strcmp (answer, "НЕт") == 0)
            || (my_strcmp (answer, "НЕТ") == 0)
            || (my_strcmp (answer, "нет") == 0)
            || (my_strcmp (answer, "неТ") == 0)
            || (my_strcmp (answer, "нЕт") == 0)
            || (my_strcmp (answer, "нЕТ") == 0));
}
