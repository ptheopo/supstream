#include "unit_tests.h"

int             main(void) {

    CU_pSuite   suite_ast = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    suite_ast = CU_add_suite("AST", suite_init_ast, suite_clean_ast);
    if (NULL == suite_ast) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(suite_ast, "test of ast_node_new()", test_ast_node_new)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
