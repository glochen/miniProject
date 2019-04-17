#include <stdbool.h>

#define NUM_SLOTS 15

enum slotState{Peg=0, Open=1, Legal=2, Optimal=3};

typedef struct _Slot{
    int number;             // the ID of that slot
    bool[15] neighbors;     // whether each other pin is a valid jump spot
                            // NOT DIRECTLY ADJACENT
    enum slotState state;   // The state of the slot
}

bool jump(int source, int dest); // return if move is legal
bool remove(int source, int dest); // return if move is legal

void updateSlots(); // update state of all slots per currently selected active peg
bool gameOver(); // return if no legal moves remain
