#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>

typedef struct Node_t
{
    int connect_to;
    struct Node_t* next;
} Node;



void print_node(Node* _node)
{
    printf("%d -> ", _node->connect_to);
}

void print_graph_one(Node* _node)
{
    while (_node != NULL)
    {
        print_node(_node);
        _node = _node->next;
    }
    printf("\n");
}

void print_graph(Node** _graph, int _vn)
{
    for (int i = 0; i < _vn; i++)
    {
        printf("%d ", i);
        print_graph_one(_graph[i]);
        printf("\n");
    }
}

void print_stack(Node* _stack_ptr)
{
    printf("stack: ");
    while (_stack_ptr != NULL)
    {
        print_node(_stack_ptr);
        _stack_ptr = _stack_ptr->next;

    }
    printf("\n");
}



Node* stack;

Node* get_last()
{
    Node* cur = stack;
    while (cur->next != NULL)
    {
        cur = cur->next;    
    } 
    return cur;
}

Node* get_prev_to_last()
{
    Node* cur = stack;
    if (cur->next == NULL)
        return NULL;
    
    while (cur->next->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

void push(int dat)
{
    if (stack == NULL)
    {
        stack = calloc(1, sizeof(Node));
        stack->connect_to = dat;
        stack->next = NULL;
    }
    else
    {
        Node* new_el = calloc(1, sizeof(Node));
        new_el->connect_to = dat;
        new_el->next = NULL;
        Node* last = get_last();

        last->next = new_el;
    }
}

int pop()
{
    Node* last = get_last();
    Node* _new_last = get_prev_to_last();
    
    int value = peek(last);

    free(last);
    if (_new_last != NULL)
        _new_last->next = NULL;
    else
        stack = NULL;

    return value;
}

int peek(Node* st)
{
    return st->connect_to;
}



Node** graph;   // Массив списков смежности


void add(int idx, int data)
{
    Node* last = graph[idx];

    while (last->next)
    {
        last = last->next;
    }

    Node* new_last = calloc(1, sizeof(Node));
    new_last->connect_to = data;
    new_last->next = NULL;
    last->next = new_last;
}

void del(int idx, int key)
{
    Node* cur = graph[idx];
    if (cur->connect_to == key)
    {
        graph[idx] = graph[idx]->next;
    }
    else
    {
        Node* tmp = cur;
        while (tmp)
        {
            if (tmp->next) // есть следующая вершина
                if (tmp->next->connect_to == key) // и она искомая
                {
                    Node* tmp2 = tmp->next;
                    tmp->next = tmp->next->next;
                    free(tmp2);
                }
            tmp = tmp->next;
        }
    }
}


FILE* fi;
FILE* fo;

int is_euler(int _v_num)
{
    int count;

    for (int i = 0; i < _v_num; i++)
    {
        count = 0;
        Node* tmp = graph[i];

        while (tmp) // считаем степень
        {
            count++;
            tmp = tmp->next;
        }
        if (count % 2 == 1) // степень нечетная
        {
            printf("count is odd\n");
            return 0;
        }
    }

    return 1;
}


void find_euler_cycle(int _from_vertex)
{
    stack = NULL;

    int v = _from_vertex; // start

    int u = 0;

    push(v); 
 
    while (stack)
    {
        v = peek(get_last()); 

        if (!graph[v]) // если нет инцидентных ребер
        {    
            v = pop();
            fprintf(fo, "%d\ ", v);
            printf("%d\n", v);
        }
        else
        {
            u = graph[v]->connect_to;

            push(u);  //проходим в следующую вершину
            
            //удаляем пройденное ребро
            del(v, u);
            del(u, v); 
        }
    }
}

int main()
{
    int vertex_num = 0;
    int edges_num = 0;
    int start = 0;
    int val;

    fi = fopen("config.txt", "r");
    if (fi == NULL)
    {
        printf("config file not found!\vertex_num");
        return (-1);
    }

    fo = fopen("result.txt", "w");
    if (fo == NULL)
    {
        printf("result file not found!\vertex_num");
        return (-1);
    }


    fscanf(fi, "%d %d %d", &vertex_num, &edges_num, &start);
    printf("%d %d %d\n", vertex_num, edges_num, start);


    int* arr = calloc(vertex_num*vertex_num, sizeof(int));
 
    for (int i = 0; i < edges_num; i++)
    {
        int fst = 0, snd = 0;
        fscanf(fi, "%d %d", &fst, &snd);
        printf("%d %d\n", fst, snd);
        
        arr[fst + snd * vertex_num] = 1;
        arr[snd + fst * vertex_num] = 1;
    }

    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
        {
            printf("%d ", arr[i + j * vertex_num]);
        }
        printf("\n");
    }


    graph = (Node**)malloc(vertex_num * sizeof(Node*));
    if (graph == NULL)
    {
        return -1;
    }
    
    for (int i = 0; i < vertex_num; i++)
    {
        graph[i] = NULL;      
    }
    

    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
        {
            val = arr[i + j * vertex_num];
            if (val)
            {
                
                if (graph[i] == NULL)
                {
                    graph[i] = calloc(1, sizeof(Node));
                    graph[i]->connect_to = j;
                    graph[i]->next = NULL;
                }
                else
                    add(i, j);

                if (i == j)
                {
                    add(i, j);
                }
            }


        }
    }

 
    if (is_euler(vertex_num))
    {
        find_euler_cycle(start);
    }
    else
    {
        fprintf(fo, "Граф не является эйлеровым.");
    }

    return(0);
}