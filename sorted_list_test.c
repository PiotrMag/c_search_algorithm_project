#include <stdio.h>
#include "sorted_list.h"

void PrintList(SORTED_LIST list)
{
    int index = 0;
    ELEMENT element = (ELEMENT)list;
    while(element != NULL)
    {
        printf("i=%d, val=%d, con=%d\n", index, (int)element->value, *((int*)(element->content)));
        element = element->next_element;
        index++;
    }
}

int main()
{
    SORTED_LIST list = NULL;
    int content1 = 1;
    int content2 = 2;
    int content3 = 3;
    printf("Step 1\n");
    AddElement(&list, CreateListElement(1, (void*)(&content1)));
    AddElement(&list, CreateListElement(3, (void*)(&content3)));
    AddElement(&list, CreateListElement(2, (void*)(&content2)));
    printf("Step 2\n");
    PrintList(list);
    printf("Step 3\n");
    DeleteList(&list);
    printf("Step 4\n");

    char znak;
    scanf("%c", &znak);
    return 0;
}