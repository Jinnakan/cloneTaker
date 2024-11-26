#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

int numberOfTry[1000];//get from file
char status[100][100];

//map
// b = blank space
// W = unmovableWall(Map border)
// w = wall(pushable wall around the map)
// o = objective(door which required key to unlock)
// O = objective variant2(npc which can be interact if standing in surrounding tiles)
// k = key(for openiong objective)
// p = player
// m = monster(can be push around like pushable wall but get destroy if push to a wall or unmovable wall)
// t = trap(deal damage every even turn)
// y = different interval trap(deal damage every odd turn)
// u = uninterval trap(deal damage every turn)

// ? = wall & trap type 1(trap type1 with wall on top)
// ! = wall & trap type 2(trap type2 with wall on top)
// % = wall & trap type 3(trap type2 with wall on top)

// (,) = monster & trap(trap type1 with monster on top)
// (.) = monster & trap type 2(trap type2 with monster on top)
// (/) = monster & trap type 3(trap type3 with monster on top)

//defining space each map take
#define row1 8
#define column1 9

#define row2 8
#define column2 9

#define row3 9
#define column3 10

#define row4 7
#define column4 10

#define row5 9
#define column5 8

#define row6 9
#define column6 8

struct Static {
    int stage;
    int bestscore;
    int try_count;
};
struct Static *statics;
int numStages = 0;

//map 1 datas
char map1[row1][column1] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W','W'},
    {'W','W','W', 'W', 'W', 'b','b','W','W'},
    {'W','W','b', 'b', 'm', 'b', 'b','W', 'W'},
    {'W','W','b', 'm', 'b', 'm', 'W','W','W'},
    {'W','b', 'b', 'W', 'W', 'W','W','W','W'},
    {'W','b', 'w', 'b', 'b', 'w','b','W','W'},
    {'W','b', 'w', 'b', 'w', 'b','b','O','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W','W'}
    };

//map 2 datas
char map2[row2][column2] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W','W'},
    {'W','W','b', 'b', 'b', 'b','W','W','W'},
    {'W','W','m', 'W', 'u', 'u', 'b','b', 'W'},
    {'W','b','u', 'W', 'W', '?', '?','w','W'},
    {'W','b', 'b', 'W', 'W', 'b','u','b','W'},
    {'W','b', 'b', 'W', 'W', 'b','m','b','W'},
    {'W','W', 'W', 'W', 'W', 'O','b','m','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W','W'}
    };

//map 3 datas
char map3[row3][column3] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W','W','W'},
    {'W','W','W', 'W', 'O', 'O','O','b', 'W','W'},
    {'W','W','W', 'W', 'W', 'W', 'W','o','W','W'},
    {'W','W','W', 'b', 'u', 'u', 'b','b','b','W'},
    {'W','W', 'W', 'u', 'W', 'u','W','b','b','W'},
    {'W','W', 'W', 'b', 'b', 'm','u','u','W','W'},
    {'W','k', 'W', 'u', 'W', 'u','W','b','W','W'},
    {'W','b', 'b', 'b', 'b', 'b','m','b','W','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W','W','W'}
    };

//map 4 datas
char map4[row4][column4] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W','W','W'},
    {'W','b','W', 'k', 'b', 'w','W','W', 'W','W'},
    {'W','b','w', 'u', '%', 'b', 'o','b','W','W'},
    {'W','w','b', 'w', 'b', 'w', 'w','b','O','W'},
    {'W','b', 'w', 'b', 'w', 'b','w','w','b','W'},
    {'W','W', 'b', 'w', 'b', 'w','b','W','W','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W','W','W'}
    };

//map 5 datas
char map5[row5][column5] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W'},
    {'W','W','W', 'W', 'b', 'O','W', 'W'},
    {'W','W','W', 'b', 'o', 'w','b', 'W'},
    {'W','b','W', 't', 'b', 'w','b', 'W'},
    {'W','b', 'W', 'b','t', 'b', 't','W'},
    {'W','m', 'W', 'w', 'w', 'w','w','W'},
    {'W','t', 'b', 't', 'b', 'b','t','W'},
    {'W','W', 'W', 'W', 'W', 'W','k','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W'}
    };

//map 6 datas
char map6[row6][column6] = {
    {'W','W', 'W', 'W', 'W', 'W','W','W'},
    {'W','W','W', 'W', 'O', 'b','W', 'W'},
    {'W','W','W', 'W', 'b', 'o','b', 'W'},
    {'W','b','k', 'W', 'w', 'w','w', 'W'},
    {'W','m', 'w', 'b','m', 'w', 'b','W'},
    {'W','b', 'W', 'm', 'b', 'b','b','W'},
    {'W','t', 'W', 'W', 'y', 'W','W','W'},
    {'W','y', 't', 'y', 't', 'W','W','W'},
    {'W','W', 'W', 'W', 'W', 'W','W','W'}
    };

//declare functions
int checkTrap();
int checkTrap2();
int checkNext();
void victory();
void youLose();
int checkInteraction();
void save() {
    FILE *outputFile;
    int turn, i;

    outputFile = fopen("map.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file map.txt for writing.\n");
        return;  // Don't free statics as it's not allocated yet
    }

    // Write the modified data back to the file
    for (i = 0; i < numStages; i++) {
        fprintf(outputFile, "%d. %d avg. %d try\n", statics[i].stage, statics[i].bestscore, statics[i].try_count);
    }

    fclose(outputFile);

    // Example of updating the best score for a specific stage, e.g. after a turn
    if (statics[i].bestscore > turn) {
        statics[i].bestscore = turn;
    }
    statics[i].try_count = 5; // New try count (example value)
}

vvoid stat() {
    FILE *inputFile;
    char line[256];
    int stage, bestscore, try_count, i = 0;

    // Open the file map.txt in read mode
    inputFile = fopen("map.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening file map.txt\n");
        return;
    }

    // Count the number of stages
    while (fgets(line, sizeof(line), inputFile)) {
        if (sscanf(line, "%d. %d avg. %d try", &stage, &bestscore, &try_count) == 3) {
            numStages++;
        }
    }

    // Allocate memory for the structures based on the number of stages
    statics = (struct Static *)malloc(numStages * sizeof(struct Static));

    // Go back to the beginning of the file
    fseek(inputFile, 0, SEEK_SET);

    // Read the file again and store the data in the statics array
    while (fgets(line, sizeof(line), inputFile)) {
        if (sscanf(line, "%d. %d avg. %d try", &statics[i].stage, &statics[i].bestscore, &statics[i].try_count) == 3) {
            i++;
        }
    }

    fclose(inputFile);
}

void playMap();
void displayScoreboard() {
void displayScoreboard() {
    int i;

    stat();  // Load data from file
    printf("\n=====================================\n");
    printf("              SCOREBOARD             \n");
    printf("=====================================\n");
    printf("|   Stage   |   Score   |     try    |\n");
    printf("-------------------------------------\n");

    for (i = 0; i < numStages; i++) {
        printf("|   Stage %d  |     %d     |     %d     |\n", statics[i].stage, statics[i].bestscore, statics[i].try_count);
    }

    printf("=====================================\n");
    printf("\nPress any key to go back to the main menu...\n");
    getchar();  // Use getchar to pause and wait for input
}

}

void startScreen() {
    char choice;
    while (1) {
        printf("\n=====================================\n");
        printf("        WELCOME TO THE GAME        \n");
        printf("=====================================\n");
        printf("        1. Start Game            \n");
        printf("        2. Show Scoreboard       \n");
        printf("=====================================\n");
        printf("Enter your choice (1 or 2): ");
        
        choice = getch();
        
        if (choice == '1') {
            printf("\nStarting the game...\n\n");
            break; 
        } else if (choice == '2') {
            displayScoreboard();
            free(Static);
        } else {
            printf("\nInvalid choice. Please enter 1 or 2.\n");
        }
    }
}

int main()
{
    
    int level;
    
    while(1){
        startScreen();
        while(1){
            //display mainMenu interface
            printf("level\t\tnumberOfTry\t\tAvg.\t\tstatus");
            printf("\n1\t\t%d\t\t\t999\t\t%s",numberOfTry[1],status[1]);
            printf("\npress (-1) to back");
            printf("\nlevel(1-10): ");
            
            scanf("%d", &level);
            //get stage number
            if (level >= 1 && level <= 10) {
                playMap(level);
                break;
            }
            else if(level == -1){
                break;
            }
            system("cls");
        }
        //start playing selected stage     
        system("cls");
    }
}

//this is a function for running a stage
void playMap(int selectedMap){
    system("cls");
    //declaring nessessary variables
    int turn,Rturn;//turn is for traps timer & Rturn is remaining turn
    int row,column;//row & column is use for assigning selected stage
    int playerX,playerY,key=0;//initial player position & key status
    char map[100][100];//use for assigning selected map

    //set up for stage 1
    if(selectedMap == 1){
        turn = 24;//numbers of turn for this stage
        Rturn = turn;
        row = row1;//assign number of row and collumn of stage 1
        column = column1;
        playerX = 6;//player initial position
        playerY = 1;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map1[i][j];
            }
        }
    }

    //set up for stage 2
    if(selectedMap == 2){
        turn = 24;//numbers of turn for this stage
        Rturn = turn;
        row = row2;//assign number of row and collumn of stage 2
        column = column2;
        playerX = 1;//player initial position
        playerY = 5;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map2[i][j];
            }
        }
    }

    //set up for stage 3
    if(selectedMap == 3){
        turn = 32;//numbers of turn for this stage
        Rturn = turn;
        row = row3;//assign number of row and collumn of stage 3
        column = column3;
        playerX = 8;//player initial position
        playerY = 3;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map3[i][j];
            }
        }
    }

    //set up for stage 4
    if(selectedMap == 4){
        turn = 23;//numbers of turn for this stage
        Rturn = turn;
        row = row4;//assign number of row and collumn of stage 4
        column = column4;
        playerX = 1;//player initial position
        playerY = 1;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map4[i][j];
            }
        }
    }

    //set up for stage 5
    if(selectedMap == 5){
        turn = 23;//numbers of turn for this stage
        Rturn = turn;
        row = row5;//assign number of row and collumn of stage 5
        column = column5;
        playerX = 1;//player initial position
        playerY = 3;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map5[i][j];
            }
        }
    }

    //set up for stage 6
    if(selectedMap == 6){
        turn = 32;//numbers of turn for this stage
        Rturn = turn;
        row = row6;//assign number of row and collumn of stage 6
        column = column6;
        playerX = 6;//player initial position
        playerY = 5;
        for (int i = 0; i < row; i++) {//assign the stage data
            for (int j = 0; j < column; j++) {
                map[i][j] = map6[i][j];
            }
        }
    }


    //start playing the stage
    while(1){
        //display number of turn and time fail
        if(turn > -1){
            printf("turn left : %d\n",turn);
            printf("Time failed : %d\n",numberOfTry[selectedMap]);
        }
        //for not displaying turn = -1
        else{
            printf("turn left : 0\n");
            printf("Time failed : %d\n",numberOfTry[selectedMap]);
        }

        //display game interface
        for(int i=0;i<(column*2)+1;i++){
                printf("_");
            }
        printf("\n");

        //displaying elements in stage
        //checking the char in each positions and display accordingly
        for(int i=0;i<row;i++){
            //display game interface
            printf("|");
            for(int j=0;j<column;j++){
                //check if this position is player
                if(j == playerX && i == playerY){
                    printf("@");
                }
                //trap type 1
                else if(map[i][j] == 't'){
                    if((Rturn % 2) == 0){
                        printf("t");
                    }
                    else{
                        printf("#");
                    }
                }
                //trap type 2
                else if(map[i][j] == 'y'){
                    if((Rturn % 2) != 0){
                        printf("y");
                    }
                    else{
                        printf("#");
                    }
                }
                //trap type 3
                else if(map[i][j] == 'u'){
                    printf("u");
                }
                //blank space
                else if(map[i][j] == 'b'){
                    printf(" ");
                }
                //monster
                else if(map[i][j] == 'm'){
                    printf("m");
                }
                //wall & traps
                else if(map[i][j] == '?' || map[i][j] == '!' || map[i][j] == '%'){
                    printf("?");
                }
                //monster & traps
                else if(map[i][j] == '/' || map[i][j] == '.' || map[i][j] == ','){
                    printf("M");
                }
                //objective
                else if(map[i][j] == 'o'){
                    printf("o");
                }
                //objective2
                else if(map[i][j] == 'O'){
                    printf("O");
                }
                //key
                else if(map[i][j] == 'k'){
                    printf("k");
                }
                //movable wall
                else if(map[i][j] == 'w'){
                    printf("w");
                }
                //unmovable wall
                else if(map[i][j] == 'W'){
                    printf("W");
                }

                //display space after each postion
                if(j+1 != column && map[i][j+1] != ' '){
                    printf(" ");
                }
            }
            //display game interface
            printf("|\n");
        }

        //display game interface
        for(int i=0;i<(column*2)+1;i++){
                printf("-");
        }
        
        //check if npc is close to the player
        if(checkInteraction(playerX, playerY,map) == 1){
            //call function displaying win interface
            victory();
            //change this stage status to clear
            strcpy(status[selectedMap], "clear");
            break;
         }

        char direction;//for getting input
        //check if turn run out
        if(turn <= 0){
            //call function you lose displaying lose interface
            youLose();
            save();
            //increase try counnt for this stage
            numberOfTry[selectedMap] += 1;
            printf("\nPress (R) to restart and (L) to exit this stage");
            //wait until user choose to contiue playing or take the L
            while(1){
                direction = getch();
                if(direction == 'r'){
                    playMap(selectedMap);
                    break;
                }
                else if(direction == 'l'){
                    break;
                }
            }
            break;
        }

        //display interface asking for input
        printf("\nPress (R) to restart and (L) to exit this stage");
        printf("\nEnter movement direction(W,A,S,D): ");

        //get move input and move according to the direction
        //moving with the use of checkNext function to check the map
        //result = 0 player move normally
        //result = 1 player obtained key
        int result;
        direction = getch();
        if(direction == 'w'){
            if(playerY-1 >= 0){
                result = checkNext(playerX,playerY,map,0,-1,column,key);
                if(result == 0){
                    playerY -= 1;
                }
                else if(result == 1){
                    playerY -= 1;
                    key = 1;
                }
            }
        }
        else if(direction == 's'){
            if(playerY+1 < row){
                result = checkNext(playerX,playerY,map,0,1,column,key);
                if(result == 0){
                    playerY += 1;
                }
                else if(result == 1){
                    playerY += 1;
                    key = 1;
                }
            }
        }
        else if(direction == 'a'){
            if(playerX-1 >= 0){
                result = checkNext(playerX,playerY,map,-1,0,column,key);
                if(result == 0){
                    playerX -= 1;
                }
                else if(result == 1){
                    playerX -= 1;
                    key = 1;
                }
            }
        }
        else if(direction == 'd'){
            if(playerX+1 < column){
                result =  checkNext(playerX,playerY,map,1,0,column,key);
                if(result == 0){
                    playerX += 1;
                }
                else if(result == 1){
                    playerX += 1;
                    key = 1;
                }
            }
        }

        //this check if player choose to reset the game
        else if(direction == 'r'){
            numberOfTry[1] += 1;
            playMap(selectedMap);
            break;
        }
        //this check if player choose to take the L
        else if(direction == 'l'){
            numberOfTry[selectedMap] += 1;
            break;
        }

        //check if player in on an active trap?
        //if yes turn decrease by additional 1
        if(map[playerY][playerX] == 't'){
            if((Rturn % 2) == 0){
                turn--;
            }
        }
        else if(map[playerY][playerX] == 'y'){
            if((Rturn % 2) == 0){
                turn--;
            }
        }
        else if(map[playerY][playerX] == 'u'){
            turn--;
        }
        //turn decrease
        turn--;
        Rturn--;
        system("cls");
    }
}

void victory(){
    printf("\nYou win!!!");
    printf("\nPress (B) to go to main menu");

            while(1){
                int direction = getch();
                if(direction == 'b'){
                    break;
                }
            }
    }

void youLose(){
    printf("\n\nYou lose!!!");
}

//this check if player is next to the npc
int checkInteraction(int playerX, int playerY,char map[100][100]) {
    if ((map[playerY][playerX + 1] == 'O') || (map[playerY][playerX - 1] == 'O') ||
        (map[playerY + 1][playerX] == 'O') || (map[playerY - 1][playerX] == 'O')) {
        printf("\n\nYou have encountered a NPC!.\n");
        return 1;
    }
}

//this check the tile that player move into
int checkNext(int px, int py, char map[100][100], int x, int y,int column,int key) {
    char temp;
    //wall case
    if (map[py + y][px + x] == 'w'){
        //push wall over blank space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            printf(" %c",map[py + (2 * y)][px + (2 * x)]);
            temp = map[py + (2 * y)][px + (2 * x)];
            map[py + (2 * y)][px + (2 * x)] = map[py + y][px + x];
            map[py + y][px + x] = temp;
        }
        //push wall over trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap(px,py,map,x,y,1);
        }
        //push wall over trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap(px,py,map,x,y,2);
        }
        //push wall over trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap(px,py,map,x,y,3);
        }
    return -1;
    }

    //monster case
    else if(map[py + y][px + x] == 'm'){
        //push monster over blank space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            printf(" %c",map[py + (2 * y)][px + (2 * x)]);
            temp = map[py + (2 * y)][px + (2 * x)];
            map[py + (2 * y)][px + (2 * x)] = map[py + y][px + x];
            map[py + y][px + x] = temp;
        }
        //push monster over trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap2(px,py,map,x,y,1);
        }
        //push monster over trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap2(px,py,map,x,y,2);
        }
        //push monster over trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            checkTrap2(px,py,map,x,y,3);
        }
        //push monster against wall or unmovable wall
        else if(map[py + (2 * y)][px + (2 * x)] == 'w'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + y][px + x] = 'b';
        }
        //push monster against unmovableWall
        else if(map[py + (2 * y)][px + (2 * x)] == 'W'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + y][px + x] = 'b';
        }
    return -1;
    }

    //wall & trap type 1 case
    else if (map[py + y][px + x] == '?'){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'w';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '?';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '!';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '%';
            map[py + y][px + x] = 't';
        }
    return -1;
    }

    //wall & trap type 2 case
    else if (map[py + y][px + x] == '!'){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'w';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '?';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '!';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '%';
            map[py + y][px + x] = 'y';
        }
    return -1;
    }

    //wall & trap type 3 case
    else if (map[py + y][px + x] == '%'){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'w';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '?';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '!';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '%';
            map[py + y][px + x] = 'u';
        }
    return -1;
    }

    //monster & trap type 1 case
    else if (map[py + y][px + x] == ','){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'm';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = ',';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '.';
            map[py + y][px + x] = 't';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '/';
            map[py + y][px + x] = 't';
        }
    return -1;
    }

    //monster & trap type 2 case
    else if (map[py + y][px + x] == '.'){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'm';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = ',';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '.';
            map[py + y][px + x] = 'y';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '/';
            map[py + y][px + x] = 'y';
        }
    return -1;
    }

    //monster & trap type 3 case
    else if (map[py + y][px + x] == '/'){
        //push over black space
        if(map[py + (2 * y)][px + (2 * x)] == 'b'  && px + (2 * x) >= 0 && px + (2 * x) < column) {
            map[py + (2 * y)][px + (2 * x)] = 'm';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 1
        else if(map[py + (2 * y)][px + (2 * x)] == 't'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = ',';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 2
        else if(map[py + (2 * y)][px + (2 * x)] == 'y'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '.';
            map[py + y][px + x] = 'u';
        }
        //push over another trap type 3
        else if(map[py + (2 * y)][px + (2 * x)] == 'u'  && px + (2 * x) >= 0 && px + (2 * x) < column){
            map[py + (2 * y)][px + (2 * x)] = '/';
            map[py + y][px + x] = 'u';
        }
    return -1;
    }

    //unmovable wall case
    else if(map[py + y][px + x] == 'W'){
        return -1;
    }

    //key case(return obtained value)
    else if(map[py + y][px + x] == 'k'){
        map[py + y][px + x] = 'b';
        return 1;
    }
    //objective case
    else if(map[py + y][px + x] == 'o'){
        if(key == 1){
            map[py + y][px + x] = 'b';
        }
        else{
            return -1;
        }
    }


    return 0;  // Move normally
}

//this function is being call in checknext to check if a wall being push into a trap or not
int checkTrap(int px, int py, char map[100][100], int x, int y,int data){
    //check trap types
    if(data == 1){
        map[py + (2 * y)][px + (2 * x)] = '?';
        map[py + y][px + x] = 'b';
    }
    else if(data == 2){
        map[py + (2 * y)][px + (2 * x)] = '!';
        map[py + y][px + x] = 'b';
    }
    else if(data == 3){
        map[py + (2 * y)][px + (2 * x)] = '%';
        map[py + y][px + x] = 'b';
    }
}

//this function is being call in checknext to check if a monster being push into a trap or not
int checkTrap2(int px, int py, char map[100][100], int x, int y,int data){
    //check trap types
    if(data == 1){
        map[py + (2 * y)][px + (2 * x)] = ',';
        map[py + y][px + x] = 'b';
    }
    else if(data == 2){
        map[py + (2 * y)][px + (2 * x)] = '.';
        map[py + y][px + x] = 'b';
    }
    else if(data == 3){
        map[py + (2 * y)][px + (2 * x)] = '/';
        map[py + y][px + x] = 'b';
    }
}