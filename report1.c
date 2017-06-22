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

// プレイヤーら特有の識別コード
#define PLAYER 1991
#define COMONE 2882
#define COMTWO 3773

// 状態を表現するコード
#define AIKO 111
#define ONEWINNER 222
#define TWOWINNER 333

// ワイルドカードの使用についてのコード
#define WILDNOTYET 0
#define WILDTRUE 1

// メソッド宣言
int jankenTwoPeople(int player1, int player2, int data[3], int playerData[3][3]);
int jankenThreePeople(int data[3]);
int checkWildCard(int handOne, int handTwo, int handThree);
void showResult();
int showHandFromPlayer(int code);
void showPlayerArray();
void updateWildCardOnPlayersArray();
void showWinnerArray();
void showAIKO();
void showMultipleWildCard();
void showOneWinner();
void showTwoWinner();
void showDummyCode();
int generateJankenHandTo4(int seed);
int generateJankenHandTo3(int seed);
int convertPlayerHand(char a);
void autoJanken();
int searchDifferentHand(int data[3][3]);
void createLastPlayerArray(int data[2], int differentHand);
void determinJankenTwo(int data[2]);

// コンピュータ1, 2の手を格納するためのグローバル変数
int comOne = 0;
int comTwo = 0;
// Playerの手を格納するためのグローバル変数
int numPlayerHand = 0;

// 勝利順を格納するためのグローバル配列
int winnerArray[PLAYERCOUNT];

// プレイヤーコードと、出した手、ワイルドカードを管理する配列
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

        // 入力された手を数字に変換する
        // ここで正しい手を入力したかチェック
        int numPlayerHand = convertPlayerHand(yourHand);
        if (numPlayerHand == '\0') {
            continue;
        }

        // プレイヤーの手を格納
        playersArray[0][1] = numPlayerHand;

        // コンピュータの手をランダムに出力
        // シード値の作成
        comOne = generateJankenHandTo4(0);
        playersArray[1][1] = comOne;

        comTwo = generateJankenHandTo4(100);
        playersArray[2][1] = comTwo;

        #ifdef DEBUG
        printf("plyr: %d\n", numPlayerHand);
        printf("com1: %d\n", comOne);
        printf("com2: %d\n", comTwo);

        showPlayerArray();

        int tmp2 = numPlayerHand + comOne + comTwo;
        printf("tmp2 : %d\n", tmp2);
        #endif

        // ここでワイルドカードの処理をする
        int resultWild = checkWildCard(playersArray[0][1], playersArray[1][1], playersArray[2][1]);

        // ワイルドカードについての情報を更新する
        updateWildCardOnPlayersArray();

        #ifdef DEBUG
        showPlayerArray();
        #endif

        if (resultWild == 2) {
            // 複数人のワイルドカードが出たら、あいこと同じ扱い。
            showMultipleWildCard();

        }else if (resultWild == 1) {
            // 一人勝ちのパターン。一人の勝利が確定
            // 二人のみのじゃんけんに移行
            int lastArray[2];
            int loseCounter = 0;
            int t;
            for(t = 0; t < 3; t++){
                if (playersArray[t][1] == 4) {
                    winnerArray[0] = playersArray[t][0];
                    playersArray[t][2] = WILDTRUE;
                } else {
                    lastArray[loseCounter] = playersArray[t][0];
                    loseCounter = 1;
                }
            }

            #ifdef DEBUG
                int o;
                for(o = 0; o < 2; o++){
                    printf("Who Lose: %d\n", lastArray[o]);
                }
                showWinnerArray();
                printf("ワイルドカードは一人\n");
            #endif

            determinJankenTwo(lastArray);


        } else {
            // ワイルドカードが一枚もなかった場合
            #ifdef DEBUG
                showWinnerArray();
            #endif

            int lastArray[2];

            int resultThree = jankenThreePeople(winnerArray);
            // 3人じゃんけんの結果を返す

            if (resultThree == AIKO) {
                showAIKO();
            } else if (resultThree == ONEWINNER) {
                showOneWinner();
                int diffHand = searchDifferentHand(playersArray);
                // ここで勝者をwinnerArray[0]に入れて、のこり二人をjankenTweople()でじゃんけんさせる
                winnerArray[0] = diffHand;

                #ifdef DEBUG
                    printf("Diff: %d\n", diffHand);
                #endif

                createLastPlayerArray(lastArray, diffHand);
                showWinnerArray();
                determinJankenTwo(lastArray);

            } else if (resultThree == TWOWINNER) {
                showTwoWinner();
                int diffHand = searchDifferentHand(playersArray);
                // ここで敗者をwinnerArray[2]に入れて、のこり二人をjankenThreePeople()でじゃんけんさせる
                winnerArray[2] = diffHand;
                createLastPlayerArray(lastArray, diffHand);
                showWinnerArray();
                // ふたりじゃんけんの方針を決定します。
                determinJankenTwo(lastArray);

                #ifdef DEBUG
                    printf("Diff: %d\n", diffHand);
                #endif

                break;

            } else {
                printf("IllegalState\n");
                break;
            }
        }

    }

    showResult();
    return (0);
}

// ふたりのじゃんけんをどのように行うのかを決定します。
// 残っている二人にプレイヤー自身が含まれているなら、入力を元にするじゃんけん
// 残っている二人がコンピュータなら、autoJanken()を呼び出し、自動的にじゃんけんをおこないます。
void determinJankenTwo(int data[2]){
    int j;
    int flag = 0;
    for(j = 0; j < 2; j++){
        if (data[j] == PLAYER) {
            flag = 1;
            break;
        }
    }

    if (flag == 0) {
        autoJanken();
    } else if (flag == 1) {
        showDummyCode();
    } else {
        printf("IllegalState\n");
    }

}

// 3つのなかで、違う手であるプレイヤーコードを返します
int searchDifferentHand(int data[3][3]){
    if (playersArray[0][1] == playersArray[1][1]) {
        return playersArray[2][0];
    } else if (playersArray[0][1] == playersArray[2][1]) {
        return playersArray[1][0];
    } else {
        return playersArray[0][0];
    }
}

// 残りのプレイヤーでじゃんけんをする際に用いる配列を生成します。
void createLastPlayerArray(int data[2], int differentHand){
    int cursor = 0;
    int t;
    for(t = 0; t < 3; t++){
        if (playersArray[t][0] != differentHand) {
            data[cursor] = playersArray[t][0];
            cursor++;
        }
    }
}

// 結果を表示します
void showResult(){
    printf("*******************************************\n");
    printf("Result: \n");

    int i;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (winnerArray[i] == PLAYER) {
            printf("%d : %s\n", i+1, "PLAYER");
        } else if (winnerArray[i] == COMONE) {
            printf("%d : %s\n", i+1, "COM1");
        } else {
            printf("%d : %s\n", i+1, "COM2");
        }
    }
    printf("*******************************************\n");
}

// ３人でじゃんけんをする時に呼ばれるメソッド
// じゃんけんの結果を返します。
// return AIKO, ONEWINNER, TWOWINNER
int jankenThreePeople(int data[3]){

    int result = (playersArray[0][1] + playersArray[1][1] + playersArray[2][1]) % 3;
    if (result == 0) {
        return AIKO;
    } else if (result == 1) {
        return ONEWINNER;
    } else {
        return TWOWINNER;
    }
}

// プレイヤーコードを元に、二人でじゃんけんを行います
// あいこだった場合は 0 を、勝敗がついた場合は 1 を返します
int jankenTwoPeople(int playerCode1, int playerCode2, int data[3], int playerData[3][3]){
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
        if (position == 1) {
            winnerArray[1] = playerCode2;
            winnerArray[2] = playerCode1;
        } else {
            winnerArray[1] = playerCode1;
            winnerArray[0] = playerCode2;
        }
    } else if (handOne == ROCK && handTwo == SCISSORS) {
        if (position == 1) {
            winnerArray[1] = playerCode1;
            winnerArray[2] = playerCode2;
        } else {
            winnerArray[1] = playerCode2;
            winnerArray[0] = playerCode1;
        }
    }else if (handOne < handTwo) {
        if (position == 1) {
            winnerArray[1] = playerCode2;
            winnerArray[2] = playerCode1;
        } else {
            winnerArray[1] = playerCode1;
            winnerArray[0] = playerCode2;
        }
    } else {
        if (position == 1) {
            winnerArray[1] = playerCode1;
            winnerArray[2] = playerCode2;
        } else {
            winnerArray[1] = playerCode2;
            winnerArray[0] = playerCode1;
        }
    }

    return 0;
}

// 入力された文字を元に、じゃんけんの手を表す整数に変換します。
// 入力が正しくない場合には、空白文字を返します
int convertPlayerHand(char a){
    int hand;
    if (a == 'c') {
        hand = SCISSORS;
    } else if (a == 'g') {
        hand = ROCK;
    } else if (a == 'p') {
        hand = PAPER;
    } else if (a == 'w') {
        hand = WILD;
    } else {
        printf("Press c g p w\n");
        hand = '\0';
    }

    return hand;
}

// 与えられた整数と時間を元に乱数を生成します。
// 返される値は、1, 2, 3, 4です
int generateJankenHandTo4(int seed){
    int hand = 0;
    init_genrand((unsigned)time(NULL) + seed);
    hand = genrand_int32()%4 + 1;

    return hand;
}

// 与えられた整数と時間を元に乱数を生成します。
// 返される値は、1, 2, 3です
int generateJankenHandTo3(int seed){
    int hand = 0;
    init_genrand((unsigned)time(NULL) + seed);
    hand = genrand_int32()%3 + 1;

    return hand;
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
        printf("winnerArray[%d位]: %d\n",i+1 ,winnerArray[i]);
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

// コンピュータ二人で行うじゃんけんを自動化するメソッドです。
void autoJanken(){
    while (1) {
        playersArray[1][1] = generateJankenHandTo4(199);
        playersArray[2][2] = generateJankenHandTo4(12);

        int result = jankenTwoPeople(playersArray[1][0], playersArray[2][0], winnerArray, playersArray);
        if (result == 0) {
            break;
        }
    }

    showWinnerArray();
    #ifdef DEBUG
        printf("Com1: %d, Com2: %d\n", playersArray[1][1], playersArray[2][1]);
    #endif
}

// ダミー表示を行います。
void showDummyCode(){
    printf("This is Dummy method\n");
}

void showAIKO(){
    printf("*******************************************\n");
    printf("***                AIKO                 ***\n");
    printf("*******************************************\n");
}

void showMultipleWildCard(){
    printf("*******************************************\n");
    printf("***      Multiple Wild Card. Again     ***\n");
    printf("*******************************************\n");
}
void showOneWinner(){
    printf("*******************************************\n");
    printf("***              ONE WINNER             ***\n");
    printf("*******************************************\n");
}

void showTwoWinner(){
    printf("*******************************************\n");
    printf("***              TWO WINNER             ***\n");
    printf("*******************************************\n");
}
