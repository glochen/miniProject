#include <pegs.h>
#include <stdio.h>

Slot slots[NUM_SLOTS];
int ActiveSlot; // the currently selected slot

bool jump(int source, int dest){
    // If slot numbers are not in the correct range, return failure
    if (source < 0 || dest < 0 || source >= NUM_SLOTS || dest >= NUM_SLOTS){
        return false; 
    }
    // todo change this to use ActiveSlot states
    if (slots[source].neighbours[dest] && slots[dest].state >= Legal){
        remove(source, dest);
        slots[source].state = Open;
        slots[dest].state = Peg;
        return true;
    }
    else{
        return False;
    }
}
