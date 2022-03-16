#include "request.h"

/*char                *request_set_properties(
  const cJSON *request_json,
  ast_tree_t **root) {

  char            *property = NULL;
  cJSON           *property_json = cJSON_GetObjectItemCaseSensitive(request_json, "property");
  cJSON           *property_json = cJSON_GetObjectItemCaseSensitive(request_json, "property");

  (void)request_json;
  (void)root;

  g_print(REQUEST_SET_PROP_SUCCESS_O, );
  g_object_set(G_OBJECT (element), property, type.guint64, NULL);
  return (NULL);
  }*/

char    *request_set_properties(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Set properties");
    return (NULL);
}
