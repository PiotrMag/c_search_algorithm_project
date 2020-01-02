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

void DeleteListElement(ELEMENT element)
{
    free(element->content);
    free(element);
}

// TODO: write this function
void AddElement(SORTED_LIST list, ELEMENT new_element)
{
    ELEMENT curr_element = (ELEMENT)list;
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

// TODO: write these functions
ELEMENT GetFirstElementWithValue(int value);
void RemoveFirstElementWithValue(int value);
