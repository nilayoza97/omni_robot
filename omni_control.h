#define PI 3.14159
#define BASE_WIDTH 30.0
#define WHEEL_DIAMETER 5
#define PULSES_PER_ROTATION 2000
#define TARGET_DISTANCE 30
#define SENSOR1 0
#define SENSOR2 1

struct node
{
    float value;
    int key;
    struct node *next;
};
struct node *(head[2]);

void add_new_end(float value, int i, int key)
{
    struct node *ptr, *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    ptr = head[i];
    temp->value = value;
    temp->key = key;
    temp->next = NULL;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = temp;
}

void add_new_start(float value, int i, int key)
{  
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->key = head[i]->key;
    temp->value = head[i]->value;
    head[i]->value = value;
    head[i]->key = key;
    temp->next = head[i]->next;
    head[i]->next = temp;
}

void printList(int i)
{
    struct node *temp;
    temp = head[i];

    while(temp->next != NULL)
    {
        temp = temp->next;
        printf("sensor item %d : %f \n",temp->key, temp->value);
    }
}

void initialize(int index)
{
    head[index] = (struct node *)malloc(sizeof(struct node));
    head[index]->next = NULL;
}

void initializeAll()
{
    initialize(SENSOR1);
    initialize(SENSOR2);
}
