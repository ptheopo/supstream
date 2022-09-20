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
}