/*
 Arduino_LCD_Menu Library
Copyright Dustin Andrews, David Andrews 2012 
Licensed under the follwing license:

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef MenuEntry_h
#define MenuEntry_h 1

#include <Arduino.h>
#include "MenuAction.h"


typedef MENU_ACTION_RESULT (*MENU_ACTION_CALLBACK_FUNC)( const char * pMenuText, void * pUserData );


//To use these functions, pass a function pointer as the argument to the MenuEntry constructor.
//pUserData should point to an unsigned int that will be set to true or false.
void MenuEntry_BoolTrueCallbackFunc( char * pMenuText, void * pUserData );
void MenuEntry_BoolFalseCallbackFunc( char * pMenuText, void * pUserData );

//Use this callback function for a "Back" menu item for hardware that doesn't include a back button
//pUserData should point to a MenfsuManager object.
template <class T>
MENU_ACTION_RESULT MenuEntry_BackCallbackFunc( const char * pMenuText, void * pUserData );

template <class T>
class MenuManager;

//The MenuEntry class represents one menu item in the overall menu system, such as "Set Time" or "Back"
//The MenuEntry classes point to each other to create a tree of menu items.  You can navigate
// the classes using the get* calls.  MenuManager uses the get* calls to figure out what to draw to the LCD
template <class T>
class MenuEntry
{
  public:
  //Constructor to create each entry.
  MenuEntry( const char * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func);
  MenuEntry( const __FlashStringHelper * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func);
  //add a child menu item.  They will be kept it the order they are added, from top to bottom.
  bool addChild( MenuEntry* child);
  //Add a menu item as a sibling of this one, at the end of the sibling chain.
  bool addSibling( MenuEntry* sibling);
  //Sets the previous sibling, mostly used during menu creation to notify a new entry where it's
  //previous pointer needs to point.
  void setPrevSibling( MenuEntry* prevSibling);
  //Can set the action call back dynamically. Overrides what was passed to the constructor.
  bool addActionCallback( MENU_ACTION_CALLBACK_FUNC pCallback);
  
  const char* getMenuText();
  bool isProgMem();
  //Sets the previous sibling, mostly used during menu creation to notify a new entry where it's
  //previous pointer needs to point.
  void setParent( MenuEntry* parent );
  
  MenuEntry<T> *getNextSibling();
  MenuEntry<T> *getPrevSibling();
  MenuEntry<T> *getChild();
  MenuEntry<T> *getParent();
  //This call will call the action callback for use when the menu item is selected.
  //if this menu entry has any children, the callback will not be executed.
  MENU_ACTION_RESULT ExecuteCallback();

  bool isBackEntry() { return (m_callback == MenuEntry_BackCallbackFunc<T>); }
  
  private:
  void* m_userData;
  const char* m_menuText;
  bool m_isProgMem;
  MenuEntry<T>* m_parent;
  MenuEntry<T>* m_child;
  MenuEntry<T>* m_nextSibling;
  MENU_ACTION_CALLBACK_FUNC m_callback;
  MenuEntry<T>* m_prevSibling;
};

template <class T>
MenuEntry<T>::MenuEntry( const char * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func):
	m_userData(userData), m_menuText(menuText), m_parent(NULL), m_child(NULL),
	m_nextSibling(NULL), m_callback(func), m_prevSibling(NULL), m_isProgMem(false) {}

template <class T>
MenuEntry<T>::MenuEntry( const __FlashStringHelper * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func):
	m_userData(userData), m_menuText((const char*)menuText), m_parent(NULL), m_child(NULL),
	m_nextSibling(NULL), m_callback(func), m_prevSibling(NULL), m_isProgMem(true) {}

template <class T>
MENU_ACTION_RESULT MenuEntry<T>::ExecuteCallback()
{
  if( m_callback != NULL )
  {
    return m_callback(m_menuText, m_userData);
  }
  return MENU_ACTION_RESULT_NONE;
}

template <class T>
bool MenuEntry<T>::addChild(MenuEntry<T>* child)
{
  child->setParent( this );
  if(m_child != NULL)
  {
    m_child->addSibling( child );
  }
  else
  {
    m_child = child;
  }
  return true;
}

template <class T>
bool MenuEntry<T>::addSibling( MenuEntry<T>* sibling)
{
  sibling->setParent( m_parent );
  if( m_nextSibling != NULL )
  {
    m_nextSibling->addSibling(sibling);
  }
  else
  {
    m_nextSibling = sibling;
    sibling->setPrevSibling( this );
  }
  return true;
}

template <class T>
void MenuEntry<T>::setPrevSibling( MenuEntry<T> * pPrevSibling)
{
  m_prevSibling = pPrevSibling;
}

template <class T>
const char * MenuEntry<T>::getMenuText()
{
  return m_menuText;
}

template <class T>
bool MenuEntry<T>::isProgMem()
{
  return m_isProgMem;
}

template <class T>
MenuEntry<T> *MenuEntry<T>::getNextSibling()
{
  return m_nextSibling;
}

template <class T>
MenuEntry<T> *MenuEntry<T>::getPrevSibling()
{
  return m_prevSibling;
}

template <class T>
MenuEntry<T> *MenuEntry<T>::getChild()
{
  return m_child;
}

template <class T>
MenuEntry<T> *MenuEntry<T>::getParent()
{
  return m_parent;
}

template <class T>
void MenuEntry<T>::setParent( MenuEntry<T> * parent)
{
  m_parent = parent;
}

template <class T>
void MenuEntry_BoolTrueCallbackFunc( const char * pMenuText, void * pUserData )
{
  *((unsigned int *)pUserData) = true;
}

template <class T>
void MenuEntry_BoolFalseCallbackFunc( const char * pMenuText, void * pUserData )
{
  *((unsigned int *)pUserData) = false;
}

//template <class T>
//void MenuEntry_BoolToggleCallbackFunc( char * pMenuText, void * pUserData )
//{
//  *((unsigned int *)pUserData) =  !*((unsigned int *)pUserData) ;
//}

template <class T>
MENU_ACTION_RESULT MenuEntry_BackCallbackFunc( const char * pMenuText, void * pUserData )
{
  ((MenuManager<T> *)pUserData)->DoMenuAction( MENU_ACTION_BACK );
  return MENU_ACTION_RESULT_NONE;
}


#endif

