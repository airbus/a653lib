/*
 * Copyright (c) 2022-2023 Airbus Defence and Space
 *
 * This file is part of liba653.
 *
 * liba653 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * liba653 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with liba653; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file      a653_i_sync.c
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     a653 shared memory handling
 * @details
 */

#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <spawn.h>
//--------------------
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
//--------------------

#include "a653_i_time_lib.h"
#include "a653_i_sync.h"

#include "a653Init.h"

// Die globale Umgebungsvariable
extern char **environ;

typedef struct {
  int time_slice;
  int curr_partition_idx[MAX_CORE_NUM];
} partition_local_info_t;


struct timespec t1;
struct timespec t2;


int64_t diff;
extern a653_shm_info_t *shm_ptr;

extern a653_global_config_t global_config;
extern a653_channel_config_t channel_config[];


partition_local_info_t l_p_info;
long cores = 0; /* number of cores on this system */

void set_core_affinity(int pid, int core_id) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);


    if (core_id >= 0 && core_id >= cores){
      printDebug(0,"ERROR: core_id is out of range (%d) !!!\n",core_id);
      exit(1);
    }
    
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &mask) == -1) {
      printDebug(0,"ERROR: can not set core affinity !!!\n");
      exit(1);
    }
}

pid_t start_partition_process(char *p_name) {
    pid_t pid = -1;
    char *argv[] = {p_name, NULL}; // argv[0] sollte meist der Programmname sein

    // Prozess spawnen (file_actions und attrp sind hier NULL für Standardeinstellungen)
    int status = posix_spawn(&pid, p_name, NULL, NULL, argv, environ);

    if (status == 0) {
        printDebug(0,"child with PID %d started.\n", pid);
    } else {
        printDebug(0,"ERROR: posix_spawn failed!!!\n");
	exit(1);
    }

    return pid;
}


void a653_i_init_channels(void) {
  int idx;
  for (idx = 0; idx < MAX_CHANNEL; idx++){
    memset(&shm_ptr->channel_info[idx],0,sizeof(a653_channel_shm_info_t));
    shm_ptr->channel_info[idx].Id = channel_config[idx].ChannelId;
  }
}

void a653_i_init_sync(void) {

  char buf[256];
  int idx = 0;
  pid_t pid;

  
  if (a653_shm_init()){

    /* get number of cores on system */
    cores = sysconf(_SC_NPROCESSORS_ONLN);
    printDebug(0,"cores on system: %ld\n", cores);

    /* run scheduler on last core */
    set_core_affinity(getpid(),cores-1);

    /* clear total shared memory */
    memset(shm_ptr,0,sizeof(a653_shm_info_t));
    
    /* wait to kill running partitions - running will be zero */
    usleep(500);

    /* create channels on shared memory */
    a653_i_init_channels();
 
    /* go through all cores and set to idle */
    for (idx = 0;idx < global_config.core_number; idx++){
      l_p_info.curr_partition_idx[idx]  = -1;
    }

    /* now start all patitions in configuration */
    for (idx = 0;idx < global_config.partition_number; idx++){

      shm_ptr->partition_info[idx].init = 1;
      shm_ptr->partition_info[idx].running = 1;
      strcpy(shm_ptr->partition_info[idx].name, global_config.partition[idx].name_str);
      
      if (sem_init(&(shm_ptr->partition_info[idx].sem_lock),1,0) != 0) {
	printDebug(0,"ERROR: sem_int get failure!!!\n");
	exit(1);
      }
      
      //sem_wait(&(shm_ptr->partition_info[idx].sem_lock));
      /*
	sem_wait(&(fifo_ptr->sem_lock));
	sem_post(&(fifo_ptr->sem_lock));
      */

      /* start partition code now */ 
      sprintf(buf,"./%s",global_config.partition[idx].name_str);
      pid = start_partition_process(buf);

      /* check and store pid */
      if (pid > 0){
	shm_ptr->partition_info[idx].pid = pid;
	/* run first on core 0 */
	set_core_affinity(pid, 0); // run all first on core 0	
      } else {
	printDebug(0,"ERROR: can not start partition process!!!\n");
	exit(1); 
      }
      
      /* wait until partition initialisation is finished */
      while(shm_ptr->partition_info[idx].init == 1){
	usleep(50);
      }

      //   kill(shm_ptr->partition_info[idx].pid, SIGSTOP);
       
      printDebug(1,"a653 start (other pid) %d\n", shm_ptr->partition_info[idx].pid);

      usleep(500);
    } /* for */
   
    initTime();
    usleep(50000);

    for (idx = 0;idx < global_config.partition_number; idx++){
/* #ifndef S_DEBUG    */
/*       kill(shm_ptr->partition_info[idx].pid, SIGSTOP); */
/* #endif */
      usleep(500);
      shm_ptr->partition_info[idx].init = 1;
    }

    while(shm_ptr->partition_info[idx].init != 0){
      usleep(50);
    }

    usleep(500000);
    
  } else {
    printDebug(0,"ERROR: shared memory get failure!!!\n");
    exit(1);
  }
}

void a653_i_set_first(void){
  t2 = getTime();
  my_time_next(&t2,global_config.time_slice_size);
}

void a653_i_set_next(void){
  my_time_next(&t2,global_config.time_slice_size);
}

void a653_i_wait_next(void) {
  
  do {
    t1 = getTime();
    diff = my_time_diff(&t2,&t1);
    usleep(100);
  } while(diff > 0);

   a653_i_set_next();
  
}

void a653_i_update_partitions(void){
  
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
#ifndef S_DEBUG
	kill(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid, SIGSTOP);
#endif
      }
      /* schedule new partition if needed */
      if (global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx >= 0 &&
	  global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx < global_config. partition_number){
	/* partition must be started on this core */
	l_p_info.curr_partition_idx[core_idx] = global_config.time_slice[l_p_info.time_slice][core_idx].PatitionIdx;
	sem_post(&(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].sem_lock));
	/* new !!! */
	if (shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].core != core_idx){
	  set_core_affinity(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid, core_idx);
	  shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].core = core_idx;
	}
	//	printDebug(3,"SIGCONT %d\n",shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid);
#ifndef S_DEBUG
	kill(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].pid, SIGCONT);
#endif
      } else {
	/* no partition is scheduled in this time slice for this core */
	l_p_info.curr_partition_idx[core_idx]  = -1;
      }
    } else {
      if (l_p_info.time_slice == 0){
	sem_post(&(shm_ptr->partition_info[l_p_info.curr_partition_idx[core_idx]].sem_lock));
      }
    }
  } /* for all cores */

  // printDebug(6,"window %d\n",l_p_info.time_slice);

    /* update time slice */
  l_p_info.time_slice++;

  if (l_p_info.time_slice >= global_config.time_slice_number){
    l_p_info.time_slice = 0;
  }

}

