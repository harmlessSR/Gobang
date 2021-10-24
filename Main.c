#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 11            //白子
#define BLACK 12            //黑子
#define TemWHITE 21         //白子上一步
#define TemBLACK 22         //黑子上一步

int Board[SIZE][SIZE];      //用于储存棋盘
int nFlag = 1;              //储存此时轮到谁的回合，黑子为1白子为2
int nMode;              //游戏模式（1：人人对战；2：人机对战机器执黑；3：人机对战机器执白）

void InitBoard();           //初始化棋盘
void ShowBoard();           //显示棋盘
void SelectMode();          //选择游戏模式

int main()
{
    SelectMode();       //选择模式      
    InitBoard();        //初始化棋盘
    ShowBoard();        //打印棋盘
    
   
    system("pause");
    return 0;
}

void SelectMode()
{
    int temMode,temFlag;                //临时储存模式与先下方
    ReSelect:                           //当玩家输入意料之外的数字时用goto返回这里重新开始选择
    system("cls");
    printf("请选择模式，人机对战输入1，人人对战输入2：");
    scanf("%d",&temMode);
    if(temMode == 1){
        nMode = 1;
        printf("\n请选择电脑执黑执白，黑输入1白输入2：");
        scanf("%d",&temFlag);
        if(temFlag == 1){
            nMode = 2;
        }
        else if(temFlag == 2){
            nMode = 3;
        }
        else{
            goto ReSelect;
        }
    }
    else if(temMode == 2){
        nMode = 1;
    }
    else{
        goto ReSelect;
    }

}

void InitBoard()            //初始化棋盘
{
    int i,j;
    
    Board[0][0] = 1;
    Board[0][SIZE - 1] = 2;
    Board[SIZE - 1][0] = 3;
    Board[SIZE - 1][SIZE - 1] = 4;

    for(i = 0, j = 1; j < SIZE - 1; j++)
        Board[i][j] = 5;
    for(j = 0, i = 1; i < SIZE - 1; i++)
        Board[i][j] = 6;
    for(i = SIZE - 1, j = SIZE - 2; j > 0; j--)
        Board[i][j] = 7;
    for(j = SIZE - 1, i = SIZE - 2; i > 0; i--)
        Board[i][j] = 8;

    for(i = 1; i < SIZE - 1; i++)
        for(j = 1; j < SIZE - 1; j++)
            Board[i][j] = 9;
    
}

void ShowBoard()            //打印棋盘
{
    system("cls");          //清屏
    
    int i, j;
    int line = 15;       //生成列数的数字标号
    char ary = 'A';      //生成行数的字母标号
    
    /*生成标题行，说明模式与此时下棋方*/
    if(nMode == 1)  printf("             人人对战\n");
    else            printf("             人机对战\n");
    if(nFlag == 1)  printf("               黑下\n");
    else            printf("               白下\n");

    /*打印棋盘*/
    printf("\n");
    for(j = SIZE - 1; j >= 0; j--){        //按行从上往下逐行打印
        printf("%2d",line);
        line--;
        for(i = 0; i < SIZE; i++){
            switch(Board[i][j]){
                case 1:
                    printf("┗");
                    break;
                case 2:
                    printf("┏");
                    break;
                case 3:
                    printf("┛");
                    break;
                case 4:
                    printf("┓");
                    break;
                case 5:
                    printf("┠");
                    break;
                case 6:
                    printf("┷");
                    break;
                case 7:
                    printf("┨");
                    break;
                case 8:
                    printf("┯");
                    break;
                case 9:
                    printf("┼");
                    break;
                case BLACK:
                    printf("●");        //黑子通常
                    break;
                case WHITE:
                    printf("○");        //白子通常
                    break;
                case TemBLACK:
                    printf("▲");        //黑子前一子
                    break;
                case TemWHITE:
                    printf("△");        //白子前一子
                    break;
            }
        }
        printf("\n");
    }
    for(printf("   "); ary <= 'O'; ary++) //输出字母标号
        printf("%c ",ary);
    printf("\n");
}