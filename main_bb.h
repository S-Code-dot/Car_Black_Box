#ifndef MAIN_H
#define MAIN_H
int view_flag;
int download_flag;
int clear_flag;
int set_flag;
int change_p;
int press_time;


#define FALSE 0
#define TRUE  1


void clear_scr(void);
void password_display(void);
void display_menu(void);
void clear_scr(void);
void _menu(int , int);
void change_password(void);
void store_password(void);
void view_log(void);
void read_rtc(void);
void store_data(char *);
void save_data_to_eeprom(char *);
void download_log(void);
void clear_log(void);
void set_time(void);
void check_event(void);
void read_event(void);
void event_backup(void);
#endif