#include "Window.h"

Window::Window() {
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab() {
    if(isEmpty())
        return Tab();
    else
        return (tabs.getNodeAtIndex(activeTab))->data;

}

bool Window::isEmpty() const {
    return tabs.isEmpty();
}

void Window::newTab(const Tab &tab) {
    
    if(isEmpty())
    {
        tabs.append(tab);
        activeTab=0;
    }
    else
    {
        tabs.insertAfterNode(tab,tabs.getNodeAtIndex(activeTab));
        activeTab++;
    }
}

void Window::closeTab() {
    if(isEmpty()) 
        activeTab=-1;
    else
    {
        tabs.removeNodeAtIndex(activeTab);
        if(activeTab==tabs.getSize() && tabs.getSize()>1)
            activeTab=activeTab-1;
    }

}

void Window::moveActiveTabTo(int index) {
    tabs.moveToIndex(activeTab,index);
    activeTab=index;
}

void Window::changeActiveTabTo(int index) {
    if(index>=0 && index<tabs.getSize())
      activeTab=index;
}

void Window::addTab(Node<Tab> &tab) {
    if(tabs.isEmpty())
        activeTab=0;
    tabs.append(tab.data);
    
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}
