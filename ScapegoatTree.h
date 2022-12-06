#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    int getHeight(Node<T>*r)const;
    void deleteScapegoatTree(Node<T>*&r);
    void copy(Node<T>*&r,  Node<T>* const &obj);
    const T & get(Node<T>*r,const T &element) const;
    void getCeiling(Node<T>* r, Node<T> *&tmp, const T &element) const;
    void getFloor(Node<T>* r, Node<T> *&tmp, const T &element) const;
    const T & getMin(Node<T>*r) const;
    const T & getMax(Node<T>*r) const;
    void getNext(Node<T>* r, Node<T> *&tmp, const T &element) const;
    bool insert(Node<T>*&r, const T &element);
    bool contains(Node<T>*r,const T &element);
    int getSize(Node<T>*r) const;
    bool remove(Node<T>* &r, const T &element);
    void build_arr(Node<T>*&r, T *&arr, int &num);
    void balance(T*&arr, Node<T>*&r, int low, int high);
    void balance_helper(Node<T>*&root);
    void find_parent(Node<T>*&r,T &data,Node<T>* &child,Node<T>* &parent,int &w_child);

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;

};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree() {
    root = NULL;
    upperBound = 0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj) {
    
    copy(root,obj.root);
    upperBound = obj.upperBound;
    
}
template<class T>
void ScapegoatTree<T>::copy(Node<T>*&r, Node<T>* const &obj){//insert ile yaz
    if(obj  == NULL) r = NULL;
    else{
        r = new Node<T>(obj -> element, NULL, NULL);
        copy(r -> left, obj -> left);
        copy(r -> right, obj -> right);
    }
    
    
    
}
template<class T>
ScapegoatTree<T>::~ScapegoatTree() {
    deleteScapegoatTree(root);
}
template<class T>
void ScapegoatTree<T>::deleteScapegoatTree(Node<T>*&r) {
    if(!r) return;
    else if(r != NULL){
        deleteScapegoatTree(r->left);
        deleteScapegoatTree(r->right);
        delete r;
        r = NULL;
        
    }
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const {
    if(root == NULL){
        return true;
    }
    else return false;
}

template<class T>
int ScapegoatTree<T>::getHeight() const {
    return getHeight(root);
}

template<class T>
int ScapegoatTree<T>::getHeight(Node<T> *r)const {//max kullan˝m˝
    if (r == NULL)
        return -1;
    else
        return (std::max(getHeight(r->left), getHeight(r->right)) + 1);
}

template<class T>
int ScapegoatTree<T>::getSize() const {
    return getSize(root);
}
template<class T>
int ScapegoatTree<T>::getSize(Node<T>*r) const {
    if(!r) return 0;
    else{
        return 1 + getSize(r->left) + getSize(r->right);
    }
}
template<class T>
bool ScapegoatTree<T>::insert(const T &element) {
    bool b,check = false; int height,size;
    Node<T>* parent = NULL;Node<T>* child = NULL;Node<T>* Scapegoat= NULL;
    T data = element;
    b = insert(root,element);
    height = getHeight(root); size = getSize(root);
    if(b){
        upperBound++;
        if(pow(1.5, height) > upperBound){//root durumu
                for(int i = 0; i < size; i++){
                    int w_child = 0;
                    find_parent(root,data,child,parent,w_child);
                    if(3*getSize(child)>2*getSize(parent)){
                        Scapegoat = parent;
                        check = true;
                        break;
                    }
                }    
                if(check){
                    Node<T>* Scapegoat_parent=NULL;
                    int w_child = 0;
                    find_parent(root,data,Scapegoat,Scapegoat_parent,w_child);
                    balance_helper(Scapegoat);
                    if(Scapegoat_parent){
                        if(w_child == 1)Scapegoat_parent -> right = Scapegoat;
                        else if(w_child == 2) Scapegoat_parent -> left = Scapegoat;
                    }
                }
        }
    }
    
    return b;

}
template<class T>
void ScapegoatTree<T>::find_parent(Node<T>*&r,T &data,Node<T>*&child,Node<T>*&parent,int &w_child){
    
    if(!r||(r && (r-> left == NULL && r -> right == NULL)));//??
    else if(r->left && r-> left -> element == data){
        parent = r;
        child = r -> left;
        data = r -> element;
        w_child += 2;
    }
    else if(r->right && r -> right -> element == data){
        parent = r;
        child = r -> right;
        data = r -> element;
        w_child++;
    }
    else{
        if(r){
            if(data > r -> element) find_parent(r->right,data,child,parent,w_child);
            else find_parent(r->left,data,child,parent,w_child);
        }
    }
    
}
template<class T>
bool ScapegoatTree<T>::insert(Node<T>*&r,const T &element) {
    if(r == NULL){
        r = new Node<T>(element,NULL,NULL);
        return true;
    }
    else if(element < r -> element){
        return insert(r->left,element);
    }
    else if(element > r -> element){
        return insert(r -> right, element);
        
    }
    else{
        return false;
    }
    
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element) {
    bool b = remove(root,element);
    int num = getSize(root);
    if(b && (upperBound > 2*num || num > upperBound)){
        
        balance_helper(root);
        upperBound = num;
        
    }
    return b;
}
template<class T>
bool ScapegoatTree<T>::remove(Node<T>*&r, const T &element) {
    if(contains(root,element)){
        if(r->element > element) return remove(r->left,element);
        else if(r->element < element) return remove(r->right,element);
        else{
            if(r->left != NULL && r->right != NULL){
                r -> element = getMax(r->left);
                return remove(r->left,r-> element);
            }
            else{
                if(r->left != NULL){
                    Node<T>*tmp = r;
                    r = r->left;
                    delete tmp;
                    tmp = NULL;
                }
                else if(r-> right != NULL){
                    Node<T>*tmp = r;
                    r = r -> right;
                    delete tmp;
                    tmp = NULL;
                }
                else{
                    delete r;
                    r = NULL;
                }
            }
        }
    }
    else return false;
    
}
template<class T>
bool ScapegoatTree<T>::contains(Node<T>*r,const T &element) {
    if(r == NULL) return false;
    else if(r-> element == element) return true;
    else if(element > r -> element) return contains(r->right,element);
    else return contains(r->left,element);
}

template<class T>
void ScapegoatTree<T>::removeAllNodes() {
    deleteScapegoatTree(root);
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const {
    return get(root,element);
}
template<class T>
const T &ScapegoatTree<T>::get(Node<T>*r,const T &element) const {
    if(r == NULL){
        throw NoSuchItemException();
    }
    else{
        if(r -> element == element){
            return element;
        }
        else if(element > r -> element) get(r-> right, element);
        else{
            get(r->left, element);
        }
    }
    
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const {

    if (tp == preorder) {
        if (isEmpty()) {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        if (isEmpty()) {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const {

    if (tp == preorder) {
        if (node == NULL)
          return;
        std::cout << "\t" << node->element;
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
    } else if (tp == inorder) {
        // check if the node is NULL?
        if (node == NULL)
          return;

        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->element;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } else if (tp == postorder) {
        if (node == NULL)
          return;
        print(node->left, postorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }
        
        std::cout << "\t" << node->element;
    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) {
    if(this != &rhs){
        
        deleteScapegoatTree(root);
        copy(root,rhs.root);
    }
    upperBound = rhs.upperBound;
    return *this;
}
template<class T>
void ScapegoatTree<T>::balance(){
    balance_helper(root);
}
template<class T>
void ScapegoatTree<T>::balance_helper(Node<T>*&root) {
    int low = 0,num = 0,high = getSize(root) - 1,size = getSize(root);// num < size kontrol¸ gerekli mi??
    T* arr = new T[size];
    if(root){
        build_arr(root,arr,num);
        deleteScapegoatTree(root);
        balance(arr,root,low,high);
    }
    delete [] arr;
}
template<class T>
void ScapegoatTree<T>::build_arr(Node<T>*&r, T*&arr, int&num){//?
    if(!r);
    else{
        build_arr(r->left,arr,num);
        arr[num] = r -> element;
        num++;
        build_arr(r->right,arr,num);
    }

}

template<class T>
void ScapegoatTree<T>::balance(T*&arr, Node<T>*&r, int low, int high){
    if(low > high);
    else{
        int mid = (low + high)/2;
        r = new Node<T>(arr[mid],NULL,NULL);
        balance(arr,r->left,low,mid-1);
        balance(arr,r->right,mid+1,high);
    }
    
    
}


template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const {
    Node<T>* tmp = NULL;
    getCeiling(root,tmp,element);
    if(!tmp) throw NoSuchItemException();
    else return tmp -> element;
}
template<class T>
void ScapegoatTree<T>::getCeiling(Node<T>* r, Node<T>*&tmp,const T &element) const {
    if(r == NULL) return;
    if(r -> element == element){
        tmp = r;
        return;
    }
    else if(r -> element > element){
        tmp = r;
        getCeiling(r -> left,tmp,element);
    }
    else{
        getCeiling(r -> right,tmp,element);
    }
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const {
    Node<T>* tmp = NULL;
    getFloor(root,tmp,element);
    if(!tmp) throw NoSuchItemException();
    else return tmp -> element;
}
template<class T>
void ScapegoatTree<T>::getFloor(Node<T>* r, Node<T>*&tmp,const T &element) const {
    if(r == NULL) return;
    if(r -> element == element){
        tmp = r;
        return;
    }
    else if(r -> element < element){
        tmp = r;
        getFloor(r -> right,tmp,element);
    }
    else{
        getFloor(r -> left,tmp,element);
    }
}
template<class T>
const T &ScapegoatTree<T>::getMin() const {
    if(root == NULL) throw NoSuchItemException();
    else{
        return getMin(root);
    }
}
template<class T>
const T &ScapegoatTree<T>::getMin(Node<T>*r) const {
    if(r-> left == NULL){
        return r-> element;
    }
    else{
        return getMin(r->left);
    }
}

template<class T>
const T &ScapegoatTree<T>::getMax() const {
    if(root == NULL) throw NoSuchItemException();
    else{
        return getMax(root);
    }
}
template<class T>
const T &ScapegoatTree<T>::getMax(Node<T>*r) const {
    if(r-> right == NULL){
        return r-> element;
    }
    else{
        return getMax(r->right);
    }
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const {
    Node<T>* tmp = NULL;
    getNext(root,tmp,element);
    if(!tmp) throw NoSuchItemException();
    else return tmp -> element;
}
template<class T>
void ScapegoatTree<T>::getNext(Node<T>* r, Node<T>*&tmp,const T &element) const {
    if(r == NULL) return;
    else if(r -> element > element){
        tmp = r;
        getNext(r -> left,tmp,element);
    }
    else{
        getNext(r -> right,tmp,element);
    }
}


