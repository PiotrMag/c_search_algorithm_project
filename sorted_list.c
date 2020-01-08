#include <stdlib.h>

typedef struct Element *ELEMENT;
typedef ELEMENT SORTED_LIST;

struct Element
{
    int value;
    void *content;
    ELEMENT next_element;
};

ELEMENT CreateListElement(int value, void *content)
{
    ELEMENT new_element = (ELEMENT)malloc(sizeof(struct Element));
    new_element->value = value;
    new_element->content = content;
    new_element->next_element = NULL;
    return new_element;
}

void AddElement(SORTED_LIST *list, ELEMENT new_element)
{
    ELEMENT curr_element = (ELEMENT)(*list);
    if (curr_element == NULL)
    {
        *list = new_element;
        return;
    }
    ELEMENT next_element = NULL;
    if (curr_element->next_element != NULL)
        next_element = curr_element->next_element;

    while(next_element != NULL && new_element->value > next_element->value)
    {
        curr_element = next_element;
        next_element = curr_element->next_element;
    }

    curr_element->next_element = new_element;
    new_element->next_element = next_element;
}

void* PopFirstElement(SORTED_LIST *list)
{
    if (*list == NULL)
        return NULL;
    ELEMENT element = (*list);
    (*list) = (*list)->next_element;
    void * content = element->content;
    free(element);
    return content;
}

void DeleteList(SORTED_LIST *list)
{
    void *popped_value;

    while(1)
    {
        popped_value = PopFirstElement(list);
        if (popped_value == NULL)
            break;  
        free(popped_value);
    }
}
