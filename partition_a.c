/**
 * @file      prcs_a.c
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     partition main function
 * @details 
 */


#include <stdio.h>
#include <string.h>
#include "a653Lib.h"
/* #include "a653_config.h" */

/* a653_global_config_t global_config = A653_PARTITION_CONFIG_DEF; */
/* a653_process_config_t A653_PROCESS_CONFIG[] = A653_PROCESS_CONFIG_DEF; */
/* a653_sampling_port_config_t A653_SP_CONFIG[] = A653_SP_CONFIG_DEF; */
/* a653_queuing_port_config_t A653_QP_CONFIG[] = A653_QP_CONFIG_DEF; */


void PeriodicProcess(void){
  RETURN_CODE_TYPE return_code;
  MESSAGE_SIZE_TYPE length;
  VALIDITY_TYPE     validity;

  SYSTEM_TIME_TYPE system_time;
  
  unsigned short  count = 0;  

  A653_BYTE data_sp_tx[32] = "sp_test1234";
  A653_BYTE data_sp_rx[32] = "           ";
  A653_BYTE data_qp_tx[32] = "qp_test1234";
  A653_BYTE data_qp_rx[32] = "           ";
  
  SAMPLING_PORT_ID_TYPE sp_id_tx;
  SAMPLING_PORT_ID_TYPE sp_id_rx;
  
  QUEUING_PORT_ID_TYPE qp_id_tx;
  QUEUING_PORT_ID_TYPE qp_id_rx;

  GET_SAMPLING_PORT_ID ("SP_Prcs_A to Prcs B             ",
			&sp_id_tx,
			&return_code);
  GET_SAMPLING_PORT_ID ("SP_Prcs_B to Prcs A             ",
			&sp_id_rx,
			&return_code);
  
  GET_QUEUING_PORT_ID("QP_Prcs_A to Prcs B             ",
		      &qp_id_tx,
		      &return_code);
  GET_QUEUING_PORT_ID("QP_Prcs_B to Prcs A             ",
		      &qp_id_rx,
		      &return_code);


  int index = 0;
  
  while (1){

    count++;
    
    sprintf((char *)data_sp_tx,"Prcs A: 0x%04x",count);	

    WRITE_SAMPLING_MESSAGE(sp_id_tx,          /* sampling port id */
			   data_sp_tx,        /* pointer to data */
			   32,                /* send size */ 
			   &return_code);     /* return code */

    
    SEND_QUEUING_MESSAGE(qp_id_tx,
			 data_qp_tx,
			 30, // size
			 0, //timeout
			 &return_code);
    
#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
    printDebug(3,"%06d Prcs A send %s \n",index++,data_sp_tx);
#endif /* #ifndef __wasm__ */

    READ_SAMPLING_MESSAGE(sp_id_rx,
    			  data_sp_rx,
    			  &length,
    			  &validity,
    			  &return_code);
#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
    if (validity == VALID && length > 0){
      printDebug(3,"Prcs A: SP we got this : >%s<\n",(char *)data_sp_rx);
    } else {
      printDebug(3,"Prcs A:  validity >%d< length >%d<\n",validity,length);
    }
#endif /* #ifndef __wasm__ */
    
    RECEIVE_QUEUING_MESSAGE(qp_id_rx,        
			    0,                /* time out */
    			    data_qp_rx,       /* pointer to data */
    			    &length,          /* received length */
    			    &return_code);    /* return code */
#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
    if(return_code == NO_ERROR && length !=0){
      printDebug(3,"Prcs A: QP we got this : >%s<\n",(char *)data_qp_rx);
    } else {
      printDebug(3,"Prcs A: return >%d<\n",return_code);
    }
#endif /* #ifndef __wasm__ */

    GET_TIME (&system_time, &return_code);
#ifndef __wasm__ /* Do not expose non ARINC653 functions into WebAssembly */
    printDebug(3,"Prcs A: GET_TIME >%lld<\n",system_time);
#endif /* #ifndef __wasm__ */
    
    PERIODIC_WAIT(&return_code);
  }
}

/* have a look to init.c beforeMain is called before main!!!!! */
int main (int argc, char *argv[]){
  
  RETURN_CODE_TYPE               return_code;
  RETURN_CODE_TYPE               Init_Process_ret;
  PROCESS_ID_TYPE                Init_process_ID;
  PROCESS_ATTRIBUTE_TYPE         process_data;
  
  A653_INTEGER                   PortId;
  
  int ret_val = 0;

  /*                    0        1         2         3   */ 
  /*                    01234567890123456789012345678901 */ 
  CREATE_SAMPLING_PORT("SP_Prcs_A to Prcs B             ",
		       32, // Size in Bytes
		       0,  // 0 tx : 1 rx
		       40000000,                     // currently unused
		       &PortId,
		       &return_code
		       );

  /*                    0        1         2         3   */ 
  /*                    01234567890123456789012345678901 */ 
  CREATE_SAMPLING_PORT("SP_Prcs_B to Prcs A             ",
		       32, // Size in Bytes
		       1,  // 0 tx : 1 rx
		       40000000,                     // currently unused
		       &PortId,
		       &return_code
		       );

  /*                   0        1         2         3   */ 
  /*                   01234567890123456789012345678901 */ 
  CREATE_QUEUING_PORT("QP_Prcs_A to Prcs B             ",
  		      32, // Size in Bytes
  		      4,  // number of entries
  		      0,  // 0 tx : 1 rx
  		      FIFO,
  		      &PortId,
  		      &return_code
  		      );

  /*                   0        1         2         3   */ 
  /*                   01234567890123456789012345678901 */ 
  CREATE_QUEUING_PORT("QP_Prcs_B to Prcs A             ",
  		      32, // Size in Bytes
  		      4,  // number of entries
  		      1,  // 0 tx : 1 rx
  		      FIFO,
  		      &PortId,
  		      &return_code
  		      );
  
  //Start initialization
  // InitSamplingPorts();

   //Initialization of the processes
   memset((char*)(process_data.NAME), 0, sizeof(process_data.NAME));
   sprintf((char*)(process_data.NAME), "Process A                    ");
   process_data.PERIOD = 100000000LL; /* nsec 0,1 sec*/
   process_data.TIME_CAPACITY = 0;
   process_data.STACK_SIZE = 0x5000000;
   process_data.ENTRY_POINT = &PeriodicProcess; //Entrypoint to periodic process
   process_data.BASE_PRIORITY = 40 ;
   process_data.DEADLINE = SOFT;


   CREATE_PROCESS(&process_data, &Init_process_ID, &Init_Process_ret);

   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */


   //Creation of ports
   /* for(portCnt=0;portCnt<NbOfPorts;portCnt++) */
   /* { */
   /*    CREATE_SAMPLING_PORT(Ports_cfg[portCnt].PortName, */
   /*                         Ports_cfg[portCnt].MsgSize, */
   /*                         Ports_cfg[portCnt].Direction, */
   /*                         Ports_cfg[portCnt].RefreshPeriod, */
   /*                        &(Ports[portCnt].PortID), */
   /*                        &(Ports[portCnt].Create_ret)); */
   /*    Ports[portCnt].size = Ports_cfg[portCnt].MsgSize; */
   /* } */

   //Starting processes
   START(Init_process_ID, &Init_Process_ret);

   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */


   //Set normal mode
   SET_PARTITION_MODE(NORMAL, &Init_Process_ret);
   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */

   return ret_val;
}
