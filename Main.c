#include <stdio.h>
#include <stdlib.h>

#define SIZE 15
#define WHITE 10           //����
#define BLACK 20           //����
#define TemWHITE 11        //������һ��
#define TemBLACK 21        //������һ��

int Board[SIZE][SIZE];      //���ڴ�������
int Pattern[4][SIZE];       //�����ĸ���������ͣ�˳�򣺺ᡢ����\��/
int nFlag = 1;              //�����ʱ�ֵ�˭�Ļغϣ�����Ϊ1����Ϊ2
int nMode;                  //��Ϸģʽ��1�����˶�ս��2���˻���ս����ִ�ڣ�3���˻���ս����ִ�ף�
int nWinner;                //�������Ӯ��,1��2��

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
//�����δ�֣����ص÷�,index��ʾ�����ҷ��÷ֻ�������ֹ�з��÷�,��Ϊ1�з�Ϊ2
int AssessPattern(int index);
//�������Ͽ��õ�һ�������ó��÷֣����ط���
int AssessPoint(int x,int y);
//�������̷��ط�����ߵĵ�
void GetPoint();
int BestPoint[2];   //����getpoint�ҵ�����õ�

int main()
{
    /*�������ڲ���comparepattern�Ĳ��֣������Ż�*/
    // int testsource[15] = {1,2,1,2,1,6,7,8,9,10,11,12,13,14,15};
    // int testtarget[] = {1,2,1,99};
    // printf("%d",ComparePattern(testsource, testtarget));
    // system("pause");

    SelectMode();       //ѡ��ģʽ 
    InitBoard();        //��ʼ������
    ShowBoard();        //��ӡ����

    nWinner = PlayGobang();
    
    return 0;
}

void SelectMode()
{
    int temMode,temFlag;                //��ʱ����ģʽ�����·�
    ReSelect:                           //�������������֮�������ʱ��goto�����������¿�ʼѡ��
    system("cls");
    printf("��ѡ��ģʽ���˻���ս����1�����˶�ս����2��");
    scanf("%d",&temMode);
    getchar();
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
            nFlag = 1;
            scanf("%c%d",&tBlackx,&tBlacky);
            getchar();
            tBlackx = tBlackx - 'a';
            tBlacky = tBlacky - 1;
            Board[tBlackx][tBlacky] = TemBLACK;
            ShowBoard();
            
            /*��������*/
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

            if(iFlag_FirstRound == 0) iFlag_FirstRound = 1;     //���������ǵ�һ��
        }
    }   
   
   if(nMode == 2){
        while(1){
            /* ���ӻ����� */
            if(iFlag_FirstRound == 1){
                Board[tBlackx][tBlacky] = BLACK;        //������ǵ�һ�֣���ǰһ������Ϊͨ����ʾ
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
            
            /*��������*/
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
        Board[x][y] == Index;
    }//���Ҫ����������ȴ����ԭʼ״̬�ٵ�������

    for(int i = 0; i < 4; i++){                 //��ʼ��
        for(int j = 0; j < 15; j++)
            Pattern[i][j] = 3;
    }

    for(int i = 0;i < 15; i++){
        Pattern[0][i] = PatternNumber(Board[i][y],Index);       //��
    }
    for(int i = 0;i < 15; i++){
        Pattern[1][i] = PatternNumber(Board[x][i],Index);       //��
    }

    int x0 = x + y - 14;   //��б����������x����
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
        for(int j = 0; source[i+j] == target[j] && ((i+j) <= 14) && target[j] != '\0'; j++)
            if(target[j+1] == 99){
                count++;
                break;
            }       //99��ʾ���δ�����ֹ
    }
    
    return count;
}

int AssessPattern(int index)
{
    int p;
    int score = 0;
    if(index == 1) p = 0;
    else if(index == 2) p = 1;
    else printf("error in assesspattern\n");        //ѡ��ʹ�����ֱ����һ��
    
    int TargetPatterns[10][7] = {{1,1,1,1,1,99},{0,1,1,1,1,0,99},{0,1,1,1,1,2,99},{2,1,1,1,1,0,99},{0,1,1,1,0,99},{0,1,1,1,2,99},{2,1,1,1,0,99},{0,1,1,0,99}
    ,{0,1,2,99},{2,1,0,99}};
    int Scores[10][2] = {{10000,1000},{200,100},{50,20},{50,20},{30,10},{8,5},{8,5},{2,1},{-1,0},{-1,0}};
    //������������ձ�Ŀǰ���ܼ�ª!

    for(int i = 0; i < 4; i++ ){
        for(int j = 0; j < 8; j++){
            score += Scores[j][p] * ComparePattern(Pattern[i],TargetPatterns[j]);
        }
    }
    
    return score;
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
