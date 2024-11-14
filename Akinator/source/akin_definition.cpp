#include "../akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../My_lib/Assert/my_assert.h"
#include "../../My_lib/Logger/logging.h"
#include "../../My_lib/My_stdio/my_stdio.h"
#include "../../My_lib/helpful.h"

static enum TreeErrorAkin PrintComparisonDefinition (const stack_t first_stack, const stack_t second_stack);
static enum TreeErrorAkin MakeDefinition            (node_t* const node, const tree_elem_t subject,
                                                     stack_t const stack, const bool answer);
static enum TreeErrorAkin PrintDefinition           (stack_t const stack);

enum TreeErrorAkin CompareSubjects (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n");

    fprintf (stdout, "А теперь введи имена двух существ, определения которых ты хочешь сравнить\n");

    enum TreeErrorAkin result = kDoneTreeAkin;

    stack_t stack_first = 0;
    stack_t stack_second = 0;

    if (STACK_INIT_SHORT (stack_first, kDepthStack) == CANT_CREATE)
    {
        return kCantCtorStackComparison;
    }

    if (STACK_INIT_SHORT (stack_second, kDepthStack) == CANT_CREATE)
    {
        STACK_DTOR_SHORT (stack_first);
        return kCantCtorStackComparison;
    }

    tree_elem_t subject_first = "";

    if (fgets (subject_first, kLenElement, stdin) == NULL)
    {
        STACK_DTOR_SHORT (stack_second);
        STACK_DTOR_SHORT (stack_first);
        return kCantReadSubjectAkin;
    }

    if (strchr (subject_first, '\n') != NULL)
    {
        *(strchr (subject_first, '\n')) = '\0';
    }

    LOG (DEBUG, "Subject = \"%s\"\n", subject_first);

    result = MakeDefinition (root, subject_first, stack_first, true);

    if (result != kDoneTreeAkin)
    {
        STACK_DTOR_SHORT (stack_second);
        STACK_DTOR_SHORT (stack_first);
        return result;
    }

    tree_elem_t subject_second = "";

    if (fgets (subject_second, kLenElement, stdin) == NULL)
    {
        STACK_DTOR_SHORT (stack_second);
        STACK_DTOR_SHORT (stack_first);
        return kCantReadSubjectAkin;
    }

    if (strchr (subject_second, '\n') != NULL)
    {
        *(strchr (subject_second, '\n')) = '\0';
    }

    LOG (DEBUG, "Subject = \"%s\"\n", subject_second);

    result = MakeDefinition (root, subject_second, stack_second, true);

    if (result != kDoneTreeAkin)
    {
        STACK_DTOR_SHORT (stack_second);
        STACK_DTOR_SHORT (stack_first);
        return result;
    }

    result = PrintComparisonDefinition (stack_first, stack_second);

    if (result != kDoneTreeAkin)
    {
        STACK_DTOR_SHORT (stack_second);
        STACK_DTOR_SHORT (stack_first);
        return result;
    }

    if (STACK_DTOR_SHORT (stack_first) == CANT_DESTROY)
    {
        STACK_DTOR_SHORT (stack_second);
        return kCantDtorStackComparison;
    }

    if (STACK_DTOR_SHORT (stack_second) == CANT_DESTROY)
    {
        return kCantDtorStackComparison;
    }

    return result;
}

enum TreeErrorAkin DefinitionFlag (node_t* const root)
{
    ASSERT (root != NULL, "Invalid argument root = %p\n", root);

    enum TreeErrorAkin result = kDoneTreeAkin;

    fprintf (stdout, "А теперь введи имя существа, определение которого ты хочешь получить\n");

    stack_t stack = 0;

    if (STACK_INIT_SHORT (stack, kDepthStack) == CANT_CREATE)
    {
        return kCantCtorStackDefinition;
    }

    tree_elem_t subject = "";

    if (fgets (subject, kLenElement, stdin) == NULL)
    {
        STACK_DTOR_SHORT (stack);
        return kCantReadSubjectAkin;
    }

    if (strchr (subject, '\n') != NULL)
    {
        *(strchr (subject, '\n')) = '\0';
    }

    LOG (DEBUG, "Subject = \"%s\"\n", subject);

    result = MakeDefinition (root, subject, stack, true);

    if (result != kDoneTreeAkin)
    {
        STACK_DTOR_SHORT (stack);
        return result;
    }

    fprintf (stdout, "\n%s такое - \n", subject);

    result = PrintDefinition (stack);

    if (result != kDoneTreeAkin)
    {
        STACK_DTOR_SHORT (stack);
        return result;
    }

    if (STACK_DTOR_SHORT (stack) == CANT_DESTROY)
    {
        return kCantDtorStackDefinition;
    }

    return result;
}

static enum TreeErrorAkin PrintComparisonDefinition (const stack_t first_stack, const stack_t second_stack)
{
    stack_elem first_data = NULL;
    stack_elem second_data = NULL;

    stack_elem first_comp = NULL;
    stack_elem second_comp = NULL;

    STACK_ERROR result_first_stack = DONE;
    STACK_ERROR result_second_stack = DONE;

    result_first_stack = stack_pop (first_stack, &first_comp);

    if (result_first_stack != CANT_POP)
    {
        LOG (DEBUG, "First comparison = \"%s\"\n", first_comp);

        result_first_stack = stack_pop (first_stack, &first_data);

        if (result_first_stack != CANT_POP)
        {
            LOG (DEBUG, "First data = \"%s\"\n", first_data);
        }
    }

    result_second_stack = stack_pop (second_stack, &second_comp);

    if (result_second_stack != CANT_POP)
    {
        LOG (DEBUG, "Second comparison = \"%s\"\n", second_comp);

        result_second_stack = stack_pop (second_stack, &second_data);

        if (result_second_stack != CANT_POP)
        {
            LOG (DEBUG, "Second data = \"%s\"\n", second_data);
        }
    }

    if ((result_first_stack == CANT_POP) && (result_second_stack == CANT_POP))
    {
        fprintf (stdout, "\nСравнение завершено)\n\n");

        return kDoneTreeAkin;
    }

    if ((result_first_stack != CANT_POP)
        && (result_second_stack != CANT_POP)
        && (strcmp (first_data, second_data) == 0)
        && (strcmp (first_comp, second_comp) == 0))
    {
        fprintf (stdout, "\nДругие характеристики у них одинаковые:\n"
                         "Оно %s %s, ", first_comp, first_data);

        return PrintDefinition (first_stack);
    }

    if ((result_first_stack != CANT_POP)
        && (result_second_stack != CANT_POP))
    {
        fprintf (stdout, "\nПервое существо %s %s, а второе существо %s %s\n",
                         first_comp, first_data, second_comp, second_data);
    }

    if ((result_first_stack != CANT_POP)
        && (result_second_stack == CANT_POP))
    {
        fprintf (stdout, "\nВторое существо не имеет больше характеристик, а первое такое: \n"
                         "Оно %s %s, ", first_comp, first_data);

        return PrintDefinition (first_stack);
    }

    if ((result_first_stack == CANT_POP)
        && (result_second_stack != CANT_POP))
    {
        fprintf (stdout, "\nПервое существо не имеет больше характеристик, а второе такое: \n"
                         "Оно %s %s, ", second_comp, second_data);

        return PrintDefinition (second_stack);
    }

    return PrintComparisonDefinition (first_stack, second_stack);
}

static enum TreeErrorAkin MakeDefinition (node_t* const node, const tree_elem_t subject,
                                          stack_t const stack, const bool answer)
{
    ASSERT (node    != NULL, "Invalid argument node = %p\n", node);
    ASSERT (subject != NULL, "Invalid argument node = %p\n", subject);

    LOG (DEBUG, "Subject      = \"%s\"\n"
                "Data in node = \"%s\"\n",
                subject, node->data);

    enum TreeErrorAkin result = kDoneTreeAkin;

    stack_elem answer_not_in_definition = NULL;

    stack_elem data_not_in_definition = NULL;

    if (answer)
    {
        strcpy (node->answer, "");
    }
    else
    {
        strcpy (node->answer, "не");
    }

    if (stack_push (stack, node->answer) == CANT_PUSH)
    {
        return kCantMakeDefinition;
    }

    if (strcmp (node->data, subject) == 0)
    {
        return kDoneTreeAkin;
    }

    if ((node->left == NULL) && (node->right == NULL))
    {
        if (stack_pop (stack, &answer_not_in_definition) == CANT_POP)
        {
            return kCantMakeDefinition;
        }

        return kDidntFoundSubject;
    }

    if (stack_push (stack, node->data) == CANT_PUSH)
    {
        return kCantMakeDefinition;
    }

    result = MakeDefinition (node->left, subject, stack, false);

    LOG (DEBUG, "For left tree result = \"%s\"\n", EnumToStr (result));

    if ((result == kDoneTreeAkin) || (result == kCantMakeDefinition))
    {
        return result;
    }

    result = MakeDefinition (node->right, subject, stack, true);

    LOG (DEBUG, "For right tree result = \"%s\"\n", EnumToStr (result));

    if ((result == kDoneTreeAkin) || (result == kCantMakeDefinition))
    {
        return result;
    }

    if (result == kDidntFoundSubject)
    {
        LOG (DEBUG, "kDidntFoundSubject with both answers of token \"%s\"\n", node->data);

        if (stack_pop (stack, &data_not_in_definition) == CANT_POP)
        {
            return kCantMakeDefinition;
        }

        LOG (DEBUG, "Check data in stack \"%s\"\n", data_not_in_definition);

        if (stack_pop (stack, &answer_not_in_definition) == CANT_POP)
        {
            return kCantMakeDefinition;
        }

        LOG (DEBUG, "Check answer in stack \"%s\"\n", answer_not_in_definition);
    }

    LOG (DEBUG, "LOH\n");

    return result;
}

static enum TreeErrorAkin PrintDefinition (stack_t const stack)
{
    stack_elem element = NULL;
    stack_elem answer = NULL;

    while ((stack_pop (stack, &answer) != CANT_POP) && (stack_pop (stack, &element) != CANT_POP))
    {
        fprintf (stdout, "Оно %s %s, ", answer, element);
    }

    fputc ('\n', stdout);

    return kDoneTreeAkin;
}
