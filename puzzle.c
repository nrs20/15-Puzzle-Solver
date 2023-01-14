#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

/*Solving a 15-puzzle problem needs to keep swapping the locations of the

blank tile and a tile adjacent to it

(i.e., above/below it or to the left/right of the blank title),

such that in the end all the  tiles are moved from their

initial locations to their goal locations.

*/

#define N 4

#define NxN (N * N)

#define TRUE 1

#define FALSE 0



struct node

{

    int tiles[N][N]; // 4x4

    int f, g, h;

    short zero_row, zero_column; /* location (row and colum) of blank tile 0 */

    struct node *next;

    struct node *parent; /* used to trace back the solution */

};



int goal_rows[NxN];

int goal_columns[NxN];

struct node *start, *goal;

struct node *open = NULL, *closed = NULL;

struct node *succ_nodes[4];



void print_a_node(struct node *pnode)

{ // supplied in skeleton code

    int i, j;

    for (i = 0; i < N; i++)

    {

        for (j = 0; j < N; j++)

            printf("%2d ", pnode->tiles[i][j]);

        printf("\n");

    }

    printf("\n");

}



struct node *initialize(char **argv)

{ // supplied in skeleton code

    int i, j, k, index, tile;

    struct node *pnode;



    pnode = (struct node *)malloc(sizeof(struct node));

    index = 1;

    for (j = 0; j < N; j++)

        for (k = 0; k < N; k++)

        {

            tile = atoi(argv[index++]);

            pnode->tiles[j][k] = tile;

            if (tile == 0)

            {                           // looking for 0 tile and marking it.

                pnode->zero_row = j;    // storing 0 j pos

                pnode->zero_column = k; // storing 0 k pos

            }

        }

    pnode->f = 0; // f,g,h initially 0.

    pnode->g = 0;

    pnode->h = 0;

    pnode->next = NULL;

    pnode->parent = NULL;

    start = pnode;

    printf("initial state\n");

    print_a_node(start);



    pnode = (struct node *)malloc(sizeof(struct node));

    goal_rows[0] = 3;

    goal_columns[0] = 3;



    for (index = 1; index < NxN; index++)

    {

        j = (index - 1) / N;

        k = (index - 1) % N;

        goal_rows[index] = j;

        goal_columns[index] = k;

        pnode->tiles[j][k] = index;

    }

    pnode->tiles[N - 1][N - 1] = 0; /* empty tile=0 */

    pnode->f = 0;

    pnode->g = 0;

    pnode->h = 0;

    pnode->next = NULL;

    goal = pnode;

    printf("goal state\n");

    print_a_node(goal);



    return start;

}

/* merge the remaining nodes pointed by succ_nodes[] into open list.

 * Insert nodes based on their f values --- keep f values sorted. */

void merge_to_open()

{ 

    struct node *temp;    

    struct node *insertNodes;

    bool null;

    bool tempNull;

    bool nextNull;

    int fValue;

    int insertNodeVal;

    int succG;

    int succF;

    int succH;

    int tempVal;

    int succCol, succPar, succRow;

    bool max = false;

    bool check;

    int insertNodeFalse;

    int tally = 0;

    insertNodes = (struct node *)malloc(sizeof(struct node));

    int i = 0;

    for (i; i < 4; i++)

    {

        tally = 1;

        temp = open;

        if (open != NULL)

            null = false;

        else

            null = true;

        if (succ_nodes[i] != NULL)

        {

            insertNodes = (struct node *)malloc(sizeof(struct node));

            int *insert = (void *)insertNodes->tiles;

            for (int j= 0; j < 4; j++)

            {

                for (int k = 0; k < 4; k++)

                {

                    insertNodes->tiles[j][k] = succ_nodes[i]->tiles[j][k];

                }

            } 

            if(insertNodes != NULL)

            {

            succG = succ_nodes[i]->g;

            succF = succ_nodes[i] -> f;

            succH = succ_nodes[i] -> h;

    


            insertNodes->g = succG;

            insertNodes->f = succF;

            insertNodes->h = succH;



            succCol = succ_nodes[i]->zero_column;




            insertNodes->zero_column = succCol;

            insertNodes->parent = succ_nodes[i] -> parent;

            insertNodes->zero_row = succ_nodes[i] -> zero_row;

            }

            else

            insertNodeFalse = 1;

            if (null == true)

                open = insertNodes;

            if (null)

                continue;

            if (temp != NULL)

                tempNull = true;

            else

                tempNull = false;

            if (temp->next != NULL)

                nextNull = true;

            else

                nextNull = false;

            while (temp->next != ((void *)0)) // sorting f's

            {

                nextNull = false;

                fValue = temp->next->f;

                insertNodeVal = insertNodes->f;

                if (fValue > insertNodeVal)

                {

                    max = true;

                    if (max == true)

                    {

                        insertNodes->next = temp->next;

                        temp->next = insertNodes;

                        break;

                    }

                    else

                    break;

                }



                else

                {

                    temp = temp->next;

                    if (temp == NULL)

                    {

                        tempNull = true;

                        break;

                    }

                }

            }

            if (!max)

            check = false;

            else

            check = true;


            if(check != true)

            temp->next = insertNodes;


            if(tally == 0)

            insertNodes = temp -> next;



        }

        else

            continue;

        // i++;

    }

}



/*swap two tiles in a node*/

// taken from GenGemPuzzle

void swap(int row1, int column1, int row2, int column2, struct node *pnode)

{                                                              // have to fill in

    int tile = pnode->tiles[row1][column1];                    // tile (temp) = value of first tile

    pnode->tiles[row1][column1] = pnode->tiles[row2][column2]; // value of first tile, = value of second tile.

    pnode->tiles[row2][column2] = tile;                        // value of second tile  = value of first tile (was storedd in temp)

}



// from ding slides:

// heuristic: choice 1 is number of misplaced tiles

// choice 2 = sum of manhattan distances of all non empty tiles

// Manhattan distance: number of squares (x and y) from current location

//  to the desired location



int manDistance(int tilePos, int row, int column)//heuristic

{ 

    int sum = 0;

    bool zeroFlag = true;

    bool unequal = true;

    int uneq = 0;

    if (zeroFlag)

    {

        // break;

        for (int i = 0; i < 16; i++)

        {

            for (int j = 0; j < 16; j++)

            {

                if (tilePos == 0)

                    continue;

                else if (goal->tiles[i][j] != tilePos)

                    unequal = false;

                else if (goal->tiles[i][j] == tilePos)

                {                                       // tiles in goal pos

                    sum += abs(row - i) + abs(column - j); // distance to other points from point given

                    zeroFlag = false;

                    return sum;

                }

                else

                    uneq += 2;

                // printf(uneq);

            }

        }

    }

    if (!unequal)

        uneq += 1;

    // printf("UNEQUAL FLAG FALSE");

    // else

    // return 0;

}

/*update the f,g,h function values for a node */

void update_fgh(int i){

 	struct node *pnode = succ_nodes[i];

    int parentGVAL;

    int nodeGVAL;

    int rightTile = 1;

    int heuristic;

    bool manHat;

    bool parentNull;

    parentGVAL = pnode -> parent -> g;

    heuristic = 0;

    while(rightTile < 16)

    {

    for(int i = 0; i < 16; i++){

        for(int j = 0; i < 16; i++){

            rightTile++;

            heuristic += manDistance(pnode->tiles[j][i], j, i);

        }

    }

    }

    if(pnode->parent == NULL)

        parentNull = true;

    else

        parentNull = false;

    if(!parentNull)

        pnode->g = parentGVAL + 2; //g value is updated to include parent's g value

    else if(parentNull == true)

        pnode -> g = 2; //2 seems to make it run faster

    pnode -> h = heuristic; //update h value with manhattan 

    pnode->f = pnode->g + pnode->h;     //f=g+h

 //   pnode->f = pnode->g + pnode->h;

}









/* 0 goes down by a row */

// move up and down deals with row

void move_down(struct node *pnode)

{                                      // has to be filled in

    int placeholder = pnode->zero_row; // location of blank tile in row

    placeholder += 1;

    if (!(placeholder > 4))

    { // means it is swappable

        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row + 1, pnode->zero_column, pnode);

        pnode->zero_row++;

    }

    else if (placeholder > 4) //

        pnode = NULL;

}

// move left and right deals with column

/* 0 goes right by a column */

void move_right(struct node *pnode)

{                                         // has to be filled in

    int placeholder = pnode->zero_column; // location of blank tile in row

    placeholder += 1;

    if (!(placeholder > 4))

    { // means it is swappable, check if it can move

        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column + 1, pnode);

        pnode->zero_column++;

    }

    else if (placeholder > 4) //

        pnode = NULL;

}



/* 0 goes up by a row */

void move_up(struct node *pnode)

{

    int placeholder = pnode->zero_row; // location of blank tile in row

    placeholder -= 1;

    if (!(placeholder < 0))

    {

        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row - 1, pnode->zero_column, pnode);

        pnode->zero_row--;

    }

    else if (placeholder < -1) //

        pnode = NULL;

}



/* 0 goes left by a column */

void move_left(struct node *pnode)

{

    int placeholder = pnode->zero_column; // location of blank tile in row

    placeholder -= 1;

    if (!(placeholder < 0))

    {

        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column - 1, pnode);

        pnode->zero_column--;

    }

    else if (placeholder < -1) //

        pnode = NULL;

}





void moving(struct node *nodes)

{

    move_right(nodes);

}

void moving2(struct node *nodes)

{

    move_up(nodes);

}

void moving3(struct node *nodes)

{

    move_down(nodes);

}

void moving4(struct node *nodes)

{

    move_left(nodes);

}



/* expand a node, get its children nodes, and organize the children nodes using

 * array succ_nodes.

 */

void expand(struct node *selected)

{ // selected node is the best node in algo

    // want the selected node's children

    bool nullFlag;

    for (int i = 0; i < 4; i++)

    {

        succ_nodes[i] = (struct node *)malloc(sizeof(struct node)); // allocate mem

    }

    int *selectedTiles = (void *)selected->tiles;

    for (int i = 0; i < 4; i++)

    {

        // succ_nodes[i]->tiles = selectedTiles;

        // signature: void *memcpy(void *dest, const void * src, size_t n)

        memcpy(succ_nodes[i]->tiles, selectedTiles, 16 * sizeof(int));

        succ_nodes[i]->parent = selected; // update parent for each

    }

    if(succ_nodes != NULL)

    nullFlag = false;

    else

    nullFlag = true;

    int j = 0;

    if(nullFlag == false)

    {

        while (j < 4)

        {

            succ_nodes[j]->zero_column = 0;

            succ_nodes[j]->zero_row = 0;

            j += 1;

        }

    }

    int r = 0;

    if(j == 4 && nullFlag != true)

    {

        while (r < 4)

    {

        succ_nodes[r]->zero_column = selected->zero_column;

        succ_nodes[r]->zero_row = selected->zero_row;

        r += 1;

    }

    }

    for(int z =0; z<4; z++)

    {

        if(z==0)

        {

            moving(succ_nodes[z]);

        }

        else if(z==1)

        {

            moving3(succ_nodes[z]);

        }

        else if(z==2)

        {

          moving4(succ_nodes[z]);

      

        }

        else

            moving2(succ_nodes[3]);



    }



    for (int i = 0; i < 4; i++)

    {

        update_fgh(i);

    }

}



int nodes_same(struct node *a, struct node *b)

{ // given in skeleton code

    int flg = FALSE;

    if (memcmp(a->tiles, b->tiles, sizeof(int) * NxN) == 0)

        flg = TRUE;

    return flg;

}



/* Filtering. Some states in the nodes pointed by succ_nodes may already be included in

 * either open or closed list. There is no need to check these states. Release the

 * corresponding nodes and set the corresponding pointers in succ_nodes to NULL. This is

 * important to reduce execution time.

 * This function checks the (i)th node pointed by succ_nodes array. You must call this

 * function in a loop to check all the nodes in succ_nodes. Free the successor node and

 * set the corresponding pointer to NULL in succ_node[] if a matching node is found in

 * the list.

 */

void filter(int i, struct node *pnodeL)

{ // had to fill in

    bool null = true;

    bool tempNullCheck;

    bool nodesSame;

    struct node *temp = pnodeL;

    struct node *succ = succ_nodes[i];

    if(temp == ((void *)0))

    tempNullCheck = true;

    else

    tempNullCheck =false;

    while (temp != NULL || tempNullCheck) //make sure doesnt cause issues

    {

        if (succ == NULL)

        {

            null = false;

            break;

        }

        else if (pnodeL == NULL)

        {

            null = false;

            break;

        }

        else if (nodes_same(succ_nodes[i], temp))

        {

            succ_nodes[i] = NULL; // setting corresponding pointers to NULL.

            nodesSame = true;

            break;

        }

        temp = temp->next;

    }

    if (!null)

        return;

    if(nodesSame)

    return;

}



int main(int argc, char **argv)

{

    int iter, cnt;

    bool found = false;

    struct node *copen, *cp, *solution_path;

    int ret, i, pathlen = 0, index[N - 1];



    solution_path = NULL;

    start = initialize(argv);/* init initial and goal states */

    open = start;



    iter = 0;

    while (open != NULL)/* Termination cond with a solution is tested in expand. */

    {

        copen = open;

        open = open->next; /* get the first node from open to expand */

        if (nodes_same(copen, goal))/* goal is found */

        {

            found = true;

            do /* trace back and add the nodes on the path to a list */

            {

                copen->next = solution_path;

                solution_path = copen;

                copen = copen->parent;

                pathlen++;

            } while (copen != NULL);

            printf("Path (lengh=%d):\n", pathlen);

            /* print out the nodes on the list */

            do

            {

                print_a_node(solution_path);

                solution_path = solution_path->next;

            } while (solution_path != NULL);

            break;

        }

        expand(copen);

        for (i = 0; i < 4; i++)

        {

            filter(i, open);

            filter(i, closed);

        }



        merge_to_open(); /* New open list */

        copen->next = closed;

        closed = copen; /* New closed */

        /* print out something so that you know your 

		 * program is still making progress 

		 */

        iter++;

        if (iter % 1000 == 0)

        {

            printf("iter %d\n", iter);

        }

    }

    if(open == NULL && found == false)

    printf("No solution.");



    return 0;

} /* end of main */
