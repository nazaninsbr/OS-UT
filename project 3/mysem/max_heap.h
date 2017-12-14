#ifndef MIN_HEAP

#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

struct maxHeap_node{
	struct task_struct *task; 
	int priority; 
	int waitTime;
};
struct maxHeap{
	int size;
	struct maxHeap_node *head;
};


struct maxHeap maxHeap_init(int size);
void swap_maxHeap_nodes(struct maxHeap_node *n1, struct maxHeap_node *n2);
void heapify_maxHeap(struct maxHeap *hp, int i);
void buildMaxHeap(struct maxHeap *hp, int *arr, int size);
void insertNode_maxHeap(struct maxHeap *hp, struct maxHeap_node *mhn);
struct maxHeap_node deleteNode_maxHeap(struct maxHeap *hp);
int getMaxNode(struct maxHeap *hp, int i);
int is_maxHeap_empty(struct maxHeap *hp);


#endif