#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>


//a linked list node to store a queue, a stack and a adjacent list of a vertex;
struct Node{
    int data;
    struct Node* nextNode;
};

// A structure to represent a stack
struct Queue{
    struct Node* front;
    struct Node* rear;
};

// A structure to represent a stack
struct Stack{
    struct Node* front;
};

//a structure to represent a Graph
struct Graph{
    char name[40];
    int numVertices;
    int numEdge;
    int* hashTable;
    struct Node** adjList;
    int* belongVertexCover; // 0 or 1 corresponds to Vertex i is in the Vertex cover or not
};


// A utility function to create a new linked list node.
struct Node* createNode(int value){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->nextNode = NULL;
    return newNode;
}

// A utility function to create an empty queue
struct Queue* createQueue() {
    struct Queue* newQueue = (struct Queue*)malloc(sizeof(struct Queue));
    newQueue->front = NULL;
    newQueue->rear = NULL;
    return newQueue;
}


// The function to add a value to queue 
void enQueue(struct Queue* q, int value){
    //create a LL node
    struct Node* temp = createNode(value);
    
    //if queue empty
    if(q->rear == NULL) {
        q->rear = temp;
        q->front = temp;
        return;
    }

    //add a node at the end of Queue
    q->rear->nextNode = temp;
    q->rear = temp;
}

//the function to remove the front node
void deQueue(struct Queue* q){
    //if queue is empty, return NULL
    if(q->front == NULL) {
        return;
    }

    // Store previous front and move front one node ahead
    struct Node* temp = q->front;
    q->front = q->front->nextNode;

    // if front becomes NULL, then change rear also as NULL
    if(q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
}

// Check the queue is empty or not
int isQueueEmpty(struct Queue* q) {
    if(q->front == NULL) {
        return 1;
    }
    return 0;
}

// A utility function to create an empty stack
struct Stack* createStack() {
    struct Stack* newStack = (struct Stack*)malloc(sizeof(struct Stack));
    newStack->front = NULL;
    return newStack;
}


//A utility function to add an element to the stack
void push(struct Stack* s, int value) {
    struct Node* temp = createNode(value);

    //add temp at the front of stack
    temp->nextNode = s->front;
    s->front = temp;
}


// a Utility function to pop a front element from the stack
void pop(struct Stack* s) {

    // check stack is empty or not
    if(s->front == NULL) {
        return;
    }

    // Store previous front and move front one node ahead
    struct Node* temp = s->front;
    s->front = s->front->nextNode;
    free(temp);
}

// Check the stack is empty or not
int isStackEmpty(struct Stack* s) {
    if(s->front == NULL) {
        return 1;
    }
    return 0;
}

//a utility function to create a Graph
struct Graph* createGraph(int numVertices, int numEdge, char* name){
    struct Graph* newGraph = (struct Graph*)malloc(sizeof(struct Graph));
    int j = 0;

    //add the graph name
    while( name[j] != '\0'){
        newGraph->name[j] = name[j];
        j++;
    }

    newGraph->numVertices = numVertices;
    newGraph->numEdge  = numEdge;
    newGraph->hashTable = (int*)malloc(numVertices*sizeof(int));
    newGraph->adjList = (struct Node**)malloc((numVertices)*sizeof(struct Node*));
    newGraph->belongVertexCover =(int*)malloc(numVertices*sizeof(int));

    //initialize the initial values for adjlist, hashTable, belongVertexCover
    for(int i = 0; i < numVertices; i++) {
        newGraph->adjList[i] = NULL;
        newGraph->hashTable[i] = -1;
        newGraph->belongVertexCover[i] = 0;
    }
    return newGraph;
}


//function to calculate hash
int getHash(int key, int numVertices) {
    return (key % numVertices);
}

//function to get index of key in hash table
int getIndex(int key, struct Graph* g) {
    int index = getHash(key, g->numVertices);
    
    //if there is no collision
    if(g->hashTable[index] == key) {
        return index;
    }
    
    
    if(g->hashTable[index] != -1) {
        // deal with collisions
        int i = 1;
        while(1) {
            int newindex = (index+i) % g->numVertices;
            if(g->hashTable[newindex] == -1 || g->hashTable[newindex] == key) {
                g->hashTable[newindex] = key;
                index = newindex;
                break;
            }
            i++;
        }
    }else{ 
        //insert key into hash table
        g->hashTable[index] = key;
    }
    return index;
}


//add a edge to graph
void addEdge(struct Graph* g, int src, int dest) {
    //using hash table to get index of src and dest.
    int index_src = getIndex(src, g);
    int index_dest = getIndex(dest, g);

    //add edge from src to dest
    struct Node* newNode = createNode(index_dest);
    newNode->nextNode = g->adjList[index_src];
    g->adjList[index_src] = newNode;

    //add edge from dest to src
    newNode = createNode(index_src);
    newNode->nextNode = g->adjList[index_dest];
    g->adjList[index_dest] = newNode;
}


 // a utility function to create a array to keep track of whether a vertex has been discovered or not
int* initFunction(int length) {
    int* isVisited = (int*)malloc(length*sizeof(int));
    for(int i = 0; i < length; i++) {
        isVisited[i] = 0;
    }
    return isVisited;
}


//testttt
//a utility function to print a graph
void printGraph(struct Graph* g) {
    printf("%d \n", g->numVertices);
    printf("%d \n", g->numEdge);
    for(int i = 0; i < g->numVertices; i++) {
        int key = g->hashTable[i];
        struct Node* temp = g->adjList[i];
        printf("%d. ", key);
        while(temp != NULL) {
            printf("%d ", temp->data);
            temp = temp->nextNode;
        }
        printf("\n");
    }
}


//a utility function to create a name of result file
void createFileName(char* name, char* fileName, char* prefix) {

    strcpy(fileName, prefix);
    
    int k = strlen(fileName);
    int i = k;
    while(name[i-k] != ' '){
        fileName[i] = name[i-k];
        i++;
    }
    fileName[i] = '\0';
    
    char postfix[10] = ".txt";
    strcat(fileName,postfix);

    return ;
}


// a function to Perform iterative BFS
void iterativeBFS(struct Graph* g, int src, int* isVisited, FILE* fptr){

    struct Queue* q = createQueue();
    enQueue(q,src);
    isVisited[src] = 1;

    while(isQueueEmpty(q) == 0) {
        int u = q->front->data;
        // printf("%d ", g->hashTable[u]);
        fprintf(fptr, "%d \n", g->hashTable[u]);
        deQueue(q);
        struct Node* temp = g->adjList[u];
        while(temp != NULL) {
            if(isVisited[temp->data] == 0){
                isVisited[temp->data] = 1;
                enQueue(q,temp->data);
            }
            temp = temp->nextNode;
        }
    }
}


//BFS traversal 
void BFS(struct Graph* g, int src) {

    char FileName[40];
    char prefix[10] = "BFS_";
    createFileName(g->name, FileName, prefix);
    // printf("%s", FileName);
    

    FILE *fptr;
    fptr = (fopen(FileName, "w"));
    if(fptr == NULL){
       printf("Error!");
       exit(1);
    }

    // to keep track of whether a vertex is discovered or not
    int* isVisited = initFunction(g->numVertices);

    iterativeBFS(g,getIndex(src, g), isVisited, fptr);

    for(int i = 0; i < g->numVertices; i++) {
        if(isVisited[i] == 0) {
            iterativeBFS(g,i, isVisited, fptr);
        }
    }
    fclose(fptr);
}


// a function to Perform iterative DFS on graph starting from vertex 'src'
void iterativeDFS(struct Graph* g, int src, int* isVisited, FILE* fptr) {
    struct Stack* s = createStack();
    push(s, src);

    while(isStackEmpty(s) == 0) {
        int v = s->front->data;
        pop(s);

        if(isVisited[v] == 1)
            continue;
        
        isVisited[v] = 1;
        // printf("%d ", g->hashTable[v]);
        fprintf(fptr, "%d \n", g->hashTable[v]);
        struct Node* temp = g->adjList[v];
        while(temp != NULL) {
            if(isVisited[temp->data] == 0){
                push(s,temp->data);
            }
            temp = temp->nextNode;
        }
    }
}



// DFS traversal 
void DFS(struct Graph* g, int src){
   char FileName[40];
   char prefix[10] = "DFS_";
    createFileName(g->name, FileName, prefix);
    // printf("%s", FileName);
    
    int k = 0;
    FILE *fptr;
    fptr = (fopen(FileName, "w"));
    if(fptr == NULL){
       printf("Error!");
       exit(1);
    }

    // to keep track of whether a vertex is discovered or not
    int* isVisited = initFunction(g->numVertices);

    iterativeDFS(g, getIndex(src, g),isVisited, fptr);

    for(int i = 0; i < g->numVertices; i++) {
        if(isVisited[i] == 0) {
            iterativeDFS(g,i, isVisited, fptr);
        }
    }
    fclose(fptr);
}



//a utility function to add a vertex into the Vertext Cover
void addVertexCover(struct Graph* g, int VC) {
    int index_src = getIndex(VC, g);
    g->belongVertexCover[index_src] = 1;
}


//a utility function to import graph data from file into graph struct 
struct Graph* get_Graph(char* fileName){
    FILE* ptr;
    ptr = fopen(fileName,"r");

    struct Graph* g;
    int V, E;

    if (ptr == NULL){
        printf("file can't be opened \n");
        return NULL;
    }
    
    char kl[100];
    fgets(kl, 100, ptr);
    
    int done = 0;
    char name[40];
    fseek(ptr, 2, 1);
    if (fgets(name, 40, ptr) != NULL){
        int m;
        done++;
    }   
    
    fseek(ptr, 8, 1);

    if (fscanf(ptr, "%d", &V) == 1)
        done++;
    
    fseek(ptr, 7, 1);

    if (fscanf(ptr, "%d", &E) == 1)
        done++;
    
    if(done == 3) {
        // printf("%s", name);
        g = createGraph(V, E, name);
    }
    else{
        printf("Read file is error!");
        return NULL;
    }

    
    fseek(ptr, 25, 1);
    // printf("%c", fgetc(ptr));
    int x, y;  
    for(int i = 0; i < E; i++) {
        fscanf(ptr, "%d %d", &x, &y);
        addEdge(g, x, y);
    }
    // printf("xxdonexxx\n");
    fclose(ptr);
    return g;
}

//a utility function to import vertexcover data from file into graph struct 
void get_VertexCover(struct Graph* g, char* fileName) {
    FILE* ptr;
    ptr = fopen(fileName,"r");
    int VC;
    if (ptr == NULL){
        printf("file can't be opened \n");
        return;
    }
    
    char kl[100];

    fgets(kl, 100, ptr);
    fgets(kl, 100, ptr); 
 
    fseek(ptr, 17, 1);
    
    fscanf(ptr, "%d", &VC);
       
    fseek(ptr, 2, 1);
    
    printf("\n");
    char x;
    int y;
    for(int i = 0; i < VC; i++) {
        fscanf(ptr, "%s %d", &x, &y);
        
        addVertexCover(g, y);
        // printf("%d \n", y);
        // if(i == 10) break;
    }

    // printf("xxdonexxx\n");
    fclose(ptr);
}


//kieem tra khi khong lien thong
//a utility function to check Vertex Cover
int check_VertexCover(struct Graph* g, int src, int* isVisited) {
    
    struct Queue* q = createQueue();
    enQueue(q, src);
    isVisited[0] = 1;
    while(isQueueEmpty(q) == 0) {
        int u = q->front->data;
        deQueue(q);
        struct Node* temp = g->adjList[u];
        while(temp != NULL) {
            if(isVisited[temp->data] == 0){
                if(g->belongVertexCover[u] == 0 && g->belongVertexCover[temp->data] == 0){
                    return 0;
                }
                isVisited[temp->data] = 1;
                enQueue(q,temp->data);
            }
            temp = temp->nextNode;
        }
    }
   return 1;
}

int VertexCover_solution(struct Graph* g, int* isVisited){
    for(int i = 0; i < g->numVertices; i++) {
        if(isVisited[i] == 0 && check_VertexCover(g, i, isVisited) == 0) {
            return 0;
        }
    }
    return 1;
}


void get_fileName(int key, char* name) {
    // char CA[40] = "data.txt";
    char CA[40] = "roadNet-CA.txt";
    char PA[40] = "roadNet-PA.txt";
    char TX[40] = "roadNet-TX.txt";

    // char CA_VC[40] = "data.vc";
    char CA_VC[40] = "roadNet-CA.vc";
    char PA_VC[40] = "roadNet-PA.vc";
    char TA_VC[40] = "roadNet-TX.vc";

    switch (key){
    case 1:
        strcpy(name, CA);
        break;
    case 2:
        strcpy(name, PA);
        break;
    case 3:
        strcpy(name, TX);
        break;
    case 4:
        strcpy(name, CA_VC);
        break;

    case 5:
        strcpy(name, PA_VC);
        break;
    case 6:
        strcpy(name, TA_VC);
        break;
    }
}


// a utility function to get choise from user
int get_choise(int a, int b) {
    char key = getchar();
    // scanf("%c",&key);
    getchar();
    while(key - '0' < a || key - '0' > b){
        printf("Ban phai nhap trong khoang %d toi %d: ", a, b);
        key = getchar();
        getchar();
    }
    return key - '0';
}



void exitOption(int* run){
    *run = 0;
    printf("\nBam phim bat ky de thoat!");
    getch();
}


void GraphLoadingOption(struct Graph** g, int* temp) {
    int run = 1;
    int key;
    while(run) {
        system("cls");
        printf("===== Project I - Vo Hoai Nam 20204592 ======\n");
        printf("||  Ban muon tai do thi nao?               ||\n");
        printf("||    1. Road network of California        ||\n");
        printf("||    2. Road network of Pennsylvania      ||\n");
        printf("||    3. Road network of Texas             ||\n");
        printf("||    0. Thoat                             ||\n");
        printf("=============================================\n");
        printf("Lua chon cua ban la: ");

        key = get_choise(0, 3);

        if(key == 0) {
            exitOption(&run);
        }else {
            *temp = key;
            char fileName[40];
            get_fileName(key, fileName);
            *g = get_Graph(fileName);
            printf("\nTai do thi thanh cong!");
            run = 0;
            printf("\nBam phim bat ky de tiep tuc!");
            getch();
        }
    }
}

void graphTraversalOption(struct Graph* g) {
    int run = 1;
    int key;
    while(run) {
        system("cls");
        printf("===== Project I - Vo Hoai Nam 20204592 ======\n");
        printf("||  Ban muon theo DFS hay BFS?             ||\n");
        printf("||    1. DFS                               ||\n");
        printf("||    2. BFS                               ||\n");
        printf("||    0. Thoat                             ||\n");
        printf("||                                         ||\n");
        printf("=============================================\n");
        printf("Lua chon cua ban la: ");

        key = get_choise(0, 2);

        if(key == 0) {
            exitOption(&run);
        }else if(key == 1){
            DFS(g, 0);
            printf("\nDuyet do thi thanh cong!");
            run = 0;
            printf("\nBam phim bat ky de tiep tuc!");
            getch();
        }else{
            BFS(g, 0);
            printf("\nDuyet do thi thanh cong!");
            run = 0;
            printf("\nBam phim bat ky de tiep tuc!");
            getch();
        }
    
    }
}  

void checkVertexCover(int temp, struct Graph* g) {
    system("cls");
    printf("===== Project I - Vo Hoai Nam 20204592 ======\n");
    printf("|| Ban da chon kiem tra tap dinh bao phu!  ||\n");
    printf("||                                         ||\n");
    printf("||                                         ||\n");
    printf("||                                         ||\n");
    printf("||                                         ||\n");
    printf("=============================================\n");

    char fileName[40];
    get_fileName((temp + 3), fileName);
    get_VertexCover(g, fileName);
    int* isVisited = initFunction(g->numVertices);
    int bl = VertexCover_solution(g, isVisited);

    system("cls");
    printf("===== Project I - Vo Hoai Nam 20204592 ======\n");
    printf("|| Ban da chon kiem tra tap dinh bao phu!  ||\n");
    printf("||                                         ||\n");
    if(bl) {
        printf("||     Tap dinh bao phu     do thi         ||\n");
    }else {
        printf("||     Tap dinh khong bao phu do thi       ||\n");
    }
    printf("||                                         ||\n");
    printf("||                                         ||\n");
    printf("=============================================\n");
    printf("\nBam phim bat ky de tiep tuc!");
    getch();
}

void Display() {
    struct Graph* g = NULL;
    int key;
    int run = 1;
    int temp = 0;
    while(run) {
        system("cls");
        printf("===== Project I - Vo Hoai Nam 20204592 ======\n");
        printf("||  Ban muon thuc hien gi?                 ||\n");
        printf("||    1. Tai do thi                        ||\n");
        printf("||    2. Duyet do thi                      ||\n");
        printf("||    3. Kiem tra tap dinh bao phu         ||\n");
        printf("||    0. Thoat                             ||\n");
        printf("=============================================\n");
        printf("Lua chon cua ban la: ");

        key = get_choise(0, 3);

        switch (key)
        {
        case 0:
            exitOption(&run);
            break;
        case 1:
            GraphLoadingOption(&g, &temp);
        break;
        case 2:
            if(g != NULL) {
                graphTraversalOption(g);
            }else {
                printf("\nBan chua chon do thi!");
                printf("\nBam phim bat ky de tiep tuc!");
                getch();
            }
        break;
        case 3:
            if(g != NULL) {
                checkVertexCover(temp, g);
            }else {
                printf("\nban chua nhap du lieu!");
                printf("\nBam phim bat ky de tiep tuc!");
                getch();
            }
        break;
        }
    }
}

int main() {

    Display();

    printf("\n done");
    return 0;
}