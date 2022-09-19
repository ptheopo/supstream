#include "unit_tests.h"

/* Variables must be global for tests */

int suite_init_ast(void)
{
    return 0;
}

int suite_clean_ast(void)
{
    return 0;
}

void test_ast_node_new(void)
{
    ast_node_t *node = ast_node_new("key", iKEY);
    CU_ASSERT_STRING_EQUAL(node->str, "key");
}
