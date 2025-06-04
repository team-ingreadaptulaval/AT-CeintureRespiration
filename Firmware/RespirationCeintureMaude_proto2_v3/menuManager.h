/**
 * @file menuManager.h
 * @author Simon Latour
 * @brief  The logic manager for menu pages 
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUMANGER_H
#define MENUMANGER_H



class menu {

    public:
    
    // Code called in the loop
    virtual void handle() = 0;

    //Each pages need to implement what happens during these actions
    virtual void onPressUp() {}//= 0;
    virtual void onDoubleClickUp(){}
    virtual void onLongPressUp(){}

    virtual void onPressDown() {}//= 0;
    virtual void onDoubleClickDown(){}
    virtual void onLongPressDown(){}

    virtual void onPressReturn() {}//= 0;
    virtual void onDoubleClickReturn(){}
    virtual void onLongPressReturn(){}

    virtual void onPressEnter() {}//= 0;
    virtual void onDoubleClickEnter(){}
    virtual void onLongPressEnter(){}

    virtual void onFocus() = 0;


};

class menuManager
{
    private:   

        menu* m_activePage;
        menu* m_rootPage;
    public:

        
        menuManager();

        void setRootPage(menu* rootMenu );
        void setActivePage ( menu* activeMenu );
        void returnToParent();
        
        void handle();

        // void onPressUp();
        // void onPressDown();
        // void onPressReturn();
        // void onPressEnter();

        void onPressUp();
        void onDoubleClickUp();
        void onLongPressUp();

        void onPressDown();
        void onDoubleClickDown();
        void onLongPressDown();

        void onPressReturn();
        void onDoubleClickReturn();
        void onLongPressReturn();

        void onPressEnter();
        void onDoubleClickEnter();
        void onLongPressEnter();

};

class menuPage: public menu {

    public:

    menu* m_parentPage;

    menuManager* m_menuManager;
    bool m_focusFlag = false;
    //menuPage(menuManager* context, menu* parentMenu);

    virtual void handle() = 0;

    virtual void onPressUp() = 0;
    virtual void onPressDown() = 0;
    virtual void onPressReturn() = 0;
    virtual void onPressEnter() = 0;
    virtual void onFocus() = 0;

};




#endif