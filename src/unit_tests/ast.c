#include "unit_tests.h"

/* Variables must be global for tests */

ast_node_t *root = NULL;
ast_node_t *block = NULL;
ast_node_t *line = NULL;
ast_node_t *scalar = NULL;
ast_node_t *key = NULL;
ast_node_t *value = NULL;

int suite_init_ast(void) {
    scalar = ast_iscalar_new_simple("key", "value");
    block = ast_iblock_new("block");
    root = ast_iblock_new("root");
    key = scalar->left;
    value = scalar->right;
    line = ast_iline_new(scalar);
    return 0;
}

int suite_clean_ast(void) {
    //ast_node_free(key);
    return 0;
}

void test_ast_node_new(void) {
    CU_ASSERT_STRING_EQUAL(key->str, "key");
    CU_ASSERT_STRING_EQUAL(value->str, "value");
}

void test_ast_node_is_iblock(void) {
    CU_ASSERT((ast_node_is_iblock(block) == true));
    CU_ASSERT((ast_node_is_iblock(line) == false));
    CU_ASSERT((ast_node_is_iblock(scalar) == false));
    CU_ASSERT((ast_node_is_iblock(key) == false));
    CU_ASSERT((ast_node_is_iblock(value) == false));
}

void test_ast_node_is_iline(void) {
    CU_ASSERT((ast_node_is_iline(block) == false));
    CU_ASSERT((ast_node_is_iline(line) == true));
    CU_ASSERT((ast_node_is_iline(scalar) == false));
    CU_ASSERT((ast_node_is_iline(key) == false));
    CU_ASSERT((ast_node_is_iline(value) == false));
}

void test_ast_node_is_iscalar(void) {
    CU_ASSERT((ast_node_is_iscalar(block) == false));
    CU_ASSERT((ast_node_is_iscalar(line) == false));
    CU_ASSERT((ast_node_is_iscalar(scalar) == true));
    CU_ASSERT((ast_node_is_iscalar(key) == false));
    CU_ASSERT((ast_node_is_iscalar(value) == false));
}

void test_ast_iscalar_value(void) {
    CU_ASSERT_STRING_EQUAL(ast_iscalar_value(scalar), "value");
}

void test_ast_ilb_add_simple(void) {
    ast_ilb_add_simple(&block, line);

    CU_ASSERT((ast_node_is_iblock(block) == true));
    CU_ASSERT((ast_node_is_iline(block->right) == true));
    CU_ASSERT((ast_node_is_iscalar(block->right->left) == true));
    CU_ASSERT_STRING_EQUAL(block->right->left->left->str, "key");
    CU_ASSERT_STRING_EQUAL(block->right->left->right->str, "value");
}

void test_ast_ilb_add(void) {

    list_t *deepblock = ast_deepblock_create(1, "block");

    ast_ilb_add(&block, line, deepblock);

    CU_ASSERT((ast_node_is_iblock(block) == true));
    CU_ASSERT((ast_node_is_iline(block->left) == true));
    CU_ASSERT((ast_node_is_iscalar(block->left->left) == true));
    CU_ASSERT_STRING_EQUAL(block->left->left->left->str, "key");
    CU_ASSERT_STRING_EQUAL(block->left->left->right->str, "value");

    ast_deepblock_free(deepblock);

}