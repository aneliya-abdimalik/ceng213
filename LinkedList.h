#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList()
{
    head=NULL;
    size=0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    if(obj.head==NULL)
        head=NULL;
    else
    {
        head = new Node<T>(obj.head->data);
        Node<T> *curr = head;
        Node<T> *tmp = head;
        Node<T> *obj_curr = obj.head;
        while(obj_curr->next != obj.head)
        {
            curr->next = new Node<T>(obj_curr->next->data);
            obj_curr = obj_curr->next;
            curr = curr->next;
            curr->prev = tmp;
            tmp = tmp->next;
        }
        curr->next = head;
        head->prev = curr;
    }
    size=obj.size;
    
    
}

template<class T>
LinkedList<T>::~LinkedList()
{
    removeAllNodes();
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if(this==&rhs)
        return *this;
        
    removeAllNodes();
    
    Node<T>* p=rhs.head;
    for(int i=0;i<rhs.size;i++)
    {
        append(p->data);
        p=p->next;
    }
    
    size=rhs.size;
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    return size==0;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    if(size)
    {
        Node<T> *temp=head;
        while(temp->next!=head)
        {
            if(temp==node)
                return true;
            temp=temp->next;
        }
        if(temp==node)
            return true;
    }
    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    if(head)
        return head;
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    if(head)
        return head->prev;
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    if(size)
    {
        Node<T> *temp=head;
        while(temp->next!=head)
        {
            if(temp->data==data)
                return temp;
            temp=temp->next;
        }
        if(temp->data==data)
        return temp;
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    if(size)
    {
        Node<T> *temp=head;
        for(int i=0;i<size;i++)
        {
            if(i==index)
                return temp;
            temp=temp->next;
        }
        
    }
    return NULL;
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    if(size==0)
    {
        head=new Node<T>(data);
        head->prev=head;
        head->next=head;
        size++;
    }
    
    else
    {
        Node<T> *p=new Node<T> (data);
        Node<T> *tail = head->prev;
        p->prev = tail;
        head->prev = p;
        p->next = head;
        tail->next = p;
        
        size++;
    }
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    if(size==0 )
    {
        head=new Node<T>(data);
        head->prev=head;
        head->next=head;
        size++;
    }
    
    else
    {
        Node<T> *p=new Node<T>(data);
        Node<T> *tail=head->prev;
        
        p->prev=tail;
        head->prev=p;
        p->next=head;
        tail->next=p;
        head=head->prev;
        size++;

    }
    
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    if(containsNode(node))
    {
        Node<T> *p=new Node<T>(data);
        Node<T> *temp=head;
        Node<T> *n;
        for(int i=0;i<size;i++)
        {
             if(temp==node)
            {
                n=temp->next;
                p->prev=temp;
                n->prev=p;
                p->next=n;
                temp->next=p;
            }
            temp=temp->next;
        }
        size++;
    }
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if(index>=0 && index<size)
    {
        if(index==0)
        {
            Node<T> *n=new Node<T>(data);
            n->prev=head->prev;
            n->next=head;
            head->prev->next=n;
            head->prev=n;
            head=n;
            size++;
        }
        Node<T> *temp=head;
        for(int i=0;i<index-1;i++)
            temp=temp->next;
        insertAfterNode(data,temp);
        
    }
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    if(containsNode(node))
    {
        Node<T> *temp=head;
        for(int i=0;i<size;i++)
        {
            if(temp==node)
                return i;
            temp=temp->next;
        }
    }
    
    return -1;
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{

    if(currentIndex<size && currentIndex!=newIndex)
    {
        
        Node<T> *second,*first,*n,*p;
        if(newIndex>size)
            second=head->prev;
        else
        {
            Node<T> *temp=head;
            for(int i=0;i<size;i++)
            {
                if(i==currentIndex)
                    {
                        first=temp;
                        p=temp->prev;
                        n=temp->next;
                    }
                if(i==newIndex)
                    second=temp;
                temp=temp->next;
            }
        }
        
        
        p->next=p->next->next;
        n->prev=n->prev->prev;
        
        if(currentIndex<newIndex)
        {
            first->prev=second;
            second->next->prev=first;
            first->next=second->next;
            second->next=first;
        }
        else
        {
            Node<T> *tmp=second->prev;
            first->prev=tmp;
            tmp->next->prev=first;
            first->next=tmp->next;
            tmp->next=first;
        }

        if(currentIndex==0)
            head=n;
        if(newIndex==0)
            head=first;
        
    }
}


template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    if(node!=NULL && containsNode(node))
    {
        if(node==head)
        {
            if(size==1)
            {
                Node<T> *del=head;
                head=NULL;
                delete del;
                size=0;
            }
            
            else
            {
                if(size==2)
                {
                    Node<T> *del=head;
                    head=head->next;
                    head->next=head->prev=head;
                    delete del;
                    size=1;
                }
                
                else
                {
                    Node<T> *del=head;
                    head->next->prev=head->prev;
                    head->prev->next=head->next;
                    
                    head=head->next;
                    
                    delete del;
                    size--;
                }
            }
            
        }
        else
        {
            Node<T> *temp=head;
            for(int i=0;i<size;i++)
            {
                if(temp==node)
                    break;
                temp=temp->next;
            }
            temp->prev->next=temp->next;
            temp->next->prev=temp->prev;

            delete temp;
            size--;
        }
        
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    if(getNode(data))
        removeNode(getNode(data));
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    if(getNodeAtIndex(index))
        removeNode(getNodeAtIndex(index));
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    if(size)
    {
        Node<T> *temp=head;
        for(int i=0;i<size;i++)
        {
            Node<T> *del=temp;
            temp=temp->next;
            delete del;
        }
        size=0;
    }
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}

#endif //LINKEDLIST_H
