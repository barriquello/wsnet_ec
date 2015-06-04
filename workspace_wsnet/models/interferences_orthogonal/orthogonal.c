/**
 *  \file   orthogonal.c
 *  \brief  Orthogonal interference
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include "modelutils.h"


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Orthogonal",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_INTERFERENCES, 
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
double interfere(wsnet_t* ws, call_t *c, int channel0, int channel1) {
    if (channel0 == channel1) {
        return 1;
    } else {
        return 0;
    }
}

/* ************************************************** */
/* ************************************************** */
interferences_methods_t methods = {interfere};
