#define _CRT_SECURE_NO_WARNINGS
#include "../libwinbgi/src/graphics.h"
#include <malloc.h>
#include <stdio.h>
#include "math.h"
#include <windows.h> 

#define X_ZERO 200
#define Y_ZERO 200
#define RAD 100


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
        stack = (Node*)calloc(1, sizeof(Node));
        stack->connect_to = dat;
        stack->next = NULL;
    }
    else
    {
        Node* new_el = (Node*)calloc(1, sizeof(Node));
        new_el->connect_to = dat;
        new_el->next = NULL;
        Node* last = get_last();

        last->next = new_el;
    }
}
int peek(Node* st)
{
    return st->connect_to;
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




Node** graph;   // Массив списков смежности


void add(int idx, int data)
{
    Node* last = graph[idx];

    while (last->next)
    {
        last = last->next;
    }

    Node* new_last = (Node*)calloc(1, sizeof(Node));
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

int* found_path;
int stack_size = 0;

int find_stack_size()
{
    stack_size = 0;
    Node* ptr = stack;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        stack_size++;
    }

    return stack_size;
}

void copy_stack_to_buf()
{
    found_path = (int*)calloc(stack_size, sizeof(int));

    for (int i = 0; i < stack_size; i++)
    {
        found_path[i] = pop();
    }

}


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
            find_stack_size();
            copy_stack_to_buf();
            /*v = pop();
            fprintf(fo, "%d\ ", v);
            printf("%d\n", v);*/
            return;
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


float* map;

int init_map(int _v_num)
{
    map = (float*)calloc(_v_num * 2, sizeof(float)); //  0 - x, 1 - y
    if (map == NULL)
        return -1;
    
    float deg_delta = 360.0 / (float)_v_num;
    float alpha = 0;

    for (int i = 0; i < _v_num; i++)
    {
        
        printf("alpha = %f\n", alpha);

        map[i * 2 + 0] = X_ZERO + RAD * cos((double)alpha * 0.017); // x
        map[i * 2 + 1] = Y_ZERO + RAD * sin((double)alpha * 0.017); // y

        alpha += deg_delta;
        
    }

    return 0;

}

void print_map_coordinates(int _v_num)
{
    for (int i = 0; i < _v_num; i++)
    {
        printf("%f %f\n", map[i * 2 + 0], map[i * 2 + 1]);
        
    }
}


int* arr;

void drav_points(int _v_num, int _st)
{

    for (int i = 0; i < _v_num; i++)
    {
        char message[4];
        if (i == _st) setcolor(15);
        else
        setcolor(7);
        
        sprintf_s(message, "%d", i);
        outtextxy(map[i * 2 + 0], map[i * 2 + 1], message);
    }
}

void draw_connect(int _v_num)
{
    
    setcolor(12);
    setlinestyle(SOLID_LINE, 0, 1);
    for (int i = 0; i < _v_num; i++)
    for (int j = 0; j < _v_num; j++)
    { 
        int val = arr[i + j * _v_num];
        if (val == 1)
        {
            line(map[i * 2 + 0], map[i * 2 + 1], 
                map[j * 2 + 0], map[j * 2 + 1]);          
        }
    }
}

void draw_path(int _v_num)
{
    setcolor(13);
    setlinestyle(SOLID_LINE, 0, 3);
    int cur = 0;
    int prev = found_path[0];
    for (int i = 1; i < stack_size; i++)
    {
        cur = found_path[i];
        
        line(map[cur * 2 + 0], map[cur * 2 + 1],
            map[prev * 2 + 0], map[prev * 2 + 1]);
        prev = cur;
        Sleep(1000);
    }

}



void draw_m_graph(int _v_num, int _st)
{
    initwindow(1000, 700);

    while (true)
    {
        clearviewport();
        drav_points(_v_num, _st);
        
        draw_connect(_v_num);
        Sleep(1500);
        draw_path(_v_num);

    }

    closegraph();
}


void to_graph(int _v_num, int _st)
{
    init_map(_v_num);
    draw_m_graph(_v_num, _st);
}

void message_not_euler()
{
    
    initwindow(700, 300);
    char message[] = "no euler cycle\0";
    setcolor(15);


    sprintf_s(message, "%s", message);
    settextstyle(0, 0,4);
    outtextxy(20, 30, message);
    getch();
    closegraph();
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


    arr = (int*)calloc(vertex_num*vertex_num, sizeof(int));
 
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
                    graph[i] = (Node*)calloc(1, sizeof(Node));
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
        message_not_euler();
    }


    to_graph(vertex_num, start);

    return(0);
}