#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MT.h"

#define DEBUG

// 初期値0と混同することを回避
#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define WILD 4

#define PLAYER 1991
#define COMONE 2882
#define COMTWO 3773

int jankenTwoPeople(int player1, int player2);
int jankenThreePeople();

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
    // コンピュータの手をランダムに出力
    // シード値の作成
    init_genrand((unsigned)time(NULL));
    int comOne = 0;
    comOne = genrand_int32()%4 + 1;

    init_genrand((unsigned)time(NULL));
    int comTwo = 0;
    comTwo = genrand_int32()%4 + 1;

    printf("You throw >> ");
    // 入力された手を格納しておく変数 char型
    char yourHand = '\0';

    scanf("%s", &yourHand);

    // 入力された手を数字に変換しておくための変数
    // ここで正しい手を入力したかチェック
    int numYourHand = 0;
    if (yourHand == 'c') {
        numYourHand = SCISSORS;
    } else if (yourHand == 'g') {
        numYourHand = ROCK;
    } else if (yourHand == 'p') {
        numYourHand = PAPER;
    } else if (yourHand == 'w') {
        numYourHand = WILD;
    } else {
        printf("Press c g p w\n");
        continue;
    }

    #ifdef DEBUG
    printf("com1:%d\n", comOne);
    printf("com2:%d\n", comTwo);
    printf("numYourHand : %d\n", numYourHand);

    int tmp1 = numYourHand + comOne;
    int tmp2 = tmp1 + comTwo;
    printf("tmp2 : %d\n", tmp2);
    #endif

    int result = (numYourHand + comOne + comTwo) % 3;
    if (result == 0) {
        printf("あいこ\n");
        break;
    } else if (result == 1) {
        printf("まだ書いてないよ\n");
    } else {
        if (comOne == comTwo) {
            printf("1位 Player, 2位 com1, 2位 com2\n");
        } else if (comOne == numYourHand) {
            printf("1位 com2, 2位 Player, 2位 com1\n");
        } else {
            printf("1位 com1, 2位 Player, 2位 com2\n");
        }
    }

  }

  return (0);
}

// ３人でじゃんけんをする時に呼ばれるメソッド
int jankenThreePeople(){

}

int jankenTwoPeople(int player1, int player2){

}
