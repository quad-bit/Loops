#pragma once
#include<stdint.h>

// NOT GETTING USED  ...  !!!!!!!!!!


class SceneNode
{
private:
	SceneNode *prev, *next, *child;

public:
	SceneNode()
	{
		prev = NULL;
		next = NULL;
		child = NULL;
	}

	virtual ~SceneNode()
	{

	}

	void AddChild(SceneNode * node);
	void AddSibling(SceneNode * node);
};
