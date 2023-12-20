/* a653_config.h */

#ifndef __A653_CONFIG_H
#define __A653_CONFIG_H

#define PRCS_A 1
#define PRCS_B 2

/* pseudo processes */
#define MILBUS_AV_NUM 50
#define MILBUS_CP_NUM 51
#define MILBUS_WP_NUM 52
#define PANLINK_NUM   60

#include "a653Init.h"


#define MS10 10000000ll
#define S1 1000000000ll
#define WT 100000000ll

#define A653_PARTITION_CONFIG_DEF { \
  2,   /* core number used*/ \
  2,   /* number of partition */ \
  10,  /* number of time slices */ \
  WT,  /* time slice size in nsec */ \
  { /*0*/{"partition_a"},\
    /*1*/{"partition_b"},\
    /*2*/{""},\
    /*3*/{""},\
    /*4*/{""},\
    /*5*/{""},\
    /*6*/{""},\
    /*7*/{""},\
    /*8*/{""},\
    /*9*/{""}\
  },		\
  {  /*0*/{ {0},{-1} }, \
     /*1*/{ {-1},{-1} }, \
     /*2*/{ {-1},{-1} }, \
     /*3*/{ {-1},{-1} }, \
     /*4*/{ {-1},{-1} }, \
     /*5*/{ {-1},{-1} }, \
     /*6*/{ {1},{-1} }, \
     /*7*/{ {-1},{-1} }, \
     /*8*/{ {-1},{-1} }, \
     /*9*/{ {-1},{-1} } \
  }, \
  0xaffe \
}


/* string must be 30 bytes long */
#define A653_PROCESS_CONFIG_DEF { \
  /*process id                              */ \
  /*|      name                             */ \
    {PRCS_A, 0,"Process A                      " },\
    {PRCS_B, 1,"Process B                      " },	\
    {0, 0,     "" }\
}






/* string must be 30 bytes long */
#define A653_SP_CONFIG_DEF { \
    /*              0        1         2         3   */		\
    /*              01234567890123456789012345678901 */		\
    { 1000, 0, 1 , "SP_Prcs_A to Prcs B             " }, \
    { 1001, 1, 0 , "SP_Prcs_B to Prcs A             " }, \
    { 0, 0, 0,     "END                             " }  /* END OF TABLE MARKER   */ \
}



/* typedef struct { */
/*   unsigned short PortId; */
/*   unsigned short SrcPrcs; */
/*   unsigned short DstPrcs; */
/*   unsigned short Type; */
/*   char           name_str[32]; */
/* } a653_queuing_port_config_t; */


/* string must be 30 bytes long */
#define A653_QP_CONFIG_DEF { \
    /*      | rx - tx                            */	     \
    /*                0        1         2         3   */ \
    /*                01234567890123456789012345678901 */  \
    { 1000, 0, 1, 2, "QP_Prcs_A to Prcs B             " }, \
    { 1001, 1, 0, 2, "QP_Prcs_B to Prcs A             " }, \
    { 0, 0, 0, 0,    "END                             " }  /* END OF TABLE MARKER   */ \
}

/* #include "a653_pseudo_ports.h" */




/* #define A653_PSEUDO_SAMPLING_PORT_CONFIG_DEF { \ */
/*     { 2000 , D_AV_MILBUS, {{{ D_MODE_TX, 7, 1, 8, 0 }}} },		\ */
/*     { 0, 0,               {{{         0, 0, 0, 0, 0 }}} }		\ */
/* }  */

/* #define A653_PSEUDO_QUEUING_PORT_CONFIG_DEF { \ */
/*     { 2000, D_AV_MILBUS,  {{{ D_MODE_TX, 0, 0, 32, 0 }}} },	\ */
/*     { 0, 0,               {{{         0, 0, 0, 0, 0 }}} }	\ */
/* } */

#endif /* __A653_CONFIG_H */

