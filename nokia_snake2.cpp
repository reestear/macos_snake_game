
#include <cstdlib>
#include <ncurses.h>
using namespace std;

bool gameOver;
int width = 0, height = 0; // max size will be 100 by 100
int x, y, fruitX, fruitY, score, level = 1, wall = 1;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

int tailX[10000], tailY[10000]; // size of the tail will be similar as the size of the score, so score should be updated first and only after that tail
int nTail = 0;

void setUp(){
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = width/2;
    y = height/2;
    fruitX = rand() % width + 1;
    fruitY = rand() % height + 1;
    score = 0;
}

void print(){
    clear();

    for(int i = 0; i < height + 2; i++){
        for(int j = 0; j < width + 2; j++){
            if(i == 0 || i == height + 1) mvprintw(i, j, "*");
            else if(j == 0 || j == width + 1) mvprintw(i, j, "*");
            else if(i == y && x == j) mvprintw(i, j, "O");
            else if(i == fruitY && j == fruitX) mvprintw(i, j, "@");
            else{
                for(int t = 0; t < score; t++){
                    if(tailX[t] == j && tailY[t] == i) mvprintw(i, j, "o");
                }
            }
        }
    }

    mvprintw(height + 3, 0, "Score: %i", score);
    if(level == 1) mvprintw(height + 5, 0, "Level: HARD");
    else if(level == 2) mvprintw(height + 5, 0, "Level: MIDDLE");
    else if(level == 3) mvprintw(height + 5, 0, "Level: EASY");
    refresh();
}

void input(){
    keypad(stdscr, TRUE);
    halfdelay(level);

    int c = getch();

    switch (c)
    {
    case 'A' | 'a':
        if(dir != RIGHT && dir != LEFT) dir = LEFT;
        break;
    case 'D' | 'd':
        if(dir != LEFT && dir != RIGHT) dir = RIGHT;
        break;
    case 'W' | 'w':
        if(dir != DOWN && dir != UP) dir = UP;
        break;
    case 'S' | 's':
        if(dir != UP && dir != DOWN) dir = DOWN;
        break;
    case 'Q' | 'q':
        gameOver = true;
        break;
    }
}

void operations(){

    int prevx = x;
    int prevy = y;
    for(int i = 0; i < nTail; i++){
        int prev2x = tailX[i];
        int prev2y = tailY[i];
        tailX[i] = prevx;
        tailY[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case DOWN:
        y++;
        break;
    case UP:
        y--;
        break;
    default:
        break;
    }

    if(wall) {
        if(x > width || x < 1 || y < 1 || y > height) gameOver = true;
    }
    else{
        if(x < 1) x = width;
        else if(x > width) x = 1;
        else if(y < 1) y = height;
        else if(y > height) y = 1;
    }

    if(x == fruitX && y == fruitY){
        score++;
        fruitX = rand() % width + 1;
        fruitY = rand() % height + 1;
        nTail++;
    }

    for(int i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y) gameOver = true;
    }

}

int main(){

    while(height < 1 || height > 50 || width < 1 || width > 50){
        printf("Enter the height and width of the field [height and width seperated by space]: ");
        scanf("%i %i", &height, &width);
    }
    getch();
    level = 0;
    while(level < 1 || level > 3){
        printf("Choose the level [1 - hard, 2 - middle, 3 - easy]: ");
        scanf(" %i", &level);
    }
    wall = -1;
    while(wall == -1){
        printf("Choose the field [1 - with walls, 0 - open field]: ");
        scanf(" %i", &wall);
    }


    setUp();

    while(!gameOver){
        print();
        input();
        operations();
    }
    getch();
    endwin();

    return 0;
}