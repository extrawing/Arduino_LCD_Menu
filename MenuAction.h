#ifndef _MenuAction_h_
# define _MenuAction_h_

enum MENU_ACTION { MENU_ACTION_NONE, MENU_ACTION_UP, MENU_ACTION_DOWN, MENU_ACTION_SELECT, MENU_ACTION_BACK };
typedef enum MENU_ACTION_RESULT { MENU_ACTION_RESULT_NONE, MENU_ACTION_RESULT_RETAIN_DISPLAY } MENU_ACTION_RESULT;

#endif // __MenuAction_h__
