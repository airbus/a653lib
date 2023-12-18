/**
 * @file      apex_i_sync.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//--------------------
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
//--------------------

#include "apex_i_time_lib.h"
#include "apex_i_sync.h"

#include "apexInit.h"

typedef struct {
  int time_slice;
  int curr_partition_idx[MAX_CORE_NUM];
} partition_local_info_t;


struct timespec t1;
struct timespec t2;


int64_t diff;

extern apex_global_config_t global_config;
extern apex_shm_info_t *shm_ptr;

partition_local_info_t l_p_info;


void apex_i_init_sync(void) {

  char buf[256];
  int idx = 0;
  
  if (apex_shm_init()){
      
    memset(shm_ptr,0,sizeof(apex_shm_info_t));

    /* t1 = getTime(); */
    /* sleep(1); */
    /* t2 = getTime(); */

    /* diff = my_time_diff(&t2,&t1); */

    /* printDebug(3,"timediff: %ld\n\n",diff); */

    /* l_p_info.time_slice = 0; */
  
    /* go through all cores and set to idle */
    for (idx = 0;idx < global_config.core_number; idx++){
      l_p_info.curr_partition_idx[idx]  = -1;
    }
  
    for (idx = 0;idx < global_config.partition_number; idx++){
      sprintf(buf,"taskset --cpu-list %d ./%s &",idx,global_config.partition[idx].name_str);
	
      printDebug(0,"> %s : \n\n",buf);
      system(buf);

      //	printDebug(buf);

      {

	strcpy(buf,"pidof ");
	strcat(buf,global_config.partition[idx].name_str);
	//printDebug(3,">> %s \n",buf);
	FILE *fp = popen(buf,"r");
	if (fp != 0){
	  fscanf(fp, "%d",&shm_ptr->partition_info[idx].pid);
	  pclose(fp);
	}
      }
    
      strcpy(shm_ptr->partition_info[idx].name, global_config.partition[idx].name_str);
      shm_ptr->partition_info[idx].init = 1;
    
      while(shm_ptr->partition_info[idx].init){
	usleep(50);
      }

      kill(shm_ptr->partition_info[idx].pid, SIGSTOP);
      printDebug(3,"a653 start (other pid) %d\n", shm_ptr->partition_info[idx].pid);

    } /* for */
  
    initTime();

  }
}

void apex_i_set_first(void){
  t2 = getTime();
  my_time_next(&t2,global_config.time_slice_size);
}

void apex_i_set_next(void){
  my_time_next(&t2,global_config.time_slice_size);
}

void apex_i_wait_next(void) {
  do {
    t1 = getTime();
    diff = my_time_diff(&t2,&t1);
    usleep(10);
  } while(diff > 0);

   apex_i_set_next();
  
}

void apex_i_update_partitions(void){
  
  int core_idx = 0;


  /* go through all cores */
  for (core_idx = 0;core_idx < global_config.core_number; core_idx++){

    /* do we have a change ? */
    if (global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx !=
	l_p_info.curr_partition_idx[core_idx]) {

      /* partition change for this core */
      /* deactivate current if active */
      if (l_p_info.curr_partition_idx[core_idx] >= 0 &&
	  l_p_info.curr_partition_idx[core_idx] < global_config. partition_number) {
	/* current partition must be stopped on this core */
	//	printDebug(3,"SIGSTOP %d\n",shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid);
	kill(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid, SIGSTOP);
      }
      /* schedule new partition if needed */
      if (global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx >= 0 &&
	  global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx < global_config. partition_number){
	/* partition must be started on this core */
	l_p_info.curr_partition_idx[core_idx] = global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx;
	shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].go = 1;
	//	printDebug(3,"SIGCONT %d\n",shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid);
	kill(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid, SIGCONT);
      } else {
	/* no partition is scheduled in this time slice for this core */
	l_p_info.curr_partition_idx[core_idx]  = -1;
      }
    }    
  } /* for all cores */

  printDebug(3,"window %d\n",l_p_info.time_slice);

    /* update time slice */
  l_p_info.time_slice++;

  if (l_p_info.time_slice >= global_config.time_slice_number){
    l_p_info.time_slice = 0;
  }

}

