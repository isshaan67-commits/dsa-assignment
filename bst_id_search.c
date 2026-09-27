#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char id[10];
    struct Node *left, *right;
} Node;

Node* createNode(const char *id) {
    Node *n = (Node*)malloc(sizeof(Node));
    strcpy(n->id, id);
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node *root, const char *id, int *comparisons) {
    if (root == NULL) return createNode(id);
    if (comparisons) (*comparisons)++;
    if (strcmp(id, root->id) < 0)
        root->left = insert(root->left, id, comparisons);
    else if (strcmp(id, root->id) > 0)
        root->right = insert(root->right, id, comparisons);
    return root;
}

void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%s ", root->id);
    inorder(root->right);
}

int height(Node *root) {
    if (root == NULL) return -1;
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

int bstSearch(Node *root, const char *key, int *comparisons) {
    if (root == NULL) return 0;
    (*comparisons)++;
    int cmp = strcmp(key, root->id);
    if (cmp == 0) return 1;
    if (cmp < 0) return bstSearch(root->left, key, comparisons);
    return bstSearch(root->right, key, comparisons);
}

int linearSearch(char arr[][10], int n, const char *key, int *comparisons) {
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (strcmp(arr[i], key) == 0) return 1;
    }
    return 0;
}

int main() {
    char ids[8][10] = {"A102","A25","A7","B100","B12","A120","B3","A45"};
    int n = 8;

    printf("=== Part (a): BST Construction ===\n");
    Node *root = NULL;
    for (int i = 0; i < n; i++) {
        int c = 0;
        root = insert(root, ids[i], &c);
        printf("Inserted %-6s (comparisons: %d)\n", ids[i], c);
    }
    printf("\nInorder Traversal: ");
    inorder(root);
    printf("\nTree Height (edges from root): %d\n", height(root));

    printf("\n=== Part (b): BST Search vs Linear Search ===\n");
    char *searchKeys[] = {"A102", "B3", "A45", "C1"};
    printf("%-6s %-10s %-14s %s\n", "Key", "BST_Comps", "Linear_Comps", "Result");
    for (int i = 0; i < 4; i++) {
        int bstC = 0, linC = 0;
        int foundB = bstSearch(root, searchKeys[i], &bstC);
        linearSearch(ids, n, searchKeys[i], &linC);
        printf("%-6s %-10d %-14d %s\n", searchKeys[i], bstC, linC,
               foundB ? "found" : "not found");
    }

    printf("\n=== Part (c): Effect of Insertion Order (sorted-order insertion) ===\n");
    char sortedIds[8][10] = {"A102","A120","A25","A45","A7","B100","B12","B3"};
    Node *root2 = NULL;
    for (int i = 0; i < n; i++)
        root2 = insert(root2, sortedIds[i], NULL);
    printf("Inorder Traversal: ");
    inorder(root2);
    printf("\nTree Height (edges from root): %d\n", height(root2));
    int c2 = 0;
    bstSearch(root2, "B3", &c2);
    printf("Search 'B3' in sorted-insertion tree -> comparisons: %d\n", c2);

    return 0;
}
