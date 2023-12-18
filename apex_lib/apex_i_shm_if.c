/**
 * @file      apex_i_shm_if.c
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

#include <sys/shm.h>

#include "apexType.h"

#define APEX_APEX_SHM_IF_INTERN

#include "apex_i_shm_if.h"


#define APEX_SHM_KEY_FILE    "/apex_shm.dat"   /* apex shm memory     */
#define TOTAL_APEX_SHM_SIZE  0xffffff          /* Shared memory size  */


int    shm_id  = 0;
apex_shm_info_t *shm_ptr = NULL;

/*
 * Basic function to get a shared memory segment
 */
void* apex_shm_get( int* shm_id, char* key_file, size_t segsize ) {

  char  key_full[512];
  key_t key;
  FILE  *fp;
  int flags = IPC_CREAT|0600;
  void* shm_ptr = 0x0;

  strcpy(key_full, (char*)getenv("HOME"));
  strcat(key_full, key_file);

  /* test key file */
  if ( ( fp = fopen( key_full, "w")) == (FILE*)0 ) {
    fprintf(stderr, "Could not open key file: %s\n", key_full);
    exit(1);
  }
  fclose(fp);

  /* Create unique key via call to ftok() */
  key = ftok(key_full, 'X');

  if(key == -1) {
    fprintf(stderr, "Could not create key from %s\n", key_full);
    exit(1);
  }

  /* Open the shared memory segment - create if necessary */
  *shm_id = shmget( key, segsize, flags );

  if( *shm_id == -1 ) {
    fprintf(stderr, "Could not open/create share memory for %s\n", key_full);
    exit(1);
  }

  /* Attach (map) the shared memory segment into the current process */
  shm_ptr = shmat( *shm_id, 0, 0 );

  if( !shm_ptr ) {
    fprintf(stderr, "Could not map shared memory into process for %s\n", key_full);
    exit(1);
  }

  printDebug(0,"apex_shm id: 0x%04x ptr: %p\n",*shm_id,shm_ptr);

  //  memset(shm_ptr,0,TOTAL_APEX_SHM_SIZE);

  return shm_ptr;
}


/*
 * Basic function to detach from a shared memory
 */
int apex_shm_remove( int* shm_id) {

  if ( shm_id != 0 ) {
    return shmctl( *shm_id, IPC_RMID, NULL);
  }
  return -1;
}


/*
 * get ptr to shared memory
 */
int apex_shm_init(void){
  int ret_val = 0;

  if (shm_ptr == NULL){
    if ((shm_ptr = apex_shm_get(&shm_id, APEX_SHM_KEY_FILE, TOTAL_APEX_SHM_SIZE)) != 0){
      ret_val = 1;
    }
  }
  return(ret_val);
}

