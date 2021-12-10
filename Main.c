#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 10           //����
#define BLACK 20           //����
#define TemWHITE 11        //������һ��
#define TemBLACK 21        //������һ��


int Board[SIZE][SIZE];      //���ڴ�������
int Pattern[4][9];       //�����ĸ���������ͣ�˳�򣺺ᡢ����\��/
int nFlag = 1;              //�����ʱ�ֵ�˭�Ļغϣ�����Ϊ1����Ϊ2
int nMode;                  //��Ϸģʽ��1�����˶�ս��2���˻���ս����ִ�ڣ�3���˻���ս����ִ�ף�
int nWinner;                //�������Ӯ��,1��2��
int pcflag,hmflag;                 //����ִ��һ��

struct Situation {//��ǰλ�õ���ʽ����ָ����������
	int win5;//5����
	int alive4;//��4
	int die4;//��4
	int lowdie4;//��4�ͼ��汾
	int alive3;//��3
	int tiao3;//��3
	int die3;//��3
	int alive2;//��2
	int lowalive2;//�ͼ���2
	int die2;//��2
	int nothreat;//û����в
};

#define WIN5 0//0->5����
#define ALIVE4 1//1->��4
#define DIE4 2//2->��4
#define LOWDIE4 3//3->��4�ĵͼ��汾
#define ALIVE3 4//3->��3
#define TIAO3 5//5->��3
#define DIE3 6//6->��3
#define ALIVE2 7//7->��2
#define LOWALIVE2 8//8->�ͼ���2
#define DIE2 9//9->��2
#define NOTHREAT 10//10->û����в

#define Level1 100000//����
#define Level2 10000//�ɻ�4 �� ˫��4 �� ��4��3
#define Level3 5000//˫��3
#define Level4 1000//��3�߼���3
#define Level5 500//����
#define Level6 400//�ͼ�����
#define Level7 100//����3
#define Level8 90//����3
#define Level9 50//˫��2
#define Level10 10//��2
#define Level11 9//�ͼ���2
#define Level12 5//��3
#define Level13 2//��2
#define Level14 1//û����в
#define Level15 0//������

#define mycolor 1//�ҵ���ɫ
#define empty 0//��
#define hiscolor 2//�з���ǽ

void InitBoard();           //��ʼ������
void ShowBoard();           //��ʾ����
void SelectMode();          //ѡ����Ϸģʽ
int PlayGobang();           //����������

//�����ζ�ȡ��Pattern,x,yΪĿ���λ,Index�����Ժη��ӽǼƷ֣�BLACK/WHITE��empty_flag���ڱ����Ƿ������������������������������1
void GetPattern(int x,int y,int Index); 
//getpattern�ĸ����������᷵�ش�ʱ��pattern��Ӧ��Ϊʲô����;BoardContent:�����ϵ����ݣ�һ����Board[][],Indexͬ��      
int PatternNumber(int BoardContent,int Index);        


//�����ε�һ�����򷵻�����һ�������index��ʾ�ҷ�or�з����ҷ�1�з�2
int AssessPatternLine(int linenumber, int index);
//�������Ͽ��õ�һ�������ó��÷֣����ط���
int AssessPoint(int x,int y,int flag);
//�������̷��ط�����ߵĵ�
void GetPoint();
int BestPoint[2];   //����getpoint�ҵ�����õ�
//����õ�δ��ռ�÷���1
int WhetherOccupied(int x,int y);
//ʤ���ж�,����Ϊ�ڰ� 
int WhetherWin(int x,int y,int index);



int main()
{

    SelectMode();       //ѡ��ģʽ 
    InitBoard();        //��ʼ������
    ShowBoard();        //��ӡ����

    nWinner = PlayGobang();
    
    
    
    system("pause");
    return 0;
}

void SelectMode()
{
    int temMode,temFlag;                //��ʱ����ģʽ�����·�
    ReSelect:                           //�������������֮�������ʱ��goto�����������¿�ʼѡ��
    system("cls");
    printf("��ѡ��ģʽ���˻���ս����1�����˶�ս����2:");
    scanf("%d",&temMode);
    if(temMode == 1){
        nMode = 1;
        printf("\n��ѡ�����ִ��ִ�ף�������1������2:");
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

void InitBoard()            //��ʼ������
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

void ShowBoard()            //��ӡ����
{
    system("cls");          //����
    
    int i, j;
    int line = 15;       //�������������ֱ��
    char ary = 'A';      //������������ĸ���
    
    /*���ɱ����У�˵��ģʽ���ʱ���巽*/
    if(nMode == 1)  printf("             ���˶�ս\n");
    else            printf("             �˻���ս\n");
    if(nFlag == 1)  printf("               ����\n");
    else            printf("               ����\n");

    /*��ӡ����*/
    printf("\n");
    for(j = SIZE - 1; j >= 0; j--){        //���д����������д�ӡ
        printf("%2d",line);
        line--;
        for(i = 0; i < SIZE; i++){
            switch(Board[i][j]){
                case 1:
                    printf("��");
                    break;
                case 2:
                    printf("��");
                    break;
                case 3:
                    printf("��");
                    break;
                case 4:
                    printf("��");
                    break;
                case 5:
                    printf("��");
                    break;
                case 6:
                    printf("��");
                    break;
                case 7:
                    printf("��");
                    break;
                case 8:
                    printf("��");
                    break;
                case 9:
                    printf("��");
                    break;
                case BLACK:
                    printf("��");        //����ͨ��
                    break;
                case WHITE:
                    printf("��");        //����ͨ��
                    break;
                case TemBLACK:
                    printf("��");        //����ǰһ��
                    break;
                case TemWHITE:
                    printf("��");        //����ǰһ��
                    break;
            }
        }
        printf("\n");
    }
    for(printf("   "); ary <= 'O'; ary++) //�����ĸ���
        printf("%c ",ary);
    printf("\n");
}

int PlayGobang()
{
    int tBlacky , tWhitey;                    //��¼��������
    char tBlackx, tWhitex;
    int iFlag_FirstRound = 0;               //��¼�Ƿ��ǵ�һ�غϣ����ڵ���������ǰһ�ֻ���ͨ����ʾ�� 

   if(nMode == 1){
        while(1){
            /* �������� */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //������ǵ�һ�֣���ǰһ������Ϊͨ����ʾ
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            nFlag = 2;
            ShowBoard();
            

            /*��������*/
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

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //���������ǵ�һ��
        }
    }   
   
   if(nMode == 2){
        nFlag = 2;
        while(1){
            /* ���ӻ����� */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //������ǵ�һ�֣���ǰһ������Ϊͨ����ʾ
            }
            GetPoint();
            tBlackx = BestPoint[0];
            tBlacky = BestPoint[1];
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            if(WhetherWin(tBlackx,tBlacky,BLACK)){
                printf("��ɫӮ\n");
                break;
            }
            
     

            /*��������*/
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
                printf("��ɫӮ\n");
                break;
            }

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //���������ǵ�һ��
        }
   }
     

    if(nMode == 3){
        nFlag = 1;
        while(1){
            /* �������� */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //������ǵ�һ�֣���ǰһ������Ϊͨ����ʾ
            }
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            if(WhetherWin(tBlackx,tBlacky,BLACK)){
                printf("��ɫӮ\n");
                break;
            }
     

            /*���ӻ�����*/
            if(iFlag_FirstRound == 1){
                Board[tWhitex][tWhitey] = WHITE;
            }
            GetPoint();
            tWhitex = BestPoint[0];
            tWhitey = BestPoint[1];
            Board[tWhitex][tWhitey] = TemWHITE;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE)){
                printf("��ɫӮ\n");
                break;
            }

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //���������ǵ�һ��
        }
   }

    return 0;
}

void GetPattern(int x,int y,int Index)
{

    for(int i = 0; i < 4; i++){                 //��ʼ��
        for(int j = 0; j < 9; j++)
            Pattern[i][j] = 2;
    }

    
    for(int i = 1; i <= 4 && (x-i) >= 0; i++){  //��
        Pattern[0][4-i] = PatternNumber(Board[x-i][y],Index);
    }
    for(int i = 1; i <= 4 && (x+i) <= 14; i++){
        Pattern[0][4+i] = PatternNumber(Board[x+i][y],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0; i++){  //��
        Pattern[1][4-i] = PatternNumber(Board[x][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14; i++){
        Pattern[1][4+i] = PatternNumber(Board[x][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y-i) >= 0 && (x-i) >= 0; i++){  //��б
        Pattern[2][4-i] = PatternNumber(Board[x-i][y-i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x+i) <= 14; i++){
        Pattern[2][4+i] = PatternNumber(Board[x+i][y+i],Index);
    }   
    
    for(int i = 1; i <= 4 && (y+i) <= 14 && (x-i) >= 0; i++){  //��б
        Pattern[3][4-i] = PatternNumber(Board[x-i][y+i],Index);
    }
    for(int i = 1; i <= 4 && (y+i) >= 0 && (x-i) <=14; i++){
        Pattern[3][4+i] = PatternNumber(Board[x+i][y-i],Index);
    }       
    }

//0Ϊ�� 1Ϊ�ҷ� 2Ϊ�з���ǽ
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
        }       //�Ϻ�

        for(q = 0; WhetherOccupied(7+p-q,7-p) && q < 2*p; q++){
            score = AssessPoint(7+p-q,7-p,pcflag);
            if(score > HiScore && WhetherOccupied(7+p-q,7-p)){
                HiScore = score;
                BestPoint[0] = 7+p-q;
                BestPoint[1] = 7-p;
            }
        }       //�º�
    }
   
        for(q = 0; WhetherOccupied(7+p,7+p-q) && q < 2*p; q++){
            score = AssessPoint(7+p,7+p-q,pcflag);
            if(score > HiScore && WhetherOccupied(7+p,7+p-q)){
                HiScore = score;
                BestPoint[0] = 7+p;
                BestPoint[1] = 7+p-q;
            }
        }       //����

        for(q = 0; WhetherOccupied(7-p,7-p+q) && q < 2*p; q++){
            score = AssessPoint(7-p,7-p+q,pcflag);
            if(score > HiScore && WhetherOccupied(7-p,7-p+q)){
                HiScore = score;
                BestPoint[0] = 7-p;
                BestPoint[1] = 7-p+q;
            }
        }       //����


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
