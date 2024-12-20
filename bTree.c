#include <stdio.h>
#include <stdlib.h>

#define ORDER 2
#define MAX_KEY 4
#define MAX_POINTER 5

typedef struct node
{
    int counter;
    struct node *father;
    struct node *pointer[MAX_POINTER];
    int keys[MAX_KEY];
} node;

node *createNode(int val)
{
    node *new = (node *)malloc(sizeof(node));
    new->keys[0] = val;
    new->pointer[0] = new->pointer[1] = NULL;
    new->counter++;
}

void addToTable(node **element, int index, int val)
{
    (*element)->keys[index] = val;
    (*element)->pointer[index] = (*element)->pointer[index + 1] = NULL;
    (*element)->counter++;
}

void insertionSort(int *tab, int size)
{
    int j, tmp;
    for (int i = 0; i < size - 1; i++)
    {
        j = i + 1;
        if (tab[i] > tab[j])
        {
            tmp = tab[i];
            tab[i] = tab[j];
            tab[j] = tmp;
            if (i > 0)
            {
                i = i - 2;
            }
        }
    }
}

int indexFinder(int *tab, int size, int verify)
{
    for (int i = 0; i < size; i++)
    {
        if (verify > tab[i])
        {
            return i;
        }
    }
}

void printTable(int tab[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", tab[i]);
    }
    printf("\n");
}

int *copyTable(int tab[], int size, int newVal)
{
    int *newTab = malloc((size + 1) * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        newTab[i] = tab[i];
    }
    newTab[size] = newVal;
    return newTab;
}
node *insertAfterDeviding(int *newTab, node *origin, int start, int end);
void insertElement(node **tree, int val);

int main()
{
    node *BTree;
    insertElement(&BTree, 11);
    insertElement(&BTree, 22);
    insertElement(&BTree, 33);
    insertElement(&BTree, 44);
    insertElement(&BTree, 10);
    printf("%d\n", BTree->keys[0]);
    printf("the left son\t");
    printf("%d\n", BTree->pointer[0]->keys[0]);
    printf("%d\n", BTree->pointer[0]->keys[1]);
    printf("the right son\t");
    printf("%d\n", BTree->pointer[1]->keys[0]);
    printf("%d\n", BTree->pointer[1]->keys[1]);
    if (BTree->pointer[1]->pointer[0] == NULL)
    {
        printf("true\n");
    }

    return 0;
}

node *insertAfterDeviding(int *newTab, node *origin, int start, int end)
{
    node *newElement = (node *)malloc(sizeof(node));
    for (int i = start; i <= end; i++)
    {
        insertElement(&newElement, newTab[i]);
    }
    newElement->father = origin;
    for (int i = 0; i < MAX_KEY; i++)
    {
        newElement->pointer[i] = NULL;
    }
    return newElement;
}

void insertElement(node **tree, int val)
{
    if (*tree == NULL)
    {
        *tree = createNode(val);
    }
    else
    {
        if ((*tree)->pointer[0] == NULL)
        {
            if ((*tree)->counter < MAX_KEY)
            {
                addToTable(&(*tree), (*tree)->counter, val);
            }
            else
            {
                int *new = copyTable((*tree)->keys, (*tree)->counter, val);
                insertionSort(new, (*tree)->counter + 1);
                printf("new table is   ");
                printTable(new, 5);
                int midVal = new[(MAX_KEY / 2)];
                node *smaller = insertAfterDeviding(new, (*tree), 0, (MAX_KEY / 2) - 1);
                printf("the smaller are\t");
                printTable(smaller->keys, 4);
                node *bigger = insertAfterDeviding(new, (*tree), (MAX_KEY / 2) + 1, MAX_KEY);
                printf("the bigger are\t");
                printTable(bigger->keys, 4);
                if ((*tree)->father == NULL)
                {
                    printf("the facther doesn't exist");
                    node *newfather;
                    insertElement(&newfather, midVal);
                    newfather->pointer[0] = smaller;
                    newfather->pointer[1] = bigger;
                    *tree = newfather;
                    smaller->father = newfather;
                    bigger->father = newfather;
                }
                else
                {
                    insertElement(&(*tree)->father, midVal);
                }
            }
        }
        else
        {
            int index = indexFinder((*tree)->keys, MAX_KEY, val);
            insertElement((*tree)->pointer[index], val);
        }
    }
}
