/*
ATTENTION!!!
このプログラムはメルセンヌ・ツイスタを使用しています。
かならずディレクトリ内にMT.hを含めてください

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MT.h"

#define DEBUG

#define PLAYERCOUNT 3

// 初期値0と混同することを回避
#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define WILD 4

#define PLAYER 1991
#define COMONE 2882
#define COMTWO 3773

#define AIKO 111
#define ONEWINNER 222
#define TWOWINNER 333

#define WILDNOTYET 0
#define WILDTRUE 1

// メソッド宣言
int janken_two_people(int player1, int player2, int data[3], int playerData[3][3]);
int janken_three_people(int data[3]);
int checkWildCard(int handOne, int handTwo, int handThree);
void showResult();
int showHandFromPlayer(int code);
void showPlayerArray();
void updateWildCardOnPlayersArray();
void showWinnerArray();

// コンピュータ1, 2の手を格納するためのグローバル変数
int comOne = 0;
int comTwo = 0;
// Playerの手を格納するためのグローバル変数
int numPlayerHand = 0;

// 勝利順を格納するためのグローバル配列
int winnerArray[PLAYERCOUNT];

// プレイヤーコードと、出した手を管理する配列
// playersArray[0][??] : player
// playersArray[1][??] ; comOne
// playersArray[2][??] : comTwo
int playersArray[3][3];

int main(void){

    printf("\n");
    printf("*****************************************************\n");
    printf("Rock-Paper-Scissors!!!\n");
    printf("\n");
    printf("Press g:Rock, c:Scissors, p:Paper, w:WildCard\n");
    printf("You can use WildCard only once. WildCard means that a player who set WildCard will win when he is only a WildCard player. However,if he is not, the last players throw again please\n");
    printf("\n");
    printf("Get Ready??\n");
    printf("*****************************************************\n");
    printf("\n");

    // プレイヤーコードを全て入力
    playersArray[0][0] = PLAYER;
    playersArray[1][0] = COMONE;
    playersArray[2][0] = COMTWO;

    // ワイルドカードの使用を判定するためのフラグを格納する
    playersArray[0][2] = WILDNOTYET;
    playersArray[1][2] = WILDNOTYET;
    playersArray[2][2] = WILDNOTYET;

    while(1){

        printf("You throw >> ");
        // 入力された手を格納しておく変数 char型
        char yourHand = '\0';

        scanf("%s", &yourHand);

        // 入力された手を数字に変換しておくための変数
        // ここで正しい手を入力したかチェック
        if (yourHand == 'c') {
            numPlayerHand = SCISSORS;
        } else if (yourHand == 'g') {
            numPlayerHand = ROCK;
        } else if (yourHand == 'p') {
            numPlayerHand = PAPER;
        } else if (yourHand == 'w') {
            numPlayerHand = WILD;
        } else {
            printf("Press c g p w\n");
            continue;
        }

        // プレイヤーの手を格納
        playersArray[0][1] = numPlayerHand;

        // コンピュータの手をランダムに出力
        // シード値の作成
        init_genrand((unsigned)time(NULL));
        comOne = genrand_int32()%4 + 1;
        playersArray[1][1] = comOne;

        init_genrand((unsigned)time(NULL) + 100);
        comTwo = genrand_int32()%4 + 1;
        playersArray[2][1] = comTwo;

        #ifdef DEBUG
        printf("plyr: %d\n", numPlayerHand);
        printf("com1: %d\n", comOne);
        printf("com2: %d\n", comTwo);

        showPlayerArray();

        int tmp2 = numPlayerHand+comOne+comTwo;
        printf("tmp2 : %d\n", tmp2);
        #endif

        // ここでワイルドカードの処理をする
        int resultWild = checkWildCard(playersArray[0][1], playersArray[1][1], playersArray[2][1]);

        updateWildCardOnPlayersArray();

        #ifdef DEBUG
        showPlayerArray();
        #endif

        if (resultWild == 2) {
            // 複数人のワイルドカードが出たら、あいこと同じ扱い。
            // whileを抜ける
            printf("Multipule Wild Card. Again\n");


        }else if (resultWild == 1) {
            // 一人勝ちのパターン。一人の勝利が確定
            // 二人のみのじゃんけんに移行
            int whoLoseArray[2];
            int loseCounter = 0;
            int t;
            for(t = 0; t < 3; t++){
                if (playersArray[t][1] == 4) {
                    winnerArray[0] = playersArray[t][0];
                    playersArray[t][2] = WILDTRUE;
                } else {
                    whoLoseArray[loseCounter] = playersArray[t][0];
                    loseCounter = 1;
                }
            }
            #ifdef DEBUG
            int o;
            for(o = 0; o < 2; o++){
                printf("Who Lose: %d\n", whoLoseArray[o]);
            }
            int i;
            for (i = 0; i < 3; i++) {
                printf("winnerArray: %d\n", winnerArray[i]);
            }
            printf("ワイルドカードは一人\n");
            #endif
            /*
            while(true){
            int result = janken_two_people(whoLoseArray[0], whoLoseArray[1], winnerArray, playersArray);
            if (result == 0) {
            break;
        }
    }
    */

            } else {
                // ワイルドカードが一枚もなかった場合
                #ifdef DEBUG
                    printf("WILD COUNT : %d\n", resultWild);
                    showWinnerArray();
                    #endif

                int resultThree = janken_three_people(winnerArray);
                // 3人じゃんけんの結果を返す

                if (resultThree == AIKO) {
                    printf("AIKO\n");
                } else if (resultThree == ONEWINNER) {
                    printf("ONE WINNER\n");
                    // ここで勝者をwinnerArray[0]に入れて、のこり二人をjanken_two_people()でじゃんけんさせる
                } else if (resultThree == TWOWINNER) {
                    printf("TWO WINNER\n");
                    // ここで敗者をwinnerArray[2]に入れて、のこり二人をjanken_two_people()でじゃんけんさせる
                } else {
                    printf("IlligalState\n");
                    break;
                }
            }

        }

    return (0);
}

void showResult(){
    printf("結果は、\n");

    int i;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (winnerArray[i] == PLAYER) {
            printf("%d位: %s\n", i+1, "PLAYER");
        } else if (winnerArray[i] == COMONE) {
            printf("%d位: %s\n", i+1, "COM1");
        } else {
            printf("%d位: %s\n", i+1, "COM2");
        }
    }
}

// ３人でじゃんけんをする時に呼ばれるメソッド
// じゃんけんの結果を返す。
// return AIKO, ONEWINNER, TWOWINNER
// ただし、どのプレイヤが買ったのか判定できない
int janken_three_people(int data[3]){

    int result = (numPlayerHand + comOne + comTwo) % 3;
    if (result == 0) {
        return AIKO;
    } else if (result == 1) {
        return ONEWINNER;
    } else {
        return TWOWINNER;
    }
}

// プレイヤーコードを元に、二人でじゃんけんを行います
int janken_two_people(int playerCode1, int playerCode2, int data[3], int playerData[3][3]){
    int i;
    int position;
    // ここで、winnerArrayのどこが埋まっているのか判定する。
    for(i = 0; i < PLAYERCOUNT; i++){
        if (data[i] != 0) {
            position = i;
            break;
        }
    }

    int handOne = 0;
    int handTwo = 0;
    int j;
    for (j = 0; j < PLAYERCOUNT; j++) {
        if (playerData[j][0] == playerCode1) {
            handOne = playerData[j][1];
        }

        if (playerData[j][0] == playerCode2) {
            handTwo = playerData[j][1];
        }
    }

    // ここから実際のじゃんけんを行う
    if (handTwo == handOne) {
        return 1;
    }

    if (handOne == SCISSORS && handTwo == ROCK) {
        // return code2;
    } else if (handOne == ROCK && handTwo == SCISSORS) {
        // return code1;
    }else if (handOne < handTwo) {
        // return code2;
    } else {

    }

    return 0;
}

// ワイルドカードを使ってしまった人を記録します
void updateWildCardOnPlayersArray(){
    int r;
    for(r = 0; r < PLAYERCOUNT; r++){
        if (showHandFromPlayer(playersArray[r][0]) == 4) {
            playersArray[r][2] = WILDTRUE;
        }
    }
}

// 勝者を順に格納しているwinnerArrayを表示します
void showWinnerArray(){
    int i;
    for (i = 0; i < 3; i++) {
        printf("winnerArray: %d\n", winnerArray[i]);
    }
}

// playersArrayを、表形式で表示します
void showPlayerArray(){
    int i, j;
    printf("P\tH\tW\n");
    for (i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++){
            if (j == 2) {
                printf("%d\n", playersArray[i][j]);
            }else {
                printf("%d\t", playersArray[i][j]);
            }
        }
    }
}

// プレイヤーコードを元に、その人の手を表示します
int showHandFromPlayer(int code){
    int hand;
    int i;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (playersArray[i][0] == code) {
            hand = playersArray[i][1];
            break;
        }
    }

    return hand;
}

// プレイヤーの手にあるワイルドカードの枚数をチェックする
// Wildカードがない場合は0を、一枚だけある場合は1を、複数人の場合は2を返す
int checkWildCard(int handOne, int handTwo, int handThree){
    int array[PLAYERCOUNT] = {handOne, handTwo, handThree};

    int i;
    int total = 0;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (array[i] == 4) {
            total++;
        }
    }

    if (total == PLAYERCOUNT) {
        return 2;
    }else {
        return total;
    }
}
