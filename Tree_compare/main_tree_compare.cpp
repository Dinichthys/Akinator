#include "tree_compare.h"

#include <stdio.h>
#include <stdlib.h>

#include "../My_lib/Logger/logging.h"
#include "../My_lib/Assert/my_assert.h"

#define ERROR_HANDLER(error)                                \
    if (error != kDoneTree)                                 \
    {                                                       \
        fprintf (stderr, "Error number = {%d}\n", error);   \
        TreeDtor (&root);                                   \
        fclose (error_file);                                \
        return EXIT_FAILURE;                                \
    }

int  Compare (void* const first, void* const second);
void PrintData (void* const data, FILE* const dump_file);

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

    node_t root = {0};

    size_t element = 100;

    enum TreeError result = kDoneTree;

    result = TreeCtor (&root, &element, sizeof (element));

    ERROR_HANDLER (result);

    result = PushNode (&root, &element, Compare);

    ERROR_HANDLER (result);

    result = DumpTree (&root, PrintData);

    ERROR_HANDLER (result);

    result = TreeDtor (&root);

    ERROR_HANDLER (result);

    fclose (error_file);

    return EXIT_SUCCESS;
}

#undef ERROR_HANDLER

int Compare (void* const first, void* const second)
{
    ASSERT (first  != NULL, "Invalid argument first = %p\n", first);
    ASSERT (second != NULL, "Invalid argument second = %p\n", second);

    return ((*((size_t*) first) - *((size_t*) second)) == 0) ? 0
            : ((*((size_t*) first) - *((size_t*) second)) > 0) ? 1 : -1;
}

void PrintData (void* const data, FILE* const dump_file)
{
    ASSERT (data      != NULL, "Invalid argument data = %p\n", data);
    ASSERT (dump_file != NULL, "Invalid argument dump_file = %p\n", dump_file);

    fprintf (dump_file, "%lu", *((size_t*) data));
}
