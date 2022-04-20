#include "request.h"

static char         *request_properties_json_str(const cJSON *request_json, int ret) {

    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *code_json = NULL;
    char            *result_str = NULL;

    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    code_json = cJSON_CreateNumber(ret);
    cJSON_AddItemToObject(response_json, "code", code_json);
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);

}

static int          request_set_properties_count_digits(double n) {

    return ((n == 0) ? 1 : log10(n) + 1);

}

static char         *request_set_properties_convert(cJSON *property) {

    char            *result = NULL;
    int             count = 0;

    if (cJSON_IsBool(property) && cJSON_IsTrue(property))
        result = g_strdup("True");
    else if (cJSON_IsBool(property) && !cJSON_IsTrue(property))
        result = g_strdup("False");
    else if (cJSON_IsString(property))
        result = g_strdup(property->valuestring);
    else if (cJSON_IsNumber(property)) {

        count = request_set_properties_count_digits(property->valuedouble) + 1;
        result = (char *)malloc(sizeof(char) * count);
        if (result == NULL)
            return (NULL);
        snprintf(result, count, "%f", property->valuedouble);

    }
    return (result);

}

char                *request_set_properties(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    cJSON           *properties_json = cJSON_GetObjectItemCaseSensitive(request_json, "properties");
    cJSON           *property_json = NULL;
    cJSON           *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON           *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    GParamSpec      *spec = NULL;
    list_t          *deepblock = ast_deepblock_create(5, "document", "pipelines", pipeline_json->valuestring, "elements", element_json->valuestring);
    list_t          *deepblock_properties = ast_deepblock_create(1, "properties");
    ast_node_t      **root_ptr = root;
    ast_node_t      **element = aast_iblock_get(root_ptr, deepblock);
    ast_node_t      **properties = aast_iblock_get(element, deepblock_properties);
    GObjectClass    *objClass = G_OBJECT_GET_CLASS((*element)->sdata->gstelement);
    GstCaps         *caps = NULL;
    char            *result_json_str = NULL;
    const char      *type_name = NULL;
    char            *convert_value = NULL;

    if (*element == NULL || (*element)->sdata->gstelement == NULL) {

        /* Element unfound */

        result_json_str = request_properties_json_str(request_json, 1);

    } else {

        /* Element found, treat JSON properties each by one */

        g_print("[REQUEST] Set properties\n");

        cJSON_ArrayForEach(property_json, properties_json) {

            /* Find correct property type */

            spec = g_object_class_find_property(objClass, property_json->string);

            if (spec != NULL) {

                type_name = g_type_name(spec->value_type);

                if (strcmp(type_name, "gchararray") == 0) {

                    /* gchar * */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (gchar *)property_json->valuestring, NULL);

                } else if (strcmp(type_name, "gfloat") == 0 || strcmp(type_name, "gdouble") == 0) {

                    /* gfloat */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (gfloat)property_json->valuedouble, NULL);

                } else if (strcmp(type_name, "guint64") == 0) {

                    /* guint64 */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (guint64)property_json->valueint, NULL);

                } else if (strcmp(type_name, "gboolean") == 0) {

                    /* gboolean */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (gboolean)cJSON_IsTrue(property_json), NULL);

                } else if (strcmp(type_name, "gint") == 0) {

                    /* gint */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (gint)property_json->valueint, NULL);

                } else if (strcmp(type_name, "guint") == 0) {

                    /* guint (maybe need to use stroul) */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (guint)property_json->valueint, NULL);

                } else if (strcmp(type_name, "GstCaps") == 0) {

                    /* GstCaps */
                    caps = (GstCaps *)gst_caps_from_string((gchar *)property_json->valuestring);
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, caps, NULL);

                } else {

                    /* gint */
                    g_object_set(G_OBJECT ((*element)->sdata->gstelement), property_json->string, (gint)property_json->valueint, NULL);

                }

                /* Update AST */
                convert_value = request_set_properties_convert(property_json);
                ast_iscalar_set_simple(properties, property_json->string, convert_value);

            }

        }

        result_json_str = request_properties_json_str(request_json, 0);

    }

    return (result_json_str);
}

/* Just replace element by pad */

char                *request_set_pad_properties(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    /* Modify AST tree browsing/settings */
    cJSON           *pad_json = cJSON_GetObjectItemCaseSensitive(request_json, "pad_props");
    cJSON           *properties_json = cJSON_GetObjectItemCaseSensitive(request_json, "properties");
    cJSON           *property_json = NULL;
    cJSON           *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON           *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    GParamSpec      *spec = NULL;
    list_t          *deepblock = ast_deepblock_create(5, "document", "pipelines", pipeline_json->valuestring, "elements", element_json->valuestring);
    list_t          *deepblock_pad_props = ast_deepblock_create(2, "pad_props", pad_json->valuestring);
    ast_node_t      **root_ptr = root;
    ast_node_t      **element = aast_iblock_get(root_ptr, deepblock);
    GstCaps         *caps = NULL;
    ast_node_t      *pad_props_node = ast_iblock_get(*element, deepblock_pad_props);
    ast_node_t      *pad_node = ast_iscalar_get_by_key(pad_props_node, "pad");
    GstPad          *pad = gst_element_get_static_pad((*element)->sdata->gstelement, pad_node->right->str);
    GObjectClass    *objClass = G_OBJECT_GET_CLASS(pad);
    char            *result_json_str = NULL;
    const char      *type_name = NULL;
    //char            *convert_value = NULL;

    if (*element == NULL || (*element)->sdata->gstelement == NULL || pad == NULL) {

        /* Element unfound */

        result_json_str = request_properties_json_str(request_json, 1);

    } else {

        /* Element found, treat JSON properties each by one */

        g_print("[REQUEST] Set pad properties\n");

        cJSON_ArrayForEach(property_json, properties_json) {

            /* Find correct property type */

            spec = g_object_class_find_property(objClass, property_json->string);

            if (spec != NULL) {

                type_name = g_type_name(spec->value_type);

                if (strcmp(type_name, "gchararray") == 0) {

                    /* gchar * */
                    g_object_set(G_OBJECT (pad), property_json->string, (gchar *)property_json->valuestring, NULL);

                } else if (strcmp(type_name, "gfloat") == 0 || strcmp(type_name, "gdouble") == 0) {

                    /* gfloat */
                    g_object_set(G_OBJECT (pad), property_json->string, (gfloat)property_json->valuedouble, NULL);

                } else if (strcmp(type_name, "guint64") == 0) {

                    /* guint64 */
                    g_object_set(G_OBJECT (pad), property_json->string, (guint64)property_json->valueint, NULL);

                } else if (strcmp(type_name, "gboolean") == 0) {

                    /* gboolean */
                    g_object_set(G_OBJECT (pad), property_json->string, (gboolean)cJSON_IsTrue(property_json), NULL);

                } else if (strcmp(type_name, "gint") == 0) {

                    /* gint */
                    g_object_set(G_OBJECT (pad), property_json->string, (gint)property_json->valueint, NULL);

                } else if (strcmp(type_name, "guint") == 0) {

                    /* guint (maybe need to use stroul) */
                    g_object_set(G_OBJECT (pad), property_json->string, (guint)property_json->valueint, NULL);

                } else if (strcmp(type_name, "GstCaps") == 0) {

                    /* GstCaps */
                    caps = (GstCaps *)gst_caps_from_string((gchar *)property_json->valuestring);
                    g_object_set(G_OBJECT (pad), property_json->string, caps, NULL);

                } else {

                    /* gint */
                    g_object_set(G_OBJECT (pad), property_json->string, (gint)property_json->valueint, NULL);

                }

                /* Update AST (after) */
                /*convert_value = request_set_properties_convert(property_json);
                ast_iscalar_set_simple(properties, property_json->string, convert_value);*/

            }

        }

        result_json_str = request_properties_json_str(request_json, 0);

    }

    /* don't forget to free deepblocks.. */

    return (result_json_str);
}
