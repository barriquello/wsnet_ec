/**
 *  \file   scheduler.h
 *  \brief  Scheduler declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __scheduler_public__
#define __scheduler_public__

#include "types.h"


/** 
 * \brief Schedule the callback of a function at a given time.
 * \param clock time of the callback.
 * \param c the call parameter given to the callback function.
 * \param callback the function that is called back.
 * \param arg a paramater that given to the callback function.
 * \return An opaque object that references the callback-associated event.
 **/
event_t *scheduler_add_callback(uint64_t clock, call_t *c, callback_t callback, void *arg);


/** 
 * \brief Delete an event from the Scheduler.
 * \param c the call parameter given to the callback function.
 * \param event a paramater that describe the event we want to delete.
 **/
void scheduler_delete_callback(call_t *c, event_t *event);


#endif //__scheduler_public__
