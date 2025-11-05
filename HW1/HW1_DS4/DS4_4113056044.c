#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SPACE 0
#define WALL 1
#define PERSON 2
#define CORE 3
#define EXIT 4



typedef struct{
    int x;
    int y;
} Position;

typedef struct{
    Position person;
    Position core;
    int steps;
} State;

typedef struct{
    int vert;
    int horiz;
} Move;

//定義上右下左
Move move[4] = {{.vert = -1, .horiz = 0}, {.vert = 0, .horiz = 1}, {.vert = 1, .horiz = 0}, {.vert = 0, .horiz = -1}};

typedef struct{
    Position *p;
    int front;
    int rear;
    int size;
    bool empty;
} Queue_Person;

typedef struct{
    State *s;
    int front;
    int rear;
    int size;
    bool empty;
} Queue;

bool can_person_reach(int *map, int m, int n, Position person, Position target, Position core, bool *person_visited);
int solve(int *map, int m, int n, Position person, Position core, Position exit, bool *visited, bool*visited_core);
void visit(bool *mark, Position p, int n);

Queue_Person* create_person(int size){
    Queue_Person *queue = (Queue_Person*) malloc(sizeof(Queue_Person));
    Position *position = (Position *) malloc(sizeof(Position)*size);
    queue->p = position;
    queue->front = -1;
    queue->rear = -1;
    queue->size = size;
    queue->empty = true;
    return queue;
}

bool isFull_person(Queue_Person *queue){
    if(queue->rear == queue->size - 1){
        return true;
    } else {
        return false;
    }
}

bool isEmpty_person(Queue_Person *queue){
    return queue->empty;
}

void addQ_person(Queue_Person *queue, Position p){
    if(!isFull_person(queue)){
        if(queue->front == -1 && queue->rear == -1){
            queue->front++;
            queue->rear++;
            *(queue->p + queue->front) = p;
            queue->empty = false;
        } else if(isEmpty_person(queue)){
            *(queue->p + queue->front) = p;
            queue->empty = false;
        } else {
            queue->rear++;
            *(queue->p + queue->rear) = p;
            queue->empty = false;
        }
    }
}

Position deleteQ_person(Queue_Person *queue){
    if(!isEmpty_person(queue)){
        if(queue->front == queue->rear){
            Position *p = queue->p;
            Position position = *(p + queue->front);
            queue->empty = true;
            return position;
        } else {
            int index = queue->front++;
            Position *p = queue->p;
            Position position = *(p + index);
            return position;
        }
    }
}

Queue* create(int size){
    Queue *queue = (Queue*) malloc(sizeof(Queue));
    State *state = (State *) malloc(sizeof(State)*size);
    queue->s = state;
    queue->front = -1;
    queue->rear = -1;
    queue->empty = true;
    queue->size = size;
    return queue;
}

bool isFull(Queue *queue){
    if(queue->rear == queue->size - 1){
        return true;
    } else {
        return false;
    }
}

bool isEmpty(Queue *queue){
    return queue->empty;
}

void addQ(Queue *queue, State s){
    if(!isFull(queue)){
        if(queue->front == -1 && queue->rear == -1){
            queue->front++;
            queue->rear++;
            *(queue->s + queue->front) = s;
            queue->empty = false;
        } else if(isEmpty(queue)){
            *(queue->s + queue->front) = s;
            queue->empty = false;
        } else {
            queue->rear++;
            *(queue->s + queue->rear) = s;
        }
    }
}

State deleteQ(Queue *queue){
    if(!isEmpty(queue)){
        if(queue->front == queue->rear){
            State *s = queue->s;
            State state = *(s + queue->front);
            queue->empty = true;
            return state;
        } else {
            int index = queue->front++;
            State *s = queue->s;
            State state = *(s + index);
            return state;
        }
    }
}

int main()
{
    FILE *fp_r;
    FILE *fp_w;
    fp_r = fopen("testcase4.txt", "r");
    fp_w = fopen("output4.txt", "w");

    int T = 0;
    fscanf(fp_r, "%d", &T);

    for(int repeat = 0;repeat < T;repeat++){
        int result = 0;
        int m = 0, n = 0;
        fscanf(fp_r, "%d%d", &m, &n);
        int *map = (int *) malloc(sizeof(int)*m*n);
        bool *visited = (bool *) calloc(m*n, sizeof(bool));
        bool *visited_core = (bool *) calloc(m*n, sizeof(bool));
        
        //read map
        for(int i = 0;i < m;i++){
            for(int j = 0;j < n;j++){
                fscanf(fp_r, "%d", (map + i*n +j));
            }
        }

        Position person = {.x = -1, .y = -1};
        Position core = {.x = -1, .y = -1};
        Position exit = {.x = -1, .y = -1};
        //store person, core, exit
        for(int i = 0;i < m;i++){
            for(int j = 0;j < n;j++){
                if(*(map + i*n +j) == PERSON){
                    person.x = i;
                    person.y = j;
                } else if(*(map + i*n +j) == CORE){
                    core.x = i;
                    core.y = j;
                } else if(*(map + i*n +j) == EXIT){
                    exit.x = i;
                    exit.y = j;
                }
            }
        }


        if(person.x == -1||core.x == -1||exit.x == -1){
            result = -1;
        } else {
            result = solve(map, m, n, person, core, exit, visited, visited_core);
        }
        fprintf(fp_w, "%d\n", result);
        free(visited_core);
        free(visited);
        free(map);
    }
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}

bool can_person_reach(int *map, int m, int n, Position person, Position target, Position core, bool *person_visited){
    //initailize
    memset(person_visited, 0, sizeof(bool)*m*n);
    
    if (person.x == target.x && person.y == target.y) return true; //reach destination

    Queue_Person *queue = create_person(m * n);
    addQ_person(queue, person);
    visit(person_visited, person, n);

    while (!isEmpty_person(queue))
    {
        Position current = deleteQ_person(queue);
        for(int i = 0;i < 4;i++){
            Position next = {.x = current.x + move[i].vert, .y = current.y + move[i].horiz};

            if (next.x < 0 || next.x >= m  || next.y < 0 || next.y >= n) continue;
            if (*(map + next.x*n + next.y) == WALL) continue;
            if (next.x == core.x && next.y == core.y) continue;

            if (next.x == target.x && next.y == target.y) {
                free(queue);
                return true;
            }

            if (!(*(person_visited + next.x*n + next.y))) {
                visit(person_visited, next, n);
                addQ_person(queue, next);
            }
        }
    }
    free(queue);
    return false;
}

int solve(int *map, int m, int n, Position person, Position core, Position exit, bool *visited, bool*visited_core){
    bool *visited_person = (bool *) calloc(m*n, sizeof(bool));
    int max_queue_size = m * n * m * n;
    Queue *queue = create(max_queue_size);
    State init = {.person = person, .core = core, .steps = 0};
    addQ(queue, init);
    visit(visited, person, n);
    visit(visited_core, core, n);

    while (!isEmpty(queue))
    {
        State current = deleteQ(queue);
        //reach exit
        if (current.core.x == exit.x && current.core.y == exit.y) {
            free(queue);
            return current.steps;
        }

        for(int i = 0;i < 4;i++){
            Position next_core = {.x = current.core.x + move[i].vert, .y = current.core.y + move[i].horiz}; 
            Position next_person = {.x = current.core.x, .y = current.core.y};
            Position required_person = {.x = current.core.x - move[i].vert, .y = current.core.y - move[i].horiz};

            if (next_core.x < 0 || next_core.x >= m || next_core.y < 0 || next_core.y >= n) continue;
            if (*(map + next_core.x*n + next_core.y) == WALL) continue;
            if (required_person.x < 0 || required_person.x >= m || required_person.y < 0 || required_person.y >= n) continue;
            if (*(map + required_person.x*n + required_person.y) == WALL) continue;
        
            if (can_person_reach(map, m, n, current.person, required_person, current.core, visited_person)){
                //test whether been there or not
                if (!(*(visited + next_person.x*n + next_person.y)) || !(*(visited_core + next_core.x*n + next_core.y))) {
                    visit(visited, next_person, n);
                    visit(visited_core, next_core, n);
                    State next_state = {.person = next_person, .core = next_core, .steps = current.steps + 1};
                    addQ(queue, next_state);
                }
            }
        }
    }

    free(visited_person);
    free(queue);
    return -1;
}

void visit(bool *mark, Position p, int n){
    *(mark + p.x*n + p.y) = true;
}