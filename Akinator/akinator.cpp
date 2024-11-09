#include "akinator.h"

#include "../My_lib/Assert/my_assert.h"
#include "../My_lib/Logger/logging.h"

enum TreeError PushNode (node_t* node, const bool next, tree_elem_t element)
{
    ASSERT (node != NULL, "Invalid argument node = %p\n", node);

    return kDoneTree;
}
