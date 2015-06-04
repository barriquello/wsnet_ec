/**
 *  \file   types.h
 *  \brief  Type declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __types__
#define __types__

#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "das.h"


/** \typedef array_t
 * \brief An array of integers containing its size.
 **/
/** \struct _array 
 * \brief An array of integers containing its size. Should use type array_t.
 **/
typedef struct _array {
    int size;  /**< array size **/
    int *elts; /**< array elements **/
} array_t;


/** \typedef nodeid_t 
 * \brief A node identifier. 
 **/
/** \struct _nodeid
 * \brief A node identifier. Should use type nodeid_t.
 **/
typedef int nodeid_t;


/** \typedef entityid_t 
 * \brief An entity identifier. 
 **/
/** \struct _entityid 
 * \brief An entity identifier. Should use type entityid_t.
 **/
typedef int entityid_t;


/** \typedef measureid_t
 * \brief A measure identifier. 
 **/
/** \struct _measureid
 * \brief A measure identifier. Should use type measureid_t.
 **/
typedef int measureid_t;


/** \typedef packetid_t 
 * \brief A packet identifier. 
 **/
/** \struct _packetid
 * \brief A packet identifier. Should use type packetid_t.
 **/
typedef int packetid_t;


/** \typedef position_t
 * \brief A position in the 3D space. 
 **/
/** \struct _position
 * \brief A position in the 3D space. Should use type position_t.
 **/
typedef struct _position {
    double x; /**< x position **/
    double y; /**< y position **/ 
    double z; /**< z position **/ 
} position_t;


/** \typedef angle_t
 * \brief An angle in the 3D space.
 **/
/** \struct _angle
 * \brief An angle in the 3D space. Should use type angle_t.
 **/
typedef struct _angle {
    double xy; /**< angle on the xy plane **/ 
    double z;  /**< angle between the xy plane and the z axis**/ 
} angle_t;


/** \typedef destination_t
 * \brief A packet destination. May be a node address or a geographical position.
 **/
/** \struct _destination
 * \brief A packet destination. May be a node address or a geographical position. Should use type destination_t.
 **/
typedef struct _destination {
    nodeid_t id;         /**< the destination node id **/
    position_t position; /**< the destination position **/
} destination_t;


/** \typedef call_t
 * \brief A parameter that identifies who we are calling and who has called us. Kind of a self pointer.
 **/
/** \struct _call
 * \brief A parameter that identifies who we are calling and who has called us. Kind of a self pointer. Should use type call_t.
 **/
typedef struct _call {
    entityid_t entity; /**< the called entity id **/
    nodeid_t node;     /**< the called node id **/
    entityid_t from;   /**< the entity that made the call **/
} call_t;


/** \typedef packet_t
 * \brief A radio packet.
 **/
/** \struct _packet
 * \brief A radio packet. Should use type packet_t.
 **/
typedef struct _packet {
    packetid_t id;         /**< the packet id **/
    int size;              /**< size of the packet data **/
    int real_size;         /**< real size of the packet data **/
    int type;              /**< type of the packet (for multistandard nodes) **/

    uint64_t clock0;       /**< packet rx start **/
    uint64_t clock1;       /**< packet rx end **/
    uint64_t duration;     /**< packet tx/rx duration **/

    nodeid_t node;         /**< node that has created the packet **/
    entityid_t antenna;    /**< antenna that has emitted the packet **/

    /* edit by Loic Lemaitre */
    int    worldsens_mod;  /**< worldsens modulation id (wsim radio modulation id) **/
    double worldsens_freq; /**< worldsens radio frequency **/
    /* end of edition */

    double txdBm;          /**< tx power in dBm **/
    int channel;          
    entityid_t modulation; /**< modulation entity **/
    uint64_t Tb;           /**< radio bandwidth: time to send a bit **/

    double PER;            /**< packet error rate **/
    double rxdBm;          /**< rx power in dBm **/
    double rxmW;           /**< rx power in mW **/
    double *noise_mW;      /**< packet noise in mW**/
    double *ber;           /**< packet ber **/

    char *data;            /**< packet data **/
} packet_t;


/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
/** \typedef ioctl_message_t
 * \brief An ioctl message.
 **/
/** \struct _io_ctl_message
 * \brief An ioctl message. Should use type ioctl_message_t.
 **/
typedef struct _ioctl_message{
    /* message body type */
    int type;
    /* message body size */
    int size;
    /* message body real size */
    int real_size;
    /* message body */
    void *body;
} ioctl_message_t;
/* end of edition */


/** \typedef param_t
 * \brief A parameter for the "init" and "setnode" entity functions.
 **/
/** \struct _param
 * \brief A parameter for the "init" and "setnode" entity functions. Should use type param_t.
 **/
typedef struct _param {
    char *key;   /**< the parameter key **/
    char *value; /**< the parameter value **/
} param_t;


/** \typedef callback_t 
 * \brief Prototype of a callback function. 
 **/
typedef int (* callback_t) (void* ws, call_t *c, void *arg);

/** \typedef event_t
 * \brief A scheduler event.
 **/
/** \struct _event
 * \brief A scheduler event. Should use type event_t.
 **/
typedef struct _event {
    uint64_t clock;    /* event time     */
    int      priority; /* event priority */
    int      id;       /* event id       */

    union {
        struct {
            call_t call;
            callback_t callback;
            void *arg;
        } cb;
        struct {
            packet_t *packet;
            call_t call;
        } rx;
        nodeid_t nodeid;
    } u;

} event_t;


typedef struct
{
	uint64_t (*get_time)(void);
	position_t * (*get_node_position)(nodeid_t);
	int (*get_node_count)(void);
	position_t * (*get_topology_area)(void);
	double (*distance)(position_t *position0, position_t *position1);
	double (*get_random_distance)(void);
	double (*get_random_x_position)(void);
	double (*get_random_y_position)(void);
	double (*get_random_z_position)(void);
	double (*get_random_double)(void);
	double (*get_random_double_range)(double min, double max);
	int (*get_random_integer)(void);
	int (*get_random_integer_range)(int min, int max);
	uint64_t (*get_random_time)(void);
	uint64_t (*get_random_time_range)(uint64_t min, uint64_t max);
	nodeid_t (*get_random_node)(nodeid_t exclusion);
	void (*set_node_private_data)(call_t *, void *);
	void* (*get_node_private_data)(call_t *);
	int (*get_entity_type)(call_t *c);
	void (*set_entity_private_data)(call_t *, void *);
	void * (*get_entity_private_data)(call_t *);
	array_t * (*get_entity_bindings_up)(call_t *);
	array_t * (*get_entity_bindings_down)(call_t *);
	entityid_t * (*get_entity_links_down)(call_t *);
	int (*get_entity_links_down_nbr)(call_t *c);
	void (*das_init_traverse)(void *);
	void *(*das_traverse)(void *);
	int (*das_init)(void);
	void * (*das_create)(void);
	void (*das_destroy)(void *das);
	int (*das_getsize)(void *das);
	void (*das_insert)(void *das, void *data);
	void * (*das_pop)(void *das);
	void * (*das_pop_FIFO)(void *das);
	int (*das_find)(void *das, void *data);
	void (*das_delete)(void *das, void *data);
	void (*das_selective_delete)(void *d, das_delete_func_t delete, void *arg);
	event_t * (*scheduler_add_callback)(uint64_t clock, call_t *c, callback_t callback, void *arg);
	void (*scheduler_delete_callback)(call_t *c, event_t *event);
	int (*get_param_x_position)(char *, double *);
	int (*get_param_y_position)(char *, double *);
	int (*get_param_z_position)(char *, double *position);
	int (*get_param_distance)(char *, double *);
	int (*get_param_double)(char *, double *);
	int (*get_param_double_range)(char *, double *, double, double);
	int (*get_param_time)(char *, uint64_t *);
	int (*get_param_integer)(char *, int *);
	int (*get_param_nodeid)(char *, nodeid_t *, nodeid_t);
	int (*get_param_entity)(char *, entityid_t *);
	packet_t * (*packet_clone) (packet_t *);
	packet_t * (*packet_create)(call_t *, int, int);
	packet_t * (*packet_alloc)(call_t *, int);
	void (*packet_dealloc)(packet_t *);
	void (*radio_cs)(call_t *, packet_t *);
	int (*modulation_bit_per_symbol)(entityid_t);
	void (*TX)(call_t *, packet_t *);
	void (*RX)(call_t *, packet_t *);
	int (*IOCTL)(call_t *, int, void *, void **);
	int (*SET_HEADER)(call_t *, packet_t *, destination_t *);
	int (*GET_HEADER_SIZE)(call_t *);
	int (*GET_HEADER_REAL_SIZE)(call_t *);
	void (*MEDIA_TX)(call_t *, packet_t *);
	double (*MEDIA_GET_NOISE)(call_t *, int);
}wsnet_t;


#endif //__types__
