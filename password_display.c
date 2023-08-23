/*
 * File:   password_display.c
 * Author: Soleman Mollick
 *
 * Created on 10 August, 2023, 6:13 PM
 */


#include <xc.h>
#include "main_bb.h"
#include "clcdbb.h"
#include "keypad_bb.h"
#include "main_bb.h"
#include "eeprom_bb.h"

extern char dash_flag;
extern char password_flag;
extern char menu_flag;
int delay = 0;
int count = 0;
int pos = 0;
char main_pswd[5] = "1111";
char user_pswd[5] = {};
char key;
extern int wr_flag;
void clear_scr()
{
    clcd_print("                ",LINE1(0));
    clcd_print("                ",LINE2(0));
}
void store_password()
{
    for(int i=0;i<5;i++)
    {
       write_external_eeprom((100+i),main_pswd[i]);
    }
}

int check_password()
{
    for(int i=0;i<5;i++)
    {
        main_pswd[i] = read_external_eeprom(100+i);
        if(user_pswd[i] != main_pswd[i])
        {
            return 0;
        }
    }
    return 1;
    
}
void wrong_password(int attempt)
{
    clear_scr();
    
    while(delay++ <= 2000)
    {   
      clcd_print("x Try Again",LINE1(0));
      clcd_putch('0'+attempt,LINE2(0));
      clcd_print("attempt left",LINE2(2));
    }
    
}
void password_display()
{
    
    static int attempt = 3;
    store_password();  // After Storing once for initially commenting the function call..
    clear_scr();
    clcd_print(" Enter Password ",LINE1(0));
    while(attempt > 0)
    {
      if(delay++ <= 500)
         clcd_putch('_',LINE2(count));
      else if(delay <= 1000)
         clcd_putch(' ',LINE2(count));
      else
         delay = 0;
      
      key = read_switches(STATE_CHANGE); 
      for(int i=200;i>0;i--);
      if (count < 4 && key == MK_SW11) 
      {
         
         clcd_putch('*', LINE2(count));
         user_pswd[count] = '0';
         count++;
      } 
      else if (count < 4 && key == MK_SW12) 
      {
       
         clcd_putch('*', LINE2(count));
         user_pswd[count] = '1';
         count++;
      }
      else if(count == 4)
      {
          
         user_pswd[count] = '\0';
         count++;
      }
      else if(count == 5)
      {
  
         if (check_password() == 0)
         {
             attempt--;
             if(attempt > 0)
             {
                
               wrong_password(attempt);
               clear_scr();
               clcd_print(" Enter Password ",LINE1(0));
               count = 0;
             }
             else
             {
                  
               clear_scr();
               clcd_print("  Login Failed  ",LINE1(0));
               for(int i=1000;i>0;i--)
                 for(int j=1000;j>0;j--);
               clcd_print(" Enter Password ",LINE1(0));
               attempt = 3;
               count = 0;
             }
         }
         else
         {
             clear_scr();
             attempt = 3;
             count = 0;
             clcd_print("Login Success",LINE1(0));
             for(int i=3000;i>0;i--);
             password_flag = 0;
             display_menu();
             break;
         }   
         
      }
      
    }
} 
