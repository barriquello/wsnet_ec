/**
 *  \file   radio.c
 *  \brief  Radio functions
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
double radio_get_noise(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = MIN_DBM;
	else
		ret = entity->methods->radio.get_noise(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

double radio_get_cs(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = MIN_DBM;
	else
		ret = entity->methods->radio.get_cs(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

double radio_get_power(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = MIN_DBM;
	else
		ret = entity->methods->radio.get_power(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

void radio_set_power(call_t *c, double power) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_RADIO){
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->radio.set_power(&wsnet,c, power);
    ENTITY_CLOSE(entity);
}

int radio_get_channel(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    int ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = -1;
	else
		ret = entity->methods->radio.get_channel(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

void radio_set_channel(call_t *c, int channel) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_RADIO){
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->radio.set_channel(&wsnet,c, channel);
    ENTITY_CLOSE(entity);

}

entityid_t radio_get_modulation(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    entityid_t ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = -1;
	else
		ret = entity->methods->radio.get_modulation(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

void radio_set_modulation(call_t *c, entityid_t modulation) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_RADIO){
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->radio.set_modulation(&wsnet,c, modulation);
    ENTITY_CLOSE(entity);
}

uint64_t radio_get_Tb(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    uint64_t ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = 0;
	else
		ret = entity->methods->radio.get_Tb(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
} 

void radio_set_Ts(call_t *c, uint64_t Ts) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_RADIO){
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->radio.set_Ts(&wsnet, c, Ts);
    ENTITY_CLOSE(entity);

}

void radio_cs(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    if (entity->model->type != MODELTYPE_RADIO){
    	ENTITY_CLOSE(entity);
    	return;
    }
    entity->methods->radio.cs(&wsnet,c, packet);
    ENTITY_CLOSE(entity);
}

void radio_set_sensibility(call_t *c, double sensibility) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
	if (entity->model->type != MODELTYPE_RADIO){
		ENTITY_CLOSE(entity);
		return;
	}
	entity->methods->radio.set_sensibility(&wsnet,c, sensibility);
	ENTITY_CLOSE(entity);

}

double radio_get_sensibility(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    double ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = MIN_DBM;
	else
		ret = entity->methods->radio.get_sensibility(&wsnet,c);
	ENTITY_CLOSE(entity);
	return ret;
}

void radio_sleep(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
	if (entity->model->type != MODELTYPE_RADIO){
		ENTITY_CLOSE(entity);
		return;
	}
	entity->methods->radio.sleep(&wsnet,c);
	ENTITY_CLOSE(entity);

}

void radio_wakeup(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
	if (entity->model->type != MODELTYPE_RADIO){
		ENTITY_CLOSE(entity);
		return;
	}
	entity->methods->radio.wakeup(&wsnet,c);
	ENTITY_CLOSE(entity);
}


/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
int radio_get_modulation_bit_per_symbol(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    int ret;
	if (entity->model->type != MODELTYPE_RADIO)
		ret = 0;
	else
		ret = entity->methods->radio.get_modulation_bit_per_symbol(&wsnet,c);
	ENTITY_CLOSE(entity);
    return ret;
}
/* end of edition */

