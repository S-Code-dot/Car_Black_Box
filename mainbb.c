/*
 * File:   mainbb.c
 * Author: Soleman Mollick
 *
 * Created on 9 August, 2023, 11:22 AM
 */


#include <xc.h>
#include "clcdbb.h"
#include "keypad_bb.h"
#include "download_bb.h"
#include "adc_bb.h"
#include "i2c_bb.h"
#include "eeprom_bb.h"
#include "main_bb.h"
#include "rtc_bb.h"

unsigned char time[9];
char event[][3] = {"ON","GR","GN","G1","G2","G3","G4","C ","DL","CL","ST","CP"};
char data[11]={}; 
char gr_flag = 0;
char key;
int speed = 0;
char dash_flag = 1;
char password_flag = 0;
char menu_flag = 1;
int wr_flag = -1;
char ov_flag = 0;
extern int s_lap;

void int_config(void) {
    PEIE = 0;
    GIE = 0;
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_i2c();
    init_ds1307();
}
void event_backup(void)
{
    check_event();
    write_external_eeprom(104,s_lap/10);
    write_external_eeprom(104,s_lap%10);
}
void line1(void)
{
  clcd_print("TIME",LINE1(0));
  clcd_print("EVNT",LINE1(9));
  clcd_print("SP",LINE1(14));
}
void line2(char gr_flag,int speed)
{
  clcd_print(time,LINE2(0));
  clcd_print(event[gr_flag],LINE2(9));
  clcd_putch((speed / 10) + 48,LINE2(14));
  clcd_putch((speed % 10) + 48,LINE2(15));
}

void store_data(char *event)
{
    int i = 0 ; int j = 0;
    while(time[i] != '\0')
    {
        if(time[i] == ':')
        {
            i++;
            continue;
        }
        data[j++] = time[i];
        i++;
    }
    i = 0; 
    while(event[i] != '\0')
    {
        data[j++] = event[i];
        i++;
    }
    
        data[j++] = speed / 10 + 48;
        data[j] = speed % 10 + 48;
}

void save_data_to_eeprom(char *data)
{
 
    if(wr_flag < 9)
        wr_flag++;
     else
    {
        ov_flag = 1;
        wr_flag = 0;
    }
   
    for(int i=0;i<10;i++)
    {
       write_external_eeprom((wr_flag * 10) + i, data[i]); 
    }   
}

void read_rtc(void)
{
    
    unsigned char arr[3];
    arr[0] = read_ds1307(HOUR_ADDR);
    time[1] = (arr[0] & 0x0F) + '0';
    time[0] = ((arr[0] >> 4) & 0x01) + '0';
    time[2] = ':';
    arr[1] = read_ds1307(MIN_ADDR);
    time[4] = (arr[1] & 0x0F) + '0';
    time[3] = ((arr[1] >> 4) & 0x07) + '0';
    time[5] = ':';
    arr[2] = read_ds1307(SEC_ADDR);
    time[7] = (arr[2] & 0x0F) + '0';
    time[6] = ((arr[2] >> 4) & 0x07) + '0';
    time[8] = '\0';
    
}
void main(void) {
     int_config();
     read_rtc();
     line1();
     line2(gr_flag,speed);
     store_data(event[gr_flag]);
     save_data_to_eeprom(data);  
     
    while(1)
    {
        
        key = read_switches(STATE_CHANGE); 
        for(int i=200;i>0;i--);      // Ignore bouncing effect.
        if(key == MK_SW11)
        {
            password_flag = 1;
            dash_flag = 0;
            menu_flag = 0;
        }
        
        if(dash_flag)
        {
          check_event();
          event_backup();
          password_flag = 0;
          read_rtc();
          line1();
          line2(gr_flag,speed);
     
          if(key == MK_SW2)
          {
            if(gr_flag == 7)
                  gr_flag = 2;
            else if(gr_flag < 6)
             gr_flag++; 
             store_data(event[gr_flag]);
             save_data_to_eeprom(data);  
          }
          else if(key == MK_SW3)
          {
            if(gr_flag == 7)
                gr_flag = 2;
            else if(gr_flag > 1)
              gr_flag--; 
            store_data(event[gr_flag]);
            save_data_to_eeprom(data);   
          }
          else if(key == MK_SW1)
          {
             gr_flag = 7;
              store_data(event[gr_flag]);
              save_data_to_eeprom(data);  
          }
      
          if(gr_flag != 0)
             speed = read_adc(4) / 10.33;    // Scaling b/w 0 - 99; 
          else
             speed = 0;
          
        }
        else if(password_flag == 1)
        {
            password_display();
            password_flag = 0;
            dash_flag = 1;
        }
     
    }
}
