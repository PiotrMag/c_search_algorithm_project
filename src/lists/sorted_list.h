typedef struct Element *ELEMENT;
typedef ELEMENT SORTED_LIST;

struct Element
{
    int value;
    int help_value;
    void *content;
    ELEMENT next_element;
};

int GetListLength(SORTED_LIST list);
ELEMENT CreateListElement(int value, int help_value, void *content);
void AddElement(SORTED_LIST *list, ELEMENT new_element);
void* PopFirstElement(SORTED_LIST *list);
void DeleteList(SORTED_LIST *list);