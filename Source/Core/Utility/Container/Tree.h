#pragma once
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>

namespace Loops::Core::Utils::Container
{
    template <class T>
    class Tree;

    template <class T>
    class TreeNode
    {
    	friend class Tree<T>;

    private:
    	T data;
    	TreeNode<T> * parent;
    	TreeNode<T> *next, *prev, *child;

    	void AddSibling(TreeNode<T> * node);

    public:
    	TreeNode(T data);
    	~TreeNode();
    	void AddChild(TreeNode<T> * child);
    	void RemoveChild(TreeNode<T> * child);
    	void DeleteChild(TreeNode<T> * child);
    	void SetParent(TreeNode<T> * parent);
    };

    template<class T>
    inline TreeNode<T>::TreeNode(T data)
    {
    	this->data = data;
    	next = NULL;
    	prev = NULL;
    	child = NULL;
    }

    template<class T>
    inline TreeNode<T>::~TreeNode()
    {
    }

    template<class T>
    inline void TreeNode<T>::AddChild(TreeNode<T>* node)
    {
    	if (this->child == NULL)
    	{
    		this->child = node;
    	}
    	else
    	{
    		child->AddSibling(node);
    	}
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
    		node->prev = this;
    	}
    }

    template<class T>
    inline void TreeNode<T>::SetParent(TreeNode<T>* parent)
    {
    	this->parent = parent;
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
    	bool Search(TreeNode<T> * root, T data);
    	bool Search(T data);
    	void Traversal(TreeNode<T> * node);
    	void Traversal();
    };

    template<class T>
    inline Tree<T>::Tree()
    {
    	root = new TreeNode<T>(-1);
    }

    template<class T>
    inline Tree<T>::~Tree()
    {
    	delete root;
    }

    template<class T>
    inline void Tree<T>::AddToTree(TreeNode<T>* node)
    {
    	node->SetParent(root);
    	root->AddChild(node);
    }

    template<class T>
    inline bool Tree<T>::Search(TreeNode<T>* parent, T data)
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
    inline bool Tree<T>::Search(T data)
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
}
