/**
 *  \file   static.c
 *  \brief  no mobility
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include "types.h"
#include "modelutils.h"


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "No mobility",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MOBILITY, 
    {NULL, 0}
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
    param_t *param;

    /* default values */
    ws->get_node_position(c->node)->x = ws->get_random_x_position();
    ws->get_node_position(c->node)->y = ws->get_random_y_position();
    ws->get_node_position(c->node)->z = ws->get_random_z_position();
    
   /* get parameters */
    ws->das_init_traverse(params);
    while ((param = (param_t *) ws->das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "x")) {
            if (ws->get_param_x_position(param->value, &(ws->get_node_position(c->node)->x))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "y")) {
            if (ws->get_param_y_position(param->value, &(ws->get_node_position(c->node)->y))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "z")) {
            if (ws->get_param_z_position(param->value, &(ws->get_node_position(c->node)->z))) {
                goto error;
            }
        }
    }

    return 0;
    
 error:
    return -1;
}

int unsetnode(wsnet_t* ws, call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(wsnet_t* ws, call_t *c) {
    PRINT_REPLAY("mobility %"PRId64" %d %lf %lf %lf\n", ws->get_time(), c->node,
    		ws->get_node_position(c->node)->x, ws->get_node_position(c->node)->y,
			ws->get_node_position(c->node)->z);
    return 0;
}

int ioctl(wsnet_t* ws, call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void update_position(wsnet_t* ws, call_t *c) {
    return;
}


/* ************************************************** */
/* ************************************************** */
mobility_methods_t methods = {update_position};
