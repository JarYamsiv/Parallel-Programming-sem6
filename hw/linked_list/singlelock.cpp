#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


typedef struct node
{
    int data;
    struct node * next;
}node;

class linked_list
{
private:
    node* head;
public:
    linked_list(int data);
    int insert(int data);
    void printlist();
    node* search(int data);
    ~linked_list();
};

linked_list::linked_list(int data)
{
    node * new_node = (node*)malloc(sizeof(node));
    new_node->data = data;
    new_node->next = NULL;
    head = new_node;
}

linked_list::~linked_list()
{
    node * cursor;
    node * del_ptr;
    cursor = head;
    while(cursor != NULL){
        del_ptr = cursor;
        cursor = cursor->next;
        free(del_ptr);
    }
    
}

int linked_list::insert(int data)
{
    node * new_node = (node*)malloc(sizeof(node));
    new_node->data = data;
    new_node->next = head;
    head = new_node;
}

void linked_list::printlist()
{
    node *cursor;
    cursor = head;
    while(cursor!=NULL){
        printf("%d ",cursor->data);
        cursor = cursor->next;
    }
    
}


node * linked_list::search(int search_item)
{
    node* cursor;
    cursor = head;
    while(cursor != NULL)
    {
        if(cursor->data==search_item)
        {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    class linked_list ll(1);
    ll.insert(12);
    ll.insert(34);
    ll.insert(45);
    ll.printlist();
    node* res = ll.search(456);
    if(res !=NULL)
    {
        printf("found");
    }
    else
    {
        printf("not found");
    }
    
    return 0;
}
