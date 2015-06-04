/**
 *  \file   omnidirectional.c
 *  \brief  Omni-directionnal antenna
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include "types.h"
#include "modelutils.h"

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Omnidirectionnal antenna",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_ANTENNA, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    double noise;
    angle_t angle;
    double gain_tx;
    double gain_rx;
};


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
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;

    /* default values */
    nodedata->noise = 0;
    nodedata->gain_tx = 0;
    nodedata->gain_rx = 0;
    nodedata->angle.xy = 0; //get_random_double_range(0, 2 * M_PI);
    nodedata->angle.z = 0; //get_random_double_range(0, 2 * M_PI);
    
#if 1
   /* get parameters */
    ws->das_init_traverse(params);
    while ((param = (param_t *) ws->das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "gain-tx")) {
            if (ws->get_param_double(param->value, &(nodedata->gain_tx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "gain-rx")) {
            if (ws->get_param_double(param->value, &(nodedata->gain_rx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "loss")) {
            if (ws->get_param_double(param->value, &(nodedata->noise))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "angle-xy")) {
            if (ws->get_param_double_range(param->value, &(nodedata->angle.xy), 0, 2*M_PI)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "angle-z")) {
            if (ws->get_param_double_range(param->value, &(nodedata->angle.z), 0, 2*M_PI)) {
                goto error;
            }
        }
    }
#endif
    
    ws->set_node_private_data(c, nodedata);
    return 0;

 error:
    free(nodedata);
    return -1;
}

int unsetnode(wsnet_t* ws,call_t *c) {
    free(ws->get_node_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(wsnet_t* ws, call_t *c) {
    return 0;
}

int ioctl(wsnet_t* ws, call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double get_loss(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->noise;
}

angle_t *get_angle(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return &(nodedata->angle);
}

void set_angle(wsnet_t* ws, call_t *c, angle_t *angle) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    nodedata->angle.xy = angle->xy;
    nodedata->angle.z = angle->z;
    return;
}


/* ************************************************** */
/* ************************************************** */
void cs(wsnet_t* ws, call_t *c, packet_t *packet) {


    array_t *up = ws->get_entity_bindings_up(c);
    int i = up->size;

    while (i--) {
        call_t c_up = {up->elts[i], c->node, c->entity};
        ws->radio_cs(&c_up, packet);
    }

    
    return;
}

void rx(wsnet_t* ws,call_t *c, packet_t *packet) {


    array_t *up = ws->get_entity_bindings_up(c);
    int i = up->size;
    
    while (i--) {
        call_t c_up = {up->elts[i], c->node, c->entity};
        packet_t *packet_up;
        
        if (i > 0) {
            packet_up = ws->packet_clone(packet);
        } else {
            packet_up = packet;
        }
        ws->RX(&c_up, packet_up);

    }
    
    return;
}


/* ************************************************** */
/* ************************************************** */
double  gain_tx(wsnet_t* ws,call_t *c, position_t *pos) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->gain_tx;
}

double  gain_rx(wsnet_t* ws,call_t *c, position_t *pos) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->gain_rx;
}


/* ************************************************** */
/* ************************************************** */
antenna_methods_t methods = {rx, 
                             cs,
                             get_loss, 
                             get_angle, 
                             set_angle, 
                             gain_tx, 
                             gain_rx};

