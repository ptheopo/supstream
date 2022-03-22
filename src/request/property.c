#include "request.h"

char                        *request_properties_json_str(const cJSON *request_json, int ret) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;

    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    code_json = cJSON_CreateNumber(ret);
    cJSON_AddItemToObject(response_json, "code", code_json);
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);

}

char                *request_set_properties(
        const cJSON *request_json,
        ast_tree_t **root) {

    cJSON           *properties_json = cJSON_GetObjectItemCaseSensitive(request_json, "properties");
    cJSON           *property_json = NULL;
    cJSON           *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON           *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    cJSON           *property_scalar_json = NULL;
    GParamSpec      *spec = NULL;
    // request get element
    list_t          *deepblock = ast_deepblock_create(4, "document", "pipelines", pipeline_json->valuestring, "elements", element_json->valuestring);
    ast_node_t      *element = ast_iblock_get(*root, deepblock);
    GObjectClass    *objClass = G_OBJECT_GET_CLASS(element);
    GstCaps         *caps = NULL;
    char            *result_json_str = NULL;
    const char      *type_name = NULL;

    if (element == NULL) {

        /* Element unfound */

        result_json_str = request_properties_json_str(request_json, 1);

    } else {

        /* Element found, treat JSON properties each by one */

        cJSON_ArrayForEach(property_json, properties_json) {

            cJSON_ArrayForEach(property_scalar_json, property_json) {

                /* Find correct property type */

                spec = g_object_class_find_property(objClass, property_scalar_json->string);
                if (spec != NULL) {

                    type_name = g_type_name(spec->value_type);
                    if (strcmp(type_name, "gchararray") == 0) {

                        /* gchar * */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (gchar *)property_scalar_json->valuestring, NULL);

                    } else if (strcmp(type_name, "gfloat") == 0) {

                        /* gfloat */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (gfloat)property_scalar_json->valuedouble, NULL);

                    } else if (strcmp(type_name, "guint64") == 0) {

                        /* guint64 */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (guint64)property_scalar_json->valueint, NULL);

                    } else if (strcmp(type_name, "gboolean") == 0) {

                        /* gboolean */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (gboolean)cJSON_IsTrue(property_scalar_json), NULL);

                    } else if (strcmp(type_name, "gint") == 0) {

                        /* gint */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (gint)property_scalar_json->valueint, NULL);

                    } else if (strcmp(type_name, "guint") == 0) {

                        /* guint (maybe need to use stroul) */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (guint)property_scalar_json->valueint, NULL);

                    } else if (strcmp(type_name, "GstCaps") == 0) {

                        /* GstCaps */
                        caps = (GstCaps *)gst_caps_from_string((gchar *)property_scalar_json->valuestring);
                        g_object_set(G_OBJECT (element), property_scalar_json->string, caps, NULL);

                    } else {

                        /* gint */
                        g_object_set(G_OBJECT (element), property_scalar_json->string, (gint)property_scalar_json->valueint, NULL);

                    }

                }

            }

        }

        result_json_str = request_properties_json_str(request_json, 0);

    }

    return (result_json_str);
}
