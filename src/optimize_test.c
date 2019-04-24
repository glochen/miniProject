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

    return 0;
}
