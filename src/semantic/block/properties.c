#include "semantic.h"

void                    semantic_block_properties(ast_node_t *node, GstElement *element) {

    GObjectClass        *objClass = G_OBJECT_GET_CLASS(element);
    const gchar         *type_name = NULL;
    char                *property = NULL;
    char                *value = NULL;
    char                *endPtr = NULL;
    dtype_t             type;
    GParamSpec          *spec = NULL;

    if (!node->left)
        return ;

    node = node->left;
    do {

        if (!node || !node->left)
            return ;

        property = node->left->left->str;
        value = node->left->right->str;
        spec = g_object_class_find_property(objClass, property);
        if (spec != NULL) {
            type_name = g_type_name(spec->value_type);

            if (strcmp(type_name, "gchararray") == 0) {

                /* gchar * */
                g_object_set(G_OBJECT (element), property, (gchar *)value, NULL);

            } else if (strcmp(type_name, "gfloat") == 0 || strcmp(type_name, "gdouble") == 0) {

                /* gfloat */
                type.gfloat = (gfloat)strtof(value, &endPtr);
                g_object_set(G_OBJECT (element), property, type.gfloat, NULL);

            } else if (strcmp(type_name, "guint64") == 0) {

                /* guint64 */
                type.guint64 = (guint64)strtol(value, &endPtr, 10);
                g_object_set(G_OBJECT (element), property, type.guint64, NULL);

            } else if (strcmp(type_name, "gboolean") == 0) {

                /* gboolean */
                type.gboolean = strtogboolean(value);
                g_object_set(G_OBJECT (element), property, type.gboolean, NULL);

            } else if (strcmp(type_name, "gint") == 0) {

                /* gint */
                type.gint = (gint)atoi(value);
                g_object_set(G_OBJECT (element), property, type.gint, NULL);

            } else if (strcmp(type_name, "guint") == 0) {

                /* guint (maybe need to use stroul) */
                type.guint = (guint)atoi(value);
                g_object_set(G_OBJECT (element), property, type.guint, NULL);

            } else if (strcmp(type_name, "GstCaps") == 0) {

                /* GstCaps */
                type.gstcaps = (GstCaps *)gst_caps_from_string(value);
                g_object_set(G_OBJECT (element), property, type.gstcaps, NULL);

            } else {

                /* gint */
                type.gint = (gint)atoi(value);
                g_object_set(G_OBJECT (element), property, type.gint, NULL);

            }

        }
        g_print(SEMANTIC_SET_PROPERTY_O, property, value);
        node = node->right;
    } while (node);
}
