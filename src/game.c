#include <pegs.h>
#include <stdio.h>

Slot slots[NUM_SLOTS];
int ActiveSlot; // the currently selected slot
int pegsRemaining = NUM_SLOTS;

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

        // if not a neighbor, descend to open
        if(slots[ActiveSlot].neighbors[i] < 0){
            descend(i);
        }
        // if a neighbor and between slot has peg, elevate to legal
        else if(slots[slots[ActiveSlot].neighbors[i]].state == Peg){
            elevate(i);
        }
        // if a nieghbor, but between slot is empty, descend to Open
        else{
            descend(i);
        }
    }
    findOptimal()
}

bool gameOver(){
    // if there's only one pin left, the game must be over
    if (pegsRemaining == 1){
        return true;

    for(int s = 0; s < NUM_SLOTS; s++){
        // if any slot is a legal move, return false
        if(slots[s].state >= Legal){
            return false;
        }
    }
    // otherwise, game over
    return true;
}

bool jump(int dest){
    // If slot numbers are not in the correct range, return failure
    if (dest < 0 || dest >= NUM_SLOTS){
        return false; 
    }
    if (slots[dest].state >= Legal){
        remove(ActiveSlot, dest);
        // move peg to new slot
        slots[ActiveSlot].state = Open;
        slots[dest].state = Peg;
        // remove jumped peg
        slots[slots[ActiveSlot].neighbors[dest]].state = Open;
        pegsRemaining -= 1;
        // clear active slot
        ActiveSlot = -1;

        return true;
    }
    else{
        return false;
    }
}

bool slotSelect(int slot){
    // Check that the int passed is in the valid range
    if(slot < 0 || slot > NUM_SLOTS){
        // todo print error message to lcd
        return false;
    }
    // if no active peg
    if(activeSlot < 0){
        // Check that there is a peg in that slot
        if(slots[slot].state != SlotSate.Peg){
            // todo print error message to lcd (no peg in slot)
            return false;
        }
        // select slot as active peg
        activeSlot = slot;
        updateSlots();
        return true;
    }
    // if active peg
    else{
        // check that the slot is a legal jump location
        if(slots[slot].state < SlotState.Legal){
            // todo print error message (illegal jump location)
            return false;
        }
        else{
            jump(slot);
            return true;
        }
    }
}
