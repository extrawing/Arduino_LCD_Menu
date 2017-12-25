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

#ifndef MenuLCD_H
#define MenuLCD_H 1

template <class T>
class MenuLCD
{
  public:
  // Note: LCD must be created and initialized outside:
  MenuLCD(T *pLCD, int characters, int lines);
  template <typename MYSTR> bool PrintMenu( const MYSTR * pString[], int nLines, int nSelectedLine /*= 0*/);
  bool PrintLineRight( const char* pString, int iRow );
  bool PrintLine( const char* pString, int iRow );
  int getLines();
  int getCharacters();
  void ClearLCD();
  T * getLCD();

  typedef enum { LEFT, RIGHT } Direction;


  //void WipeMenu( const char* pString[], int nLines, typename MenuLCD<T>::Direction dir );

  
  private:
  T* m_pLCD;
  int m_characters;
  int m_lines;
};

template <class T>
MenuLCD<T>::MenuLCD(T *pLCD, int characters, int lines)
: m_pLCD(pLCD),
  m_characters( characters ),
  m_lines( lines )
{
}

template <class T>
template <typename MYSTR>
bool MenuLCD<T>::PrintMenu( const MYSTR* pString[], int nLines, int nSelectedLine /*= 0*/)
{
  m_pLCD->clear();
  for( int i =0; i < nLines; i++ )
  {
    m_pLCD->setCursor(0, i);
	if (nSelectedLine >= 0) {
      if(i == nSelectedLine)
      {
         m_pLCD->write( '>');
      }
      else
      {
        m_pLCD->write(' ');
      }
      m_pLCD->setCursor(1,i);
	}
    m_pLCD->print( pString[i] );
  }
  return true;
}

/*template <class T>
void MenuLCD<T>::WipeMenu( const char* pString[], int nLines, typename MenuLCD<T>::Direction dir )
{
  char lineBuff[ 256 ];
  m_pLCD->clear();
  for( int i = 0; i < m_characters; ++i )
  {
    for( int j =0; j < nLines; ++j )
    {
      m_pLCD->setCursor( 0, j );
      m_pLCD->setCursor(0,j);
      if (strlen( pString[j] ) > i )
      {
        if( dir == LEFT )
        {
          strcpy(lineBuff, (pString[j] + i ) );
          strcat(lineBuff, "  " );
        }
        else
        {
          lineBuff[0] = '\0';
          for( int k = 0; k < i; ++k )
          {
            strcat(lineBuff, " " );
          }
          strcat(lineBuff, pString[j]);
        }
      }
      else
      {
        strcpy(lineBuff, " " );
      }
      m_pLCD->print( lineBuff );

    }
  delay(50);
  }
}*/

template <class T>
bool MenuLCD<T>::PrintLineRight( const char* pString, int iRow )
{
  //clear the line
  char buff[ m_characters ];
  for( int i = 0; i < m_characters; ++i )
  {
    buff[i] = ' ';
  }
  m_pLCD->setCursor( 0, iRow );
  m_pLCD->print( buff );
  //now print the new number
  m_pLCD->setCursor(m_characters - strlen(pString),iRow);
  m_pLCD->print( pString );
  return true;
}

template <class T>
bool MenuLCD<T>::PrintLine( const char* pString, int iRow )
{
  //clear the line
  m_pLCD->setCursor( 0, iRow );
  m_pLCD->print( pString );
  return true;
}

template <class T>
int MenuLCD<T>::getLines()
{
  return m_lines;
}

template <class T>
int MenuLCD<T>::getCharacters()
{
  return m_characters;
}

template <class T>
void MenuLCD<T>::ClearLCD()
{
  m_pLCD->clear();
}

template <class T>
T * MenuLCD<T>::getLCD()
{
  return m_pLCD;
}

#endif
