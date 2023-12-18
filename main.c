/**
 * @file      main.c
 * @copyright Airbus Defence and Space
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

#include <time.h>

#include "apexInit.h"
#include "apex_i_sync.h"
#include "apex_config.h"

/* apex_global_config_t global_config = APEX_PARTITION_CONFIG_DEF; */
/* apex_process_config_t APEX_PROCESS_CONFIG[] = APEX_PROCESS_CONFIG_DEF; */
/* apex_sampling_port_config_t APEX_SP_CONFIG[] = APEX_SP_CONFIG_DEF; */
/* apex_queuing_port_config_t APEX_QP_CONFIG[] = APEX_QP_CONFIG_DEF; */

/* int apex_prcs_num = sizeof(APEX_PROCESS_CONFIG)/sizeof(apex_process_config_t); */

int main (int argc, char *argv[]){

  time_t rawtime;
  struct tm * timeinfo;

  setDebug(5);
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printDebug(0,"Current local time and date: %s", asctime (timeinfo) );
  
  apex_i_init_sync();
  
  apex_i_set_first();
  while (1){
    printDebug(2,"start next loop\n");
    apex_i_update_partitions();
    apex_i_wait_next();
  }
  
  return 0;
}
