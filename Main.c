#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 10           //白子
#define BLACK 20           //黑子
#define TemWHITE 11        //白子上一步
#define TemBLACK 21        //黑子上一步


int Board[SIZE][SIZE];      //用于储存棋盘
int Pattern[4][9];       //储存四个方向的棋型，顺序：横、竖、\、/
int nFlag = 1;              //储存此时轮到谁的回合，黑子为1白子为2
int nMode;                  //游戏模式（1：人人对战；2：人机对战机器执黑；3：人机对战机器执白）
int nWinner;                //储存最后赢家,1黑2白

const int WIN5 = 0;//0->5连珠
const int ALIVE4 = 1;//1->活4
const int DIE4 = 2;//2->死4
const int LOWDIE4 = 3;//3->死4的低级版本
const int ALIVE3 = 4;//3->活3
const int TIAO3 = 5;//5->跳3
const int DIE3 = 6;//6->死3
const int ALIVE2 = 7;//7->活2
const int LOWALIVE2 = 8;//8->低级活2
const int DIE2 = 9;//9->死2
const int NOTHREAT = 10;//10->没有威胁

const int Levelone = 100000;//成五
const int Leveltwo = 10000;//成活4 或 双死4 或 死4活3
const int Levelthree = 5000;//双活3
const int Levelfour = 1000;//死3高级活3
const int Levelfive = 500;//死四
const int Levelsix = 400;//低级死四
const int Levelseven = 100;//单活3
const int Leveleight = 90;//跳活3
const int Levelnine = 50;//双活2
const int LevelTen = 10;//活2
const int Leveleleven = 9;//低级活2
const int Leveltwelve = 5;//死3
const int Levelthirteen = 2;//死2
const int Levelfourteen = 1;//没有威胁
const int Levelfiveteen = 0;//不能下

void InitBoard();           //初始化棋盘
void ShowBoard();           //显示棋盘
void SelectMode();          //选择游戏模式
int PlayGobang();           //五子棋内容

//将棋形读取进Pattern,x,y为目标点位,Index表征以何方视角计分（BLACK/WHITE）empty_flag用于表征是否假想如果在这里下棋会怎样，若是填1
void GetPattern(int x,int y,int Index,int empty_flag); 
//getpattern的辅助函数，会返回此时在pattern中应该为什么数字;BoardContent:棋盘上的内容，一般填Board[][],Index同上      
int PatternNumber(int BoardContent,int Index);        
//比较两个字符串是否能匹配，source是需比较的,target是目标,返回source中有几个target字段
int ComparePattern(int source[],int target[]);  
/**
 * @brief 给棋形打分，返回得分
 * @param index 表示是算我方得分还是算阻止敌方得分,我为1敌方为2
 **/
int AssessPattern(int index);
//对棋盘上空置的一点评估得出得分，返回分数
int AssessPoint(int x,int y);
//遍历棋盘返回分数最高的点
void GetPoint();
int BestPoint[2];   //储存getpoint找到的最好点
//如果该点未被占用返回1
int WhetherOccupied(int x,int y);
//胜负判断,参数为黑白 
int WhetherWin(int x,int y,int index);



int main()
{

    SelectMode();       //选择模式 
    InitBoard();        //初始化棋盘
    ShowBoard();        //打印棋盘

    nWinner = PlayGobang();
    
    
    
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
    getchar();

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

int PlayGobang()
{
    int tBlacky , tWhitey;                    //记录下棋坐标
    char tBlackx, tWhitex;
    int iFlag_FirstRound = 0;               //记录是否是第一回合（用于调整棋子是前一手还是通常显示） 

   if(nMode == 1){
        while(1){
            /* 黑子人下 */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //如果不是第一局，将前一步调整为通常显示
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            nFlag = 2;
            ShowBoard();
            if(WhetherWin(tBlackx,tBlacky,BLACK)){
                printf("黑色赢\n");
                break;
            }
            
            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            Board[tWhitex][tWhitey] = TemWHITE;
            nFlag = 1;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE)){
                printf("白色赢\n");
                break;
            }

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
    }   
   
   if(nMode == 2){
        nFlag = 2;
        while(1){
            /* 黑子机器下 */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //如果不是第一局，将前一步调整为通常显示
            }
            GetPoint();
            tBlackx = BestPoint[0];
            tBlacky = BestPoint[1];
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            if(WhetherWin(tBlackx,tBlacky,BLACK)){
                printf("黑色赢\n");
                break;
            }
            
     

            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE)){
                printf("白色赢\n");
                break;
            }

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
   }
     

    if(nMode == 3){
        nFlag = 1;
        while(1){
            /* 黑子人下 */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //如果不是第一局，将前一步调整为通常显示
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            if(WhetherWin(tBlackx,tBlacky,BLACK)){
                printf("黑色赢\n");
                break;
            }
     

            /*白子机器下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            GetPoint();
            tWhitex = BestPoint[0];
            tWhitey = BestPoint[1];
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE)){
                printf("白色赢\n");
                break;
            }

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
   }

    return 0;
}

void GetPattern(int x,int y,int Index,int empty_flag)
{

    int TemMemory;  //储存被修改的格子原始状态
    if(empty_flag == 1){
        TemMemory = Board[x][y];
        Board[x][y] = Index;
    }//如果要假想下棋就先储存好原始状态再调整棋盘

    for(int i = 0; i < 4; i++){                 //初始化
        for(int j = 0; j < 9; j++)
            Pattern[i][j] = 2;
    }

    
    for(int i = 1; i <= 4 && (x-i) >= 0; i++){  //横
        Pattern[0][4-i] = PatternNumber(Board[x-i][y],Index);
    }
    for(int i = 1; i <= 4 && (x+i) <= 14; i++){
        Pattern[0][4+i] = PatternNumber(Board[x+i][y],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0; i++){  //竖
        Pattern[1][4-i] = PatternNumber(Board[x][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14; i++){
        Pattern[1][4+i] = PatternNumber(Board[x][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0 && (x-i) >= 0; i++){  //左斜
        Pattern[2][4-i] = PatternNumber(Board[x-i][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x+i) <= 14; i++){
        Pattern[2][4+i] = PatternNumber(Board[x+i][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x-i) >= 0; i++){  //右斜
        Pattern[3][4-i] = PatternNumber(Board[x-i][y+i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) >= 0 && (x-i) <=14; i++){
        Pattern[3][4+i] = PatternNumber(Board[x+i][y-i],Index);
    }   
    
    if(empty_flag == 1){
        Board[x][y] = TemMemory;
    }
    //恢复至原始状态
    }

//0为空 1为我方 2为敌方或墙
int PatternNumber(int a,int Index)
{
    if(a == WHITE || a == TemWHITE){
        if(Index == WHITE) return 1;
        else return 2;
    }
    else if(a == BLACK || a == TemBLACK){
        if(Index == BLACK) return 1;
        else return 2;
    }
    else return 0;
}

int ComparePattern(int source[],int target[])
{
    int count = 0;
    
    for(int i = 0; i < 14; i++){         //target的第一位与source的哪一位匹配
        for(int j = 0; source[i+j] == target[j] && ((i+j) <= 14) && target[j] != 99; j++)
            if(target[j+1] == 99){
                count++;
                break;
            }       //99表示棋形代码中止
    }
    
    return count;
}

int AssessPattern(int index)
{
    int count = 1;
}

int AssessPoint(int x,int y)
{
    int Before_we, After_we, Before_they, After_they; //我方敌方在此点下棋之前之后得分
    int score;

    int we, they;
    if(nMode == 2){
        we = BLACK;
        they = WHITE;
    }
    else if(nMode == 3){
        we = WHITE;
        they = BLACK;
    }                           //确定敌我颜色

    GetPattern(x,y,we,0);
    Before_we = AssessPattern(1);
    GetPattern(x,y,we,1);
    After_we = AssessPattern(1);
    GetPattern(x,y,they,0);
    Before_they = AssessPattern(2);
    GetPattern(x,y,they,1);
    After_they = AssessPattern(2);      //算分

    score = After_we - Before_we + After_they - Before_they;
    printf("%d",score);


    return score;
}

void GetPoint()
{
    int HiScore = 0,score;
    if(WhetherOccupied(7,7)){
    HiScore = AssessPoint(7,7);
    BestPoint[0] = 7;
    BestPoint[1] = 7;
    }
    int p,q;


    for(p = 0; p < 8; p++){
        for(q = 0; WhetherOccupied(7-p+q,7+p) && q < 2*p; q++){
            score = AssessPoint(7-p+q,7+p);
            if(score > HiScore && WhetherOccupied(7-p+q,7+p)){
                HiScore = score;
                BestPoint[0] = 7-p+q;
                BestPoint[1] = 7+p;
            }
        }       //上横

        for(q = 0; WhetherOccupied(7+p-q,7-p) && q < 2*p; q++){
            score = AssessPoint(7+p-q,7-p);
            if(score > HiScore && WhetherOccupied(7+p-q,7-p)){
                HiScore = score;
                BestPoint[0] = 7+p-q;
                BestPoint[1] = 7-p;
            }
        }       //下横
    }
   
        for(q = 0; WhetherOccupied(7+p,7+p-q) && q < 2*p; q++){
            score = AssessPoint(7+p,7+p-q);
            if(score > HiScore && WhetherOccupied(7+p,7+p-q)){
                HiScore = score;
                BestPoint[0] = 7+p;
                BestPoint[1] = 7+p-q;
            }
        }       //右竖

        for(q = 0; WhetherOccupied(7-p,7-p+q) && q < 2*p; q++){
            score = AssessPoint(7-p,7-p+q);
            if(score > HiScore && WhetherOccupied(7-p,7-p+q)){
                HiScore = score;
                BestPoint[0] = 7-p;
                BestPoint[1] = 7-p+q;
            }
        }       //左竖


}

int WhetherOccupied(int x,int y)
{
    if(Board[x][y] == WHITE || Board[x][y] == TemWHITE || Board[x][y] == BLACK || Board[x][y] == TemBLACK){
        return 0;
    }
    else{
        return 1;
    }
}

int WhetherWin(int x,int y,int index)
{
    int WinPattern[6] = {1,1,1,1,1,99};
    
    if(index == BLACK){
        GetPattern(x,y,BLACK,0);
        for(int i = 0; i < 4; i++){
            if(ComparePattern(Pattern[i],WinPattern)){
                return 1;
            }
        }
    }

    if(index == WHITE){
        GetPattern(x,y,WHITE,0);
        for(int i = 0; i < 4; i++){
            if(ComparePattern(Pattern[i],WinPattern)){
                return 1;
            }
        }
    }

    return 0;
}
