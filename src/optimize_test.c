#include <stdio.h>
#include "stdbool.h"
#include "pegs.h"

int main(){
    printf("fuck\n");

    initJumps();
    initBoard();

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++){
            printf("%d ", jumps[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");

    showboard(board);

    MoveList m = moves(board);
    for(int i = 0; i < m.numMoves; i++){
        printf("(%d, %d)\n", m.moves[i].src, m.moves[i].dest);
    }
    makemove(board, m.moves[0]);
    showboard(board);
    printf("gameover: %d\n", gameover(board));
    unmove(board, m.moves[0]);
    showboard(board);
    printf("gameover: %d\n", gameover(board));
    for(int i = 0; i < 15; i++){
        board[i] = false;
    }
    board[0] = true;
    board[9] = true;
    showboard(board);
    printf("gameover: %d\n", gameover(board));
    for(int i = 0; i < 15; i++){
        board[i] = true;
    }
    board[0] = false;
    showboard(board);
    MoveValue t = {.move = {.src = 63, .dest = 63}, .value = 99};
    MoveValue mv = minimize(board, t, 14);
    printf("(%d, %d)\t%d\n", mv.move.src, mv.move.dest, mv.value);
    

    return 0;
}
