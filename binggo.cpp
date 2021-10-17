#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;


#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

//게임 판 랜덤 생성
void random(int comBoard[5][5], int myBoard[5][5]);
//게임판 2개가 비슷한지 확인
bool check(int comBoard[5][5], int myBoard[5][5]);
//선택한 숫자가 이미 선택된 숫자인지 확인
bool checkPlay(int board[5][5], char check[5][5], int play);
//선택한 숫자 체크
void choice(int board[5][5], char check[5][5], int play);
//빙고 개수 확인
void binggo(char comCheck[5][5], char myCheck[5][5], int result[2]);
//내림차순 비교연산 함수
int compare(const void* a, const void* b);
//컴퓨터 숫자 선택 알고리즘
int comChoice(int board[5][5], char check[5][5]);

int main()
{
    int comBoard[5][5], myBoard[5][5];
    char comCheck[5][5] = { {0, } }, myCheck[5][5] = { {0, } };
    int result[2] = { 0, 0 };
    int comPlay, myPlay;
    int count = 0;

    while (1) {
        random(comBoard, myBoard);

        if (check(comBoard, myBoard)) break;
    }

    cout << "빙고판이 생성되었습니다. \n게임시작!" << endl;

    while (1) {
        binggo(comCheck, myCheck, result);

        cout << "\n#" << count << "컴퓨터(왼쪽): " << result[0] << "줄 나(오른쪽) : " << result[1] <<"줄" << endl;
        
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                
                if (comCheck[i][j] == 1) cout << "(" << comBoard[i][j] << ")";
                else  cout << "    ";
            }
            cout << "          ";
            for (int j = 0; j < 5; j++) {
                if (myCheck[i][j] == 1) cout << "(" << myBoard[i][j] << ")";
                else cout << " " << myBoard[i][j] << " ";
            }
            cout << endl;
        }

        if (result[0] == 5 || result[1] == 5) break;

        if (count % 2 == 0) {
            cout << "당신의 차례입니다. 몇 번을 열까요? ";
            while (1) {
                cin >> myPlay;
                if (checkPlay(myBoard, myCheck, myPlay)) break;
                else cout << "이미 열린 숫자입니다. 다시 선택해주세요." << endl;
            }
            choice(comBoard, comCheck, myPlay);
        }

        if (count % 2 == 1) {
            comPlay = comChoice(comBoard, comCheck);

            /*
            while (1) {
                comPlay = rand() % 25 + 1;
                if (checkPlay(comBoard, comCheck, comPlay)) break;
            }*/

            choice(myBoard, myCheck, comPlay);
            cout << "컴퓨터의 차례입니다. " << comPlay << "번을 선택했습니다." << endl;
        }

        count++;
    }

    if (result[0] == 5) {
        if (result[1] == 5) cout << "비겼습니다.";
        else cout << "컴퓨터가 이겼습니다...";
    }
    else cout << "당신이 이겼습니다!";
}

void random(int comBoard[5][5], int myBoard[5][5]) {
    int randint[25] = { 0, }, a, flag;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 25; j++) {
            //랜덤한 수 생성
            while (1) {
                flag = 0; //0이면 중복 없음
                int k;
                a = rand() % 25 + 1;
                for (k = 0; k < j + 1; k++) {
                    //printf("1 a: %d k: %d %d    ", a, k, randint[k]);
                    //if (j == 24) break;
                    if (randint[k] == a) {
                        flag = 1;
                    }
                }
                if (flag == 0) break;
                //printf("\n");
            }
            //printf("\n j: %d a: %d \n\n", j, a);
            randint[j] = a;
        }
        //생성한 수 배열에 삽입
        int index = 0;
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (i == 0) comBoard[j][k] = randint[index];
                else myBoard[j][k] = randint[index];
                index++;
            }
        }
    }
}

bool check(int comBoard[5][5], int myBoard[5][5]) {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (comBoard[i][j] == myBoard[i][j]) count++;
        }
    }

    if (count > 10) {
        return false;
    }

    return true;
}

bool checkPlay(int board[5][5], char check[5][5], int play) {
    int index[2] = { 0,0 };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == play) {
                index[0] = i;
                index[1] = j;
            }
        }
    }

    if (check[index[0]][index[1]] == 1) return false;

    check[index[0]][index[1]] = 1;

    return true;
}

void choice(int board[5][5], char check[5][5], int play) {
    int index[2] = { 0,0 };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == play) {
                index[0] = i;
                index[1] = j;
            }
        }
    }

    check[index[0]][index[1]] = 1;
}

void binggo(char comCheck[5][5], char myCheck[5][5], int result[2]) {
    /*
    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡ프린트 판ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf(" %c ", comCheck[i][j]);
        }
        printf("          ");
        for (int j = 0; j < 5; j++) {
            printf(" %c ", myCheck[i][j]);
        }
        printf("\n");
    }
    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
    */
    char myRow[5] = { 1, 1, 1, 1, 1 }, myCollum[5] = { 1, 1, 1, 1, 1 };
    char comRow[5] = { 1, 1, 1, 1, 1 }, comCollum[5] = { 1, 1, 1, 1, 1 };
    char myCross[2] = { 1, 1 }, comCross[2] = { 1, 1 };
    result[0] = 0, result[1] = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (comCheck[i][j] == 0) comRow[j] = 0;
            if (myCheck[i][j] == 0) myRow[j] = 0;
            if (comCheck[j][i] == 0) comCollum[j] = 0;
            if (myCheck[j][i] == 0) myCollum[j] = 0;
        }
        if (comCheck[i][i] == 0) comCross[0] = 0;
        if (myCheck[i][i] == 0) myCross[0] = 0;
        if (comCheck[i][4 - i] == 0) comCross[1] = 0;
        if (myCheck[i][4 - i] == 0) myCross[1] = 0;
    }

    for (int i = 0; i < 5; i++) {
        result[0] += comRow[i] + comCollum[i];
        result[1] += myRow[i] + myCollum[i];
    }

    result[0] += comCross[0] + comCross[1];
    result[1] += myCross[0] + myCross[1];
}

int compare(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

int comChoice(int comBoard[5][5], char comCheck[5][5]) {
    int comPlay, randlist[2][5] = { {2, 1, 3, 0, 4}, { 2, 3, 1, 4, 0 } }, randint;

    if (comCheck[2][2] == 0) {
        comPlay = comBoard[2][2];
        comCheck[2][2] = 1;
        return comPlay;
    }
    else {
        int checklist[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }; //0~4 row, 5~9 collum, 10~11 cross

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (comCheck[i][j] == 1) {
                    checklist[i] += 100;
                    checklist[j + 5] += 100;
                }
            }
            if (comCheck[i][i] == 1) checklist[10] += 100;
            if (comCheck[i][4 - i] == 1) checklist[11] += 100;
        }

        qsort(checklist, 12, sizeof(int), compare);

        for (int i = 0; i < 12; i++) {
            printf("%d ", checklist[i]);
        }
        printf("\n");

        randint = rand() % 2;

        for (int i = 0; i < 12; i++) {
            //printf("%d ", checklist[i]);
            int index = checklist[i] % 100;
            switch (index) {
            case 10:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[randlist[randint][j]][randlist[randint][j]];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 11:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[randlist[randint][j]][4 - randlist[randint][j]];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 2:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[2][randlist[randint][j]];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 7:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[randlist[randint][j]][2];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 1:
            case 3:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[index][randlist[randint][j]];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 6:
            case 8:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[randlist[randint][j]][index - 5];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 0:
            case 4:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[index][randlist[randint][j]];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            case 5:
            case 9:
                for (int j = 0; j < 5; j++) {
                    comPlay = comBoard[randlist[randint][j]][index - 5];
                    if (checkPlay(comBoard, comCheck, comPlay)) return comPlay;
                }
            }
        }
    }
}