#include <stdbool.h>

#define NUM_SLOTS 15

enum SlotState{Peg=0, Open=1, Legal=2, Optimal=3};
enum SlotColor{Black=0, Red=1, Green=2, Yellow=3};
enum GameMode{Hints=0,Regular=1,Timed=2};

typedef struct _Slot{
    int number;             // the ID of that slot
    int  neighbors[NUM_SLOTS];      // whether each other pin is a valid jump spot
                            // The int value at that index is the slot
                            // that would be jumped over. If it is -1, that
                            // is not a valid jump.
    enum SlotState state;   // The state of the slot
} Slot;

Slot* slots;
int ActiveSlot;
int pegsRemaining;

void initSlots();
void resetBoard();

bool jump(int dest); // return if move is legal

void updateSlots(); // update state of all slots per currently selected active peg
bool gameOver(); // return if no legal moves remain

int min(int x, int y);
void descend(int slot); // changes a slot down to Open if it is Legal or higher
void elevate(int slot); // changes a slot up to Legal if it is Open or Optimal

void findOptimal(); // sets the optimal move for the current active

bool pinSelect(int slot); // figures out what to do based on what pin is pushed

// Move optimizer info

typedef struct _Move{
    char src;
    char dest;
} Move;

typedef struct _MoveList{
    char numMoves;
    Move moves[64];
} MoveList;

typedef struct _Jumps{
    char src;
    char jmpd;
} Jumps;

typedef struct _MoveValue{
    Move move;
    float value;
} MoveValue;

typedef struct _MoveValueList{
    char numMoves;
    MoveValue moveValues[64];
} MoveValueList;

char jumps[15][15];
bool board[15];
void initJumps();
void initBoard();

MoveList moves(bool* b);
void makemove(bool* b, Move m);
void unmove(bool* b, Move m);
int gameover(bool* b);
MoveValue minimize(bool* b, MoveValue best, int k);
MoveValue optimize(bool* b, int k);
float lookahead(bool* b, int k);
MoveValueList lookaheadoptions(bool* b, int k);

void showboard(bool* b);
char c(bool peg);
void doOpt(bool* b);
void showlookahead(bool* b, int k);
