/**
 *  \file   antenna.c
 *  \brief  Antenna utilities
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
double antenna_get_loss(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);

    ENTITY_OPEN(entity);
    double ret;
    if (entity->model->type != MODELTYPE_ANTENNA)
        ret = 0;
    else
    	ret = entity->methods->antenna.get_loss(&wsnet, c);
    ENTITY_CLOSE(entity);
    return ret;
}

angle_t *antenna_get_angle(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    angle_t* ret;
    if (entity->model->type != MODELTYPE_ANTENNA)
        ret = 0;
    else
    	ret = entity->methods->antenna.get_angle(&wsnet, c);
    ENTITY_CLOSE(entity);
    return ret;
}

void antenna_set_angle(call_t *c, angle_t *angle) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
    {
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->antenna.set_angle(&wsnet, c, angle);
    ENTITY_CLOSE(entity);

}

void antenna_rx(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
    {
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->antenna.rx(&wsnet, c, packet);
    ENTITY_CLOSE(entity);

}

double  antenna_gain_tx(call_t *c, position_t *pos) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
    if (entity->model->type != MODELTYPE_ANTENNA)
        ret = 0;
    else
    	ret = entity->methods->antenna.gain_tx(&wsnet, c, pos);
    ENTITY_CLOSE(entity);
    return ret;
}

double  antenna_gain_rx(call_t *c, position_t *pos) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
    if (entity->model->type != MODELTYPE_ANTENNA)
        ret = 0;
    else
    	ret = entity->methods->antenna.gain_rx(&wsnet, c, pos);
    ENTITY_CLOSE(entity);
    return ret;
}


