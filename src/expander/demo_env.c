#include <stdio.h>
#include <stdlib.h>

typedef struct s_node {
  char data;
  struct s_node *next;
} t_node;

typedef struct s_queue {
  t_node *front;
  t_node *rear;
} t_queue;

// time to implement a queue for our codebase

// Create a node

t_node *create_node(char data) {
  t_node *n = malloc(sizeof(t_node));
  if (!n)
    return (NULL);
  n->data = data;
  n->next = NULL;
  return (n);
}

t_queue *init_q(void) {
  t_queue *q = malloc(sizeof(t_queue));
  if (!q)
    return (NULL);
  q->front = NULL;
  q->rear = NULL;
  return (q);
}

int isEmpty(t_queue *q) { return (!q->front); }

void enqueue(t_queue *q, char data) {
  t_node *node = create_node(data);
  if (!node)
    return;
  if (q->rear == NULL) {
    q->front = node;
    q->rear = node;
    return;
  }
  q->rear->next = node;
  q->rear = node;
}

char dequeue(t_queue *q) {
  t_node *temp;
  if (isEmpty(q))
    return (-1);
  temp = q->front;
  q->front = temp->next;
  if (q->front == NULL)
    q->rear = NULL;
  char value = temp->data;
  free(temp);
  temp = NULL;
  return (value);
}

void display(t_queue *q) {
  if (isEmpty(q)) {
    printf("The queue is empty\n");
    return;
  }
  printf("Queue : ");
  t_node *temp = q->front;
  while (temp != NULL) {
    printf("%c ", temp->data);
    temp = temp->next;
  }
  printf("\n");
}

char *queue_to_str(t_queue *q) {
  int len = 0;
  if (isEmpty(q))
    return (NULL);
  t_node *tmp = q->front;
  while (tmp != NULL) {
    len++;
    tmp = tmp->next;
  }
  tmp = q->front;
  char *str = malloc(sizeof(char) * (len + 1));
  if (!str)
    return (NULL);
  char *s = str;
  s[len] = '\0';
  while (len--) {
    *s = dequeue(q);
    s++;
  }
  return (str);
}

int main(void) {
  t_queue *q = init_q();
  enqueue(q, 'a');
  enqueue(q, 'b');
  enqueue(q, 'c');
  enqueue(q, 'd');
  enqueue(q, 'a');
  enqueue(q, 'b');
  enqueue(q, 'c');
  enqueue(q, 'd');
  enqueue(q, 'a');
  enqueue(q, 'b');
  enqueue(q, 'c');
  enqueue(q, 'd');
  enqueue(q, 'a');
  enqueue(q, 'b');
  enqueue(q, 'c');
  enqueue(q, 'd');
  display(q);
  char *str = queue_to_str(q);
  printf("The str is {%s}\n", str);
  return (EXIT_SUCCESS);
}
