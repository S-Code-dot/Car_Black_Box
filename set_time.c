/*
 * File:   set_time.c
 * Author: Soleman Mollick
 *
 * Created on 21 August, 2023, 8:16 PM
 */


#include <xc.h>
#include "main_bb.h"
#include "rtc_bb.h"
#include "clcdbb.h"
#include "keypad_bb.h"
int sec = 0;
int min = 0;
int hr = 0;
unsigned char time[9];
int field_flag = 1;
extern char event[][3];
extern char data[11];
void current_time()
{
   
    hr = (time[0] - 48) * 10 ;
    hr = (time[1]-48) + hr;
    min = (time[3] - 48) * 4;
    min = (time[4]-48) + min;
    sec = (time[6]-48) * 10;
    sec = time[7]-48 + sec;   
}
void save_time(void)
{
    char hr = (time[0] - 48) << 4 ;
    hr = (time[1]-48) | hr;
    char min = (time[3] - 48) << 4;
    min = time[4] | min;
    char sec = (time[6]-48) << 4;
    sec = time[7] | sec;
   write_ds1307(HOUR_ADDR,hr);
   write_ds1307(MIN_ADDR,min);
   write_ds1307(SEC_ADDR,sec); 
}
void store_time(void)
{
    time[0] = hr / 10 + 48;
    time[1] = hr % 10 + 48;
    time[2] = ':';
    time[3] = min / 10 + 48;
    time[4] = min % 10 + 48;
    time[5] = ':';
    time[6] = sec / 10 + 48;
    time[7] = sec % 10 + 48;
    time[8] = '\0';
    
}
void set_time(void) {
   clear_scr();
   read_rtc(); 
   clcd_print("HH:MM:SS",LINE1(2));
   clcd_print(time,LINE2(2));
   current_time();
   char pre_key;
   char key;
   int delay=0;
   unsigned long int wait = 0;
   while(1)
   {
       store_time();
       if(wait ++ <= 1000)
        {  
           clcd_putch((sec / 10) + '0',LINE2(8)); 
           clcd_putch((sec % 10) + '0',LINE2(9));
           clcd_putch((min / 10) + '0',LINE2(5)); 
           clcd_putch((min % 10) + '0',LINE2(6));
           clcd_putch((hr / 10) + '0',LINE2(2)); 
           clcd_putch((hr % 10) + '0',LINE2(3));     
        }
        else if(wait > 1000 && wait < 2000)
        {
            if(field_flag == 1)
              clcd_print("  ",LINE2(8));
            if(field_flag == 2)
              clcd_print("  ",LINE2(5));
            if(field_flag == 3)
              clcd_print("  ",LINE2(2));
        }
        else 
        {
            wait = 0;
        } 
       pre_key = key;
       key = read_switches(LEVEL_CHANGE);
       if(key != 0xff)
       {
           if(delay++ == 1000)
           {
               delay = 0;
               if(key == MK_SW11)
               {
                  
                   store_data(event[10]);
                   save_data_to_eeprom(data);
                   save_time();
                   break;
               }
               else if(key == MK_SW12)
               {
                   break;
               }
                       
           }
       }
       else if(delay < 1000 && delay > 0)
       {
           delay = 0;
           if(pre_key == MK_SW11)
           {
             if(field_flag  == 1)
             {
                if(sec++ == 60)
                {
                    sec = 0;
                }
               clcd_putch((sec / 10) + '0',LINE2(8)); 
               clcd_putch((sec % 10) + '0',LINE2(9));
             }
                else if(field_flag  == 2)
                {
                    if(min++ == 60)
                    {
                        min = 0;
                    }
                 clcd_putch((min / 10) + '0',LINE2(5)); 
                 clcd_putch((min % 10) + '0',LINE2(6));
                }
                else if(field_flag  == 3)
                {
                    if(hr < 24)
                        hr++;
                    else
                        hr = 0;
               clcd_putch((hr / 10) + '0',LINE2(2)); 
               clcd_putch((hr % 10) + '0',LINE2(3));
                }
             }
           
             else if(pre_key == MK_SW12)
             {
               if(field_flag < 3)
                   field_flag ++;
               else
                   field_flag  = 1;
             }
       }
       }
     
   }
   

