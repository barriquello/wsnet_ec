/**
 *  \file   filestatic.c
 *  \brief  No mobility and position from file
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include "types.h"
#include "modelutils.h"


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Position extracted from file",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MOBILITY, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    FILE *file;
};


/* ************************************************** */
/* ************************************************** */
int init(wsnet_t* ws,call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    char *filepath = NULL;

    /* default values */
    filepath = "mobility.data";

    /* get parameters */
    ws->das_init_traverse(params);
    while ((param = (param_t *) ws->das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "file")) {
            filepath = param->value;
        }
    }

    /* open file */
    if ((entitydata->file = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "filestatic: can not open file %s in init()\n", filepath);
        goto error;
    }

    ws->set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(wsnet_t* ws, call_t *c) {
    struct entitydata *entitydata = ws->get_entity_private_data(c);

    if (entitydata->file != NULL) {
        fclose(entitydata->file);
    }

    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(wsnet_t* ws, call_t *c, void *params) {
    int id, found = 0;
    double x, y, z;
    struct entitydata *entitydata = ws->get_entity_private_data(c);
    char str[128];

    /* position at the beginning */
    fseek(entitydata->file, 0L, SEEK_SET);
    while (fgets(str, 128, entitydata->file) != NULL) {
        sscanf(str, "%d %lf %lf %lf\n",  &id, &x, &y, &z);
        if (id == c->node) {
            found = 1;
            ws->get_node_position(c->node)->x = x;
            ws->get_node_position(c->node)->y = y;
            ws->get_node_position(c->node)->z = z;
            break; 
        }
    }

    if (found == 0) {
        fprintf(stderr, "filestatic: node %d position not found (setnode())\n", c->node);
        return -1;       
    }

    return 0;
}

int unsetnode(wsnet_t* ws, call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(wsnet_t* ws, call_t *c) {
    PRINT_REPLAY("mobility %"PRId64" %d %lf %lf %lf\n", get_time(), c->node, 
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
