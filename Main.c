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

const int WIN5 = 0;//0->5����
const int ALIVE4 = 1;//1->��4
const int DIE4 = 2;//2->��4
const int LOWDIE4 = 3;//3->��4�ĵͼ��汾
const int ALIVE3 = 4;//3->��3
const int TIAO3 = 5;//5->��3
const int DIE3 = 6;//6->��3
const int ALIVE2 = 7;//7->��2
const int LOWALIVE2 = 8;//8->�ͼ���2
const int DIE2 = 9;//9->��2
const int NOTHREAT = 10;//10->û����в

const int Levelone = 100000;//����
const int Leveltwo = 10000;//�ɻ�4 �� ˫��4 �� ��4��3
const int Levelthree = 5000;//˫��3
const int Levelfour = 1000;//��3�߼���3
const int Levelfive = 500;//����
const int Levelsix = 400;//�ͼ�����
const int Levelseven = 100;//����3
const int Leveleight = 90;//����3
const int Levelnine = 50;//˫��2
const int LevelTen = 10;//��2
const int Leveleleven = 9;//�ͼ���2
const int Leveltwelve = 5;//��3
const int Levelthirteen = 2;//��2
const int Levelfourteen = 1;//û����в
const int Levelfiveteen = 0;//������

void InitBoard();           //��ʼ������
void ShowBoard();           //��ʾ����
void SelectMode();          //ѡ����Ϸģʽ
int PlayGobang();           //����������

//�����ζ�ȡ��Pattern,x,yΪĿ���λ,Index�����Ժη��ӽǼƷ֣�BLACK/WHITE��empty_flag���ڱ����Ƿ������������������������������1
void GetPattern(int x,int y,int Index,int empty_flag); 
//getpattern�ĸ����������᷵�ش�ʱ��pattern��Ӧ��Ϊʲô����;BoardContent:�����ϵ����ݣ�һ����Board[][],Indexͬ��      
int PatternNumber(int BoardContent,int Index);        
//�Ƚ������ַ����Ƿ���ƥ�䣬source����Ƚϵ�,target��Ŀ��,����source���м���target�ֶ�
int ComparePattern(int source[],int target[]);  
/**
 * @brief �����δ�֣����ص÷�
 * @param index ��ʾ�����ҷ��÷ֻ�������ֹ�з��÷�,��Ϊ1�з�Ϊ2
 **/
int AssessPattern(int index);
//�������Ͽ��õ�һ�������ó��÷֣����ط���
int AssessPoint(int x,int y);
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
    printf("��ѡ��ģʽ���˻���ս����1�����˶�ս����2��");
    scanf("%d",&temMode);
    if(temMode == 1){
        nMode = 1;
        printf("\n��ѡ�����ִ��ִ�ף�������1������2��");
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
            nFlag = 1;
            ShowBoard();
            if(WhetherWin(tWhitex,tWhitey,WHITE)){
                printf("��ɫӮ\n");
                break;
            }

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

void GetPattern(int x,int y,int Index,int empty_flag)
{

    int TemMemory;  //���汻�޸ĵĸ���ԭʼ״̬
    if(empty_flag == 1){
        TemMemory = Board[x][y];
        Board[x][y] = Index;
    }//���Ҫ����������ȴ����ԭʼ״̬�ٵ�������

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
    
    if(empty_flag == 1){
        Board[x][y] = TemMemory;
    }
    //�ָ���ԭʼ״̬
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

int ComparePattern(int source[],int target[])
{
    int count = 0;
    
    for(int i = 0; i < 14; i++){         //target�ĵ�һλ��source����һλƥ��
        for(int j = 0; source[i+j] == target[j] && ((i+j) <= 14) && target[j] != 99; j++)
            if(target[j+1] == 99){
                count++;
                break;
            }       //99��ʾ���δ�����ֹ
    }
    
    return count;
}

int AssessPattern(int index)
{
    int count = 1;
}

int AssessPoint(int x,int y)
{
    int Before_we, After_we, Before_they, After_they; //�ҷ��з��ڴ˵�����֮ǰ֮��÷�
    int score;

    int we, they;
    if(nMode == 2){
        we = BLACK;
        they = WHITE;
    }
    else if(nMode == 3){
        we = WHITE;
        they = BLACK;
    }                           //ȷ��������ɫ

    GetPattern(x,y,we,0);
    Before_we = AssessPattern(1);
    GetPattern(x,y,we,1);
    After_we = AssessPattern(1);
    GetPattern(x,y,they,0);
    Before_they = AssessPattern(2);
    GetPattern(x,y,they,1);
    After_they = AssessPattern(2);      //���

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
        }       //�Ϻ�

        for(q = 0; WhetherOccupied(7+p-q,7-p) && q < 2*p; q++){
            score = AssessPoint(7+p-q,7-p);
            if(score > HiScore && WhetherOccupied(7+p-q,7-p)){
                HiScore = score;
                BestPoint[0] = 7+p-q;
                BestPoint[1] = 7-p;
            }
        }       //�º�
    }
   
        for(q = 0; WhetherOccupied(7+p,7+p-q) && q < 2*p; q++){
            score = AssessPoint(7+p,7+p-q);
            if(score > HiScore && WhetherOccupied(7+p,7+p-q)){
                HiScore = score;
                BestPoint[0] = 7+p;
                BestPoint[1] = 7+p-q;
            }
        }       //����

        for(q = 0; WhetherOccupied(7-p,7-p+q) && q < 2*p; q++){
            score = AssessPoint(7-p,7-p+q);
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
