#pragma once
#include "BoardManager.h"

class ListNode {
public:
	Board data;
	ListNode *link;
};

class LinkedStack {
	ListNode *head;
public :
	void insert_node(ListNode *new_node)
	{
		// 리스트가 비어있을 때
		if (head == NULL)
		{
			// 리스트에 노드를 삽입
			new_node->link = NULL;
			head = new_node;
		}
		// 리스트가 비어있지 않을 때
		else
		{
			// 리스트의 맨 앞에 노드를 삽입
			new_node->link = head;
			head = new_node;
		}
	}
};


// 노드 삽입


// 리스트 메모리 반환 함수
void free_list(ListNode** phead)
{
	ListNode* p = *phead;
	while (*phead != NULL)
	{
		*phead = (*phead)->link;
		free(p);
		p = *phead;
	}
}

// 노드 생성 함수
ListNode *create_node(Board data, ListNode *link)
{
	ListNode *new_node;
	new_node = (ListNode*)malloc(sizeof(ListNode));
	if (new_node == NULL)
		return NULL;
	new_node->data = data;
	new_node->link = link;
	return (new_node);
}