/*
 * File:   change_passwd.c
 * Author: Lenovo
 *
 * Created on 14 August, 2023, 11:26 AM
 */


#include <xc.h>
#include "clcdbb.h"
#include "keypad_bb.h"
#include "adc_bb.h"
#include "eeprom_bb.h"
#include "main_bb.h"
int count1 = 0,count2 = 0;
extern char main_pswd[5];
extern char user_pswd[5];
extern char event[][3];
extern char data[11];
char key;
int delay;
unsigned char user1[4];
unsigned char user1[4];
int ask_password(int count,int stage)
{
    key = read_switches(STATE_CHANGE);
    for(int i=100;i>0;i--);
           if(delay++ <= 500)
              clcd_putch('_',LINE2(count));
           else if(delay <= 1000)
              clcd_putch(' ',LINE2(count));
           else
              delay = 0;
           if(stage == 1)
           {
               if (key == MK_SW11) 
               {
                 clcd_putch('*', LINE2(count));
                 main_pswd[count] = '0';
                 count++;
               } 
               else if (key == MK_SW12) 
               {
                 clcd_putch('*', LINE2(count));
                 main_pswd[count] = '1';
                 count++;
               }
           }
           else if(stage == 2)
           {
              if (key == MK_SW11) 
               {
                 clcd_putch('*', LINE2(count));
                 user_pswd[count] = '0';
                 count++;
               } 
               else if (key == MK_SW12) 
               {
                 clcd_putch('*', LINE2(count));
                 user_pswd[count] = '1';
                 count++;
               } 
           }
    return count;
}
int match_password(void)
{
    int i = 0;
    while(main_pswd[i] != '\0')
    {
        if(user_pswd[i] != main_pswd[i])
        {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
void change_password(void) 
{
    store_data(event[11]);
    save_data_to_eeprom(data);
    int once = 1;
    clear_scr();
    while(1)
    {
      
       if(count1 < 4)
       {
           clcd_print("New Password  ",LINE1(0)); 
           count1 = ask_password(count1,1);
       }
       else if(count1 == 4 && count2 < 4)
       {
          clcd_print("Renter Password  ",LINE1(0));
          if(once)
          {
               clcd_print("     ",LINE2(0));
               once = 0;
          }
          count2 = ask_password(count2,2);
       }
       else
       {
         main_pswd[4] = '\0';
         user_pswd[4] = '\0';
         if(match_password() == FALSE)
         {
             clear_scr();
             clcd_print("Wrong Match",LINE1(2));
                 for(int j=1000;j>0;j--)
                     for(int i= 1000; i--;);
                 count1 = 0;
                 count2 = 0;
             break;
             
         }
         else
         {
             clear_scr();
             store_password();
             clcd_print("Password_changed",LINE1(0));
             for(int j=1000;j>0;j--)
                     for(int i= 1000;i>0;i--);
                     clear_scr();
               count1 = 0;
               count2 = 0;     
             break;
         }
       }
    }
}
