#include <pegs.h>
#include <stdio.h>

Slot slots[NUM_SLOTS];
int ActiveSlot; // the currently selected slot


int min(int x, int y){
    if (x < y){
        return x;
    }
    else{
        return y;
    }
}

void descend(int slot){
    if (slots[slot].state > 0){
        slots[slot].state = Open;
    }
}

void elevate(int slot){
    if (slots[slot].state > 0){
        slots[slot].state = Legal;
    }
}

void updateSlots(){
    for(int i = 0; i < NUM_SLOTS; i++){
        // Check the neighbor status of each slot
        if(slots[ActiveSlot].neighbours[i] < 0){
            descend(i);
        }
        else{
            elevate(i);
        }
    }
}

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
