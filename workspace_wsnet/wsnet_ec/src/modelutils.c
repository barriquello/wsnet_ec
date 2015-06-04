/**
 *  \file   modelutils.c
 *  \brief  Model utilities
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "types.h"
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
void TX(call_t *c, packet_t *packet)
{
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    entity->methods->generic.tx(&wsnet, c, packet);
    ENTITY_CLOSE(entity);
}

void RX(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    entity->methods->generic.rx(&wsnet, c, packet);
    ENTITY_CLOSE(entity);
}

int IOCTL(call_t *c, int option, void *in, void **out) {
	int ret;
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    	ret = entity->ioctl(&wsnet, c, option, in, out);
    ENTITY_CLOSE(entity);
    return ret;
}

/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */

int GET_HEADER_SIZE(call_t *c) {
	int ret;
    entity_t *entity = get_entity_by_id(c->entity);
    ENTITY_OPEN(entity);
    	ret = entity->methods->generic.get_header_size(&wsnet, c);
    ENTITY_CLOSE(entity);
    return ret;
}

int SET_HEADER(call_t *c, packet_t *packet, destination_t *dst) {
   int ret;
   entity_t *entity = get_entity_by_id(c->entity);
   ENTITY_OPEN(entity);
		ret = entity->methods->generic.set_header(&wsnet, c, packet, dst);
   ENTITY_CLOSE(entity);
   return ret;
}

 
int GET_HEADER_REAL_SIZE(call_t *c) {
   int ret;
   entity_t *entity = get_entity_by_id(c->entity);
   ENTITY_OPEN(entity);
		ret = entity->methods->generic.get_header_real_size(&wsnet, c);
   ENTITY_CLOSE(entity);
   return ret;
}

/* end of edition */

/* edit by CHB <barriquello@gmail.com> */
wsnet_t wsnet =
{
		get_time,
		get_node_position,
		get_node_count,
		get_topology_area,
		distance,
		get_random_distance,
		get_random_x_position,
		get_random_y_position,
		get_random_z_position,
		get_random_double,
		get_random_double_range,
		get_random_integer,
		get_random_integer_range,
		get_random_time,
		get_random_time_range,
		get_random_node,
		set_node_private_data,
		get_node_private_data,
		get_entity_type,
		set_entity_private_data,
		get_entity_private_data,
		get_entity_bindings_up,
		get_entity_bindings_down,
		get_entity_links_down,
		get_entity_links_down_nbr,
		das_init_traverse,
		das_traverse,
		das_init,
		das_create,
		das_destroy,
		das_getsize,
		das_insert,
		das_pop,
		das_pop_FIFO,
		das_find,
		das_delete,
		das_selective_delete,
		scheduler_add_callback,
		scheduler_delete_callback,
		get_param_x_position,
		get_param_y_position,
		get_param_z_position,
		get_param_distance,
		get_param_double,
		get_param_double_range,
		get_param_time,
		get_param_integer,
		get_param_nodeid,
		get_param_entity,
		packet_clone,
		packet_create,
		packet_alloc,
		packet_dealloc,
		radio_cs,
		modulation_bit_per_symbol,
		TX,
		RX,
		IOCTL,
		SET_HEADER,
		GET_HEADER_SIZE,
		GET_HEADER_REAL_SIZE,
		MEDIA_TX,
		MEDIA_GET_NOISE,
};

/* end of edition */

