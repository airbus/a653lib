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
#include <signal.h>
//--------------------
#include <errno.h>

#include <time.h>

#include "a653Init.h"
#include "a653_config.h"

#include "a653_i_shm_if.h"

extern int shm_id;
extern a653_shm_info_t *shm_ptr;

a653_global_config_t global_config = A653_PARTITION_CONFIG_DEF; 
a653_channel_config_t channel_config[] = A653_CH_CONFIG_DEF;
a653_partition_config_t partition_config[] = A653_PART_CONFIG_DEF;

#ifdef S_TRACE
void dump_trace(void){
  int tIdx = 0;
  FILE *fd;

  fd=fopen("/tmp/trace.csv","wb");

  if (fd != ( FILE *)-1){
    
    fprintf(fd,"sec.usec;pid;tid;code\n");
  
    for(tIdx = 0; tIdx < MAX_TRACE_ENTRIES; tIdx++){
      fprintf(fd,"%lu.%09lu;%d;%d;%d\n",
	      shm_ptr->trace_info.entry[tIdx].time.tv_sec,
	      shm_ptr->trace_info.entry[tIdx].time.tv_nsec,
	      shm_ptr->trace_info.entry[tIdx].pid,
	      shm_ptr->trace_info.entry[tIdx].tid,
	      (int)shm_ptr->trace_info.entry[tIdx].code);
    }
    
    fclose(fd);
  }
}
#endif

void sig_handler(int sig){
  int idx;
  char buf[256];
  
  switch(sig) {
  case SIGSEGV:
    fprintf(stderr,"SIGSEGV !!!! \n");
    abort();
  case SIGINT:
  case SIGQUIT:
  case SIGKILL:
    fprintf(stderr,"scheduler was stopped\n");
#ifdef S_TRACE
    dump_trace();
#endif
    for (idx = 0;idx < global_config.partition_number; idx++){
      sprintf(buf,"pkill %s",global_config.partition[idx].name_str);
      system(buf);
    }
    memset(shm_ptr,0,sizeof(a653_shm_info_t));
    a653_shm_remove(&shm_id);
    abort();
  }
}



int main (int argc, char *argv[]){

  int64_t rawtime;
  struct tm * timeinfo;

  signal(SIGINT,sig_handler);
  signal(SIGQUIT,sig_handler);
  signal(SIGKILL,sig_handler);
  signal(SIGSEGV,sig_handler);
  
  setDebug(3);

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printDebug(0,"Current local time and date: %s", asctime (timeinfo) );

  a653_i_init_sync();
  
  a653_i_set_first();
  while (1){
    printDebug(6,"start next loop\n");
    a653_i_update_partitions();
    a653_i_wait_next();
  }
  
  return 0;
}
