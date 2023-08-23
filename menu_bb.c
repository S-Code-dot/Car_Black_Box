/*
 * File:   menu_bb.c
 * Author: Lenovo
 *
 * Created on 11 August, 2023, 11:18 AM
 */


#include <xc.h>
#include "main_bb.h"
#include "clcdbb.h"
#include "keypad_bb.h"
#include "download_bb.h"
char menu[5][17] = {"View log      ","Download log    ","Clear log      ","Set time      ","Change Password "};
extern int menu_flag;
char key = 0;
void _menu(int track, int p_track)
{
   
    //clear_scr();
    if(p_track)
    {
        clcd_putch(' ',LINE1(0));
        clcd_putch('*',LINE2(0));
    }
    else
    {
        clcd_putch('*',LINE1(0));
        clcd_putch(' ',LINE2(0));
    }
        clcd_print(menu[track],LINE1(2));
        clcd_print(menu[track + 1],LINE2(2)); 
}
void display_menu(void) 
{
  int p_track = 0;
  int track = 0;  
  clear_scr();
  unsigned long int delay = 0;
  char pre_key;
  while(1)
  {
      _menu(track , p_track);
      pre_key = key;
      key = read_switches(LEVEL_CHANGE);
      if(key != 0xFF)
      {
          if(delay++ == 1000)
          {
              delay = 0;
              track = track + p_track;
              if(key == MK_SW11)
              {
                  if(track == 0)
                      view_log();
                  else if(track == 1)
                      download_log();
                  else if(track == 2)
                      clear_log();
                  else if(track == 3)
                      set_time();
                  else if(track == 4)
                      change_password();
                  track=p_track= 0;
              }
              else if(key == MK_SW12)
              {
                  clear_scr();
                  break;
              }
          
          }
      }
      else if(delay < 1000 && delay > 0)
      {
         
          delay = 0;
          if(pre_key == MK_SW11)
          {
              if(p_track == 1)
               {
                 p_track = 0;
               }
              else
               {
                  if(track > 0)
                 track--; 
               }
          }
          else if(pre_key == MK_SW12)
          {
              
              if(p_track == 0)
               {
                 p_track = 1;
              }
              else
              {
                  if(track < 3)
                 track++;
               }
          }
       }
     
    }
  
  
}
