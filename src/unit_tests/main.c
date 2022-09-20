#include "unit_tests.h"

int             main(void) {

    CU_pSuite   suite_ast_fscratch = NULL;
    CU_pSuite   suite_ast_fsample = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /********************/
    /* AST From Scratch */
    /********************/

    suite_ast_fscratch = CU_add_suite("AST From Scratch", suite_init_ast_fscratch, suite_clean_ast_fscratch);
    if (NULL == suite_ast_fscratch) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_node_is_iblock()", test_ast_fscratch_node_new)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_node_is_iblock()", test_ast_fscratch_node_is_iblock)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_node_is_iline()", test_ast_fscratch_node_is_iline)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_node_is_iscalar()", test_ast_fscratch_node_is_iscalar)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_iscalar_value()", test_ast_fscratch_iscalar_value)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_ilb_add_simple()", test_ast_fscratch_ilb_add_simple)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if ((NULL == CU_add_test(suite_ast_fscratch, "test of test_ast_fscratch_ilb_add()", test_ast_fscratch_ilb_add)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /********************/
    /* AST From Sample */
    /********************/

    suite_ast_fsample = CU_add_suite("AST From Sample", suite_init_ast_fsample, suite_clean_ast_fsample);
    if (NULL == suite_ast_fsample) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite_ast_fsample, "test of test_ast_fsample_document()", test_ast_fsample_document)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}