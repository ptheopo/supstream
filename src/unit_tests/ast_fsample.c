#include "unit_tests.h"

/* Variables must be global for tests */

FILE                *file = NULL;
yaml_parser_t       parser;
yaml_token_t        token;
gboolean            done = FALSE;
fifo_t              *lines = NULL;
list_t              *deepblock = NULL;
ast_tree_t          *root = NULL;
supstream_t         *supstream = NULL;
ast_node_t          *g_pipeline = NULL;
ast_node_t          *g_elements = NULL;
const gchar         *yaml_pipeline = "samples/matroska_video_0.yaml";

int                 suite_init_ast_fsample(void) {

    g_print("\n");
    file = fopen(yaml_pipeline, "r");
    if (file == NULL)
        return (1);
    if (!yaml_parser_initialize(&parser))
        return (1);
    yaml_parser_set_input_file(&parser, file);
    while (done != TRUE) {
        yaml_parser_scan(&parser, &token);
        done = parse_token(&token, &root, &lines, &deepblock);
        yaml_token_delete(&token);
    }
    return 0;
}

int                 suite_clean_ast_fsample(void) {

    /* Don't forget to free the AST */
    //ast_node_free(key);
    free(root);
    //ast_free(&root);
    yaml_parser_delete(&parser);
    assert(!fclose(file));
    return 0;
}

void                test_ast_fsample_document(void) {

    CU_ASSERT_STRING_EQUAL(root->str, "document");
    CU_ASSERT((ast_node_is_iblock(root) == true));
}

void                test_ast_fsample_supstream(void) {

    CU_ASSERT_STRING_EQUAL(root->left->str, "supstream");
    CU_ASSERT((ast_node_is_iblock(root->left) == true));

    /* zmq_path */
    CU_ASSERT((ast_node_is_iline(root->left->left) == true));
    CU_ASSERT((ast_node_is_iscalar(root->left->left->left) == true));
    CU_ASSERT_STRING_EQUAL(root->left->left->left->left->str, "zmq_path");
    CU_ASSERT_STRING_EQUAL(root->left->left->left->right->str, "tcp://0.0.0.0:5557");
}

void                test_ast_fsample_pipelines(void) {

    CU_ASSERT_STRING_EQUAL(root->left->right->str, "pipelines");
    CU_ASSERT((ast_node_is_iblock(root->left->right) == true));
}

void                test_ast_fsample_pipelines_pipeline(void) {

    g_pipeline = root->left->right->left;

    CU_ASSERT_STRING_EQUAL(g_pipeline->str, "matroska_video_0");
    CU_ASSERT((ast_node_is_iblock(g_pipeline) == true));

    /* init_state */
    CU_ASSERT((ast_node_is_iline(g_pipeline->left) == true));
    CU_ASSERT((ast_node_is_iscalar(g_pipeline->left->left) == true));
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->left->left->str, "init_state");
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->left->right->str, "play");

    /* type_exec */
    CU_ASSERT((ast_node_is_iline(g_pipeline->left->right) == true));
    CU_ASSERT((ast_node_is_iscalar(g_pipeline->left->right->left) == true));
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->right->left->left->str, "type_exec");
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->right->left->right->str, "thread");

    /* auto_restart */
    CU_ASSERT((ast_node_is_iline(g_pipeline->left->right->right) == true));
    CU_ASSERT((ast_node_is_iscalar(g_pipeline->left->right->right->left) == true));
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->right->right->left->left->str, "auto_restart");
    CU_ASSERT_STRING_EQUAL(g_pipeline->left->right->right->left->right->str, "true");

}

void                test_ast_fsample_pipelines_pipeline_elements(void) {

    g_elements = g_pipeline->left->right->right->right;

    CU_ASSERT((ast_node_is_iblock(g_elements->left) == true));
    CU_ASSERT_STRING_EQUAL(g_elements->left->str, "souphttpsrc");

    CU_ASSERT((ast_node_is_iblock(g_elements->left->right) == true));
    CU_ASSERT_STRING_EQUAL(g_elements->left->right->str, "matroskademux");

    CU_ASSERT((ast_node_is_iblock(g_elements->left->right->right) == true));
    CU_ASSERT_STRING_EQUAL(g_elements->left->right->right->str, "vp8dec");

    CU_ASSERT((ast_node_is_iblock(g_elements->left->right->right->right) == true));
    CU_ASSERT_STRING_EQUAL(g_elements->left->right->right->right->str, "videoconvert");

    CU_ASSERT((ast_node_is_iblock(g_elements->left->right->right->right->right) == true));
    CU_ASSERT_STRING_EQUAL(g_elements->left->right->right->right->right->str, "autovideosink");

}