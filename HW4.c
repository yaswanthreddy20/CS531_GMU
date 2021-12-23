#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0
typedef int bool;

//structure for IP addresses
struct address_t
{
    int octet[4];
    char alias[11];
    struct address_t *leftChild, *rightChild;
};
typedef struct address_t node;

//definitions of the functions
bool checkAlias(node **head, char *alias, int *flag);
bool checkIP(node **head, int ip[], int *flag);
void writeData(node *tree, char filename[]);
void inOrderTraversal(node *tree, int *count);
void findUsingLoc(node **head, int loc[], int *flag);
void searchIPbyLocation(node **head);
bool biggestIP(char *s1, char *s2);
int updateIPAddress(node **head);
void update(node **head, int ip[], char *alias);
int compareIP(char *s1, char *s2, int l1, int l2);
void insertNode(node **tree, int val[], char alias[]);
node *searchByAlias(node **head, char *alias);

//function used to updater tree
void update(node **head, int ip[], char *alias)
{
    if (*head)
    {
        update((&(*head)->leftChild), ip, alias);
        int l1 = strlen((*(head))->alias);
        int l2 = strlen(alias);
        if (compareIP((*(head))->alias, alias, l1, l2) == 1)
        {
            for (int i = 0; i < 4; i++)
                ((*head))->octet[i] = ip[i];
        }
        update((&(*head)->rightChild), ip, alias);
    }
}


//comparing IP to see if they are same
int compareIP(char *s1, char *s2, int l1, int l2)
{
    if (l1 != l2)
    {
        return 0;
    }
    for (int i = 0; i < l1; i++)
    {
        char a = toupper(s1[i]);
        char b = toupper(s2[i]);
        if (a != b)
        {
            return 0;
        }
    }
    return 1;
}

//function to check if IP already exsists in the tree
bool checkIP(node **head, int loc[], int *flag) 
{
    if (*head)
    {
        checkIP((&(*head)->leftChild), loc, &(*flag));

        if ((*head)->octet[0] == loc[0] && (*(head))->octet[1] == loc[1] && (*(head))->octet[2] == loc[2] && (*(head))->octet[3] == loc[3])
        {
            *flag = 1;
            return 1;
        }
        checkIP((&(*head)->rightChild), loc, &(*flag));
    }
    return 0;
}

//function to check if Alias already exsists in the tree
bool checkAlias(node **head, char *alias, bool *flag) 
{
    if (*head)
    {
        checkAlias(&((*head)->leftChild), alias, &(*flag));
        int l1 = strlen((*(head))->alias);
        int l2 = strlen(alias);

        if (compareIP((*(head))->alias, alias, l1, l2) == 1)
        {
            *flag = 1;
            return 1;
        }
        checkAlias(&((*head)->rightChild), alias, &(*flag));
    }
    return 0;
}

//function to insert node in BST
void insertNode(node **tree, int val[], char alias[]) 
{
    node *temp = NULL;
    if ((*tree) == NULL)
    {
        temp = (node *)malloc(sizeof(node));
        temp->leftChild = temp->rightChild = NULL;
        for (int i = 0; i < 4; i++)
            temp->octet[i] = val[i];

        strcpy(temp->alias, alias);
        *tree = temp;
        return;
    }

    if (biggestIP((*tree)->alias, alias))
    {
        insertNode(&(*tree)->leftChild, val, alias);
    }
    else
    {
        insertNode(&(*tree)->rightChild, val, alias);
    }
}

//used to build tree based on the biggest IP
bool biggestIP(char *s1, char *s2)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    for (int i = 0; i < l1; i++)
    {
        if (s1[i] > s2[i])
            return 1;
        if (s1[i] < s2[i])
            return 0;
    }
    return 0;
}

//search IP in the tree by alias name
node *searchByAlias(node **head, char *alias)
{
    if (*head)
    {
        searchByAlias((&(*head)->leftChild), alias);
        int l1 = strlen((*(head))->alias);
        int l2 = strlen(alias);

        if (compareIP((*(head))->alias, alias, l1, l2) == 1)
        {
            printf("%s %d.%d.%d.%d\n", (*head)->alias, (*head)->octet[0], (*head)->octet[1], (*head)->octet[2], (*head)->octet[3]);
            return ((*head));
        }
        searchByAlias((&(*head)->rightChild), alias);
    }
    return NULL;
}

//update the IP address after finding by alias name
int updateIPAddress(node **head)
{
    char alias[11];
    printf("Enter alias name:");
    scanf("%s", alias);
    char ip[25];
    char dot;
    int IP[4];

    bool flag = 0;
    checkAlias(&(*(head)), alias, &flag);

    if (flag != 1)
    {
        printf("Alias %s does not exists\n", alias);
        return -1;
    }
    for (int i = 0; i < 4; i++)
    {
        while (1)
        {
            printf("enter locataion #%d:", i + 1);
            scanf("%d", &IP[i]);
           
            if (IP[i] >= 0 && IP[i] <= 255)
            {
                break;
            } 
            else
            {
                printf("ERROR: must be in range of 0 to 255\n");
            }
        }
    }
    flag = 0;
    checkIP(&(*(head)), IP, &flag);

    if (flag == 1)
    {
        printf("ERROR: IP Address already exists\n");

        return -1;
    }
    update(&(*head), IP, alias);

    return 0;
}

//function to searcg IP by location
void searchIPbyLocation(node **head)
{
    int a;
    int b;
    int loc[2];
    for (int i = 0; i < 2; i++)
    {
        while (1)
        {
            printf("enter locataion #%d:", i + 1);
            scanf("%d", &loc[i]);
            if (loc[i]<=0)
            {
                printf("ERROR: Only integers are allowed, Please re-run the program\n");
                break;
            } 
            if (loc[i] >= 0 && loc[i] <= 255)
            {
                break;
            }
            else
            {
                printf("ERROR: must be in range of 0 to 255\n");
            }
        }
    }
    int flag = 0;
    findUsingLoc(head, loc, &flag);
    if (!flag)
        printf("ERROR:no matching location exists\n");
}


//function to insert IP addresses(helper)
int insertIPAddress(node **root) 
{
    char alias[11];
    char ip[25];
    char dot;
    int IP[4];
    struct address_t *add = NULL;
    bool flag = 0;
    printf("Enter alias name:");
    scanf("%s", alias);
    flag = 0;
    checkAlias(root, alias, &flag);

    if (flag == 1)
    {
        printf("Alias %s already exists\n", alias);
        return -1;
    }
    printf("Enter address for %s: ", alias);
    scanf("%s", ip);
    sscanf(ip, "%d%c%d%c%d%c%d", &IP[0], &dot, &IP[1], &dot, &IP[2], &dot, &IP[3]);
    struct address_t *t;
    flag = 0;
    checkIP(&(*(root)), IP, &flag);
    if (flag == 1)
    {
        printf("ERROR: IP Address already exists\n");
        return -1;
    }
    if (IP[0] < 0 || IP[0] > 255)
    {
        printf("ERROR: octet must be in range of 0 to 255\n");
        return -1;
    }
    if (IP[0] < 0 || IP[0] > 255)
    {
        printf("ERROR: octet must be in range of 0 to 255\n");
        return -1;
    }
    if (IP[0] < 0 || IP[0] > 255)
    {
        printf("ERROR: octet must be in range of 0 to 255\n");
        return -1;
    }
    if (IP[0] < 0 || IP[0] > 255)
    {
        printf("ERROR: octet must be in range of 0 to 255\n");
        return -1;
    }
    
    insertNode(&(*root), IP, alias);
    return 0;
}
//Helper function to find minimum value node in the subtree rooted at `curr`
struct address_t *minValueNode(struct address_t *node)
{
    struct address_t *current = node;
    while (current && current->leftChild != NULL)
        current = current->leftChild;
    return current;
}
struct address_t *deleteIPAddress(struct address_t *head, char *alias, int con)
{
    if (head == NULL)
    {
        printf("Alias %s not found\n", alias);
        return head;
    }
    if (biggestIP(head->alias, alias) == 1)
    {
        head->leftChild = deleteIPAddress(head->leftChild, alias, 1);
    }
    else if (biggestIP(alias, head->alias) == 1)
    {
        head->rightChild = deleteIPAddress(head->rightChild, alias, 1);
    }
    else
    {
        if (con)
        {
            char conf;
            int a = head->octet[0];
            int b = head->octet[1];
            int c = head->octet[2];
            int d = head->octet[3];
            printf("do you want to delete %s %d.%d.%d.%d choose yes(y) or no(n)?:", alias, a, b, c, d);
            scanf(" %c", &conf);
            if (conf == 'n')
            {
                return head;
            }
        }
        if (head->leftChild == NULL)
        {
            struct address_t *temp = head->rightChild;
            free(head);
            return temp;
        }
        else if (head->rightChild == NULL)
        {
            struct address_t *temp = head->leftChild;
            free(head);
            return temp;
        }
        struct address_t *temp = minValueNode(head->rightChild);
        strcpy(head->alias, temp->alias);
        head->rightChild = deleteIPAddress(head->rightChild, temp->alias, 0);
    }
    return head;
}

//function to read the file and store IP addresses in the Binary tree
int readFile(node **tree, char *file_path)
{
    char ip[15];
    int IP[4];
    char alias[32];
    int a, b, c, d;
    char dot;
    FILE *fptr = fopen(file_path, "r");
    if (fptr == NULL)
    {
        printf("ERROR: file cannot be opened\n");
        return -1;
    }
    while (!feof(fptr))
    {
        fscanf(fptr, "%d%c%d%c%d%c%d %s", &a, &dot, &b, &dot, &c, &dot, &d, alias);
        if (strlen(alias) != 0)
        {
            IP[0] = a;
            IP[1] = b;
            IP[2] = c;
            IP[3] = d;
            insertNode(&(*tree), IP, alias);
        }
        strcpy(ip, "");
        strcpy(alias, "");
    }
    fclose(fptr);
    return 0;
}


void writeData(node *tree, char filename[])
{
    if (tree == NULL)
        return;
    writeData(tree->leftChild, filename);
    FILE *fptr;
    fptr = fopen(filename, "a");
    fprintf(fptr, "%s %d.%d.%d.%d \n", tree->alias, tree->octet[0], tree->octet[1], tree->octet[2], tree->octet[3]);
    fclose(fptr);
    writeData(((tree))->rightChild, filename);
}

//function used to search IP by location
void findUsingLoc(node **head, int loc[], int *flag)
{
    if (*head)
    {
        findUsingLoc(&((*head)->leftChild), loc, flag);
        if ((*head)->octet[0] == loc[0] && (*head)->octet[1] == loc[1])
        {
            *flag = 1;
            printf("%s\n", (*head)->alias);
        }
        findUsingLoc((&(*head)->rightChild), loc, flag);
    }
}

//function that prints Inorder traversal of tree
void inOrderTraversal(node *tree, int *count)
{
    if (tree == NULL)
        return;
    inOrderTraversal(tree->leftChild, count);
    printf("%s %d.%d.%d.%d \n", tree->alias, tree->octet[0], tree->octet[1], tree->octet[2], tree->octet[3]);
    *count = *count + 1;
    inOrderTraversal(tree->rightChild, count);
}



int main()
{
    struct address_t *root;
    root = NULL;
    int choice;
    readFile(&root, "CS531_Inet.txt");
    while (1)
    {
        printf("\n*********************************************\n");
        printf("Please select an option from the below:\n");
        printf("1. Add address\n");
        printf("2. Look up address\n");
        printf("3. Update address\n");
        printf("4. Delete address\n");
        printf("5. Display list\n");
        printf("6. Display aliases for location\n");
        printf("7. Save to file\n");
        printf("8. Quit\n");
        printf("*********************************************\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        if (choice<=0)
        {
            printf("ERROR: Only integers are allowed, Please re-run the program\n");
            return 0;
        } 
        switch (choice)
        {
        case 1:
        {
            insertIPAddress(&root);
            break;
        }
        case 2:
        {
            char alias[11];
            printf("Enter alias name:");
            scanf("%s", alias);
            bool flag = 0;
            checkAlias(&root, alias, &flag);
            if (flag == 1)
            {
                searchByAlias(&root, alias);
                continue;
            }
            else
                printf("Alias name %s does not exist in the file\n", alias);
            break;
        }
        case 3:
        {
            updateIPAddress(&root);
            break;
        }
        case 4:
        {
            char alias[11];
            printf("Enter Alias name to delete:");
            scanf("%s", alias);
            deleteIPAddress(root, alias, 1);
            break;
        }
        case 5:
        {
            int count = 0;
            inOrderTraversal(root, &count);
            printf("total IP Addresses count: %d\n", count);
            break;
        }
        case 6:
        {
            searchIPbyLocation(&root);
            break;
        }
        case 7:
        {
            char filename[100];
            printf("Enter file name to save: ");
            scanf("%s", filename);
            fclose(fopen(filename, "w"));
            writeData(root, filename);
            printf("=====Succesfully saved to file %s=====",filename);
            break;
        }
        case 8:
        {
            printf("*********** Program Terminated ***********\n");
            return 0;
            break;
        }
        default:
        {
            printf("ERROR: invalid selection \n");
            break;
        }
        }
    }
    return 0;
}