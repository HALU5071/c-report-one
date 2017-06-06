#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define WILD 4

int janken(int x[]);

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

  while(1){

    // コンピュータの手をランダムに出力
    srand((unsigned)time(NULL));
    int com1 = 0;
    com1 = rand()%4 + 1;

    int com2 = 0;
    com2 = rand()%4 + 1;

    #ifdef DEBUG
    printf("com1:%d\n", com1);
    printf("com2:%d\n", com2);
    // com1 = 1;
    // com2 = 2;
    #endif

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

    printf("numYourHand : %d\n", numYourHand);
    int arrayHand[3] = {numYourHand, com1, com2};
    // janken(arrayHand);

    // printf("これ以降の処理\n");

    int result = (numYourHand + com1 + com2) % 3;
    if (result == 0) {
        printf("あいこ\n");
        break;
    } else if (result == 1) {
        printf("まだ書いてないよ\n");
    } else {
        if (com1 == com2) {
            printf("1位 Player, 2位 com1, 2位 com2\n");
        } else if (com1 == numYourHand) {
            printf("1位 com2, 2位 Player, 2位 com1\n");
        } else {
            printf("1位 com1, 2位 Player, 2位 com2\n");
        }
    }

  }

  return (0);
}

int janken(int x[]){
    int size = sizeof(x);
    printf("size : %d\n", size);
}
