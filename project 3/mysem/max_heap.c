#include "linux/max_heap.h"
#include <linux/slab.h>


struct maxHeap maxHeap_init(int size){
    struct maxHeap hp ;
    hp.size = 0 ;
    hp.elem = NULL ;
    return hp ;
    printk("Created a maxHeap!\n");
}
void swap_maxHeap_nodes(struct maxHeap_node *n1, struct maxHeap_node *n2){
    struct maxHeap_node temp = *n1 ;
    *n1 = *n2 ;
    *n2 = temp ;
    printk("Swapped two maxHeap nodes\n");
}

void heapify_maxHeap(struct maxHeap *hp, int i){
    int largest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].priority > hp->elem[i].priority) ? LCHILD(i) : i ;
    if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].priority > hp->elem[largest].priority) {
        largest = RCHILD(i) ;
    }
    if(largest != i) {
        swap_maxHeap_nodes(&(hp->elem[i]), &(hp->elem[largest])) ;
        heapify_maxHeap(hp, largest) ;
    }
}

void print_wait_queue(struct maxHeap *hp){
    int i;
    for (i = 0; i < hp->size; ++i)
    {
        printk("my priority is:%d\n",hp->elements[i].priority);
    }
}

void insertNode_maxHeap(struct maxHeap *hp, struct maxHeap_node mhn){
    maxHeap_node nd ;
    nd = mhn;

    int i = (hp->size)++ ;
    while(i && nd.priority > hp->elem[PARENT(i)].priority) {
        hp->elem[i] = hp->elem[PARENT(i)] ;
        i = PARENT(i) ;
    }
    hp->elem[i] = nd ;
    print_wait_queue(hp);
}
struct maxHeap_node deleteNode_maxHeap(struct maxHeap *hp);
int getMaxNode(struct maxHeap *hp, int i);
int is_maxHeap_empty(struct maxHeap *hp);
void buildMaxHeap(struct maxHeap *hp, int *arr, int size);

void buildMaxHeap(maxHeap *hp, int *arr, int size) {
    int i ;

    // Insertion into the heap without violating the shape property
    for(i = 0; i < size; i++) {
        if(hp->size) {
            hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(node)) ;
        } else {
            hp->elem = malloc(sizeof(node)) ;
        }
        node nd ;
        nd.priority = arr[i] ;
        hp->elem[(hp->size)++] = nd ;
    }

    // Making sure that heap property is also satisfied
    for(i = (hp->size - 1) / 2; i >= 0; i--) {
        heapify(hp, i) ;
    }
}
void deleteNode(struct maxHeap *hp) {
    if(hp->size) {
        printf("Deleting node %d\n\n", hp->elem[0].priority) ;
        hp->elem[0] = hp->elem[--(hp->size)] ;
        hp->elem = realloc(hp->elem, hp->size * sizeof(node)) ;
        heapify(hp, 0) ;
    } else {
        printf("\nMax Heap is empty!\n") ;
        free(hp->elem) ;
    }
}
int getMinNode(maxHeap *hp, int i) {
    if(LCHILD(i) >= hp->size) {
        return hp->elem[i].priority ;
    }

    int l = getMinNode(hp, LCHILD(i)) ;
    int r = getMinNode(hp, RCHILD(i)) ;

    if(l <= r) {
        return l ;
    } else {
        return r ;
    }
}