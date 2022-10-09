#include<stdio.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>

using namespace std;

#define Height 20
#define Width 40
int food_x, food_y;
enum Direction{up, down, left, right};
Direction snakedirection;
int score = 0;
int Map[Height][Width]{0};

void HideCursor()//隐藏光标
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = FALSE;
    cursor.dwSize = sizeof(handle);
    SetConsoleCursorInfo(handle, &cursor);
}

void Gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

void Init()
{
    for(int i = 0; i < Width; i++)
    {
        Map[0][i] = -1;
        Map[Height - 1][i] = -1;
    }
    for(int i = 0; i < Height; i++)
    {
        Map[i][0] = -1;
        Map[i][Width - 1] = -1;
    }
    Map[Height / 2][Width / 2] = 1;
    do
    {
        food_x = rand() % (Width - 5) + 2;
        food_y = rand() % (Height - 5) + 2;
    } while (Map[food_y][food_x] != 0);
    Map[food_y][food_x] = -2;
    snakedirection = Direction::right;
}

void show()
{
    Gotoxy(0, 0);
    for(int r = 0; r < Height; r++)
    {
        for(int c = 0; c < Width; c++)
        {
            if(Map[r][c] == -1) printf("#");
            else if(Map[r][c] == 0) printf(" ");
            else if(Map[r][c] == -2) printf("F");
            else if(Map[r][c] == 1) printf("@");
            else if(Map[r][c] > 1) printf("*");
        }
        printf("\n");
    }
    printf("当前分数为:%d\n", score);
    printf("按下空格键暂停.\n");
    Sleep(100);
}

void MoveSnakeByDirection()
{
    COORD head_xy;
    COORD tail_xy;
    int signmax = 0;
    for(int r = 0; r < Height; r++)
    {
        for(int c = 0; c < Width; c++)
        {
            if(Map[r][c] > 0)
            {
                if(Map[r][c] == 1)
                {
                    head_xy.X = c;
                    head_xy.Y = r;
                }
                if(Map[r][c] > signmax)
                {
                    signmax = Map[r][c];
                    tail_xy.X = c;
                    tail_xy.Y = r;
                }
                Map[r][c]++;
            }
        }
    }
    if(snakedirection == Direction::left) head_xy.X--;
    else if(snakedirection == Direction::right) head_xy.X++;
    else if(snakedirection == Direction::up) head_xy.Y--;
    else if(snakedirection == Direction::down) head_xy.Y++;
    if(Map[head_xy.Y][head_xy.X] == -1 || Map[head_xy.Y][head_xy.X] > 0)
    {
        system("cls");
        printf("game over!");
        exit(1);
    }
    else if(Map[head_xy.Y][head_xy.X] == -2)
    {
        score++;
        Map[head_xy.Y][head_xy.X] = 1;
        do
        {
            food_x = rand() % (Width - 5) + 2;
            food_y = rand() % (Height - 5) + 2;
        } while (Map[food_y][food_x] != 0);
        Map[food_y][food_x] = -2;
    }
    else
    {
        Map[head_xy.Y][head_xy.X] = 1;
        Map[tail_xy.Y][tail_xy.X] = 0;
    }
    if(score == (Height - 2) * (Width - 2) - 1)
    {
        system("cls");
        printf("You are the winner!");
        exit(1);
    }
}

void UpdateWithoutInput()
{
    MoveSnakeByDirection();
}
void UpdateWithInput()
{
    char input;
    if(kbhit())
    {
        input = getch();
        if(input == 'a' && snakedirection != Direction::right)
        {
            snakedirection = Direction::left;
            MoveSnakeByDirection();
        }
        else if(input == 'd' && snakedirection != Direction::left)
        {
            snakedirection = Direction::right;
            MoveSnakeByDirection();
        }
        else if(input == 'w' && snakedirection != Direction::down)
        {
            snakedirection = Direction::up;
            MoveSnakeByDirection();
        }
        else if(input == 's' && snakedirection != Direction::up)
        {
            snakedirection = Direction::down;
            MoveSnakeByDirection();
        }
        else if(input == ' ')
        {
            while(true)
            {
                input = getchar();
                if(input == ' ') break;
            }
        }
    }
}

int main()
{
    HideCursor();
    Init();
    while(true)
    {
        show();
        UpdateWithoutInput();
        UpdateWithInput();
    }
    return 0;
}

