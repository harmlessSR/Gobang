#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 10           //白子
#define BLACK 20           //黑子
#define TemWHITE 11        //白子上一步
#define TemBLACK 21        //黑子上一步
#define FORBIDDEN 25       //禁手
#define LONGFORBIDDEN 26   //长连禁手

int scoreboard[SIZE][SIZE];
int Board[SIZE][SIZE];              //用于储存棋盘（真实棋子）
int Pattern[4][9];                  //储存四个方向的棋型，顺序：横、竖、\、/
int ForbiddenPattern[4][9];         //判断禁手用
int nFlag = 1;                      //储存此时轮到谁的回合，黑子为1白子为2
int nMode;                          //游戏模式（1：人人对战；2：人机对战机器执黑；3：人机对战机器执白）
int nWinner;                        //储存最后赢家
int pcflag,hmflag;                  //电脑执哪一方

struct Situation {//当前位置的形式，打分根据这个来打
	int win5;//5连珠
	int alive4;//活4
	int die4;//死4
	int lowdie4;//死4低级版本
	int alive3;//活3
	int tiao3;//跳3
	int die3;//死3
	int alive2;//活2
	int lowalive2;//低级活2
	int die2;//死2
	int nothreat;//没有威胁
};

/*评估一行棋形得出的旗形内容*/
#define WIN5 0//5连珠
#define ALIVE4 1//活4
#define DIE4 2//死4
#define LOWDIE4 3//眠4
#define ALIVE3 4//活3
#define TIAO3 5//跳活3
#define DIE3 6//死3
#define ALIVE2 7//活2
#define LOWALIVE2 8//低级活2
#define DIE2 9//死2
#define NOTHREAT 10//无用

/*一点上各旗形组合的打分表*/
#define Level1 100000//成五
#define Level2 10000//成活4 或 双死4 或 死4活3
#define Level3 5000//双活3
#define Level4 1000//死3高级活3
#define Level5 500//死四
#define Level6 400//低级死四
#define Level7 100//单活3
#define Level8 90//跳活3
#define Level9 50//双活2
#define Level10 10//活2
#define Level11 9//低级活2
#define Level12 5//死3
#define Level13 2//死2
#define Level14 1//没有威胁
#define Level15 0//不能下

#define mycolor 1//我的颜色
#define empty 0//空
#define hiscolor 2//敌方或墙

void InitBoard();           //初始化棋盘
void ShowBoard();           //显示棋盘
void SelectMode();          //选择游戏模式
int PlayGobang();           //五子棋内容

//将棋形读取进Pattern,x,y为目标点位,Index表征以何方视角计分（BLACK/WHITE）
void GetPattern(int x,int y,int Index, int pattern[4][9]); 
//getpattern的辅助函数，会返回此时在pattern中应该为什么数字;BoardContent:棋盘上的内容，一般填Board[][],Index同上      
int PatternNumber(int BoardContent,int Index);        
//给旗形的一个方向返回是哪一种情况，linenumber为pattern中的行数（0-3）
int AssessPatternLine(int linenumber, int index, int pattern[4][9]);
//对棋盘上空置的一点评估得出得分，返回分数,当然也可用于有棋子的点评估分数
int AssessPoint(int x,int y,int flag);

//一步贪心算法，寻找好点
void GetPoint();
int BestPoint[2];   //储存getpoint找到的最好点

//如果该点未被占用返回1
int WhetherOccupied(int x,int y);
//胜负判断,参数为黑白,赢返回1，否则返回0，黑棋触发长连禁手返回-1
int WhetherWin(int x,int y,int index);


/*禁手判断部分，由于之前的读取棋形等函数都将pattern直接写在函数里了，修改不便，被迫重新写一组禁手判断专用的函数*/

//功能同getpattern
void GetForbiddenPattern(int x,int y,int Index);
//功能同assesspattern
int AssessForbiddenPatternLine(int linenumber, int flag);
//判断一个空格（若下了黑棋）是否是禁手，由于仅限黑棋有禁手故参数仅有x,y，默认指黑棋。若触发双三双四禁手返回FORBIDDEN，触发长连禁手返回LONGFORBIDDEN,否则返回1.
int WhetherForbidden(int x,int y);

int main()
{
    SelectMode();       //选择模式 
    InitBoard();        //初始化棋盘
    ShowBoard();        //打印棋盘

    nWinner = PlayGobang();     //棋局程序，返回赢家或禁手情况

    printf("\n\n");
    if(nWinner == BLACK)
        printf("黑棋胜！！！\n");
    else if(nWinner == WHITE)
        printf("白棋胜！！！\n");
    else if(nWinner == FORBIDDEN)
        printf("黑棋下出禁手，白棋胜\n");
    

    
    
    
    system("pause");
    return 0;
}

void SelectMode()
{
    int temMode,temFlag;                //临时储存模式与先下方
    ReSelect:                           //当玩家输入意料之外的数字时用goto返回这里重新开始选择
    system("cls");
    printf("请选择模式，人机对战输入1，人人对战输入2:");
    scanf("%d",&temMode);
    if(temMode == 1){
        nMode = 1;
        printf("\n请选择电脑执黑执白，黑输入1白输入2:");
        scanf("%d",&temFlag);
        if(temFlag == 1){
            nMode = 2;      
            pcflag = BLACK;
            hmflag = WHITE;
        }
        else if(temFlag == 2){
            nMode = 3;
            pcflag = WHITE;
            hmflag = BLACK;
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
    int tBlacky, tWhitey;                    //记录下棋坐标
    char tBlackx, tWhitex;
    int iFlag_FirstRound = 0;               //记录是否是第一回合（用于调整棋子是前一手还是通常显示）
    int forbidden;                          //禁手判断 

   if(nMode == 1){      //人人对战模式
        while(1){
            /* 黑子人下 */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //如果不是第一局，将前一步调整为通常显示
            }
            while(0){
            BLACKrego0:
            printf("落子于占用点或非法落子，清重试：");   //非法落子时跳回这里继续
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            if(WhetherOccupied(tBlackx,tBlacky) == 0 || tBlackx < 0 || tBlackx > 14 || tBlacky < 0 || tBlacky > 14)      //若落子非法或被占用，回到落子之前
                goto BLACKrego0;
            Board[tBlackx][tBlacky] = TemBLACK;
            nFlag = 2;
            ShowBoard();
            if((forbidden = WhetherForbidden(tBlackx,tBlacky)) == LONGFORBIDDEN)    //黑棋每一步结束之后，判断是否赢了或者下出禁手，先考虑长连，再看赢没赢，再看是否双三双四
                return FORBIDDEN;
            else if(WhetherWin(tBlackx,tBlacky,BLACK))       
                return BLACK;                           //把赢家返回到main函数
            else if(forbidden == FORBIDDEN)
                return FORBIDDEN;                       //返回禁手到main函数打印结果。
            

            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            while(0){
            WHITErego0:
            printf("落子于占用点或非法落子，清重试：");   //非法落子时跳回这里继续
            }
            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            if(WhetherOccupied(tWhitex,tWhitey) == 0 || tWhitex < 0 || tWhitex > 14 || tWhitey < 0 || tWhitey > 14)      //若落子非法或被占用，回到落子之前
                goto WHITErego0;
            Board[tWhitex][tWhitey] = TemWHITE;
            nFlag = 1;
            ShowBoard();

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
    }   
   
    if(nMode == 2){      //人机对战，电脑执黑
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
            printf("电脑落子于%c%d\n",tBlackx + 'a', tBlacky + 1);  //将落子点显示出来
            if((forbidden = WhetherForbidden(tBlackx,tBlacky)) == LONGFORBIDDEN)    //黑棋每一步结束之后，判断是否赢了或者下出禁手，先考虑长连，再看赢没赢，再看是否双三双四
                return FORBIDDEN;
            else if(WhetherWin(tBlackx,tBlacky,BLACK))       
                return BLACK;                           //把赢家返回到main函数
            else if(forbidden == FORBIDDEN)
                return FORBIDDEN;                       //返回禁手到main函数打印结果。
            
            
            

            /*白子人下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }

            while(0){
            WHITErego:
            printf("落子于占用点或非法落子，清重试：");   //非法落子时跳回这里继续
            }

            scanf("%c%d",&tWhitex,&tWhitey);
            getchar();
            tWhitex = tWhitex - 'a';
            tWhitey = tWhitey - 1;
            if(WhetherOccupied(tWhitex,tWhitey) == 0 || tWhitex < 0 || tWhitex > 14 || tWhitey < 0 || tWhitey > 14)      //若落子非法或被占用，回到落子之前
                goto WHITErego;             //这里肯定有不用goto就能做到的办法，但既然可以这么写就这样了，也比较简洁。
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE))      //判定有没有赢
                return WHITE;

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
            while(0){
            BLACKrego:
            printf("落子于占用点或非法落子，清重试：");   //非法落子时跳回这里继续
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            if(WhetherOccupied(tBlackx,tBlacky) == 0 || tBlackx < 0 || tBlackx > 14 || tBlacky < 0 || tBlacky > 14)      //若落子非法或被占用，回到落子之前
                goto BLACKrego;     
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            if((forbidden = WhetherForbidden(tBlackx,tBlacky)) == LONGFORBIDDEN)    //黑棋每一步结束之后，判断是否赢了或者下出禁手，先考虑长连，再看赢没赢，再看是否双三双四
                return FORBIDDEN;
            else if(WhetherWin(tBlackx,tBlacky,BLACK))       
                return BLACK;                           //把赢家返回到main函数
            else if(forbidden == FORBIDDEN)
                return FORBIDDEN;                       //返回禁手到main函数打印结果。
            
     

            /*白子机器下*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            GetPoint();
            tWhitex = BestPoint[0];
            tWhitey = BestPoint[1];
            Board[tWhitex][tWhitey] = TemWHITE;
            printf("电脑落子于%c%d\n",tWhitex + 'a', tWhitey + 1);
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE))
                return WHITE;
            

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //调整不再是第一局
        }
   }

    return 0;
}

void GetPattern(int x,int y,int Index, int pattern[4][9])
{

    for(int i = 0; i < 4; i++){                 //初始化
        for(int j = 0; j < 9; j++)
            pattern[i][j] = 2;
    }
    for(int i = 0; i < 4; i++)
        pattern[i][4] = 1;
    
    for(int i = 1; i <= 4 && (x-i) >= 0; i++){  //横
        pattern[0][4-i] = PatternNumber(Board[x-i][y],Index);
    }
    for(int i = 1; i <= 4 && (x+i) <= 14; i++){
        pattern[0][4+i] = PatternNumber(Board[x+i][y],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0; i++){  //竖
        pattern[1][4-i] = PatternNumber(Board[x][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14; i++){
        pattern[1][4+i] = PatternNumber(Board[x][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0 && (x-i) >= 0; i++){  //左斜
        pattern[2][4-i] = PatternNumber(Board[x-i][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x+i) <= 14; i++){
        pattern[2][4+i] = PatternNumber(Board[x+i][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x-i) >= 0; i++){  //右斜
        pattern[3][4-i] = PatternNumber(Board[x-i][y+i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) >= 0 && (x-i) <=14; i++){
        pattern[3][4+i] = PatternNumber(Board[x+i][y-i],Index);
    }       
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

int AssessPatternLine(int linenumber, int flag, int pattern[4][9])
{
    int count = 1;
    int i;
    int left,right;
    int leftpos,rightpos;
   
        for(i = 1; pattern[linenumber][4-i] == 1; i++){
            count++;
        }
        left = pattern[linenumber][4-i];
        leftpos = 4-i;
        for(i = 1; pattern[linenumber][4+i] == 1; i++){
            count++;
        }
        right = pattern[linenumber][4+i];
        rightpos = 4+i;

        if(count > 5)
            if(flag == BLACK)
                return LONGFORBIDDEN;       //长连了
            else if(flag == WHITE)
                return WIN5;
        else if(count == 5)
            return WIN5;
        else if(count == 4){
            if(left == empty && right == empty) return ALIVE4;
            else if(left == hiscolor && right == hiscolor) return NOTHREAT;
            else    return DIE4;
        }
        
        else if(count == 3){
            int left2 = pattern[linenumber][leftpos-1];
            int right2 = pattern[linenumber][rightpos + 1];
            if(left == empty && right == empty){
                if(left2 == hiscolor && right2 == hiscolor)
                return DIE3;
                else if(left2 == mycolor || right2 == mycolor)
                return LOWDIE4;
                else if(left2 == empty || right2 == empty)
                return ALIVE3;
            }
            else if(left == hiscolor && right == hiscolor)  return NOTHREAT;
            else if(left == hiscolor){
                if(right2 == empty)    return DIE3;
                else if(right2 == mycolor) return LOWDIE4;
            }
            else if(right == hiscolor){
                if(left2 == empty)    return DIE3;
                else if(left2 == mycolor) return LOWDIE4;
            }
        }
        else if(count == 2){
            int left1 = pattern[linenumber][leftpos-1];
            int right1 = pattern[linenumber][rightpos + 1];
            int left2 = pattern[linenumber][leftpos-2];
            int right2 = pattern[linenumber][rightpos + 2];

            if(left == empty && right == empty)
            {
                if((left1 == empty && left2 == mycolor) || (right1 == empty && right2 == mycolor))  return DIE3;
                else if(left1 == empty && right1  == empty) return ALIVE2;
                if((right1 == mycolor && right2 == mycolor)||(left1 == mycolor && left2 == mycolor))    return LOWDIE4;
                if((right1 == mycolor && right2 == hiscolor) || (left1 == mycolor && left2 == hiscolor))    return DIE3;
                if((right1 == mycolor && right2 == empty) || (left1 == mycolor && left2 == empty)) return TIAO3;
            }
            else if(left == hiscolor && right == hiscolor)  return NOTHREAT;
            else if(left == empty || right == empty)
            {
                if(left == hiscolor){
                    if(right1 == hiscolor || right2 == hiscolor)    return NOTHREAT;
                    else if(right1 == empty && right2 == empty)     return DIE2;
                    else if(right1 == mycolor && right2 == mycolor) return DIE4;
                    else if(right1 == mycolor || right2 == mycolor) return DIE3;
                }
                else if(right == hiscolor){
                    if(left1 == hiscolor || left2 == hiscolor)  return NOTHREAT;
                    else if(left1 == empty && left2 == empty)   return DIE2;
                    else if(left1 == mycolor && left2 == mycolor)   return DIE4;
                    else if(left1 == mycolor || left2 == mycolor)   return DIE3;
                }
            }
        }
        else if(count == 1){
            int left1 = pattern[linenumber][leftpos-1];
            int right1 = pattern[linenumber][rightpos + 1];
            int left2 = pattern[linenumber][leftpos-2];
            int right2 = pattern[linenumber][rightpos + 2];
            int left3 = pattern[linenumber][leftpos-3];
            int right3 = pattern[linenumber][rightpos + 3];

            if(left == empty && left1 == mycolor && left2 == mycolor && left3 == mycolor && right == empty && right1 == mycolor && right2 == mycolor && right3 == mycolor)  return ALIVE4;
            else if(left == empty && left1 == mycolor && left2 == mycolor && left3 == mycolor)   return LOWDIE4;
            else if(right == empty && right1 == mycolor && right2 == mycolor && right3 == mycolor)   return LOWDIE4;
            else if(left == empty && left1 == mycolor && left2 == mycolor && left3 == empty && right == empty)  return TIAO3;
            else if(right == empty && right1 == mycolor && right2 ==mycolor && right3 == empty && left == empty) return TIAO3;
            else if(left == empty && left1 == mycolor && left2 == mycolor && left3 == hiscolor && right == empty)   return DIE3;
            else if(right == empty && right1 == mycolor && right2 == mycolor && right3 == hiscolor && left == empty)    return DIE3;
            else if(left == empty && left1 == empty && left2 == mycolor && left3 == mycolor)    return DIE3;
            else if(right == empty && right1 == empty && right2 == empty && right3 == mycolor)  return DIE3;
            else if(left == empty && left1 == mycolor && left2 == empty && left3 == mycolor)    return DIE3;
            else if(right == empty && right1 == mycolor && right2 == empty && right3 == mycolor)    return DIE3;
            else if(left == empty && left1 == mycolor && left2 == empty && left3 == empty && right == empty)    return LOWALIVE2;
            else if(right == empty && right1 == mycolor && right2 == empty && right3 == empty && left == empty) return LOWALIVE2;
            else if(left == empty && left1 == empty && left2 == mycolor && left3 == empty && right == empty)    return LOWALIVE2;
            else if(right == empty && right1 == empty && right2 == mycolor && right3 == empty && left == empty) return LOWALIVE2;
        }
        return NOTHREAT;
    }

int AssessPoint(int x,int y,int flag)
{
    struct Situation situation = {0};

    GetPattern(x,y,flag,Pattern);
    for(int i = 0; i <= 3; i++){
        int line;
        line = AssessPatternLine(i,flag,Pattern);
        switch(line){
        case LONGFORBIDDEN:
        case WIN5:
			situation.win5++;
			break;
		case ALIVE4:
			situation.alive4++;
			break;
		case DIE4:
			situation.die4++;
			break;
		case LOWDIE4:
			situation.lowdie4++;
			break;
		case ALIVE3:
			situation.alive3++;
			break;
		case TIAO3:
			situation.tiao3++;
			break;
		case DIE3:
			situation.die3++;
			break;
		case ALIVE2:
			situation.alive2++;
			break;
		case LOWALIVE2:
			situation.lowalive2++;
			break;
		case DIE2:
			situation.die2++;
			break;
		case NOTHREAT:
			situation.nothreat++;
			break;
		default:
			break;
        }
    }

    int die4num = situation.die4 + situation.lowdie4;
    int alive3num = situation.alive3 + situation.tiao3;
    int alive2num = situation.alive2 + situation.lowalive2;

    if(situation.win5 >= 1) return Level1;
    if(situation.alive4 >= 1 || die4num >= 2 || (die4num >= 1 && alive3num >=1))    return Level2;
    if(alive3num >= 2)  return Level3;
    if(situation.die3 >=1 && situation.alive3 >= 1) return Level4;
    if(situation.die4 >= 1) return Level5;
    if(situation.lowdie4 >= 1)  return Level6;
    if(situation.alive3 >= 1)   return Level7;
    if(situation.tiao3 >= 1)    return Level8;
    if(alive2num >= 2)   return Level9;
    if(situation.alive2 >= 1)    return Level10;
    if(situation.lowalive2 >= 1)    return Level11;
    if(situation.die3 >= 1) return Level12;
    if(situation.die2 >= 1) return Level13;
    return Level14;
}

void GetPoint()
{
    int pchiscore,pcscore,hmhiscore,hmscore;    //hm为敌方，pc为我方
    int pctemscore = 0, hmtemscore = 0; //储存pc最高分时hm的分数与hm最高分时pc的分数，用于在平分时比较
    int x,y;
    int pcx,pcy,hmx,hmy;
    pchiscore = hmhiscore = 0;
    pcx = pcy = hmx = hmy = 0;

    // for(int temy = 14; temy > 0; temy--){
    //     for(int temx = 0; temx < 14; temx++)
    //        scoreboard[temx][temy] = 0;
    
    // }//一些调试用代码

    for(x = 0; x <= 14; x++)
        for(y = 0; y <= 14; y++)
            if(WhetherOccupied(x,y))    //若是空置点
            {
                pcscore = AssessPoint(x,y,pcflag);  //看这点上电脑我方的得分
                hmscore = AssessPoint(x,y,hmflag);  //看敌方得分
            
                if(pcflag == BLACK)     //黑子才需要判断禁手
                {
                    if(WhetherForbidden(x,y) == FORBIDDEN)   //判断禁手
                        pcscore -= 10000;                               
                    //如果产生了双三或双四的话就会减10000分，但这比连五的分数低，所以如果双三双四与连五同时构成时程序还是会选择下成五获得胜利
                    else if(pcflag == BLACK && WhetherForbidden(x,y) == LONGFORBIDDEN)
                        pcscore = -1;
                    //长连禁手直接不给下
                }
                else if(hmflag == BLACK)    //同理对对方判断是否禁手 可以不需要堵是禁手的点
                {
                    if(WhetherForbidden(x,y) == FORBIDDEN)   //判断禁手
                        hmscore -= 5000;                               
                    //如果产生了双三或双四的话就会减5000分，但这比连五的分数低，所以如果双三双四与连五同时构成时程序还是会选择下成五获得胜利
                    else if(pcflag == BLACK && WhetherForbidden(x,y) == LONGFORBIDDEN)
                        hmscore = -1;
                    //长连禁手直接不给下
                }

                if(pcscore > pchiscore || (pcscore == pchiscore && hmscore > hmtemscore) ||    //选择更好的的点：自己分数更高，或自己分数持平但是这里敌方的分数更高，或者两者都持平但更接近棋盘中央
                (pcscore == pchiscore && hmscore == hmtemscore && (abs(x-7)+abs(y-7) < abs(pcx - 7)+abs(pcy - 7)))){//利用一个绝对值判断让分数相同时会选择靠近棋盘中央的点
                    pchiscore = pcscore;
                    hmtemscore = hmscore;
                    pcx = x;
                    pcy = y;
                }

                if(hmscore > hmhiscore || (hmscore == hmhiscore && pcscore > pctemscore)
                || (hmscore == hmhiscore && pcscore == pctemscore && (abs(x-7)+abs(y-7) < abs(hmx - 7)+abs(hmy - 7)))){
                hmhiscore = hmscore;
                pctemscore = pcscore;
                hmx = x;
                hmy = y;
                }

                scoreboard[x][y] = pcscore;
            }
    
    
            

    if(pchiscore >= hmhiscore){     //若我方分更高，采取进攻策略（由于考虑到现在是我方先手，我方和敌方下一步都是同分时显然是进攻更有利）
        BestPoint[0] = pcx;
        BestPoint[1] = pcy;
    }
    else{                           //若敌方分更高，防守策略
        BestPoint[0] = hmx;
        BestPoint[1] = hmy;
    }


    // for(int temy = 14; temy > 0; temy--){
    //     for(int temx = 0; temx < 14; temx++)
    //         printf("%3d ",scoreboard[temx][temy]);
    //     printf("\n");
    // }//一些调试用代码

}

//占用返回0 否则1
int WhetherOccupied(int x,int y)
{
    if(Board[x][y] == WHITE || Board[x][y] == TemWHITE || Board[x][y] == BLACK || Board[x][y] == TemBLACK){
        return 0;
    }
    else{
        return 1;
    }
}

int WhetherWin(int x,int y,int flag)
{
    int count = 1;
    int j;
    GetPattern(x,y,flag,Pattern);
    for(int i = 0; i <= 3; i++){
        count = 1;
        for(j = 1; Pattern[i][4-j] == 1; j++){
            count++;
        }
        for(j = 1; Pattern[i][4+j] == 1; j++){
            count++;
        }
        if(count == 5)   return 1;
        if(count > 5 && flag == WHITE)  return 1;   //白棋无长连禁手
        if(count > 5 && flag == BLACK)  return -1;
    }
    return 0;
}

//判断一个空格（若下了黑棋）是否是禁手，由于仅限黑棋有禁手故参数仅有x,y，默认指黑棋。若触发禁手返回FORBIDDEN，否则返回1.
int WhetherForbidden(int x,int y)
{
    struct Situation situation = {0};

    GetPattern(x,y,BLACK,ForbiddenPattern);
    for(int i = 0; i <= 3; i++){
        int line;
        line = AssessPatternLine(i,BLACK,ForbiddenPattern);
        switch(line){
        case LONGFORBIDDEN:
            return LONGFORBIDDEN;
        case WIN5:
			situation.win5++;
			break;
		case ALIVE4:
			situation.alive4++;
			break;
		case DIE4:
			situation.die4++;
			break;
		case LOWDIE4:
			situation.lowdie4++;
			break;
		case ALIVE3:
			situation.alive3++;
			break;
		case TIAO3:
			situation.tiao3++;
			break;
		case DIE3:
			situation.die3++;
			break;
		case ALIVE2:
			situation.alive2++;
			break;
		case LOWALIVE2:
			situation.lowalive2++;
			break;
		case DIE2:
			situation.die2++;
			break;
		case NOTHREAT:
			situation.nothreat++;
			break;
		default:
			break;
        }
    }

    int num_of_Alive3 = situation.alive3 + situation.tiao3;                 //双活三或跳活三
    int num_of_4 = situation.alive4 + situation.die4 + situation.lowdie4;   //双四

    if(num_of_4 >= 2 || num_of_Alive3 >= 2)
        return FORBIDDEN;                           //双三或双四的返回，与长连的不同就是如果双三双四与成五一起出现的话可以赢，而长连就算出现了成五也不可以下。
    return 1;           


}