#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>

typedef struct Node_t
{
    int connect_to;
    struct Node_t* next;
} Node;


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


#if 1

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

#else

void push(Node* st, int dat)
{
    Node* el = calloc(1, sizeof(Node));
    el->connect_to = dat;
    el->next = st;
    st = el;
}

int pop(Node* st)
{
    int value = st->connect_to;
    Node* temp = st;
    st = st->next;
    free(temp);

    return value;
}

#endif


int peek(Node* st)
{
    return st->connect_to;
}



Node** graph;   // ћассив списков смежности
int vertex = 1; // ѕерва€ вершина

#if 0
void add(Node* list, int data)
{
   //if (&list == NULL)
   // {
   //     list = calloc(1, sizeof(Node));
   //     list.connect_to = data;
   //     list.next = 0;
   //     return;
   // }

    Node* last = list;

    while (last->next)
    {
        last = last->next;
    }

    Node* new_last = calloc(1, sizeof(Node));
    new_last->connect_to = data;
    new_last->next = NULL;
    last->next = new_last;
}

void del(Node* list, int key)
{
    if (list->connect_to == key)
    {
        Node* tmp = list;
        list = list->next;
        //free(tmp);
    }
    else
    {
        Node* tmp = list;
        while (tmp)
        {
            if (tmp->next) // есть следующа€ вершина
                if (tmp->next->connect_to == key) // и она искома€
                {  
                    Node* tmp2 = tmp->next;
                    tmp->next = tmp->next->next;
                    free( tmp2);
                }
            *(&tmp) = tmp->next;
        }
    }
}
#else

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
        //Node* tmp = graph;
        graph[idx] = graph[idx]->next;
        //free(tmp);
    }
    else
    {
        Node* tmp = cur;
        while (tmp)
        {
            if (tmp->next) // есть следующа€ вершина
                if (tmp->next->connect_to == key) // и она искома€
                {
                    Node* tmp2 = tmp->next;
                    tmp->next = tmp->next->next;
                    free(tmp2);
                }
            tmp = tmp->next;
        }
    }
}

#endif



int is_euler(Node** gr, int num)
{
    int count;

    for (int i = 0; i < num; i++)
    {
        count = 0;
        Node* tmp = gr[i];

        while (tmp) // считаем степень
        {
            count++;
            tmp = tmp->next;
        }
        if (count % 2 == 1) // степень нечетна€
        {
            printf("count is odd\n");
            return 0;
        }
    }

    return 1;
}



FILE* fi;
FILE* fo;

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

void find_euler_cycle(int _from_vertex)
{

    stack = NULL;

    int v = _from_vertex; // 1€ вершина 

    int u;
    //print_stack(stack);
    push(v); //сохран€ем ее в стек
    //print_stack(stack);

    while (stack)
    {
        v = peek(get_last()); // текуща€ вершина

        if (!graph[v]) // если нет инцидентных ребер
        {
            //print_stack(stack);
            v = pop();
            fprintf(fo, "%d\ ", v);
            printf("%d\n", v);
        }
        else
        {
            //print_stack(stack);

            u = graph[v]->connect_to;

            push(u);  //проходим в следующую вершину

            //print_stack(stack);

            //print_graph(graph, 5);

            del(v, u);

            //print_graph(graph, 5);

            del(u, v); //удал€ем пройденное ребро

            //printf("graph:\n");
            //print_graph(graph, 5);
        }
    }
}

int main()
{
    int vertex_num = 0;
    int edges_num = 0;
    int start = 0;
    int val;// “екущее значение

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
    //for (int i = 0; i < vertex_num; i++)
    //{
    //    for (int j = 0; j < vertex_num; j++)
    //    {
    //        arr[i + j * vertex_num] = calloc(1, sizeof(int));
    //        arr[i + j * vertex_num] = 0;
    //    }
    //}

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
            printf("%d ", arr[i + j * vertex_num]);// = calloc(1, sizeof(int));
            
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

    //print_graph(graph, vertex_num);

 
    if (is_euler(graph, vertex_num))
        find_euler_cycle(start);
    else
        fprintf(fo, "√раф не €вл€етс€ эйлеровым.");

    return(0);
}