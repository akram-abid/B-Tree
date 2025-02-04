#include <stdio.h>
#include <stdlib.h>

#define M 2
#define MAX_KEY (2 * M)
#define MAX_POINTER (2 * M + 1)

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
    for (int i = 1; i < MAX_KEY; i++)
    {
        new->keys[i] = 0;
    }
    for (int i = 0; i < MAX_POINTER; i++)
    {
        new->pointer[i] = NULL;
    }
    new->counter = 1;
    new->father = NULL;
    return new;
}

void addToTable(node **element, int index, int val)
{
    (*element)->keys[index] = val;
    (*element)->counter++;
}

void insertionSort(int *tab, int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = tab[i];
        int j = i - 1;
        while (j >= 0 && tab[j] > key)
        {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = key;
    }
}

int indexFinder(int *tab, int size, int verify)
{
    for (int i = 0; i < size; i++)
    {
        if (verify < tab[i])
        {
            return i;
        }
    }
    return size;
}

void printTable(int tab[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", tab[i]);
    }
    printf("\n");
}

void *shiftTable(int *tab, int size, int index, int val){
    for (int i = size - 1; i > index; i--)
    {
        tab[i] = tab[i - 1];
    }
    tab[index] = val;
}

void *shiftPointers(node *tab, int size, int index){
    for (int i = size - 1; i > index + 1; i--)
    {
        tab[i] = tab[i - 1];
    }
}

int *copyTable(int tab[], int size, int newVal){
    int *newTab = malloc((size + 1) * sizeof(int));
    if (!newTab)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
    {
        newTab[i] = tab[i];
    }
    newTab[size] = newVal;
    return newTab;
}
int getMidIndex(int size, int isRoot){
    return isRoot ? (size / 2) : M;
}

node *insertAfterDividing(int *newTab, node *origin, int start, int end){
    node *newElement = (node *)malloc(sizeof(node));
    if (!newElement)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newElement->counter = 0;
    for (int i = 0; i < MAX_KEY; i++)
    {
        newElement->keys[i] = 0;
    }
    for (int i = start; i < MAX_POINTER; i++)
    {
        newElement->pointer[i] = origin->pointer[i];
    }
    for (int i = start; i <= end; i++)
    {
        addToTable(&newElement, newElement->counter, newTab[i]);
    }
    newElement->father = origin->father;
    return newElement;
}

int findVal(int *tab, int count, int val){
    for (int i = 0; i < count; i++)
    {
        if (tab[i] == val)
            return i;
    }
}

void printBTree(node *root, int level){
    if (root != NULL)
    {
        printf("Level %d: ", level);
        printTable(root->keys, root->counter);

        for (int i = 0; i <= root->counter; i++)
        {
            if (root->pointer[i] != NULL)
            {
                printBTree(root->pointer[i], level + 1);
            }
        }
    }
}

void insertElement(node **tree, int val){
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
                addToTable(tree, (*tree)->counter, val);
                insertionSort((*tree)->keys, (*tree)->counter);
            }else{
                int *new = copyTable((*tree)->keys, (*tree)->counter, val);
                insertionSort(new, (*tree)->counter + 1);
                int midIndex = getMidIndex(MAX_KEY + 1, (*tree)->father == NULL);
                int midVal = new[midIndex];
                node *smaller = insertAfterDividing(new, *tree, 0, midIndex - 1);
                node *bigger = insertAfterDividing(new, *tree, midIndex + 1, MAX_KEY);
                if ((*tree)->father == NULL){
                    node *newFather = createNode(midVal);
                    newFather->pointer[0] = smaller;
                    newFather->pointer[1] = bigger;
                    *tree = newFather;
                    smaller->father = newFather;
                    bigger->father = newFather;
                }else{
                    int index = indexFinder((*tree)->father->keys, (*tree)->father->counter, midVal);
                    if ((*tree)->father->counter < 4)
                    {
                        shiftTable((*tree)->father->keys, 4, index, midVal);
                        (*tree)->father->counter++;
                        for (int i = (*tree)->father->counter; i > index + 1; i--)
                        {
                            (*tree)->father->pointer[i] = (*tree)->father->pointer[i - 1];
                        }
                        (*tree)->father->pointer[index] = smaller;
                        (*tree)->father->pointer[index + 1] = bigger;
                        smaller->father = (*tree)->father;
                        bigger->father = (*tree)->father;
                    }else{
                        int *newfath = copyTable((*tree)->father->keys, (*tree)->father->counter, midVal);
                        insertionSort(newfath, (*tree)->counter + 1);
                        int fathIndex = findVal(newfath, 5, midVal);
                        node *smallfath = insertAfterDividing(newfath, *tree, 0, 1);
                        for (int i = 0; i <= 2; i++)
                        {
                            smallfath->pointer[i] = (*tree)->father->pointer[i];
                        }
                        node *bigfath = insertAfterDividing(newfath, *tree, 3, 4);
                        for (int i = 3; i < 5; i++)
                        {
                            bigfath->pointer[i - 2] = (*tree)->father->pointer[i];
                        }
                        if (fathIndex >= 3 || fathIndex <= 1)
                        {
                            if (fathIndex >= 3)
                            {
                                int newBIndex = findVal(bigfath->keys, 4, midVal);
                                bigfath->pointer[newBIndex] = smaller;
                                bigfath->pointer[newBIndex + 1] = bigger;
                                smaller->father = bigfath;
                                bigger->father = bigfath;
                                if ((*tree)->father->father == NULL)
                                {
                                    node *grandFather = NULL;
                                    midVal = newfath[2];
                                    insertElement(&grandFather, midVal);
                                    grandFather->pointer[0] = smallfath;
                                    grandFather->pointer[1] = bigfath;
                                    smallfath->father = grandFather;
                                    bigfath->father = grandFather;
                                    *(*tree)->father = *(grandFather);
                                }
                            }else{
                                int newSIndex = findVal(smallfath->keys, 4, midVal);
                                smallfath->pointer[newSIndex] = smaller;
                                smallfath->pointer[newSIndex + 1] = bigger;
                                smaller->father = smallfath;
                                bigger->father = smallfath;
                            }
                        }
                    }
                }
                free(new);
            }
        }else{
            int index = indexFinder((*tree)->keys, (*tree)->counter, val);
            insertElement(&(*tree)->pointer[index], val);
        }
    }
}
int main()
{
    node *BTree = NULL;
    int values[] = {33, 22, 11, 66, 12, 14, 13, 55, 44, 24, 52, 43, 21, 1, 2, 34, 4, 77, 35,39,7};
    int numValues = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < numValues; i++)
    {
        insertElement(&BTree, values[i]);
    }
    printf("Tree after insertion:\n");
    printBTree(BTree, 0);
    return 0;
}