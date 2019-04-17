#include <stdbool.h>

#define NUM_SLOTS 15

enum SlotState{Peg=0, Open=1, Legal=2, Optimal=3};
enum GameMode{Hints=0,Regular=1,Timed=2};

typedef struct _Slot{
    int number;             // the ID of that slot
    int[NUM_SLOTS] neighbors;      // whether each other pin is a valid jump spot
                            // The int value at that index is the slot
                            // that would be jumped over. If it is -1, that
                            // is not a valid jump.
    enum SlotState state;   // The state of the slot
}

bool jump(int source, int dest); // return if move is legal
bool remove(int source, int dest); // return if move is legal

void updateSlots(); // update state of all slots per currently selected active peg
bool gameOver(); // return if no legal moves remain

int min(int x, int y);
void descend(int slot); // changes a slot down to Open if it is Legal or higher
void elevate(int slot); // changes a slot up to Legal if it is Open or Optimal

void findOptimal(); // sets the optimal move for the current active
