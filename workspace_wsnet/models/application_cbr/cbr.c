/**
 *  \file   cbr.c
 *  \brief  CBR application
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdio.h>

#include "types.h"
#include "modelutils.h"


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "CBR application",
    "Guillaume Chelius and Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct _cbr_private {
    uint64_t start;
    uint64_t period;
    int size;
    nodeid_t destination;
    position_t position;
    int overhead;
};


/* ************************************************** */
/* ************************************************** */
int callmeback(void* wsv, call_t *c, void *args);
void tx(wsnet_t* ws, call_t *c);


/* ************************************************** */
/* ************************************************** */
int init(wsnet_t* ws, call_t *c, void *params) {
    return 0;
}

int destroy(wsnet_t* ws, call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(wsnet_t* ws, call_t *c, void *params) {
    struct _cbr_private *nodedata = malloc(sizeof(struct _cbr_private));
    param_t *param;

    /* default values */
    nodedata->destination = ws->get_random_node(c->node);
    nodedata->position.x = ws->get_random_x_position();
    nodedata->position.y = ws->get_random_y_position();
    nodedata->position.z = ws->get_random_z_position();

    nodedata->start = 0;
    nodedata->period = 1000000000;
    nodedata->size = 1000;

    /* get parameters */
    ws->das_init_traverse(params);
    while ((param = (param_t *) ws->das_traverse(params)) != NULL) {

        if (!strcmp(param->key, "destination")) {
            if (ws->get_param_nodeid(param->value, &(nodedata->destination), c->node)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "destination-x")) {
            if (ws->get_param_x_position(param->value, &(nodedata->position.x))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "destination-y")) {
            if (ws->get_param_y_position(param->value, &(nodedata->position.y))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "destination-z")) {
            if (ws->get_param_z_position(param->value, &(nodedata->position.z))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "start")) {
            if (ws->get_param_time(param->value, &(nodedata->start))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "period")) {
            if (ws->get_param_time(param->value, &(nodedata->period))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "size")) {
            if (ws->get_param_integer(param->value, &(nodedata->size))) {
                goto error;
            }
        }
    }

    
    ws->set_node_private_data(c, nodedata);
    return 0;
  
 error:
    free(nodedata);
    return -1;
}

int unsetnode(wsnet_t* ws, call_t *c) {
    struct _cbr_private *nodedata = ws->get_node_private_data(c);

    free(nodedata);
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(wsnet_t* ws, call_t *c) {
    struct _cbr_private *nodedata = ws->get_node_private_data(c);
    array_t *down = ws->get_entity_bindings_down(c);
    call_t c0 = {down->elts[0], c->node, c->entity};
    uint64_t start = ws->get_time() + nodedata->start + ws->get_random_double() * nodedata->period;
  
    /* get overhead */
    nodedata->overhead = ws->GET_HEADER_SIZE(&c0);
    
    /* eventually schedule callback */
    ws->scheduler_add_callback(start, c, callmeback, NULL);
    
    return 0;
}

int ioctl(wsnet_t* ws, call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int callmeback(void* wsv, call_t *c, void *args) {
	wsnet_t *ws = (wsnet_t *) wsv;
    struct _cbr_private *nodedata = ws->get_node_private_data(c);

    ws->scheduler_add_callback(ws->get_time() + nodedata->period, c, callmeback, NULL);
    tx(ws, c);

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void tx(wsnet_t* ws, call_t *c) {
    struct _cbr_private *nodedata = ws->get_node_private_data(c);
    array_t *down = ws->get_entity_bindings_down(c);
    packet_t *packet = ws->packet_create(c, nodedata->size + nodedata->overhead, -1);
    call_t c0 = {down->elts[0], c->node, c->entity};
    destination_t destination = {nodedata->destination, 
                                 {nodedata->position.x, 
                                  nodedata->position.y, 
                                  nodedata->position.z}};
        
    if (ws->SET_HEADER(&c0, packet, &destination) == -1) {
    	ws->packet_dealloc(packet);
        return;
    }
    
    ws->TX(&c0, packet);
}


/* ************************************************** */
/* ************************************************** */
void rx(wsnet_t* ws, call_t *c, packet_t *packet) {
  printf("[CBR] node %d received a data packet : packet_size=%d  rxdBm=%lf \n", c->node, packet->size, packet->rxdBm);
  ws->packet_dealloc(packet);
}


/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};
