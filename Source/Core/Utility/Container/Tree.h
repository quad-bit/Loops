#pragma once
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>

template <class T>
class Tree;

template <class T>
class TreeNode
{
    friend class Tree<T>;

private:
    T * data;
    TreeNode<T> * parent;
    TreeNode<T> *next, *prev, *child;

    TreeNode<T> * treeRoot;

    uint32_t nodeId;
    static uint32_t idCounter;
    void AssignId();
    void AddSibling(TreeNode<T> * node);

public:
	TreeNode(T * data);
	~TreeNode();
	void AddChild(TreeNode<T> * child);
	void DetachChild(TreeNode<T> * child);
	void DeleteChild(TreeNode<T> * child);
	void SetParent(TreeNode<T> * parent);
};

template <typename T>
uint32_t TreeNode<T>::idCounter = 0;

template<class T>
inline TreeNode<T>::TreeNode(T * data)
{
	this->data = data;
	next = NULL;
	prev = NULL;
	child = NULL;
    parent = NULL;
}

template<class T>
inline TreeNode<T>::~TreeNode()
{
    if (child != NULL)
    {
        TreeNode<T> * ptr = child->next;
        if (ptr == NULL)
        {

        }
        else
        {
            while (ptr->next != NULL)
            {
                ptr = ptr->next;
            }

            while (ptr->prev != NULL) // Need unique identifier for each node
            {
                TreeNode<T> * temp = ptr;
                ptr = ptr->prev;
                delete temp;
                temp = NULL;
                ptr->next = NULL;
            }

        }

        delete child;
        child = NULL;
    }

    if (this->next != NULL && this->prev != NULL)
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
    }

    if (this->next == NULL && this->prev != NULL)
    {
        this->prev->next = NULL;
    }

    this->prev = NULL;
}

template<class T>
inline void TreeNode<T>::AddChild(TreeNode<T>* node)
{
	if (this->child == NULL)
	{
		this->child = node;
        this->child->prev = NULL;
	}
	else
	{
		child->AddSibling(node);
	}

    node->parent = this;
}

template<class T>
inline void TreeNode<T>::DetachChild(TreeNode<T>* child)
{
    // detach child from parent
    // is this the first child
    if (child->prev == NULL)
    {
        if (child->next != NULL)
        {
            TreeNode<T> * newChild = child->next;
            this->child = newChild;
            newChild->prev = NULL;
        }
        //no siblings
        else
        {
            this->child = NULL;
        }
    }
    else
    {
        if(child->prev != NULL)
            child->prev->next = child->next;

        if (child->next != NULL)
            child->next->prev = child->prev->next;
    }

    child->next = NULL;
    child->prev = NULL;
}

template<class T>
inline void TreeNode<T>::AssignId()
{
    nodeId = idCounter;
    idCounter++;
}

template<class T>
inline void TreeNode<T>::AddSibling(TreeNode<T>* node)
{
    TreeNode<T> * ptr = next;

	if (next == NULL)
	{
		next = node;
		node->prev = this;
	}
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = node;
		node->prev = ptr;
	}
    node->parent = this->parent;
}

template<class T>
inline void TreeNode<T>::DeleteChild(TreeNode<T>* child)
{
    DetachChild(child);
    delete child;
}

template<class T>
inline void TreeNode<T>::SetParent(TreeNode<T>* parent)
{
    if (this->parent == this->treeRoot)
    {
        this->parent = parent;
        parent->AddChild(this);
    }
    else
    {
        // detach child from parent
        // is this the first child
        if (this->prev == NULL) // (this->parent->child->nodeId == this->nodeId)
        {
            if (this->parent->child->next != NULL)
            {
                TreeNode<T> * newChild = this->parent->child->next;
                this->parent->child = newChild;
                newChild->prev = NULL;
            }
            //no siblings
            else
            {
                this->parent->child = NULL;
            }
        }
        else
        {
            if (this->prev != NULL)
                this->prev->next = this->next;

            if (this->next != NULL)
                this->next->prev = this->prev->next;
        }

        this->prev = NULL;
        this->next = NULL;

        //attach it to new parent
        this->parent = parent;
        parent->AddChild(this);
    }
}


template <class T>
class Tree
{
private:
    TreeNode<T> * root;

public:
    Tree();
    ~Tree();
    void AddToTree(TreeNode<T> * node);
    bool Search(TreeNode<T> * root, T * data);
    bool Search(T * data);
    virtual void Traversal(TreeNode<T> * node);
    virtual void Traversal();

    const TreeNode<T> * GetRoot() const;
};

template<class T>
inline Tree<T>::Tree()
{
    //root = new TreeNode<T>(-1);
}

template<class T>
inline Tree<T>::~Tree()
{
    delete root;
}

template<class T>
inline void Tree<T>::AddToTree(TreeNode<T>* node)
{
    if (node->parent == NULL)
    {
        node->SetParent(root);
    }
    
    node->treeRoot = root;
}

template<class T>
inline bool Tree<T>::Search(TreeNode<T>* parent, T * data)
{
    if (parent->data == data)
    	return true;

    if (parent->child != NULL)
    {
    	if (Search(parent->child, data) == true)
    		return true;
    }

    if (parent->next != NULL)
    {
    	if (Search(parent->next, data) == true)
    		return true;
    }

    return false;
}

template<class T>
inline bool Tree<T>::Search(T * data)
{
    TreeNode<T> * parent = root;

    if (parent->data == data)
    	return true;

    if (parent->child != NULL)
    {
    	if (Search(parent->child, data) == true)
    		return true;
    }

    if (parent->next != NULL)
    {
    	if (Search(parent->next, data) == true)
    		return true;
    }

    return false;
}

template<class T>
inline void Tree<T>::Traversal(TreeNode<T> * node)
{
    if (node->data != NULL)
    	std::cout << node->data;

    if (node->child != NULL)
    {
    	std::cout << std::endl;
    	Traversal(node->child);
    }

    if (node->next != NULL)
    {
    	std::cout << " ";
    	Traversal(node->next);
    }
}

template<class T>
inline void Tree<T>::Traversal()
{
    TreeNode<T> * node = root;

    if (node->data != NULL)
    	std::cout << node->data;

    if (node->child != NULL)
    {
    	std::cout << std::endl;
    	Traversal(node->child);
    }

    if (node->next != NULL)
    {
    	std::cout << " ";
    	Traversal(node->next);
    }
}

template<class T>
inline const TreeNode<T>* Tree<T>::GetRoot() const
{
    return root;
}
