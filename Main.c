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
int pcflag,hmflag;                 //电脑执哪一方

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

#define WIN5 0//0->5连珠
#define ALIVE4 1//1->活4
#define DIE4 2//2->死4
#define LOWDIE4 3//3->死4的低级版本
#define ALIVE3 4//3->活3
#define TIAO3 5//5->跳3
#define DIE3 6//6->死3
#define ALIVE2 7//7->活2
#define LOWALIVE2 8//8->低级活2
#define DIE2 9//9->死2
#define NOTHREAT 10//10->没有威胁

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

//将棋形读取进Pattern,x,y为目标点位,Index表征以何方视角计分（BLACK/WHITE）empty_flag用于表征是否假想如果在这里下棋会怎样，若是填1
void GetPattern(int x,int y,int Index); 
//getpattern的辅助函数，会返回此时在pattern中应该为什么数字;BoardContent:棋盘上的内容，一般填Board[][],Index同上      
int PatternNumber(int BoardContent,int Index);        


//给旗形的一个方向返回是哪一种情况，index表示我方or敌方，我方1敌方2
int AssessPatternLine(int linenumber, int index);
//对棋盘上空置的一点评估得出得分，返回分数
int AssessPoint(int x,int y,int flag);
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

void GetPattern(int x,int y,int Index)
{

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

int AssessPatternLine(int linenumber, int flag)
{
    int count = 1;
    int i;
    int left,right;
    int leftpos,rightpos;
   
        for(int i = 1; PatternNumber(Pattern[linenumber][4-i],flag) == 1; i++){
            count++;
        }
        left = PatternNumber(Pattern[linenumber][4-i],flag);
        leftpos = 4-i;
        for(int i = 1; PatternNumber(Pattern[linenumber][4+i],flag) == 1; i++){
            count++;
        }
        right = PatternNumber(Pattern[linenumber][4+i],flag);
        rightpos = 4+i;

        if(count >= 5){
            return WIN5;
        }
        else if(count == 4){
            if(left == empty && right == empty) return ALIVE4;
            else if(left == hiscolor && right == hiscolor) return NOTHREAT;
            else    return DIE4;
        }
        
        else if(count == 3){
            int left2 = PatternNumber(Pattern[linenumber][leftpos-1],flag);
            int right2 = PatternNumber(Pattern[linenumber][rightpos + 1],flag);
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
            int left1 = PatternNumber(Pattern[linenumber][leftpos-1],flag);
            int right1 = PatternNumber(Pattern[linenumber][rightpos + 1],flag);
            int left2 = PatternNumber(Pattern[linenumber][leftpos-2],flag);
            int right2 = PatternNumber(Pattern[linenumber][rightpos + 2],flag);

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
            int left1 = PatternNumber(Pattern[linenumber][leftpos-1],flag);
            int right1 = PatternNumber(Pattern[linenumber][rightpos + 1],flag);
            int left2 = PatternNumber(Pattern[linenumber][leftpos-2],flag);
            int right2 = PatternNumber(Pattern[linenumber][rightpos + 2],flag);
            int left3 = PatternNumber(Pattern[linenumber][leftpos-3],flag);
            int right3 = PatternNumber(Pattern[linenumber][rightpos + 3],flag);

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
    if(WhetherOccupied(x,y) == 0)   return Level15;

    GetPattern(x,y,flag);
    for(int i = 0; i <= 3; i++){
        int line;
        line = AssessPatternLine(i,flag);
        switch(line){
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
    int HiScore = 0,score;
    if(WhetherOccupied(7,7)){
    HiScore = AssessPoint(7,7,pcflag);
    BestPoint[0] = 7;
    BestPoint[1] = 7;
    }
    int p,q;


    for(p = 0; p < 8; p++){
        for(q = 0; WhetherOccupied(7-p+q,7+p) && q < 2*p; q++){
            score = AssessPoint(7-p+q,7+p,pcflag);
            if(score > HiScore && WhetherOccupied(7-p+q,7+p)){
                HiScore = score;
                BestPoint[0] = 7-p+q;
                BestPoint[1] = 7+p;
            }
        }       //上横

        for(q = 0; WhetherOccupied(7+p-q,7-p) && q < 2*p; q++){
            score = AssessPoint(7+p-q,7-p,pcflag);
            if(score > HiScore && WhetherOccupied(7+p-q,7-p)){
                HiScore = score;
                BestPoint[0] = 7+p-q;
                BestPoint[1] = 7-p;
            }
        }       //下横
    }
   
        for(q = 0; WhetherOccupied(7+p,7+p-q) && q < 2*p; q++){
            score = AssessPoint(7+p,7+p-q,pcflag);
            if(score > HiScore && WhetherOccupied(7+p,7+p-q)){
                HiScore = score;
                BestPoint[0] = 7+p;
                BestPoint[1] = 7+p-q;
            }
        }       //右竖

        for(q = 0; WhetherOccupied(7-p,7-p+q) && q < 2*p; q++){
            score = AssessPoint(7-p,7-p+q,pcflag);
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
    return 0;
}
