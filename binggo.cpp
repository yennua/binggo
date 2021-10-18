#define _CRT_SECURE_NO_WARNINGS

#include <bangtal>
using namespace bangtal;

#include <stdio.h>
#include <Windows.h>

#include <cstdlib>
#include <ctime>

//게임 판 랜덤 생성
void random(int board[5][5]);
//게임판 2개가 비슷한지 확인
bool check(int comBoard[5][5], int myBoard[5][5]);
//선택한 숫자가 이미 선택된 숫자인지 확인
bool checkPlay(int board[5][5], char check[5][5], int play);
//선택한 숫자 체크
void choice(int board[5][5], char check[5][5], int play, ObjectPtr piece);
//빙고 개수 확인
void binggo(char comCheck[5][5], char myCheck[5][5], int result[2]);
//내림차순 비교연산 함수
int compare(const void* a, const void* b);
//컴퓨터 숫자 선택 알고리즘
int comChoice(int board[5][5], char check[5][5]);

//게임 시작 함수
void play_game();
//게임 종료 함수
void game_end(int result[2]);

ScenePtr scene1, scene2, scene3;
ObjectPtr com_board[5][5], my_board[5][5];
ObjectPtr start, restart;
//ObjectPtr set1, set2;
SoundPtr num[25];

int main() {
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

    scene1 = Scene::create("시작화면", "Images/배경.png");
    //scene2 = Scene::create("게임 선택화면", "Images/select.png");
    scene3 = Scene::create("게임화면", "Images/board.png");

    start = Object::create("Images/start.png", scene1, 462, 124);
    restart = Object::create("Images/restart.png", scene3, 462, 231);
    restart->hide();

    start->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        scene3->enter();
        return true;
        });

    restart->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        scene1->enter();
        restart->hide();

        //값 초기화
        for (int i = 0; i < 5;i++) {
            for (int j = 0; j < 5;j++) {
                myCheck[i][j]=0;
                comCheck[i][j] = 0;
            }
        }
        result[0] = 0;
        result[1] = 0;

        return true;
        });

    scene3->setOnEnterCallback([](ScenePtr scene)->bool {
        play_game();
        return true;
        });

    startGame(scene1);

    return 0;
}

int index_to_x(int who, int index) { //빙고판 버튼 x 위치 지정 함수
    if (who == 1) {//나?
        return 568 + 90 * index;
    }
    else return 45 + 90 * index; //컴퓨터?
}

int index_to_y(int index) {
    return 403 - 90 * index;
}

int myBoard[5][5];
int comBoard[5][5];
char comCheck[5][5] = { {0, } }, myCheck[5][5] = { {0, } };
int result[2] = { 0, 0 };
int comPlay, myPlay;

int game_value(ObjectPtr piece) {
    for (int i = 0; i < 5;i++) {
        for (int j = 0; j < 5;j++) {
            if (my_board[i][j] == piece) return myBoard[i][j];
        }
    }

    return -1; //INVALID PIECE
}

int game_index(int board[5][5], int play) {
    for (int i = 0; i < 5;i++) {
        for (int j = 0; j < 5;j++) {
            if (board[i][j] == play) return i*10+j;
        }
    }

    return -1; //INVALID PIECE
}
void play_game() {

    SoundPtr ready1 = Sound::create("Sounds/준비.mp3");
    SoundPtr ready2 = Sound::create("Sounds/시작.mp3");

    ready1->play(false);
    
    while (1) {
        random(comBoard);
        random(myBoard);
        if (check(comBoard, myBoard)) break;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (comCheck[i][j] == 1) printf("(%2d)", comBoard[i][j]);
            else printf(" %2d ", comBoard[i][j]); //printf("    ");
        }
        printf("          ");
        for (int j = 0; j < 5; j++) {
            if (myCheck[i][j] == 1) printf("(%2d)", myBoard[i][j]);
            else printf(" %2d ", myBoard[i][j]);
        }
        printf("\n");
    }

    char path[30];
    int a;

    for (int i = 0; i < 25; i++) {
        sprintf(path, "Sounds/%d.mp3", i+1);
        num[i] = Sound::create(path);
    }
    printf("init start");

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            myPlay = myBoard[i][j];
            //printf("myPlay: %d \n", myPlay);
            sprintf(path, "Images/%d.png", myPlay);
            my_board[i][j] = Object::create(path, scene3, index_to_x(1, j), index_to_y(i));

            my_board[i][j]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
                //if (checkPlay(myBoard, myCheck, myPlay)) {
                for (int k = 0; k < 2; k++) {
                    char msg[100];
                    if (k == 0) {
                        int myPlay1 = game_value(piece);
                        printf("myPlay1: %d \n", myPlay1);
                        num[myPlay1-1]->play(false);
                        choice(myBoard, myCheck, myPlay1, piece);
                        a = game_index(comBoard, myPlay1);
                        choice(comBoard, comCheck, myPlay1, com_board[a / 10][a % 10]);

                        printf("myPlay2: %d \n", myPlay1);
                    }
                    else {
                        comPlay = comChoice(comBoard, comCheck);
                        Sleep(800);
                        sprintf(msg, "컴퓨터의 차례입니다. 컴퓨터는 %d를 골랐습니다.", comPlay);
                        showMessage(msg);
                        printf("comPlay: %d\n", comPlay);
                        num[comPlay - 1]->play(false);
                        a = game_index(myBoard, comPlay);
                        choice(myBoard, myCheck, comPlay, my_board[a / 10][a % 10]);
                        a = game_index(comBoard, comPlay);
                        choice(comBoard, comCheck, comPlay, com_board[a / 10][a % 10]);
                    }
                    printf("choice finish \n");

                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
                            if (comCheck[i][j] == 1) printf("(%2d)", myBoard[i][j]);
                            else printf(" %2d ", comBoard[i][j]); //printf("    ");
                        }
                        printf("          ");
                        for (int j = 0; j < 5; j++) {
                            if (myCheck[i][j] == 1) printf("(%2d)", myBoard[i][j]);
                            else printf(" %2d ", myBoard[i][j]);
                        }
                        printf("\n");
                    }

                    binggo(comCheck, myCheck, result);
                        
                    sprintf(msg, "<현재 점수> 컴퓨터(왼쪽): %d줄 나(오른쪽): %d줄", result[0], result[1]);
                    showMessage(msg);

                    if (result[0] == 5 || result[1] == 5) game_end(result);
                }
                //}
            return true;
            });

            //printf("mybutton finish");

            //sprintf(path, "Images/%d.png", comBoard[i][j]);
            com_board[i][j] = Object::create("Images/blank.png", scene3, index_to_x(0, j), index_to_y(i));
        }
    }
    ready2->play(false);
}

void game_end(int result[2]) {
    SoundPtr win = Sound::create("Sounds/게임클리어8.mp3");
    SoundPtr drew = Sound::create("Sounds/까마귀.mp3");
    SoundPtr lose = Sound::create("Sounds/야유.mp3");

    if (result[0] == 5) {
        if (result[1] == 5) {
            drew->play(false);
            showMessage("비겼습니다.");
            restart->show();
        }
        else {
            lose->play(false);
            showMessage("컴퓨터가 이겼습니다...");
            restart->show();
        }
    }
    else {
        win->play(false);
        showMessage("당신이 이겼습니다!");
        restart->show();
    }
}

void random(int board[5][5]) {
    //printf("random");
    int randint[25] = { 0, }, a, flag;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < 25; i++) {
        //랜덤한 수 생성
        while (1) {
            flag = 0; //0이면 중복 없음
            a = rand() % 25 + 1;
            for (int j = 0; j < i + 1; j++) {
                if (randint[j] == a) {
                    flag = 1;
                }
            }
            if (flag == 0) break;
        }
        randint[i] = a;
    }
    //생성한 수 배열에 삽입
    int index = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = randint[index];
            index++;
        }
    }
}

bool check(int comBoard[5][5], int myBoard[5][5]) {
    //printf("check");
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

void choice(int board[5][5], char check[5][5], int play, ObjectPtr piece) {
    int index[2] = { 0,0 };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == play) {
                index[0] = i;
                index[1] = j;
            }
        }
    }

    char path[50];
    sprintf(path, "Images/%d - 복사본.png", play);
    printf("myPlay3: %d \n", play);
    piece->setImage(path);

    check[index[0]][index[1]] = 1;

}

void binggo(char comCheck[5][5], char myCheck[5][5], int result[2]) {

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
            //printf("%d ", checklist[i]);
        }
        //printf("\n");

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