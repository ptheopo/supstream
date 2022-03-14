#include "gateway.h"

/* All requests */

request_t                           g_requests[NB_REQUEST] = {

    { "show",                       request_show },
    { "unlink_elements",            request_unlink_elements },
    { "link_elements",              request_link_elements },
    { "create_element",             request_create_element },
    { "remove_element",             request_remove_element },
    { "set_properties",             request_set_properties },
    { "set_caps",                   request_set_caps },
    { "version",                    request_version },
    { "play",                       request_play },
    { "pause",                      request_pause },
    { "ready",                      request_ready },
    { "null",                       request_null },
    { "exit",                       request_exit },

};

/* Global parser */

/* This function execute JSON request from zeromq IPC */

static char                         *gateway_exec_request(
                                    ast_tree_t **root,
                                    char *request) {

    cJSON                           *request_json = cJSON_Parse(request);
    const cJSON                     *command_json = NULL;
    char                            *json_stringify = NULL;

    /* Show task */
    command_json = cJSON_GetObjectItemCaseSensitive(request_json, "command");
    if (command_json != NULL) {

        /* Apply correct show type */
        for (int i = 0; i < NB_REQUEST; i++) {
            if (strcmp(g_requests[i].name, command_json->valuestring) == 0) {
                json_stringify = g_requests[i].fn(request_json, root);
            }
        }

        goto end;
    }

end:
    cJSON_Delete(request_json);
    return (json_stringify);

}

static char         *uniqid_gen() {

    uuid_t          uuid;
    char            *uniqid = malloc(37);

    uuid_generate(uuid);
    uuid_unparse_lower(uuid, uniqid);
    return (uniqid);
}

/* Using PAIR style ZMQ data sharing for IPC (REP <-> REQ) */

void                                *gateway(void *privdata) {

    gatewaydata_t                   *gatewaydata = (gatewaydata_t *)privdata;
    data_t                          data;
    char                            *_uuid = uniqid_gen();
    void                            *context = zmq_ctx_new ();
    void                            *responder = zmq_socket (context, ZMQ_REP);
    int                             rc = zmq_bind (responder, gatewaydata->config->zmq_path);
    /* MAX request size need to be reviewed */
    char                            buffer[1000] = { 0 };
    char                            *result = NULL;
    int                             ret = 0;

    /* Private data */
    data.root = gatewaydata->root;
    data.config = gatewaydata->config;
    data.uuid = _uuid;

    if (rc == -1)
        g_printerr(ZMQ_BIND_ERROR_O);
    else
        g_print(ZMQ_PATH_SUCCESS_O, data.config->zmq_path);

    while (1) {

        bzero(buffer, 1000);
        ret = zmq_recv (responder, buffer, 1000, 0);

        if (ret > 0) {
            result = gateway_exec_request(data.root, buffer);
            /* safe mode because need dynamic sending */
            if (result != NULL)
                zmq_send(responder, result, strlen(result), 0);
        }

        /* Implement event listener ? */
        usleep(42);

    }

    return (NULL);
}
