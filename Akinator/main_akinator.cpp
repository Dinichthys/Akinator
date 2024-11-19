#include "akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "../My_lib/Logger/logging.h"
#include "../My_lib/Assert/my_assert.h"

#define ERROR_HANDLER(error)                                            \
    if (error != kDoneTreeAkin)                                         \
    {                                                                   \
        fprintf (stderr, "Error number = {%d}\n"                        \
                         "Error is \"%s\"\n", error, EnumToStr (error));\
        DumpTreeAkin (&root);                                           \
        TreeDtorAkin (&root);                                           \
        fclose (error_file);                                            \
        return EXIT_FAILURE;                                            \
    }

int main ()
{
    FILE* const error_file = fopen ("My_lib/Logger/error.txt", "w");
    if (error_file == NULL)
    {
        fprintf (stderr, "Can't start logging\n");
        return EXIT_FAILURE;
    }
    set_log_file (error_file);
    set_log_lvl (DEBUG);

    setlocale (LC_ALL, "ru_RU.utf8");

    node_t root = {0};
    enum TreeErrorAkin result = kDoneTreeAkin;

    result = TreeCtorAkin (&root, "Хз что");

    ERROR_HANDLER (result);

    result = ParseFlags (&root);

    ERROR_HANDLER (result);

    result = DumpTreeAkin (&root);

    ERROR_HANDLER (result);

    result = TreeDtorAkin (&root);

    ERROR_HANDLER (result);

    fclose (error_file);

    return EXIT_SUCCESS;
}

#undef ERROR_HANDLER
