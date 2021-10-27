#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 10           //白子
#define BLACK 20           //黑子
#define TemWHITE 11        //白子上一步
#define TemBLACK 21        //黑子上一步

int Board[SIZE][SIZE];      //用于储存棋盘
int Pattern[4][SIZE];       //储存四个方向的棋型，顺序：横、竖、\、/
int nFlag = 1;              //储存此时轮到谁的回合，黑子为1白子为2
int nMode;                  //游戏模式（1：人人对战；2：人机对战机器执黑；3：人机对战机器执白）
int nWinner;                //储存最后赢家,1黑2白

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
//给棋形打分，返回得分,index表示是算我方得分还是算阻止敌方得分,我为1敌方为2
int AssessPattern(int index);
//对棋盘上空置的一点评估得出得分，返回分数
int AssessPoint(int x,int y);
//遍历棋盘返回分数最高的点
void GetPoint();
int BestPoint[2];   //储存getpoint找到的最好点

int main()
{
    /*保留用于测试comparepattern的部分，后续优化*/
    // int testsource[15] = {1,2,1,2,1,6,7,8,9,10,11,12,13,14,15};
    // int testtarget[] = {1,2,1,99};
    // printf("%d",ComparePattern(testsource, testtarget));
    // system("pause");

    SelectMode();       //选择模式 
    InitBoard();        //初始化棋盘
    ShowBoard();        //打印棋盘

    nWinner = PlayGobang();
    
    return 0;
}

void SelectMode()
{
    int temMode,temFlag;                //临时储存模式与先下方
    ReSelect:                           //当玩家输入意料之外的数字时用goto返回这里重新开始选择
    system("cls");
    printf("请选择模式，人机对战输入1，人人对战输入2：");
    scanf("%d",&temMode);
    getchar();
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
            nFlag = 1;
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            
            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            nFlag = 2;
            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
    }   
   
   if(nMode == 2){
        while(1){
            /* 黑子机器下 */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //如果不是第一局，将前一步调整为通常显示
            }
            nFlag = 1;
            GetPoint();
            tBlackx = BestPoint[0];
            tBlacky = BestPoint[1];
            if(iFlag_FirstRound == 0){
                tBlackx = 7;
                tBlacky = 7;
            }
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            
            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            nFlag = 2;
            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();

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
        Board[x][y] == Index;
    }//如果要假想下棋就先储存好原始状态再调整棋盘

    for(int i = 0; i < 4; i++){                 //初始化
        for(int j = 0; j < 15; j++)
            Pattern[i][j] = 3;
    }

    for(int i = 0;i < 15; i++){
        Pattern[0][i] = PatternNumber(Board[i][y],Index);       //横
    }
    for(int i = 0;i < 15; i++){
        Pattern[1][i] = PatternNumber(Board[x][i],Index);       //竖
    }

    int x0 = x + y - 14;   //右斜线左上起点的x坐标
    if(x0 >= 0){
        for(int i = 0; i < 15 - x0; i++){
            Pattern[2][i] = PatternNumber(Board[i+x0][14-i],Index);
        }
    }
    else if(x0 < 0){
        x0 = -x0;
        for(int i = 0; i < 15 - x0; i++){
            Pattern[2][i] = PatternNumber(Board[i][14-x0-i],Index);
        }
    }

    int y0 = x - y;
    if(y0 >= 0){
        for(int i = 0; i < 15 - y0; i++){
            Pattern[3][i] = PatternNumber(Board[y0+i][i],Index);
        }
    }
    else if(y0 < 0){
        y0 = -y0;
        for(int i = 0; i < 15 - y0; i++){
            Pattern[3][i] = PatternNumber(Board[i][y0+i],Index);
        }
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
        for(int j = 0; source[i+j] == target[j] && ((i+j) <= 14) && target[j] != '\0'; j++)
            if(target[j+1] == 99){
                count++;
                break;
            }       //99表示棋形代码中止
    }
    
    return count;
}

int AssessPattern(int index)
{
    int p;
    int score = 0;
    if(index == 1) p = 0;
    else if(index == 2) p = 1;
    else printf("error in assesspattern\n");        //选择使用评分表的哪一行
    
    int TargetPatterns[10][7] = {{1,1,1,1,1,99},{0,1,1,1,1,0,99},{0,1,1,1,1,2,99},{2,1,1,1,1,0,99},{0,1,1,1,0,99},{0,1,1,1,2,99},{2,1,1,1,0,99},{0,1,1,0,99}
    ,{0,1,2,99},{2,1,0,99}};
    int Scores[10][2] = {{10000,1000},{200,100},{50,20},{50,20},{30,10},{8,5},{8,5},{2,1},{-1,0},{-1,0}};
    //棋形与分数对照表，目前还很简陋!

    for(int i = 0; i < 4; i++ ){
        for(int j = 0; j < 8; j++){
            score += Scores[j][p] * ComparePattern(Pattern[i],TargetPatterns[j]);
        }
    }
    
    return score;
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
    for(int x = 0; x<15; x++){
        for(int y = 0; y<15 && Board[x][y] != WHITE && Board[x][y] != TemWHITE && Board[x][y] != BLACK && Board[x][y] != TemBLACK; y++){
            score = AssessPoint(x,y);
            if(score > HiScore){
                HiScore = score;
                BestPoint[0] = x;
                BestPoint[1] = y;
            }
        }
    }
}
