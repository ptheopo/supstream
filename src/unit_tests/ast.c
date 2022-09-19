#include "unit_tests.h"

/* Variables must be global for tests */

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

void test_ast_node_new(void)
{
    ast_node_t *node = ast_node_new("key", iKEY);
    CU_ASSERT_STRING_EQUAL(node->str, "key");
}
