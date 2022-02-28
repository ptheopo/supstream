__attribute__((unused)) static void debug() {
    //ast_browse_prefix(root, &ast_node_print);

    /* you need to use samples/webm_decodebin_2.yaml */

    /* AST tests with ast_iblock_get function (low level)
    ast_node_t *elements = NULL;
    ast_node_t *src_properties = NULL;
    list_t *deepblock_elements = NULL;

    lstaddlast(&deepblock_elements, lstnew("document", sizeof(char) * 9));
    lstaddlast(&deepblock_elements, lstnew("pipeline", sizeof(char) * 9));
    lstaddlast(&deepblock_elements, lstnew("elements", sizeof(char) * 9));

    elements = ast_iblock_get(root, deepblock_elements);

    printf("all elements in pipeline :\n");
    printf("- %s\n", elements->left->str);
    printf("- %s\n", elements->left->right->str);
    printf("- %s\n", elements->left->right->right->str);

    lstaddlast(&deepblock_elements, lstnew("src", sizeof(char) * 4));
    lstaddlast(&deepblock_elements, lstnew("properties", sizeof(char) * 11));

    src_properties = ast_iblock_get(root, deepblock_elements);

    printf("all properties in src elements :\n");
    printf("- %s\n", src_properties->left->left->left->str);
    printf("- %s\n", src_properties->left->left->right->str);
    */

    /* AST tests webm_decodebin on YAML
    printf("document: %s\n", root->str);
    printf("pipeline: %s\n", root->left->str);
    printf("first line key in pipeline: %s\n", root->left->left->left->left->str);
    printf("first line value in pipeline: %s\n", root->left->left->left->right->str);
    printf("all elements :\n");
    printf("- %s\n", root->left->left->right->left->str);
    printf("- %s\n", root->left->left->right->left->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->str);
    printf("properties of src :\n");
    printf("- %s\n", root->left->left->right->left->left->right->left->left->left->str);
    printf("- %s\n", root->left->left->right->left->left->right->left->left->right->str);
    printf("decodebin :\n");
    printf("- %s\n", root->left->left->right->left->right->left->left->left->str);
    printf("- %s\n", root->left->left->right->left->right->left->left->right->str);
    printf("sink :\n");
    printf("- %s\n", root->left->left->right->left->right->right->left->left->left->str);
    printf("- %s\n", root->left->left->right->left->right->right->left->left->right->str);
    */

    /* AST tests with matroska video 0 kaleidoscope v4l2
    printf("document: %s\n", root->str);
    printf("pipeline: %s\n", root->left->str);
    printf("first line key in pipeline: %s\n", root->left->left->left->left->str);
    printf("first line value in pipeline: %s\n", root->left->left->left->right->str);
    printf("all elements :\n");
    printf("- %s\n", root->left->left->right->left->str);
    printf("- %s\n", root->left->left->right->left->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->right->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->right->right->right->str);
    printf("- %s\n", root->left->left->right->left->right->right->right->right->right->right->str);
    printf("all caps from videoconvert :\n");
    printf("- %s\n", root->left->left->right->left->right->right->right->left->right->left->right->left->left->right->str);
    */
    
    /* list tests
    list_t *a = lstnew("a\0", sizeof(char) * 2);
    list_t *b = lstnew("b\0", sizeof(char) * 2);
    list_t *c = lstnew("c\0", sizeof(char) * 2);
    list_t *d = lstnew("d\0", sizeof(char) * 2);

    lstaddlast(&a, b);
    lstaddlast(&a, c);
    lstaddlast(&a, d);

    printf("[BEFORE]\n");
    list_t *tmp = a;
    while (tmp) {
        printf("> %s\n", (char *)tmp->content);
        tmp = tmp->next;
    }

    lstdellast(&a);

    printf("[AFTER]\n");
    tmp = a;
    while (tmp) {
        printf("> %s\n", (char *)tmp->content);
        tmp = tmp->next;
    }
    */

    /*printf("%d\n", wcscmp((wchar_t *)"こんにちは", (wchar_t *)"こんにちは") == 0); */

}
