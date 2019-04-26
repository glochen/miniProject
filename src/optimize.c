#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pegs.h"

void initJumps(){
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++){
            jumps[i][j] = (char)63;
        }
    }

    jumps[0][3]  =(char) 1;
    jumps[0][5]  =(char) 2;
    jumps[1][6]  =(char) 3;
    jumps[1][8]  =(char) 4;
    jumps[2][7]  =(char) 4;
    jumps[2][9]  =(char) 5;
    jumps[3][0]  =(char) 1;
    jumps[3][5]  =(char) 4;
    jumps[3][12] =(char) 7;
    jumps[3][10] =(char) 6;
    jumps[4][11] =(char) 7;
    jumps[4][13] =(char) 8;
    jumps[5][3]  =(char) 4;
    jumps[5][0]  =(char) 2;
    jumps[5][12] =(char) 8;
    jumps[5][14] =(char) 9;
    jumps[6][1]  =(char) 3;
    jumps[6][8]  =(char) 7;
    jumps[7][2]  =(char) 4;
    jumps[7][9]  =(char) 8;
    jumps[8][1]  =(char) 4;
    jumps[8][6]  =(char) 7;
    jumps[9][2]  =(char) 5;
    jumps[9][7]  =(char) 8;
    jumps[10][3] =(char) 6;
    jumps[10][12]=(char)11;
    jumps[11][4] =(char) 7;
    jumps[11][13]=(char)12;
    jumps[12][10]=(char)11; 
    jumps[12][3] =(char) 7;
    jumps[12][5] =(char) 8;
    jumps[12][14]=(char)13;
    jumps[13][11]=(char)12;
    jumps[13][4] =(char) 8;
    jumps[14][12]=(char)13;
    jumps[14][5] =(char) 9;
}

void initBoard(){
    bool temp[] = {false, true, true, true, true,
             true, true, true, true, true,
             true, true, true, true, true};
    memcpy(board, temp, sizeof(temp));
}

MoveList moves(bool* b){
    MoveList ret;
    ret.numMoves = 0;
    for(int p = 0; p < 15; p++){ // for all slots with a peg
        if (!b[p]) continue;
        for(int n = 0; n < 15; n++){ // for all slots with no peg
            if (b[n]) continue;
            if (jumps[p][n] != 63 && b[jumps[p][n]]){
                ret.moves[ret.numMoves].src = p;
                ret.moves[ret.numMoves].dest= n;
                ret.numMoves++;  
            }
        }
    }
    return ret;
}

bool* makemove(bool* b, Move m){
    char jumped = jumps[m.src][m.dest];
    b[(int)jumped] = false;
    b[m.src] = false;
    b[m.dest] = true;
    return b;
}
bool* unmove(bool* b, Move m){
    char jumped = jumps[m.src][m.dest];
    b[(int)jumped] = true;
    b[m.src] = true;
    b[m.dest] = false;
    return b;
}
    
int gameover(bool* b){
    MoveList tmp = moves(b);
    if (tmp.numMoves == 0){
        int count = 0;
        for(int i = 0; i < 15; i++){
            count += (int) b[i];
        }
        return count;
    }
    else{
        return 0;
    }
}

MoveValue minimize(bool* b, MoveValue best, int k){
    if (gameover(b)){
        MoveValue ret = {.move = {.src=63, .dest=63}, .value = gameover(board)};
        return ret;
    }
    MoveList ms = moves(b);
    for (int i = 0; i < ms.numMoves; i++){
        MoveValue score = minimize(makemove(b, ms.moves[i]), best, k-1);
        if (score.value < best.value){
            best.move = ms.moves[i];
            best.value = score.value;
        }
        unmove(b, ms.moves[i]);
        if (score.value == 1){
            //printf("\n~~~~\n");
            //printf("K = %d\n", k);
            //makemove(b, ms.moves[i]);
            //showboard(b);
            //unmove(b, ms.moves[i]);
            //printf("(%d, %d)\t%.2f\n", best.move.src, best.move.dest, best.value);
            //printf("score.value: %.2f", score.value);
            return best;
        }
    }
    return best;
}

MoveValue optimize(bool* b, int k){
    Move bestMove = {.src=-1, .dest=-1};
    MoveValue ret = {.move=bestMove, .value=99};
    // ret = minimize(b, ret, k);
    MoveValueList ms = lookaheadoptions(b, k);
    for (int i=0; i < ms.numMoves; i++){
        MoveValue cur = ms.moveValues[i];
        if (cur.value > ret.value){
            ret.move = cur.move;
            ret.value = cur.value;
        }
    }
    return ret;
}

void doOpt(bool* b){
    showboard(b);
    MoveValue mv = optimize(b, 13);
    printf("(%d, %d)\t%.2f\n", mv.move.src, mv.move.dest, mv.value);
    makemove(b, mv.move);
    showboard(b);
    printf("~~~~~~~~\n");
}

float lookahead(bool* b, int k){
    if (gameover(b)){
        return (int) gameover(b) == 1;
    }
    else{
        float total = 0;
        MoveList ms = moves(b);
        if (k == 0){
            for (int i = 0; i < ms.numMoves; i++){
                makemove(b, ms.moves[i]);
                Move bestMove = {.src=-1, .dest=-1};
                MoveValue value = {.move=bestMove, .value = 99};
                value = minimize(b, value, 13);
                unmove(b, ms.moves[i]);
                total += (int) value.value==1;
            }
        }
        else{
            for (int i = 0; i < ms.numMoves; i++){
                makemove(b, ms.moves[i]);
                float moveValue = lookahead(b, k-1);
                unmove(b, ms.moves[i]);
                total += moveValue;
            }
        }
        float avg = total/ms.numMoves;
        return avg;
    }
}

MoveValueList lookaheadoptions(bool* b, int k){
    MoveList options = moves(b);
    MoveValueList values;
    values.numMoves = options.numMoves;
    for(int i = 0; i < options.numMoves; i++){
        makemove(b, options.moves[i]);
        float val = lookahead(b, k-1);
        values.moveValues[i].move = options.moves[i];
        values.moveValues[i].value = val;
        unmove(b, options.moves[i]);
    }
    return values;
}

void showboard(bool* b){
    printf("    %c\n", c(b[0]));
    printf("   %c %c\n", c(b[1]), c(b[2]));
    printf("  %c %c %c\n", c(b[3]), c(b[4]), c(b[5]));
    printf(" %c %c %c %c\n", c(b[6]), c(b[7]), c(b[8]), c(b[9]));
    printf("%c %c %c %c %c\n", c(b[10]), c(b[11]), c(b[12]), c(b[13]), c(b[14]));
}

char c(bool peg){
    if (peg){
        return 'X';
    }
    else{
        return 'O';
    }
}

void showlookahead(bool* b, int k){
    showboard(b);
    MoveValueList values = lookaheadoptions(b, k);
    for (int i = 0; i < values.numMoves; i++){
        MoveValue cur = values.moveValues[i];
        printf("(%d, %d)\t%.2f\n", cur.move.src, cur.move.dest, cur.value);
    }
    printf("~~~~~~~~~~~~~\n");
}

bool* boardFromSlots(Slots* slots){
    for (int i = 0; i < NUM_SLOTS; i++){
        board[i] = slots[i].state == Peg;
    }
    return board;
}

bool legalMovesLeft(){
    boardFromSlots(slots);
    MoveList ms = move(board);
    return (ms.numMoves > 0);
}

void findOptimal(){
    if (mode == Easy){
        boardFromSlots(slots);
        MoveValue optimal = optimize(board, 13);
        //slots[optimal.dest =
        
        // TODO figure out what to do with this
        // Just set the optimal dest to yellow?
        // Flash the color?
        // Flash yellow for optimal src and dest?
        // Disply hint on LCD?

        // Also need to figure out what to do with updateSlots -
        // make sure that the process for updating lights actually works
    }
}
