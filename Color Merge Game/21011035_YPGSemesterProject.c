#include <stdio.h> //  ayn� ki�iye puanlar� ekleme, puan y�ksekli�in g�re s�ralama,  otomatik modu h�zland�rma i�lemi
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include<string.h>
#include<math.h>
#define MAX 15 // kullan�lacak max sayi
void drawBoard(int matris[][MAX], int);
int readFromFile(int matris[][MAX], char *fileName);
void addPlayer();
void displayPlayers();
void randMatrix();
int pathFinder();
int coordinateFinder();
int isMatrixTrue();
void resetUnits();
void resetMatrix();
int undoBoard();
void storeBoards();
void resetCoordinates();
void manuel();
void autoMode();
int legalMove();
double calculateScore();
int matrixCheck(int matrix[][MAX], int dim);
void writeCoordinates();
int main()
{
    srand(time(NULL));
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
    int i, j, menu, submenu, N, stop, file, didQuit,top,win;
    int matris[MAX][MAX] = {0};
    int startCoordinates[MAX][3] = {{0}};
    int endCoordinates[MAX][3] = {{0}};
    int visited[MAX][MAX] = {{0}};
    int finalMatrix[MAX][MAX] = {{0}};
    int connected[MAX] = {0};
    char playerNames[MAX][MAX] = {{'\0'}};
    double playerScores[MAX] = {0};
    int playerUndoCount[MAX] = {0};
    int playerGameCount[MAX] = {0};
    int store[MAX][MAX][MAX] = {{{0}}};
    int currentPlayerCount = 0;
    int currentPlayer = 0;
    char fileName[20];
    double timeSpent = 0.0;
    clock_t  start;
    clock_t  end;
    menu = 4;
    submenu = 3;
    stop = 0;
    didQuit = 0;
    top = 0;
    int *stopPtr = &stop;
    int *topPtr = &top;
    do
    {

        printf("\n\n\t###MAIN MENU###\n\n1 -> Generate a Random Matrix\n2 -> Create a Matrix From a File\n3 -> Show User Scores\n4 -> Exit\n\nMenu input : ");
        scanf("%d", &menu);
        system("cls");
        resetUnits(matris,finalMatrix,visited,startCoordinates,endCoordinates,connected,stopPtr,topPtr,playerGameCount);
        didQuit = 0;
        win = 0;
        switch (menu)
        {
        case 1: // randomizer
            addPlayer(playerNames, playerScores, &currentPlayerCount,&currentPlayer);
            printf("\nEnter the dimension of the matrix.\nDimension : ");
            scanf("%d", &N);
            randMatrix(matris, N);
            drawBoard(matris, N);
            do
            {
                printf("\n\n1 -> Play in Manuel Mode\n2 -> Play in Automatic Mode\n3 -> Return to Main Menu\n\nMenu input : ");
                scanf("%d", &submenu);
                system("cls");
                switch (submenu)
                {
                case 1:
                    coordinateFinder(matris, startCoordinates, endCoordinates, N);
                    start = clock();
                    manuel(matris, N, startCoordinates, endCoordinates, visited, connected, store, topPtr,&didQuit,playerUndoCount,&currentPlayer,&win);
                    end = clock();
                    timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
                    playerGameCount[currentPlayer]++;
                    playerScores[currentPlayer] += calculateScore(timeSpent,1,N,1,playerUndoCount[currentPlayer],playerGameCount[currentPlayer],playerScores[currentPlayer],&win);
                    printf("\nCompleted in %.2lf seconds",timeSpent);
                    printf("\nTotal score of %s\t\t: %.0lf points",playerNames[currentPlayer],playerScores[currentPlayer]);
                    break;
                case 2:
                    coordinateFinder(matris, startCoordinates, endCoordinates, N);
                	start = clock();
                    autoMode(startCoordinates[1][1], startCoordinates[1][2], N, 1, stopPtr, matris, startCoordinates, endCoordinates, visited, finalMatrix, connected);
                    end = clock();
                    timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
                    if(didQuit != 1){
                    playerGameCount[currentPlayer]++; 
					playerScores[currentPlayer] += calculateScore(timeSpent,0,N,1,playerUndoCount[currentPlayer],playerGameCount[currentPlayer],playerScores[currentPlayer],&win);
					if(stop == 1) win = 1;
                    printf("\nCompleted in %.2lf seconds",timeSpent);
                    printf("\nTotal score of %s \t\tis : %.0lf points",playerNames[currentPlayer],playerScores[currentPlayer]);
					}

                    break;
                case 3:
                    system("cls");
                    break;
                default:
                    printf("\nYour input is invalid. Please enter a valid input.");
                    break;
                }
                break;
            } while (submenu != 3);

            break;
        case 2: // file
            addPlayer(playerNames, playerScores, &currentPlayerCount,&currentPlayer);
            fflush(stdin);
            printf("\nEnter the dimension of the matrix.\nDimension : ");
            scanf("%d", &N);
            fflush(stdin);
            printf("\n\nEnter the name of the file.\nFile name : ");
            scanf("%s", fileName);
            file = readFromFile(matris, fileName);
            if (file == 0)
                drawBoard(matris, N);
            do
            {
                if (file != 0)
                {
                    printf("\nReturning to main menu...");
                    Sleep(2000);
                    submenu = 3;
                    system("cls");
                }
                else
                {
                    printf("\n\n1 -> Play in Manuel Mode\n2 -> Play in Automatic Mode\n3 -> Return to Main Menu\n\nMenu input : ");
                    fflush(stdin);
                    scanf("%d", &submenu);
                    system("cls");
                    switch (submenu)
                    {
                    case 1:
                    	didQuit = 0;
                        coordinateFinder(matris, startCoordinates, endCoordinates, N);
                        start = clock();
                        manuel(matris, N, startCoordinates, endCoordinates, visited, connected, store, topPtr,&didQuit,playerUndoCount,&currentPlayer,&win);
                        end = clock();
                        timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
   						if(didQuit != 1){ 
                        playerGameCount[currentPlayer]++;
						playerScores[currentPlayer] += calculateScore(timeSpent,1,N,0,playerUndoCount[currentPlayer],playerGameCount[currentPlayer],playerScores[currentPlayer],&win);
                    	printf("\nCompleted in %.2lf seconds",timeSpent);
                    	printf("\nTotal score of %s\t\t: %.0lf points",playerNames[currentPlayer],playerScores[currentPlayer]);
						}
                        break;
                    case 2:
                        coordinateFinder(matris, startCoordinates, endCoordinates, N);
                        start = clock();
                        autoMode(startCoordinates[1][1], startCoordinates[1][2], N, 1, stopPtr, matris, startCoordinates, endCoordinates, visited, finalMatrix, connected);
                        end = clock();
                        timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
                        playerGameCount[currentPlayer]++;
                        if(stop == 1) win = 1;
                        playerScores[currentPlayer] += calculateScore(timeSpent,0,N,0,playerUndoCount[currentPlayer],playerGameCount[currentPlayer],playerScores[currentPlayer],&win);
                        printf("\nCompleted in %.2lf seconds",timeSpent);
                        printf("\nTotal score of %s \t : %.0lf points",playerNames[currentPlayer],playerScores[currentPlayer]);
                        break;
                    case 3:
                        system("cls");
                        break;
                    default:
                        printf("\nYour input is invalid. Please enter a valid input.");
                        break;
                    }
                    break;
                }

            } while (submenu != 3);
            break;
        case 3:
            displayPlayers(playerNames, playerScores, &currentPlayerCount);
            break;
        case 4:
            break;
        default:
            printf("\nYour input is invalid. Please enter a valid input.");
            break;
        }
    } while (menu != 4);
    return 0;
} // main end



int readFromFile(int matris[][MAX], char *fileName)
{
    int i, j, temp;
    FILE *data = fopen(fileName, "r");
    if (!data)
    {
        printf("\nERROR!\nFile cannot be opened!");
        return -1;
    }
    while (!feof(data))
    {
        fscanf(data, "%d %d %d\n", &i, &j, &temp);
        matris[i][j] = temp;
    }
    fclose(data);
    return 0;
}
void drawBoard(int matris[][MAX], int N)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        printf("     %d",i);
    }
    printf("\n");
    for (i = 0; i < N; i++)
    {
    	printf("  ");
        SetConsoleTextAttribute(hConsole, 15);
        for (k = 0; k < N; k++)
        {
            printf("------");
        }
        printf("-\n");
        printf("%d |",i);
        for (j = 0; j < N; j++)
        {
            if (matris[i][j] != 0)
            {
                SetConsoleTextAttribute(hConsole, 16 * (matris[i][j]) + matris[i][j]);
                printf("  %d  ", matris[i][j]);
                SetConsoleTextAttribute(hConsole, 15);
                printf("|");
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 15);
                printf("     |");
            }
        }
        SetConsoleTextAttribute(hConsole, 15);
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 15);
    printf("  ");
    for (k = 0; k < N; k++)
    {
        printf("------");
    }

    printf("-");
}
void resetUnits(int matrix[][MAX],int finalMatrix[][MAX],int visited[][MAX],int startCoordinates[][3],int endCoordinates[][3],int connected[MAX], int*stopPtr, int* topPtr,int playerUndoCount[MAX]){
	int i;
	resetMatrix(matrix);
    resetMatrix(finalMatrix);
    resetMatrix(visited);
    resetCoordinates(startCoordinates, endCoordinates);
    for(i = 0; i<MAX; i++){
        connected[i] = 0;
        playerUndoCount[i] = 0;

	} 
		*stopPtr = 0;
		*topPtr = 0;
}
void randMatrix(int matrix[][MAX], int N)
{

    int i, j, randRow, randCol;
    int count[MAX] = {0};
    randRow = rand() % N;
    randCol = rand() % N;
    for (i = 1; i <= N; i++)
    {

        for (j = 0; j <= 1; j++)
        {

            while (matrix[randRow][randCol] != 0)
            {
                randRow = rand() % N;
                randCol = rand() % N;
            }
            matrix[randRow][randCol] = i;
        }
    }
}
void addPlayer(char playerNames[][MAX], int playerScores[MAX], int *currentPlayerCount, int* currentPlayer)
{
	char name[MAX];
    if (*currentPlayerCount >= MAX)
    {
        printf("\nThe maximum player count (15) is reached.\n");
        return;
    }
    int i, j;
    fflush(stdin);
    do{
    printf("\nEnter player name (maximum 15 characters): ");
    scanf("%s", name);
	}while(strlen(name)>=15);
    
   
    if(*currentPlayerCount != 0){
    	for(i = 0; i < *currentPlayerCount; i++){
        if(strcmp(name,playerNames[i]) == 0){
            *currentPlayer = i;
            return;
        }
    }
	}
    strcpy(playerNames[*currentPlayerCount],name);
    *currentPlayer = *currentPlayerCount;
    *currentPlayerCount+=1;
    return;
}
void displayPlayers(char playerNames[][MAX], double playerScores[MAX], int *currentPlayerCount)
{
    int i;
    if (*currentPlayerCount == 0)
    {
        printf("\nNo players in the list.");
        return;
    }
    printf("\n\n---Player List---\n\n");
    for (i = 0; i < *currentPlayerCount; i++)
    {
        printf("Player Name : %s \t\t Score : %.0lf \n", playerNames[i], playerScores[i]);
    }
    return;
}
double calculateScore(double time, int gameType, int N, int matrixType, int undoCount, int gameCount, double playerScore, int* win){
	double score = 10.0;
    //printf("\nYour starting point is 10.");
    score *= 1.0*N;
    //printf("\nYour game board's dimension is %d, score is multiplied %dx.\t Current score : %lf",N,N,score);
	if(gameType == 1){
        score *= 5.0;
		//printf("\nGame mode is manual, score is multiplied 5x.\t Current score : %lf",score);
    }
	else if (gameType == 0){
        score *= 1.0;
		//printf("\nGame mode is automatic, score is multiplied by 1x.\t Current score : %lf",score);
	}
	if(matrixType == 1){	
        score *= 10.0;	
		//printf("\nMatris is created randomly, score is multiplied by 10x.\t Current score is : %lf",score);
	}
	else if(matrixType == 0){
		score *= 3.0;
		//printf("\nMatris is created from a file, score is multiplied by 3x.\t Current score : %lf",score);
		
	}
    if(gameType == 1){
        score+= -10.0*undoCount;
        //printf("\nEach undo subtracts 10 points, %lf will be subtracted. Current score : %lf",-10.0*undoCount,score);
    }
    score *= 1.0*pow(1.1,gameCount);
	//printf("\nGame count, multiplies your score by (1.1x)^gameCount.\t Current game count : %d Current score : %lf",gameCount,score);
    score /=1.0*time*0.2;
	//printf("\nTime divides your points by (time*0.02).\t Current score : %lf",score);
	if(*win == 0 && gameType == 1){
		score/= 4;
	//	printf("\nYou lost the game, your score will be reduced %75.\t Current score : %lf",score);
	}
	else if (*win == 0 && gameType == 0){
		score = 0;
		printf("\nThis matrix is unsolvable. Your score is set to 0.");
	}
	playerScore = score;
	printf("\nYour score for this game : %.0lf",playerScore);
	return playerScore;
	
}
void resetCoordinates(int startCoordinates[][3], int endCoordinates[][3])
{
    int i, j;
    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < 3; j++)
        {
            startCoordinates[i][j] = 0;
            endCoordinates[i][j] = 0;
        }
    }
    return;
}
void writeCoordinates(int startCoordinates[][3], int endCoordinates[][3], int N)
{
    int i, j;
    for (i = 1; i <= N; i++)
    {
        printf("\n%d -> Start (%d,%d) - End (%d,%d)", i, startCoordinates[i][1], startCoordinates[i][2], endCoordinates[i][1], endCoordinates[i][2]);
    }
    return;
}
void storeBoards(int store[][MAX][MAX], int matrix[][MAX], int N, int* top)
{
    int i, j, k;
    if (*top < MAX)
    {	
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                store[*top][i][j] = matrix[i][j];
            }
        }
        *top += 1;
        return;
    }
    if (*top == MAX-1)
    {
        for (k = 0; k < MAX; k++)
        {
            for (i = 0; i < N; i++)
            {
                for (j = 0; j < N; j++)
                {
                    store[k][i][j] = store[k + 1][i][j];
                }
            }
        }
        
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                store[*top][i][j] = matrix[i][j];
            }
        }
        return;
    }
}
int undoBoard(int store[][MAX][MAX], int matrix[][MAX], int N, int *top, int playerUndoCount[MAX],int* currentPlayer)
{
    int i, j;
    *top-=1;
    if (*top == -1) return -1;
    
    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX; j++)
        {
            matrix[i][j] = store[*top][i][j];
            
        }
    }
    playerUndoCount[*currentPlayer]+=1;
    return 0;
}
void manuel(int matrix[][MAX], int N, int startCoordinates[][3], int endCoordinates[][3], int visited[][MAX], int connected[MAX],int store[][MAX][MAX], int* top, int*didQuit,int playerUndoCount[MAX],int* currentPlayer, int* win)
{
    int i, j, connect, clear, menu,exit,mainmenu,notLegal;
    menu = 1;
    exit = 0;
    mainmenu = 0;
    int sourceX, sourceY, destX, destY;
    int coordinates[2][2] = {{0}};
    printf("\n");
    clear = 0;
    while (clear == 0)
    {
        do{
        system("cls");
        drawBoard(matrix, N);
        fflush(stdin);
        printf("\nEnter source coordinate (x,y) : ");
        scanf("%d,%d", &sourceX, &sourceY);
        printf("\nSource coordinates : (%d,%d) \n", sourceX, sourceY);
        printf("\nEnter destination coordinate (x,y) : ");
        scanf("%d,%d", &destX, &destY);
        printf("\nDestination coordinates : (%d,%d) \n ", destX, destY);
		}while(sourceX < 0 || sourceX > N-1 || sourceY < 0 || sourceY > N-1 ||destX < 0 || destX > N-1 ||destY < 0 || destY > N-1);
		storeBoards(store,matrix,N,top);
        exit = 0;
        notLegal = 0;
        while((sourceX == destX || sourceY == destY) && exit == 0){
        if (sourceX == destX) // saga veya sola gidecek
        {
            if (sourceY > destY) // sol
            {
                for (i = sourceY-1; i >= destY; i--)
                {
                    if(matrix[sourceX][i] != 0 && matrix[sourceX][sourceY] != matrix[destX][destY]) notLegal = 1;
                    
                }
                if(notLegal != 1){
                for (i = sourceY; i >= destY; i--)
                {
                    matrix[sourceX][i] = matrix[sourceX][sourceY];
                }	
				}
                
                
            }
            if (sourceY < destY) // sag
            {
                for (i = sourceY+1; i <= destY; i++)
                {
                    if(matrix[sourceX][i] != 0  && matrix[sourceX][sourceY] != matrix[destX][destY]) notLegal = 1;
                }
                if(notLegal != 1){
                for (i = sourceY; i <= destY; i++)
                {
                     matrix[sourceX][i] = matrix[sourceX][sourceY];
                }
				}
                
                
            }
        }
        if (sourceY == destY) // yukari veya asagi gidecek
        {
            if (sourceX > destX) // yukari
            {
                for (i = sourceX-1; i >= destX; i--)
                {
                    if(matrix[i][sourceY] != 0 && matrix[sourceX][sourceY] != matrix[destX][destY]) notLegal = 1;
                    
                }
                if(notLegal != 1){
                for (i = sourceX; i >= destX; i--)
                {
                    matrix[i][sourceY] = matrix[sourceX][sourceY];
                }	
				}
				
            }
            
            if (sourceX < destX) // asagi
            {
                for (i = sourceX+1; i <= destX; i++)
                {
                    if(matrix[i][sourceY] != 0 && matrix[sourceX][sourceY] != matrix[destX][destY]) notLegal= 1;
                }
                
                if(notLegal != 1){
                for (i = sourceX; i <= destX; i++)
                {
                    matrix[i][sourceY] = matrix[sourceX][sourceY];
                }
				}
                
            }
        }
        
        if(notLegal == 0){
        system("cls");
        drawBoard(matrix,N);
		printf("\nPath is created from (%d,%d) -> (%d,%d) ", sourceX, sourceY, destX, destY);
        printf("\nU -> Undo move\nC -> Continue for next move\nE -> Return to Main Menu\nYour input: ");
        fflush(stdin);
        scanf("%c", &menu);
        if(menu == 'e' || menu == 'E') {
        	system("cls");
        	*didQuit = 1;
        	return;	
		}
        printf("\n");
        system("cls");
        clear = matrixCheck(matrix, N);
        if (menu == 'U' || menu == 'u')
        {
        	system("cls");
            do
            {
            if(undoBoard(store,matrix,N,top,playerUndoCount,currentPlayer) != -1){
        	system("cls");
            drawBoard(matrix,N);
            fflush(stdin);
            printf("\nMatris has been reverted!\nU -> Undo again\nC -> Continue game\nYour input :");
            scanf("%c", &menu);
			}
			else{
				printf("\nUndo limit is reached, you cannot undo anymore.\n");
				menu = 'c';
			} 
            } while (menu == 'u' || menu == 'U');
        }
		exit = 1;	
		}
		exit = 1;
		}
		if(notLegal == 1){
			system("cls");
			printf("\nYour move is illegal! You cannot cross different colours!");
			Sleep(2000);
		}
		if(sourceX != destX && sourceY != destY){
			system("cls");
			printf("\nYou can only move horizontally or vertically, not both at same time, please enter a valid move.\n");
			Sleep(2000);
		} 
        
    }
    printf("\nMatrix has been filled! Checking matrix...\n");
    for (i = 1; i <= N; i++)
    {
        isMatrixTrue(startCoordinates[i][1], startCoordinates[i][2], N, i, matrix, startCoordinates, endCoordinates, visited, connected);
    }
    connect = 0;
    for (i = 1; i <= N; i++)
    {
        if (connected[i] == 1)
        {
            connect++;
        }
    }
    if (connect == N)
    {
    	system("cls");
        printf("\nCongrats!!! You won the game!\n\n");
        drawBoard(matrix, N);
        *win = 1;
        
        return;
    }
    else
    {
    	system("cls");
        printf("\nOops! Your board is incorrect! You lost the game! Your legal connected number count is : %d\n\n", connect);
        drawBoard(matrix, N);
        *win = 0;
        
        return;
    }
}
void resetMatrix(int matrix[][MAX])
{
    int i, j;
    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX; j++)
        {
            matrix[i][j] = 0;
        }
    }
}
int coordinateFinder(int matrix[][MAX], int startCoordinates[][3], int endCoordinates[][3], int N)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (matrix[i][j] != 0 && startCoordinates[matrix[i][j]][0] == 0)
            {
                startCoordinates[matrix[i][j]][0] = 1;
                startCoordinates[matrix[i][j]][1] = i;
                startCoordinates[matrix[i][j]][2] = j;
            }
            if (matrix[i][j] != 0 && startCoordinates[matrix[i][j]][0] == 1)
            {

                endCoordinates[matrix[i][j]][0] = 1;
                endCoordinates[matrix[i][j]][1] = i;
                endCoordinates[matrix[i][j]][2] = j;
            }
        }
    }
}
int matrixCheck(int matrix[][MAX], int N)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (matrix[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}
int pathFinder(int x, int y, int N, int dest, int *stop, int matrix[][MAX], int startCoordinates[][3], int endCoordinates[][3], int visited[][MAX], int finalMatrix[][MAX], int connected[MAX])
{
    if (x < 0 || x > N - 1) return 0;
    if (y < 0 || y > N - 1) return 0;
    if(matrix[x][y] != dest && matrix[x][y] != 0) return 0;
    if(*stop == 1) return 0;
    int i, j, check, connect, move;
    check = connect = 0;
    int dx[4] = {1, 0, 0, -1};
    int dy[4] = {0, -1, 1, 0};
    if(*stop == 1) return 0;
    system("cls");
    drawBoard(matrix,N);
    Sleep(50); 
    if (dest >= N)
    {
    	if(*stop != 1)
        check = matrixCheck(matrix, N);
        if (check == 1)
        {
            for (j = 1; j <= N; j++)
            {
                resetMatrix(visited);
                isMatrixTrue(startCoordinates[j][1], startCoordinates[j][2], N, j, matrix, startCoordinates, endCoordinates, visited, connected);
            }
            for (i = 1; i <= N; i++)
            {
                if (connected[i] == 1)
                    connect++;
                else
                    connect--;
            }
            if (connect == N)
            {
                for (i = 0; i < N; i++)
                {
                    for (j = 0; j < N; j++)
                    {
                        finalMatrix[i][j] = matrix[i][j];
                    }
                }
                *stop = 1;
                return 0;
            }
        }
    }
    if (x == endCoordinates[dest][1] && y == endCoordinates[dest][2])
    {
        pathFinder(startCoordinates[dest + 1][1], startCoordinates[dest + 1][2], N, dest + 1, stop, matrix, startCoordinates, endCoordinates,visited,finalMatrix,connected);
        return 0;
    }
    if (x == startCoordinates[dest][1] && y == startCoordinates[dest][2])
    {
    	if(visited[x][y] == 0){ // baslangic koordinatlar� ile bitis koordinatlarini ayirt etmek icin konuldu, komsulardan biri baslangic koordinatina esitse fonksiyon cagirmiyor.
    	visited[x][y] = 1;
        for (i = 0; i < 4; i++)
        {
        	if(matrix[x+dx[i]][y+dy[i]] == 0) pathFinder(x + dx[i], y + dy[i], N, dest, stop, matrix, startCoordinates, endCoordinates, visited,finalMatrix,connected);
            else if (x+dx[i] == endCoordinates[dest][1] && y+dy[i] == endCoordinates[dest][2]) pathFinder(x + dx[i], y + dy[i], N, dest, stop, matrix, startCoordinates, endCoordinates, visited,finalMatrix,connected);
        }
        visited[x][y] = 0;
		}
        return 0;
    }
    if (matrix[x][y] == 0)
    {
        matrix[x][y] = dest;
        for (i = 0; i < 4; i++)
        {
            if(matrix[x+dx[i]][y+dy[i]] == 0) pathFinder(x + dx[i], y + dy[i], N, dest, stop, matrix, startCoordinates, endCoordinates, visited,finalMatrix,connected);
            else if (x+dx[i] == endCoordinates[dest][1] && y+dy[i] == endCoordinates[dest][2]) pathFinder(x + dx[i], y + dy[i], N, dest, stop, matrix, startCoordinates, endCoordinates, visited,finalMatrix,connected);
        }
        matrix[x][y] = 0;
    }
    return 0;
}
int isMatrixTrue(int x, int y, int N, int dest, int matrix[][MAX], int startCoordinates[][3], int endCoordinates[][3], int visited[][MAX], int connected[MAX])
{
    int i, j;
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {1, -1, 0, 0};
    if (x < 0 || x > N - 1)
        return -1;
    if (y < 0 || y > N - 1)
        return -1;
    if (visited[x][y] != 0 && visited[x][y] != dest)
        return -1;
    if (matrix[x][y] != dest)
        return -1;
    if (x == endCoordinates[dest][1] && y == endCoordinates[dest][2])
    {
        if (visited[x][y] == 0)
        {
            connected[dest] = 1;
            visited[x][y] = dest;
            return 0;
        }
    }
    if (matrix[x][y] == dest && visited[x][y] == 0)
    {
        visited[x][y] = dest;
        for (i = 0; i < 4; i++)
        {
            isMatrixTrue(x + dx[i], y + dy[i], N, dest, matrix, startCoordinates, endCoordinates, visited, connected);
        }
        visited[x][y] = 0;
    }
    return 0;
}

void autoMode(int x, int y, int N, int dest, int *stop, int matrix[][MAX], int startCoordinates[][3], int endCoordinates[][3], int visited[][MAX], int finalMatrix[][MAX], int connected[MAX])
{
    pathFinder(startCoordinates[1][1], startCoordinates[1][2], N, 1, stop, matrix, startCoordinates, endCoordinates, visited, finalMatrix, connected);
    system("cls");
    printf("\n # ORIGINAL MATRIX #\n\n");
    drawBoard(matrix, N);
    if(*stop == 1){
    printf("\n# FINAL MATRIX #\n\n");
    drawBoard(finalMatrix, N);
	printf("\nCongrats!!! You won the game!!!");	
	}
	else if (*stop == 0){
		printf("\n\nProgram could not find any solution.");
		
	}
   
}
