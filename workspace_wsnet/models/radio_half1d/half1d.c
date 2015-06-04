/**
 *  \file   half1d.c
 *  \brief  Halfduplex 1-d radio model
 *  \author Guillaume Chelius
 *  \date   2007
 **/

#include "types.h"
#include "modelutils.h"


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Halfduplex radio interface",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_RADIO, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    uint64_t Ts;
    double power;
    int channel;
    entityid_t modulation;
    double mindBm;
    int sleep;  
    int tx_busy;
    int rx_busy;
    double rxdBm;
};


/* ************************************************** */
/* ************************************************** */
void cs_init(wsnet_t* ws, call_t *c);


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
    nodedata->Ts = 91;
    nodedata->channel = 0;
    nodedata->power = 0;
    nodedata->modulation = -1;
    nodedata->mindBm = -92;
    nodedata->sleep = 0;

#if 1
    /* get parameters */
    ws->das_init_traverse(params);
    while ((param = (param_t *) ws->das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "sensibility")) {
            if (ws->get_param_double(param->value, &(nodedata->mindBm))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "T_s")) {
            if (ws->get_param_time(param->value, &(nodedata->Ts))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "channel")) {
            if (ws->get_param_integer(param->value, &(nodedata->channel))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "dBm")) {
            if (ws->get_param_double(param->value, &(nodedata->power))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "modulation")) {
            if (ws->get_param_entity(param->value, &(nodedata->modulation))) {
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

int unsetnode(wsnet_t* ws, call_t *c) {
    free(ws->get_node_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    nodedata->tx_busy = -1;
    nodedata->rx_busy = -1;
    nodedata->rxdBm = MIN_DBM;
    return 0;
}

int ioctl(wsnet_t* ws, call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void cs_init(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    /* log */
    if (nodedata->tx_busy != -1) {
        PRINT_REPLAY("radio-tx1 %"PRId64" %d\n", ws->get_time(), c->node);
    }
    if (nodedata->rx_busy != -1) {
        PRINT_REPLAY("radio-rx1 %"PRId64" %d\n", ws->get_time(), c->node);
    }
    /* init cs */
    nodedata->tx_busy = -1;
    nodedata->rx_busy = -1;
    nodedata->rxdBm = MIN_DBM;
}


/* ************************************************** */
/* ************************************************** */
void tx(wsnet_t *ws, call_t *c, packet_t *packet) {
    struct nodedata *nodedata = ws->get_node_private_data(c);

    array_t *down = ws->get_entity_bindings_down(c);
    int i = down->size;
    
    /* radio sleep */
    if (nodedata->sleep) {
        ws->packet_dealloc(packet);
        return;
    }

    /* radio activity */
    cs_init(ws,c);
    nodedata->tx_busy = packet->id;

    /* log tx */
    PRINT_REPLAY("radio-tx0 %"PRId64" %d 50\n", get_time(), c->node);

    /* transmit to antenna */
    while (i--) {
        packet_t *packet_down;
        
        if (i > 0) {
            packet_down = ws->packet_clone(packet);
        } else {
            packet_down = packet;
        }
        c->from = down->elts[i];
        ws->MEDIA_TX(c, packet_down);
    }

    return;
}

void tx_end(wsnet_t* ws, call_t *c, packet_t *packet) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    
#if 0
    /* consume energy */
    battery_consume_tx(c, packet->duration, nodedata->power);

    /* check wether the reception has killed us */
    if (!is_node_alive(c->node)) {
        return;
    }
#endif

    /* log tx */
    if (nodedata->tx_busy == packet->id) {
        PRINT_REPLAY("radio-tx1 %"PRId64" %d\n", ws->get_time(), c->node);
        nodedata->tx_busy = -1;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void rx(wsnet_t* ws, call_t *c, packet_t *packet) {
    struct nodedata *nodedata = ws->get_node_private_data(c);

    array_t *up = ws->get_entity_bindings_up(c);
    int i = up->size;

    /* radio sleep */
    if (nodedata->sleep) {
        ws->packet_dealloc(packet);
        return;
    }

    /* half-duplex */
    if (nodedata->tx_busy != -1) {
        ws->packet_dealloc(packet);
        return;
    }

    /* handle carrier sense */
    if (nodedata->rx_busy == packet->id) {
        nodedata->rx_busy = -1;
        nodedata->rxdBm   = MIN_DBM;
        /* log rx */
        PRINT_REPLAY("radio-rx1 %"PRId64" %d\n", get_time(), c->node);
        /* consume energy */
#if 0
        battery_consume_rx(c, packet->duration);
#endif
    } else {
        ws->packet_dealloc(packet);
        return;
    }

#if 0
    /* check wether the reception has killed us */
    if (!is_node_alive(c->node)) {
        packet_dealloc(packet);
        return;
    }
#endif
    /* drop packet depending on the FER */
    if (ws->get_random_double() < packet->PER) {
        ws->packet_dealloc(packet);
        return;
    }    

    /* forward to upper layers */
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

void cs(wsnet_t* ws, call_t *c, packet_t *packet) {
    struct nodedata *nodedata = ws->get_node_private_data(c);

    /* radio sleep */
    if (nodedata->sleep) {
        return;
    }

    /* half-duplex */
    if (nodedata->tx_busy != -1) {
        return;
    }

    /* check sensibility */
    if (packet->rxdBm < nodedata->mindBm) {
        return;
    }

    /* check channel */
    if (nodedata->channel != packet->channel) {
        return;
    }

#if 0    /* check Ts */
    if (nodedata->Ts != (packet->Tb*(radio_get_modulation_bit_per_symbol(c)))) {
        return;
    }
#endif

    /* check channel */
    if (packet->modulation != nodedata->modulation) {
        return;
    }

    /* capture effect */
    if (packet->rxdBm > nodedata->rxdBm) {
        nodedata->rxdBm = packet->rxdBm;
        nodedata->rx_busy = packet->id;
        /* log cs */
        PRINT_REPLAY("radio-rx0 %"PRId64" %d\n", ws->get_time(), c->node);
        return;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
double get_noise(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);

#if 1
    entityid_t *down = ws->get_entity_links_down(c);

    c->from = down[0];
    return ws->MEDIA_GET_NOISE(c, nodedata->channel);
#endif
    return 0;
}

double get_cs(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->rxdBm;
}

double get_power(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->power;
}

void set_power(wsnet_t* ws, call_t *c, double power) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    nodedata->power = power;
}

int get_channel(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->channel;
}

void set_channel(wsnet_t* ws, call_t *c, int channel) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->channel = channel;
}

entityid_t get_modulation(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->modulation;
}

void set_modulation(wsnet_t* ws, call_t *c, entityid_t modulation) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->modulation = modulation;
}

/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
uint64_t get_Tb(wsnet_t* ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);

    return (nodedata->Ts/(ws->modulation_bit_per_symbol(nodedata->modulation)));


}

void set_Ts(wsnet_t* ws, call_t *c, uint64_t Ts) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->Ts = Ts;
}

uint64_t get_Ts(wsnet_t *ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->Ts;
}
/* end of edition */

double get_sensibility(wsnet_t *ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    return nodedata->mindBm;
}

void set_sensibility(wsnet_t *ws, call_t *c, double sensibility) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->mindBm = sensibility;
}


/* ************************************************** */
/* ************************************************** */
static void sleep(wsnet_t *ws,call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->sleep = 1;
}

void wakeup(wsnet_t *ws, call_t *c) {
    struct nodedata *nodedata = ws->get_node_private_data(c);
    cs_init(ws, c);
    nodedata->sleep = 0;
}


/* ************************************************** */
/* ************************************************** */
/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
int get_modulation_bit_per_symbol(wsnet_t *ws, call_t *c){
    struct nodedata *nodedata = ws->get_node_private_data(c);
#if 1
    return ws->modulation_bit_per_symbol(nodedata->modulation);
#endif
}
/* end of edition */


/* ************************************************** */
/* ************************************************** */
int get_header_size(wsnet_t* ws, call_t *c) {
    return 0;
}

int get_header_real_size(wsnet_t* ws, call_t *c) {
    return 0;
}

int set_header(wsnet_t* ws, call_t *c, packet_t *packet, destination_t *dst) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
radio_methods_t methods = {rx, 
                           tx, 
                           set_header,
                           get_header_size,
                           get_header_real_size,
                           tx_end, 
                           cs,
                           get_noise,
                           get_cs,
                           get_power,
                           set_power,
                           get_channel,
                           set_channel,
                           get_modulation, 
                           set_modulation, 
                           get_Tb, 
                           get_Ts, 
                           set_Ts, 
                           get_sensibility,
                           set_sensibility, 
                           sleep, 
                           wakeup,
                           get_modulation_bit_per_symbol};
