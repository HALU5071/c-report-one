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

// メソッド宣言
int janken_two_people(int player1, int player2);
int janken_three_people();
int checkWildCard(int handOne, int handTwo, int handThree);

// コンピュータ1, 2の手を格納するためのグローバル変数
int comOne = 0;
int comTwo = 0;
// Playerの手を格納するためのグローバル変数
int numPlayerHand = 0;

// 勝利順を格納するためのグローバル配列
int winnerArray[PLAYERCOUNT];

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

// プレイヤーコードを格納する。各プレイヤーコードをじゃんけん用のメソッドに渡して
// プレイヤーを識別する
  const int codeComOne = COMONE;
  const int codeComTwo = COMTWO;
  const int codePlayer = PLAYER;

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
    // コンピュータの手をランダムに出力
    // シード値の作成
    init_genrand((unsigned)time(NULL));
    comOne = genrand_int32()%4 + 1;

    init_genrand((unsigned)time(NULL) + 100);
    comTwo = genrand_int32()%4 + 1;

    #ifdef DEBUG
    printf("com1:%d\n", comOne);
    printf("com2:%d\n", comTwo);
    printf("numPlayerHand : %d\n", numPlayerHand);

    int tmp2 = numPlayerHand + comOne + comTwo;
    printf("tmp2 : %d\n", tmp2);
    #endif

    // ここでワイルドカードの処理をする
    int resultWild = checkWildCard(numPlayerHand, comOne, comTwo);
    if (resultWild == 2) {
        // 複数人のワイルドカードが出たら、あいこと同じ扱い。
        // whileを抜ける
        break;
    }else if (resultWild == 1) {
        // 一人勝ちのパターン。一人の勝利が確定
        // 二人のみのじゃんけんに移行
    }

    #ifdef DEBUG
    printf("WILD COUNT : %d\n", resultWild);
    #endif

    int resultThree = janken_three_people();
    // 3人じゃんけんの結果を返す
    if (resultThree == AIKO) {
        printf("AIKO\n");
        break;
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

    #ifdef DEBUG
    int resultTwo = janken_two_people(codePlayer, codeComTwo);
    printf("resultTwo: %d\n", resultTwo);
    #endif

  }

  return (0);
}

// ３人でじゃんけんをする時に呼ばれるメソッド
// じゃんけんの結果を返す。
// return AIKO, ONEWINNER, TWOWINNER
// ただし、どのプレイヤが買ったのか判定できない
int janken_three_people(){

    int result = (numPlayerHand + comOne + comTwo) % 3;
    if (result == 0) {
        return AIKO;
    } else if (result == 1) {
        return ONEWINNER;
    } else {
        return TWOWINNER;
    }
}

int janken_two_people(int playerCode1, int playerCode2){
    int code1 = playerCode1;
    int code2 = playerCode2;
    int handOne = 0;
    int handTwo = 0;

    // 要改善
    // player1, player2からもらってきたプレイヤーコードから、
    // それぞれのプレイやーの手を格納していく
    if (code1 == COMONE) {
        handOne = comOne;
    } else if (code1 == COMTWO) {
        handOne = comTwo;
    } else {
        handOne = numPlayerHand;
    }

    if (code2 == COMONE) {
        handTwo = comOne;
    } else if (code2 == COMTWO) {
        handTwo = comTwo;
    } else  {
        handTwo = numPlayerHand;
    }

    // ここから実際のじゃんけんを行う
    if (handTwo == handOne) {
        return AIKO;
    }

    if (handOne == SCISSORS && handTwo == ROCK) {
        return code2;
    } else if (handOne == ROCK && handTwo == SCISSORS) {
        return code1;
    }else if (handOne < handTwo) {
        return code2;
    } else {
        return code1;
    }
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
