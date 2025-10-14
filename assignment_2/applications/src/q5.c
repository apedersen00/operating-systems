#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Struct to represent the linked list
typedef struct node {
    int node_id;        // Node ID
    struct node *next;  // Pointer to the next node
} Node;

// Top of stack
Node *top;

int global_node_id = 0;

// Mutex for thread synchronization
pthread_mutex_t stack_mutex = PTHREAD_MUTEX_INITIALIZER;

// Pushes an item on stack using the mutex approach
void push_mutex() {
    Node *new_node;
    new_node = malloc(sizeof(Node));

    // Lock the stack
    pthread_mutex_lock(&stack_mutex);

    // Get data (ID) for node by incrementing global ID counter
    new_node->node_id = global_node_id++;

    // Update top of the stack
    new_node->next = top;
    top = new_node;

    // Unlock the stack
    pthread_mutex_unlock(&stack_mutex);
}

// Pops an item from the stack using the mutex approach
int pop_mutex() {
    Node *old_node;
    int node_id = -1;

    // Lock the stack
    pthread_mutex_lock(&stack_mutex);

    // Check if stack is not empty
    if (top != NULL) {
        old_node = top;
        node_id = old_node->node_id;
        top = top->next;
        free(old_node);
    }

    // Unlock the stack
    pthread_mutex_unlock(&stack_mutex);

    // Return the node data (ID)
    return node_id;
}

// Pushes an item on the stack using the CAS approach
void push_cas() {
    Node *new_node;
    Node *old_top;

    new_node = malloc(sizeof(Node));

    do {
        old_top = top;
        new_node->node_id = __sync_fetch_and_add(&global_node_id, 1);
        new_node->next = old_top;
    } while (!__sync_bool_compare_and_swap(&top, old_top, new_node));
}

// Pops an item from the stack using the CAS approach
int pop_cas() {
    Node *old_top;
    Node *new_top;
    int node_id = -1;

    do {
        old_top = top;
        if (old_top == NULL)
        {
            return -1;
        }
        new_top = old_top->next;
        node_id = old_top->node_id;
    } while (!__sync_bool_compare_and_swap(&top, old_top, new_top));

    free(old_top);
    return node_id;
}

// Thread argument structure
typedef struct {
    int thread_id;
    int option;     // 0: Mutex, 1: CAS
} thread_arg_t;

// Thread function
void *thread_func(void *arg) {
    thread_arg_t *data = (thread_arg_t *)arg;
    int tid = data->thread_id;
    int opt = data->option;

    if (opt == 0)
    {
        push_mutex();
        push_mutex();
        pop_mutex();
        pop_mutex();
        push_mutex();
    }
    else
    {
        push_cas();
        push_cas();
        pop_cas();
        pop_cas();
        push_cas();
    }

    // printf("Thread %d: exit\n", tid);
    pthread_exit(0);
}

void print_remaining_nodes() {
    Node *current = top;
    int count = 0;
    printf("Remaining nodes: ");
    while (current != NULL) {
        printf("%d ", current->node_id);
        current = current->next;
        count++;
    }
    printf("(Total: %d)\n", count);
}

void free_remaining_nodes() {
    Node *current = top;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    top = NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    int num_threads = atoi(argv[1]);

    /* === Mutex === */
    printf("\n=== Stack Using Mutex Approach ===\n");
    top = NULL;
    global_node_id = 0;

    pthread_t *workers = malloc(num_threads * sizeof(pthread_t));
    thread_arg_t *thread_data = malloc(num_threads * sizeof(thread_arg_t));

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].option = 0;
        pthread_create(&workers[i], NULL, thread_func, &thread_data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }

    // Print remaining nodes in Stack
    printf("Mutex: ");
    print_remaining_nodes();

    // Cleanup
    free_remaining_nodes();

    /* === CAS === */
    printf("\n=== Stack Using CAS Approach ===\n");
    top = NULL;
    global_node_id = 0;

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].option = 1;
        pthread_create(&workers[i], NULL, thread_func, &thread_data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }

    // Print remaining nodes in Stack
    printf("CAS: ");
    print_remaining_nodes();

    // Cleanup
    free_remaining_nodes();
    free(workers);
    free(thread_data);

    return 0;
}