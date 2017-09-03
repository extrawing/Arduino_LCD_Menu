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


#include "MenuEntry.h"
#include "MenuLCD.h"
#include "MenuAction.h"
#include "MenuIntHelper.h"

template <class T>
class MenuManager
{
  public:
  MenuManager(MenuLCD<T>* pMenuLCD);
  MenuManager(MenuLCD<T>* pMenuLCD, bool rootAction);

  bool addMenuRoot( MenuEntry<T> * p_menuEntry);
  MenuEntry<T> * getMenuRoot();
  void DrawMenu();
  void DoMenuAction( MENU_ACTION action );
  void MenuUp();
  void MenuDown();
  void MenuSelect();
  void MenuBack();
  void addChild( MenuEntry<T> * p_menuEntry );
  void addSibling( MenuEntry<T> * p_menuEntry );
  void SelectRoot();
  void DoIntInput( int iMin, int iMax, int iStart, int iSteps, char **label, int iLabelLines, int *pInt );
  void DrawInputRow( char *pString );

  void WipeMenu(typename MenuLCD<T>::Direction dir);

  
  private:
  MenuEntry<T>* m_pRootMenuEntry;
  MenuEntry<T>* m_pCurrentMenuEntry;
  MenuLCD<T>* m_pMenuLCD;
  unsigned int m_fDoingIntInput;
  MenuIntHelper *m_pMenuIntHelper;
  int m_iIntLine;
  int *m_pInt; 
  bool m_execRootMenuAction;

};

template <class T>
MenuManager<T>::MenuManager(MenuLCD<T>* pMenuLCD): m_pMenuLCD( pMenuLCD),  m_fDoingIntInput( false ){}

// new Constructor which allows us to define in class if we want actions being executed
// on menus which have a child - or not.
template <class T>
MenuManager<T>::MenuManager(MenuLCD<T>* pMenuLCD, bool pexecRootAction ):
 m_pMenuLCD( pMenuLCD),
 m_execRootMenuAction (pexecRootAction),
 m_fDoingIntInput( false ) { }

template <class T>
bool MenuManager<T>::addMenuRoot( MenuEntry<T> * p_menuItem)
{
  m_pRootMenuEntry = p_menuItem;
  m_pCurrentMenuEntry = p_menuItem;
  return true;
}

template <class T>
void MenuManager<T>::addSibling( MenuEntry<T> * p_menuItem)
{
  m_pCurrentMenuEntry->addSibling( p_menuItem );
}

template <class T>
void MenuManager<T>::addChild( MenuEntry<T> * p_menuItem)
{
  m_pCurrentMenuEntry->addChild( p_menuItem );
}

template <class T>
MenuEntry<T> * MenuManager<T>::getMenuRoot()
{
  return m_pRootMenuEntry;
}

template <class T>
void MenuManager<T>::WipeMenu(typename MenuLCD<T>::Direction dir )
{
  if( dir == MenuLCD<T>::LEFT )
  {
    for( int i = 0; i < m_pMenuLCD->getCharacters(); ++i )
    {
      m_pMenuLCD->getLCD()->scrollDisplayLeft();
      delay(10);
    }
  }
  else
  {
    for( int i = 0; i < m_pMenuLCD->getCharacters(); ++i )
    {
      m_pMenuLCD->getLCD()->scrollDisplayRight();
      delay(10);
    }
  }
}

template <class T>
void MenuManager<T>::DrawMenu()
{
  if( m_pCurrentMenuEntry->getNextSibling() == NULL )
  {
    if( m_pCurrentMenuEntry->getPrevSibling() != NULL )
    {
      const char *pMenuTexts[2] = {m_pCurrentMenuEntry->getPrevSibling()->getMenuText(), m_pCurrentMenuEntry->getMenuText()};
      m_pMenuLCD->PrintMenu( pMenuTexts, 2, 1 );
    }
    else
    {
      const char * pText = m_pCurrentMenuEntry->getMenuText();
      m_pMenuLCD->PrintMenu( &pText, 1, 0 );
    }
  }
  else
  {
    const char *pMenuTexts[2] = {m_pCurrentMenuEntry->getMenuText(), m_pCurrentMenuEntry->getNextSibling()->getMenuText()};
    m_pMenuLCD->PrintMenu( pMenuTexts, 2, 0 );
  }
}

template <class T>
void MenuManager<T>::DoMenuAction( MENU_ACTION action )
{
  if( m_fDoingIntInput == true )
  {
    int iNewNum = m_pMenuIntHelper->getInt();

    char buff[64];
    switch (action )
    {
      case MENU_ACTION_UP:
        iNewNum = m_pMenuIntHelper->numDecrease();
        itoa( iNewNum, buff, 10 );
        DrawInputRow( buff );
        *m_pInt = iNewNum;
        break;

      case MENU_ACTION_DOWN:
        iNewNum = m_pMenuIntHelper->numIncrease();
        itoa( iNewNum, buff, 10 );
        DrawInputRow( buff );
        *m_pInt = iNewNum;
        break;

      case MENU_ACTION_SELECT:
        m_fDoingIntInput = false;
        DrawMenu();
        break;

      case MENU_ACTION_BACK:
        m_fDoingIntInput = false;
        DrawMenu();
        break;
      case MENU_ACTION_NONE:
        break;
    }
  }
  else
  {
    m_pMenuLCD->ClearLCD();
    delay(10);
    DrawMenu();
    switch (action )
    {
      case MENU_ACTION_UP:
        this->MenuUp();
        break;
      case MENU_ACTION_DOWN:
        this->MenuDown();
        break;
      case MENU_ACTION_SELECT:
    	this->MenuSelect();
        break;
      case MENU_ACTION_BACK:
        this->MenuBack();
        break;
      case MENU_ACTION_NONE:
    	break;
    }
  }
}

template <class T>
void MenuManager<T>::MenuUp()
{
  MenuEntry<T> *prev = m_pCurrentMenuEntry->getPrevSibling();
  if( prev != NULL )
  {
    m_pCurrentMenuEntry = prev;
  }
  else
  {
    //Flash?
  }
  DrawMenu();
}

template <class T>
void MenuManager<T>::MenuDown()
{
  MenuEntry<T> *next = m_pCurrentMenuEntry->getNextSibling();
  if( next != NULL )
  {
    m_pCurrentMenuEntry = next;
  }
  else
  {
    //Flash?
  }
  DrawMenu();

}

template <class T>
void MenuManager<T>::MenuSelect()
{
  MENU_ACTION_RESULT result = MENU_ACTION_RESULT_NONE;

  //
  // EDIT: Changed library to always do a callback even if the
  // menu has a child. This allows me to keep track of the menu we are in.
  //
  if ( m_execRootMenuAction == true) {
     m_pCurrentMenuEntry->ExecuteCallback();
  }

  MenuEntry<T> *child = m_pCurrentMenuEntry->getChild();
  if( child != NULL )
  {
    WipeMenu( MenuLCD<T>::LEFT);
    m_pCurrentMenuEntry = child;
    DrawMenu();
  }
  else
  {
    if( !m_pCurrentMenuEntry->isBackEntry() )
    {
      WipeMenu( MenuLCD<T>::LEFT);
    }
    MENU_ACTION_RESULT result = m_pCurrentMenuEntry->ExecuteCallback();
    if( !m_fDoingIntInput && result != MENU_ACTION_RESULT_RETAIN_DISPLAY)
    {
      DrawMenu();
    }
  }
}

template <class T>
void MenuManager<T>::MenuBack()
{
  if( m_pCurrentMenuEntry->getParent() != NULL )
  {
    WipeMenu( MenuLCD<T>::RIGHT);
    m_pCurrentMenuEntry = m_pCurrentMenuEntry->getParent();
    DrawMenu();
  }
}

template <class T>
void MenuManager<T>::SelectRoot()
{
  m_pCurrentMenuEntry = m_pRootMenuEntry;
}

template <class T>
void MenuManager<T>::DrawInputRow( char *pString )
{
  m_pMenuLCD->PrintLineRight( pString, m_pMenuLCD->getLines() - 1 );
}

template <class T>
void MenuManager<T>::DoIntInput( int iMin, int iMax, int iStart, int iSteps, char **label, int iLabelLines, int *pInt )
{

  char buff[64];
  m_fDoingIntInput = true;

  m_pInt = pInt;
  *pInt = iStart;

  //The MenuIntHelper class will keep track of the input, but all other logic will stay here
  if( m_pMenuIntHelper != NULL )
  {
    delete m_pMenuIntHelper;
  }
  m_pMenuIntHelper = new MenuIntHelper(  iMin, iMax, iStart, iSteps );
  //print the label
  m_pMenuLCD->PrintMenu( label, iLabelLines, -1 );
  m_iIntLine = iLabelLines;  //Off by one because index is zero based
  itoa( m_pMenuIntHelper->getInt(), buff, 10 );
    DrawInputRow( buff );
}
