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
#define WILDFALSE 10
#define WILDTRUE 11

// メソッド宣言
int jankenTwoPeople(int player1, int player2, int data[3], int playerData[3][3]);
int jankenThreePeople(int data[3]);
int checkWildCard(int handOne, int handTwo, int handThree);
void showResult();
int getHandFromPlayerCode(int code);
int getWildCardFromPlayerCode(int code);
void showPlayerArray();
void updateWildCardOnPlayersArray();
void showWinnerArray();
void showAIKO();
void showMultipleWildCard();
void showOneWinner();
void showTwoWinner();
void showDummyCode();
int generateJankenHand(int seed, int code);
int generateJankenHandTo4(int seed);
int generateJankenHandTo3(int seed);
int convertPlayerHandFromInput(char a);
int autoJanken();
int searchDifferentHand(int data[3][3]);
void createLastPlayersArray(int data[2], int differentHand);
int determineJankenTwo(int data[2]);
int manualJankenWithComputer(int comCode);
void showLastArray(int data[2]);
int postHandFromCode(int code, int hand);
void initWildCard();

// コンピュータ1, 2の手を格納するためのグローバル変数
int comOne = 0;
int comTwo = 0;
// Playerの手を格納するためのグローバル変数
int numPlayerHand = 0;

// 勝利順を格納するためのグローバル配列
int winnerArray[PLAYERCOUNT];
// 上の配列に入れる文字列を３つ用意しておく
const char playerName[8] = "Player";
const char computerOneName[16] = "Computer 1";
const char computerTwoName[16] = "Computer 2";

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
    initWildCard();

    int firstTimeFlag = 0;

    while(1){

      if (firstTimeFlag == 0) {
        initWildCard();
      }
        printf("You throw >> ");
        // 入力された手を格納しておく変数 char型
        char yourHand = '\0';

        scanf("%s", &yourHand);

        // 入力された手を数字に変換する
        // ここで正しい手を入力したかチェック
        // ワイルドカードの複数使用もここで制限できる
        int numPlayerHand = convertPlayerHandFromInput(yourHand);
        if (numPlayerHand == '\0') {
            continue;
        }

        // プレイヤーの手を格納
        playersArray[0][1] = numPlayerHand;

        // コンピュータの手をランダムに出力
        // シード値の作成
        comOne = generateJankenHand(0, playersArray[1][0]);
        playersArray[1][1] = comOne;

        comTwo = generateJankenHand(100, playersArray[2][0]);
        playersArray[2][1] = comTwo;

        #ifdef DEBUG
        printf("DEBUG: plyr: %d\n", numPlayerHand);
        printf("DEBUG: com1: %d\n", comOne);
        printf("DEBUG: com2: %d\n", comTwo);

        showPlayerArray();

        int tmp2 = playersArray[0][1] + playersArray[1][1] + playersArray[2][1];
        printf("DEBUG: tmp2 : %d\n", tmp2);
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
                    printf("DEBUG: Who Lose: %d\n", lastArray[o]);
                }
                showWinnerArray();
                printf("DEBUG: ワイルドカードは一人\n");
            #endif

            determineJankenTwo(lastArray);
            showResult();


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
                continue;
            } else if (resultThree == ONEWINNER) {
                showOneWinner();
                int diffHand = searchDifferentHand(playersArray);
                // ここで勝者をwinnerArray[0]に入れて、のこり二人をjankenTweople()でじゃんけんさせる
                winnerArray[0] = diffHand;

                #ifdef DEBUG
                    printf("DEBUG: Diff: %d\n", diffHand);
                    showWinnerArray();
                #endif

                createLastPlayersArray(lastArray, diffHand);
                determineJankenTwo(lastArray);
                showResult();
                firstTimeFlag = 0;

            } else if (resultThree == TWOWINNER) {
                showTwoWinner();

                #ifdef DEBUG
                    showWinnerArray();
                #endif

                int diffHand = searchDifferentHand(playersArray);

                #ifdef DEBUG
                    printf("DEBUG: Diff: %d\n", diffHand);
                #endif

                // ここで敗者をwinnerArray[2]に入れて、のこり二人をjankenThreePeople()でじゃんけんさせる
                winnerArray[2] = diffHand;
                createLastPlayersArray(lastArray, diffHand);
                // ふたりじゃんけんの方針を決定します。
                determineJankenTwo(lastArray);
                showResult();
                firstTimeFlag = 0;

                #ifdef DEBUG
                    showWinnerArray();
                #endif

            } else {
                printf("DEBUG: IllegalState\n");
            }

            break;
        }

        firstTimeFlag = 1;
    }
    return (0);
}

// ふたりのじゃんけんをどのように行うのかを決定します。
// 残っている二人にプレイヤー自身が含まれているなら、入力を元にするじゃんけん
// 残っている二人がコンピュータなら、autoJanken()を呼び出し、自動的にじゃんけんをおこないます。
int determineJankenTwo(int data[2]){
    int j;
    int position = 0;
    int flag = 0;
    for(j = 0; j < 2; j++){
        if (data[j] == PLAYER) {
            flag = 1;
        } else {
            position = j;
        }
    }

    int result = 0;
    if (flag == 0) {
        result = autoJanken();
    } else if (flag == 1) {
        #ifdef DEBUG
            printf("DEBUG: data[position]: %d\n", data[position]);
        #endif
        int code = data[position];
        result = manualJankenWithComputer(code);
    } else {
        result = '\0';
        printf("DEBUG: IllegalState\n");
    }

    return result;
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
void createLastPlayersArray(int data[2], int differentHand){
    int cursor = 0;
    int t;
    for(t = 0; t < 3; t++){
        if (playersArray[t][0] != differentHand) {
            data[cursor] = playersArray[t][0];
            cursor++;
        }
    }
    showLastArray(data);
}

void showLastArray(int data[2]){
  int i;
  for(i = 0; i < 2; i++){
    printf("DEBUG: lastArray[%d]: %d\n", i, data[i]);
  }
}

// 結果を表示します
void showResult(){
    printf("*******************************************\n");
    printf("Result: \n");

    int i;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (winnerArray[i] == PLAYER) {
            printf("%d : %s\n", i+1, playerName);
        } else if (winnerArray[i] == COMONE) {
            printf("%d : %s\n", i+1, computerOneName);
        } else {
            printf("%d : %s\n", i+1, computerTwoName);
        }
    }
    printf("*******************************************\n");
}


// ただし、どのプレイヤが買ったのか判定できない
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
    int code2 = playerCode2;
    // ここで、winnerArrayのどこが埋まっているのか判定する。
    for(i = 0; i < PLAYERCOUNT; i++){
        if (data[i] != 0) {
            position = i;
            break;
        }
    }

    int code1 = playerCode1;

    #ifdef DEBUG
        printf("DEBUG: second: %d\n", playerCode2);
        printf("DEBUG: third: %d\n", code2);
    #endif

    int handOne = 0;
    int handTwo = 0;
    int j;
    for (j = 0; j < PLAYERCOUNT; j++) {
        if (playerData[j][0] == playerCode1) {
            handOne = playerData[j][1];

            #ifdef DEBUG
                printf("DEBUG: jankenTwoPeople: %d: %d\n", playerCode1, handOne);
            #endif
        }

        if (playerData[j][0] == playerCode2) {
            handTwo = playerData[j][1];

            #ifdef DEBUG
                printf("DEBUG: jankenTwoPeople: %d: %d\n", playerCode2, handTwo);
            #endif
        }
    }

    // ここから実際のじゃんけんを行う
    if (handTwo == handOne) {
        return 1;
    }

    if (handOne == SCISSORS && handTwo == ROCK) {
        if (position == 0) {
            winnerArray[1] = playerCode2;
            winnerArray[2] = playerCode1;
        } else {
            winnerArray[1] = playerCode1;
            winnerArray[0] = playerCode2;
        }
    } else if (handOne == ROCK && handTwo == SCISSORS) {
        if (position == 0) {
            winnerArray[1] = playerCode1;
            winnerArray[2] = playerCode2;
        } else {
            winnerArray[1] = playerCode2;
            winnerArray[0] = playerCode1;
        }
    }else if (handOne < handTwo) {
        if (position == 0) {
            winnerArray[1] = playerCode2;
            winnerArray[2] = playerCode1;
        } else {
            winnerArray[1] = playerCode1;
            winnerArray[0] = playerCode2;
        }
    } else {
        if (position == 0) {
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
// ワイルドカードが 2 回以上入力されているばあいは \1 を返します
int convertPlayerHandFromInput(char a){
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

    // プレイヤーのワイルドカードの使用を確認する。
    if (playersArray[0][2] == WILDTRUE && hand == WILD) {
        printf("You have already used a wild card..\n");
        hand = '\0';
    }

    return hand;
}

// プレイヤーコードから、ワイルドカードの使用の有無を判定し、それを元にじゃんけんの手を生成します。
// 使っていなければ、WILDTRUEを使い、generateJankenHandTo4(seed)を呼び出し手を生成、
// つかっていれば、WILDFALSEを使い、generateJankenHandTo3(seed)を呼び出し手を生成します。
// エラーコードは空白文字です。
int generateJankenHand(int seed, int code){
    int wildCardBoolean = getWildCardFromPlayerCode(code);

    int hand = 0;

    if (wildCardBoolean == WILDFALSE) {
        hand = generateJankenHandTo4(seed);
    } else if (wildCardBoolean == WILDTRUE) {
        hand = generateJankenHandTo3(seed);
        #ifdef DEBUG
            printf("DEBUG: ComHand: ワイルドカードはすでに使用済みです。\n");
        #endif
    } else {
        printf("DEBUG: IllegalState\n");
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
        if (getHandFromPlayerCode(playersArray[r][0]) == 4) {
            playersArray[r][2] = WILDTRUE;
        }
    }
}

// 勝者を順に格納しているwinnerArrayを表示します
void showWinnerArray(){
    int i;
    for (i = 0; i < 3; i++) {
        printf("DEBUG: winnerArray[%d位]: %d\n",i+1 ,winnerArray[i]);
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

int getWildCardFromPlayerCode(int code){
    int wildCardBoolean;
    int i;
    for(i = 0; i < PLAYERCOUNT; i++){
        if (playersArray[i][0] == code) {
            wildCardBoolean = playersArray[i][2];
            break;
        }
    }

    return wildCardBoolean;
}

// プレイヤーコードを元に、その人の手を表示します
int getHandFromPlayerCode(int code){
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

void initWildCard(){
  int i;
  for(i = 0; i < PLAYERCOUNT; i++){
    playersArray[i][2] = WILDFALSE;
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

// 残りのプレイヤー二人のなかに、プレイヤー自身が含まれているときのじゃんけんをします。
int manualJankenWithComputer(int comCode){
    int code = 0;
    int* pointCode;
    code = comCode;

    #ifdef DEBUG
      printf("DEBUG : %d\n", code);
    #endif

    while (1) {
        printf("You throw >> ");
        char yourHand = '\0';
        scanf("%s", &yourHand);
        int numPlayerHand = convertPlayerHandFromInput(yourHand);

        int hand = generateJankenHand(100, code);
        postHandFromCode(code, hand);

        if (numPlayerHand == '\0') {
            continue;
        }

        #ifdef DEBUG
            printf("DEBUG: first: %d\n", code);
        #endif

        // プレイヤーの手を格納
        playersArray[0][1] = numPlayerHand;
        int result = jankenTwoPeople(playersArray[0][0], code, winnerArray, playersArray);

        if (result == 0) {
            break;
        }
    }

    return 1;
}

// 与えられたプレイヤーコードを元に、じゃんけんの手をplayersArrayに入力します
// 成功時には 0 を、失敗時には 1 を返します。
int postHandFromCode(int code, int hand){
  if (code == PLAYER) {
    playersArray[0][1] = hand;
  } else if (code == COMONE) {
    playersArray[1][1] = hand;
  } else if (code == COMTWO) {
    playersArray[2][1] = hand;
  } else {
    printf("IllegalState\n");
    return 1;
  }

  if (getHandFromPlayerCode(code) == hand) {
    return 0;
  } else {
    return 1;
  }
}

// コンピュータ二人で行うじゃんけんを自動化するメソッドです。
int autoJanken(){
    while (1) {
        playersArray[1][1] = generateJankenHand(199, playersArray[1][0]);
        playersArray[2][2] = generateJankenHand(12, playersArray[2][0]);

        int result = jankenTwoPeople(playersArray[1][0], playersArray[2][0], winnerArray, playersArray);
        if (result == 0) {
            break;
        }
    }

    showWinnerArray();
    #ifdef DEBUG
        printf("DEBUG: Com1: %d, Com2: %d\n", playersArray[1][1], playersArray[2][1]);
    #endif

    return 1;
}

// ダミー表示を行います。
void showDummyCode(){
    printf("DEBUG: This is Dummy method\n");
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
