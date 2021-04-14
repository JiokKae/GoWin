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
		// ����Ʈ�� ������� ��
		if (head == NULL)
		{
			// ����Ʈ�� ��带 ����
			new_node->link = NULL;
			head = new_node;
		}
		// ����Ʈ�� ������� ���� ��
		else
		{
			// ����Ʈ�� �� �տ� ��带 ����
			new_node->link = head;
			head = new_node;
		}
	}
};


// ��� ����


// ����Ʈ �޸� ��ȯ �Լ�
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

// ��� ���� �Լ�
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