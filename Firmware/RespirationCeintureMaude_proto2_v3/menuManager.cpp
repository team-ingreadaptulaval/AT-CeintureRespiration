#include <iostream>
#include <istream>
#include <string>
#include <stdio.h>
using namespace std;

#include "menuManager.h"

    menuManager::menuManager()
    {
       //m_activeMenu = rootMenu;
    }

    void menuManager::setRootPage(menu* rootMenu )
    {
        m_rootPage = rootMenu;
        m_activePage = rootMenu;
        m_activePage->onFocus();
    }

    void menuManager::setActivePage ( menu* activeMenu )
    {
        m_activePage = activeMenu;
        m_activePage->onFocus();
    }
    
    void menuManager::handle()
    {
        m_activePage->handle();
    }

    void menuManager::returnToParent()
    {
        m_activePage = m_rootPage;
    }

     void menuManager::onPressUp()
    {
        m_activePage->onPressUp();
    }
    
    void menuManager::onDoubleClickUp()
    {
       m_activePage->onDoubleClickUp();
    }
    
    void menuManager::onLongPressUp()
    {
         m_activePage->onLongPressUp();
    }
    
    void menuManager::onPressDown()
    {
        m_activePage->onPressDown();
    }
    void menuManager::onDoubleClickDown()
    {
       m_activePage->onDoubleClickDown();
    }
    
    void menuManager::onLongPressDown()
    {
         m_activePage->onLongPressDown();
    }

    void menuManager::onPressReturn()
    {
        m_activePage->onPressReturn();
    }

    void menuManager::onDoubleClickReturn()
    {
       m_activePage->onDoubleClickReturn();
    }
    
    void menuManager::onLongPressReturn()
    {
         m_activePage->onLongPressReturn();
    }


    void menuManager::onPressEnter()
    {
        m_activePage->onPressEnter();
    }

     void menuManager::onDoubleClickEnter()
    {
       m_activePage->onDoubleClickEnter();
    }
    
    void menuManager::onLongPressEnter()
    {
        m_activePage->onLongPressEnter();
    }


    

