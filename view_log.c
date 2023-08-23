/*
 * File:   view_log.c
 * Author: Soleman Mollick
 *
 * Created on 14 August, 2023, 10:17 AM
 */


#include <xc.h>
#include "clcdbb.h"
#include "keypad_bb.h"
#include "adc_bb.h"
#include "eeprom_bb.h"
#include "main_bb.h"

extern int wr_flag;
extern char ov_flag;
char key;
char pre_key;
int s_lap,in=0;
extern char event[][3];
void check_event(void)
{
   if(ov_flag == 0)
   {
       s_lap = wr_flag;
   }
   else 
       s_lap = 10;
 
}
void read_event(void)
{
    s_lap = (read_external_eeprom(104) - 48) * 10;
    s_lap = (read_external_eeprom(105) - 48) + s_lap;
}
void print_log(void)
{
  char data = 0,j = 1; 
  
  for(int i=0;i<10;i++)
  {
      
      if(i == 2 || i == 4)
      {
          clcd_putch(':',LINE2(j));
          j++;
          
      }
      else if(i == 6 || i == 8)
      {
          clcd_putch(' ',LINE2(j));
          j++;
      }
      
         data = read_external_eeprom(((s_lap+in) % 10) *10+i);
         clcd_putch(data,LINE2(j));
         j++;
  }
  
}
void view_log(void) 
{
    
    clear_scr();
    clcd_print("   My Logs       ",LINE1(0));
    check_event();
    int delay = 0;
    in=0;
    if(wr_flag != -1)
    {
       while(1)
       {
        
          print_log();
          pre_key = key;
          key = read_switches(LEVEL_CHANGE);
          if(key != 0xFF)
          {
            if(delay++ == 1000)
            {
              delay = 0;
              if(key == MK_SW12)
              {
                clear_scr();
                break;
              }
            }
         }
         else if(delay < 1000 && delay > 0)
         {
           delay = 0;
           if(pre_key == MK_SW12)
           {
             if(in < s_lap)
               in++;
           }
           else if(pre_key == MK_SW11)
           {
             if(in > 0)
               in--;
           }
         }
       
       }
    }
    else
    {
       clear_scr();
       clcd_print(" No Log Saved   ",LINE2(0));
       for(int i=0;i<5000;i++);    
    }
}