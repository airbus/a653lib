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
#include <unistd.h>

//--------------------
#include <errno.h>

#include "a653Init.h"
#include "a653_config.h"

#include "a653Type.h"
#include "a653Error.h"
#include "a653Process.h"
#include "a653Partition.h"

#include "a653_i_process.h"
#include "a653_i_shm_if.h"
#include "a653_i_sync.h"

extern a653_shm_info_t *shm_ptr;

extern int own_partition_idx;
extern PARTITION_STATUS_TYPE pertition_status;

a653_global_config_t global_config = A653_PARTITION_CONFIG_DEF;
a653_channel_config_t channel_config[] = A653_CH_CONFIG_DEF;
a653_partition_config_t partition_config[] = A653_PART_CONFIG_DEF;

/* */
void beforeMain (void) __attribute__ ((constructor));

void beforeMain (void)
{
  int idx = 0;;
  
  setDebug(2);
  
  printDebug(1,"a653 partition init (%d)\n",getpid());
  a653_init_partition();
  pertition_status.OPERATING_MODE = COLD_START;
  shm_ptr->partition_info[own_partition_idx].init = 2;

  printDebug(1,"a653 partition wait (%d)\n",getpid());
  
  while (shm_ptr->partition_info[own_partition_idx].init == 2){
    if((idx++ % 0x100000) == 0){
      printDebug(1,"a653 partition wait (%d)\n",getpid());
      idx = 0;
    }
    usleep(5000);
  }
}
