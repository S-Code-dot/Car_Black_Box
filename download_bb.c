/*
 * File:   main.c
 * Author: emertxe
 *
 * Created on June 14, 2023, 5:58 PM
 */


#include <xc.h>

#include "main_bb.h"
#include "download_bb.h"
#include "eeprom_bb.h"
#include "clcdbb.h"
//extern char gr_flag;
extern int wr_flag; 
//extern char ov_flag; 
extern char data[11];
extern char event[][3];
extern int s_lap;
void init_config(void)
{
	init_uart();

	puts("****Car Events****\n\r");
	 
}

void download_log(void)
{
    init_uart();
    check_event();
    //event_backup();
   // read_event();
    store_data(event[8]);
    save_data_to_eeprom(data);
    init_config();
    clear_scr();
    //check_event();
    for(int i = 0; i <= s_lap;i++ )
    {
        for(int j = 0;j < 10;j++)
        {
            if(j == 2 || j == 4)
                putch(':');
            else if(j == 6 || j == 8)
                putch(' ');
		    putch(read_external_eeprom(i * 10 + j));
        }
        puts("\n\r");
    }
    clcd_print("  Download Done ",LINE1(0));
    for(int i=1000;i--;)
        for(int j = 1000;j--;);
    
}