/**
 * @file      main.c
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

#include "a653Init.h"
#include "a653_config.h"

a653_global_config_t global_config = A653_PARTITION_CONFIG_DEF; 
a653_sampling_port_config_t A653_SP_CONFIG[] = A653_SP_CONFIG_DEF; 
a653_queuing_port_config_t A653_QP_CONFIG[] = A653_QP_CONFIG_DEF; 

int main (int argc, char *argv[]){

  int64_t rawtime;
  struct tm * timeinfo;

  setDebug(5);
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printDebug(0,"Current local time and date: %s", asctime (timeinfo) );
  
  a653_i_init_sync();
  
  a653_i_set_first();
  while (1){
    printDebug(2,"start next loop\n");
    a653_i_update_partitions();
    a653_i_wait_next();
  }
  
  return 0;
}
