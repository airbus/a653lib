/* a653_config.h */

#ifndef __A653_CONFIG_H
#define __A653_CONFIG_H

/* pseudo processes */
#define MILBUS_AV_NUM 50
#define MILBUS_CP_NUM 51
#define MILBUS_WP_NUM 52
#define PANLINK_NUM   60

#include "a653Init.h"


#define D_TIME_SLICE 1000000ll  /* 1000000ns = 1ms */

#ifdef __WASM_RT__
#define PART_NAME_A "wasm32_rt partition_a.wasm"
#define PART_NAME_B "wasm32_rt partition_b.wasm"
#else
#define PART_NAME_A "partition_a"
#define PART_NAME_B "partition_b"
#endif


#define A653_PARTITION_CONFIG_DEF { \
  4,   /* core number used*/ \
  2,   /* number of partition */ \
  10,  /* number of time slices */ \
  D_TIME_SLICE,  /* time slice size in nsec */ \
  { /*0*/{PART_NAME_A},\
    /*1*/{PART_NAME_B},\
    /*2*/{""},\
    /*3*/{""},\
    /*4*/{""},\
    /*5*/{""},\
    /*6*/{""},\
    /*7*/{""},\
    /*8*/{""},\
    /*9*/{""}\
  },		\
  {  /*0*/{ {0},   {-1},   {-1},   {-1} }, \
     /*1*/{ {0},   {-1},   {-1},   {-1} }, \
     /*2*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*3*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*4*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*5*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*6*/{ {1},   {-1},   {-1},   {-1} }, \
     /*7*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*8*/{ {-1},  {-1},   {-1},   {-1} }, \
     /*9*/{ {-1},  {-1},   {-1},   {-1} } \
  }, \
  0xaffe \
}

#define CHANNEL_SP_TYPE 1
#define CHANNEL_QP_TYPE 2

/*
typedef struct {
  unsigned short ChannelId;
  unsigned short ChannelType;
  unsigned short maxMsgSize;
} a653_channel_config_t;
*/
#define A653_CH_CONFIG_DEF {\
    { 1, CHANNEL_SP_TYPE, 50 },\
    { 2, CHANNEL_SP_TYPE, 50 },\
    { 3, CHANNEL_QP_TYPE, 50 },\
    { 4, CHANNEL_QP_TYPE, 50 },\
    { 0, 0              , 0  }\
}
/*
typedef struct {
  unsigned short ChannelId;
  unsigned short Dir;
  char           name_str[32];
} a653_sampling_port_config_t;

typedef struct {
  unsigned short ChannelId;
  unsigned short Dir;
  unsigned short Type;
  char           name_str[32];  
} a653_queuing_port_config_t;


typedef struct {
  unsigned short PartitionId;
  char           name_str[32];
  a653_sampling_port_config_t sp_config[MAX_S_PORT];
  a653_queuing_port_config_t qp_config[MAX_Q_PORT];
} a653_partition_config_t;
*/

#define A653_PART_CONFIG_DEF { \
  { 1, PART_NAME_A,\
    {\
        /* 01 */ {1,SOURCE,"SP_Prcs_A to Prcs B             "},\
	/* 02 */ {2,DESTINATION,"SP_Prcs_B to Prcs A             "},\
	/* 03 */ {0,0,""},\
	/* 04 */ {0,0,""},\
	/* 05 */ {0,0,""},\
	/* 06 */ {0,0,""},\
	/* 07 */ {0,0,""},\
	/* 08 */ {0,0,""},\
	/* 09 */ {0,0,""},\
	/* 10 */ {0,0,""},\
    },\
    {\
        /* 01 */ {3,SOURCE,2,"QP_Prcs_A to Prcs B             "},	\
	/* 02 */ {4,DESTINATION,2,"QP_Prcs_B to Prcs A             "},	\
	/* 03 */ {0,0,0,""},\
	/* 04 */ {0,0,0,""},\
	/* 05 */ {0,0,0,""},\
	/* 06 */ {0,0,0,""},\
	/* 07 */ {0,0,0,""},\
	/* 08 */ {0,0,0,""},\
	/* 09 */ {0,0,0,""},\
	/* 10 */ {0,0,0,""},\
    }\
  },\
  { 2, PART_NAME_B,\
    {\
        /* 01 */ {1,DESTINATION, "SP_Prcs_A to Prcs B             "},\
	/* 02 */ {2,SOURCE, "SP_Prcs_B to Prcs A             "},\
	/* 03 */ {0,0,""},\
	/* 04 */ {0,0,""},\
	/* 05 */ {0,0,""},\
	/* 06 */ {0,0,""},\
	/* 07 */ {0,0,""},\
	/* 08 */ {0,0,""},\
	/* 09 */ {0,0,""},\
	/* 10 */ {0,0,""},\
	},\
    {\
        /* 01 */ {3,DESTINATION,2,"QP_Prcs_A to Prcs B             "},\
	/* 02 */ {4,SOURCE,2,"QP_Prcs_B to Prcs A             "},	\
	/* 03 */ {0,0,0,""},\
	/* 04 */ {0,0,0,""},\
	/* 05 */ {0,0,0,""},\
	/* 06 */ {0,0,0,""},\
	/* 07 */ {0,0,0,""},\
	/* 08 */ {0,0,0,""},\
	/* 09 */ {0,0,0,""},\
	/* 10 */ {0,0,0,""},\
    }\
  },\
}

#endif /* __A653_CONFIG_H */

