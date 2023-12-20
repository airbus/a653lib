/**
 * @file      init.c
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     partition main function
 * @details 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//--------------------
#include <errno.h>

#include "a653Init.h"
#include "a653_config.h"

a653_global_config_t global_config = A653_PARTITION_CONFIG_DEF;
a653_process_config_t A653_PROCESS_CONFIG[] = A653_PROCESS_CONFIG_DEF;
a653_sampling_port_config_t A653_SP_CONFIG[] = A653_SP_CONFIG_DEF;
a653_queuing_port_config_t A653_QP_CONFIG[] = A653_QP_CONFIG_DEF;

int a653_prcs_num = sizeof(A653_PROCESS_CONFIG)/sizeof(a653_process_config_t);

