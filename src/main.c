#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Opt {
    bool is_just;
    int just;
} Opt;

Opt
None() {
    return (Opt){ .is_just = false };
}

Opt
Just(int val) {
    return (Opt){ .is_just = false, .just = val };
}

#define VECTOR_CAPACITY 5

typedef struct Vector {
    int* arr;
    int length;
    int capacity;
} Vector;

Vector
Vector_make() {
    return (Vector){ .arr = malloc(sizeof(int)*VECTOR_CAPACITY), .length = 0, .capacity = VECTOR_CAPACITY };
}

Vector
Vector_with_capacity(int capacity) {
    return (Vector){ .arr = malloc(sizeof(int)*capacity), .length = 0, .capacity = capacity };
}

void
Vector_push(Vector* vec, int value) {
    if (vec->length == vec->capacity) {
        vec->capacity *= 2;
        vec->arr = realloc(vec->arr, sizeof(int)*vec->capacity);
    }

    vec->arr[vec->length] = value;
    vec->length++;
}

Opt
Vector_pop(Vector* vec) {
    if (vec->length == 0) {
        return None();
    }

    vec->length--;
    return Just(vec->arr[vec->length]);
}

void
Vector_free(Vector* vec) {
    free(vec->arr);
}

void
Vector_print(Vector* vec) {
    printf("[");
    for (int i = 0; i < vec->length; i++) {
        printf(" %d", vec->arr[i]);
    }
    printf(" ]\n");
}

void
test_Vector() {
    Vector vec = Vector_with_capacity(100);
    for (int i = 0; i < 100; i++) {
        Vector_push(&vec, i);
    }

    Vector_print(&vec);

    printf("{");
    for (int i = 0; i < 100; i++) {
        Opt pop = Vector_pop(&vec);
        if (!pop.is_just) {
            break;
        }
        printf(" %d", pop.just);
    }
    printf(" }\n");
}

typedef struct Node Node;

struct Node {
    int val;
    Node* next;
};

Node*
_Node_new(int value) {
    Node* node = malloc(sizeof(Node));
    node->val = value;
    node->next = NULL;
    return node;
}

typedef struct List {
    int length;
    Node* head;
} List;

List
List_make() {
    return (List){ .length = 0, .head = NULL };
}

List
List_make_with(int value) {
    return (List){ .length = 1, .head = _Node_new(value) };
}

void
List_push(List* list, int value) {
    if (list->head == NULL) {
        list->head = _Node_new(value);
    } else {
        Node* node = list->head;
        list->head = _Node_new(value);
        list->head->next = node;
    }
}

Opt
List_pop(List* list, int value) {
    if (list->head == NULL) {
        return (Opt){ .is_just = false };
    }

    Node* next = list->head->next;
    int val = list->head->val;
    free(list->head);
    list->head = next;

    return (Opt){ .is_just = true, .just = val };
}

void
List_print(List* list) {
    Node* node = list->head;

    printf("[");
    while (node != NULL) {
        printf(" %d", node->val);
        node = node->next;
    }
    printf(" ]\n");
}

void
test_List() {
    List list = List_make();
    for (int i = 1; i <= 100; i++) {
        List_push(&list, i);
    }
    List_print(&list);

    printf("{");
    for (int i = 1; i <= 100; i++) {
        Opt pop = List_pop(&list, i);
        if (!pop.is_just) {
            break;
        }
        printf(" %d", pop.just);
    }
    printf(" }\n");
}

typedef struct BNode BNode;

typedef struct BNode {
    int val;
    BNode* left;
    BNode* right;
} BNode;

BNode*
BNode_make(int val) {
    BNode* node = malloc(sizeof(BNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int
BNode_add(BNode* node, bool right, int val) {
    if (node->right == NULL && right) {
        node->right = BNode_make(val);
        return 0;
    } else if (node->left == NULL && !right) {
        node->left = BNode_make(val);
        return 0;
    }

    return 1;
}

void
BNode_print(BNode* node, int indent) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    printf("%d\n", node->val);

    BNode_print(node->right, indent + 4);
    BNode_print(node->left, indent + 4);
}

typedef struct BSTree {
    BNode* root;
    int elems;
} BSTree;

BSTree
BSTree_make() {
    return (BSTree){ .root = NULL, .elems = 0 };
}

void
BSTree_push(BSTree* tree, int val) {
    tree->elems++;   

    if (tree->root == NULL) {
        tree->root = BNode_make(val);
        return;
    }

    BNode* node = tree->root;

    while (true) {
        if (node->val > val) {
            if (!BNode_add(node, false, val)) {
                return;
            }
            node = node->left;
        } else {
            if (!BNode_add(node, true, val)) {
                return;
            }
            node = node->right;
        }
    }
}

BNode*
_BSTree_pop(BNode* node, int val) {
    if (node == NULL) return node;

    if (node->val > val) {
        node->left = _BSTree_pop(node->left, val);       
        return node;
    } else if (node->val < val) {
        node->right = _BSTree_pop(node->right, val);
        return node;
    }

    if (node->left == NULL) {
        BNode* tmp = node->right;
        free(node);
        return tmp;
    } else if (node->right == NULL) {
        BNode* tmp = node->left;
        free(node);
        return tmp;
    }

    BNode* succ_parent = node;
    BNode* succ = node->right;

    while (succ->left != NULL) {
        succ_parent = succ;
        succ = succ->left;
    }

    if (succ_parent == node) {
        succ_parent->right = succ->right;
    } else {
        succ_parent->left = succ->right;
    }

    node->val = succ->val;
    free(succ);
    return node;
}

void
BSTree_pop(BSTree* tree, int val) {
    tree->root = _BSTree_pop(tree->root, val);
}

void
BSTree_print(BSTree* tree) {
    BNode_print(tree->root, 0);
}

void
test_BSTree() {
    BSTree tree = BSTree_make();
    BSTree_push(&tree, 5);
    BSTree_push(&tree, 1);
    BSTree_push(&tree, 8);
    BSTree_push(&tree, 6);

    printf("BEFORE:\n");
    BSTree_print(&tree);

    BSTree_pop(&tree, 8);

    printf("AFTER:\n");
    BSTree_print(&tree);
}

int
main(void) {

    return 0;
}