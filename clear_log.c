/*
 * File:   clear_log.c
 * Author: Lenovo
 *
 * Created on 21 August, 2023, 7:56 PM
 */


#include <xc.h>
#include "main_bb.h"
#include "clcdbb.h"
extern int wr_flag;
extern char event[][3];
extern char data[11];
void clear_log(void) {
   check_event();
   event_backup(); 
   wr_flag = -1;
   clear_scr();
   clcd_print("Log cleared  ",LINE1(2));
     for(int i=0;i<500;i++)
         for(int j=1000;j>0;j--);
     store_data(event[9]);
     save_data_to_eeprom(data);
}
