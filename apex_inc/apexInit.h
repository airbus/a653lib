/**
 * @file      apexInit.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     do apex interface initialization
 * @details 
 */

#ifndef __APEXINIT_H__
#define __APEXINIT_H__

#define MAX_PRCS_ID 0xff

#define MAX_PARTITION 10
#define MAX_PRCS 5

#define MAX_TIME_SLICE_NUM 10
#define MAX_CORE_NUM 2

#define MAX_Q_PORT 3
#define MAX_Q_PORT_ENTRIES 5
#define MAX_Q_PORT_SIZE 200

#define MAX_S_PORT 10
#define MAX_S_PORT_SIZE 200

typedef struct {
  char name_str[33];
} apex_partition_config_t;

typedef struct {
  unsigned short PatitionIdx;
} apex_time_slice_config_t;

typedef struct {
  int core_number;
  int partition_number;
  int time_slice_number;
  time_t time_slice_size; /* value in nsec */
  apex_partition_config_t partition[MAX_PARTITION];
  apex_time_slice_config_t time_slice[MAX_TIME_SLICE_NUM][MAX_CORE_NUM];
  int magic;
} apex_global_config_t;


typedef struct {
  unsigned short PrcsId;
  unsigned short PartitionIdx;
  char           name_str[33];
} apex_process_config_t;

typedef struct {
  unsigned short PortId;
  unsigned short SrcPartitionIdx;
  unsigned short DstPartitionIdx;
  char           name_str[32];
} apex_sampling_port_config_t;

typedef struct {
  unsigned short PortId;
  unsigned short SrcPartitionIdx;
  unsigned short DstPartitionIdx;
  unsigned short Type;
  char           name_str[32];
  
} apex_queuing_port_config_t;




int apex_init_queuing_ports(int max_port_num, apex_queuing_port_config_t *config);
int apex_init_sampling_ports(int max_port_num, apex_sampling_port_config_t *config);

void apex_i_init_sync(void);

void setDebug(int level);
void printDebug(int level,const char *format, ... );

#endif /* __APEXINIT_H__ */
