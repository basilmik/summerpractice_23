#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>

typedef struct Node_t
{
    int connect_to;
    struct Node_t* next;
} Node;


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

int peek(Node* st)
{
    return st->connect_to;
}




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
        free( tmp);
    }
    else
    {
        Node* tmp = list;
        while (tmp)
        {
            if (tmp->next) // ���� ��������� �������
                if (tmp->next->connect_to == key) // � ��� �������
                {  
                    Node* tmp2 = tmp->next;
                    tmp->next = tmp->next->next;
                    free( tmp2);
                }
            tmp = tmp->next;
        }
    }
}

int is_euler(Node** gr, int num)
{
    int count;

    for (int i = 0; i < num; i++)
    {
        count = 0;
        Node* tmp = gr[i];

        while (tmp) // ������� �������
        {
            count++;
            tmp = tmp->next;
        }
        if (count % 2 == 1) // ������� ��������
        {
            printf("count is odd\n");
            return 0;
        }
    }

    return 1;
}


Node** graph;   // ������ ������� ���������
int vertex = 1; // ������ �������

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

void find_euler_cycle(Node** gr)
{
    Node* stack = NULL; // ���� ���  ���������� ������
    int v = vertex; // 1� ������� (������������)
    int u;
    print_stack(stack);
    push(stack, v); //��������� �� � ����
    print_stack(stack);

    while (stack)
    {
        v = peek(stack); // ������� �������

        if (!gr[v]) // ���� ��� ����������� �����
        {
            print_stack(stack);
            v = pop(stack);
            fprintf(fo, "!%d\ ", v);
            printf("pop: !%d\n", v);
        }
        else
        {
            print_stack(stack);
            u = gr[v]->connect_to;
            push(stack, u);  //�������� � ��������� �������
            print_stack(stack);

            del(gr[v], u);
            del(gr[u], v); //������� ���������� �����

        }
    }
}

int main()
{
    int vertex_num;
    int val;// ������� ��������

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

    fscanf(fi, "%d", &vertex_num);

    graph = (Node**)malloc(vertex_num * sizeof(Node*));
    if (graph == NULL)
    {
        return -1;

    }
    
    for (int i = 0; i < vertex_num; i++)
    {
        graph[i] = NULL;
       
    }
    //print_graph(graph, vertex_num);

    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
        {
            fscanf(fi, "%d", &val);

            if (val)
            {
                
                if (graph[i] == NULL)
                {
                    graph[i] = calloc(1, sizeof(Node));
                    graph[i]->connect_to = j;
                    graph[i]->next = 0;
                }
                else
                    add(graph[i], j);
                if (i == j)
                {
                    add(graph[i], j);
                }
            }


        }
    }

    print_graph(graph, vertex_num);

    if (is_euler(graph, vertex_num))
        find_euler_cycle(graph);
    else
        fprintf(fo, "���� �� �������� ���������.");

    return(0);
}