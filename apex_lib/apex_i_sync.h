/**
 * @file      apex_i_sync.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */


#ifndef __APEX_I_SYNC_H__
#define __APEX_I_SYNC_H__

#include "apex_i_shm_if.h"


void apex_i_init_sync(void);
void apex_i_set_next(void);
void apex_i_wait_next(void);
void apex_i_update_partitions(void);


#endif /* __APEX_I_SYNC_H__ */
