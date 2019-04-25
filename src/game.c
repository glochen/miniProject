#include "pegs.h"
#include <stdio.h>

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
    /*if(ActiveSlot < 0){
        for(int i = 0; i < NUM_SLOTS; i++){
            if(slots[i].state == Legal){
                slots[i].state = Peg;
            }
        }
        return;
    }*/
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
        // if a neighbor, but between slot is empty, descend to Open
        else{
            descend(i);
        }
    }
 //   findOptimal();
}

bool gameOver(){
    // if there's only one pin left, the game must be over
    if (pegsRemaining == 1){
        return true;
    }
    for(int s = 0; s < NUM_SLOTS; s++){
        // if any slot is a legal move, return false
        if(slots[s].state >= Legal){
            return false;
        }
    }
    if(getMode() == 3 && getTime() >= MAX_TIME){
        return true;
    }
    // otherwise, game over
    return false;
}

bool jump(int dest){
    // If slot numbers are not in the correct range, return failure
    if (dest < 0 || dest >= NUM_SLOTS){
        return false; 
    }
    if (slots[dest].state >= Legal){
        // move peg to new slot, change color and state
        slots[ActiveSlot].state = Open;
        slots[ActiveSlot].color = Black;
        slots[dest].state = Peg;
        slots[dest].color = Red;
        setLights(slots);
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
    if(ActiveSlot < 0){
        // Check that there is a peg in that slot
        if(slots[slot].state != Peg){
            // todo print error message to lcd (no peg in slot)
            return false;
        }
        // select slot as active peg
        // display LCD
        char line[20];
        sprintf(line, "Selected peg: %d", slot + 1);
        display2_line1(line);
        // update color
        slots[slot].color = Green;
        setLights(slots);
        ActiveSlot = slot;
        updateSlots();
        return true;
    }
    // if active peg
    else{
        if(slot == ActiveSlot){ // deselect spot
            display2_line1("Selected peg: ");
            slots[slot].color = Red;
            setLights(slots);
            ActiveSlot = -1;
            updateSlots();
            return true;
        }
        // check that the slot is a legal jump location
        if(slots[slot].state < Legal){
            display2_line1("Invalid move");
            return false;
        }
        else{
            jump(slot);
            char line[20];
            sprintf(line, "Moved to: %d", slot + 1);
            display2_line1(line);
            return true;
        }
    }
}

/* void startGame(...){
 * init_slots();
 * while(!gameOver){
 *      int p = getPin();
 *      slotSelect(p);
 *      display stuff
 *      make sure lights are updated
 * }
 * display whatever shows after game
 * implement showHighScore(mode)
 * display final game stuff
 * }
 *
 * change gameOver to check time remaining
 */

void startGame(){
    initSlots();
    setLights(slots);
    display1_line1("Peg Game");
    int slot = get_button_pressed();
    while(slot == -1){
        printLCD(1, 2, "         Press any button to start      ", 50);
        slot = getSlot();
    }
    display1_line1("");
    display1_line2("");
    display2_line1("Selected peg: ");
    display2_line2("");
    endTimer15();
    init_tim2();
    int mode = getMode();
    // todo different things based on mode
    while(!gameOver()){
        slot = getSlot();
        slotSelect(slot);
    }
}

void initSlots(){
    ActiveSlot = -1;
    for(int i = 0; i < NUM_SLOTS; i++){
        slots[i].state = Peg;
        slots[i].color = Red;
        slots[i].number = i;
        for(int j = 0; j < NUM_SLOTS; j++){
            slots[i].neighbors[j] = -1;
        }
    }
    slots[0].state = Open;
    slots[0].color = Black;
    slots[0].neighbors[3] = 1;
    slots[0].neighbors[5] = 2;
    slots[1].neighbors[6] = 3;
    slots[1].neighbors[8] = 4;
    slots[2].neighbors[7] = 4;
    slots[2].neighbors[9] = 5;
    slots[3].neighbors[0] = 1;
    slots[3].neighbors[5] = 5;
    slots[3].neighbors[12] = 7;
    slots[3].neighbors[10] = 6;
    slots[4].neighbors[11] = 7;
    slots[4].neighbors[13] = 8;
    slots[5].neighbors[3] = 4;
    slots[5].neighbors[0] = 2;
    slots[5].neighbors[12] = 8;
    slots[5].neighbors[14] = 9;
    slots[6].neighbors[1] = 3;
    slots[6].neighbors[8] = 7;
    slots[7].neighbors[2] = 4;
    slots[7].neighbors[9] = 8;
    slots[8].neighbors[1] = 4;
    slots[8].neighbors[6] = 7;
    slots[9].neighbors[2] = 5;
    slots[9].neighbors[7] = 8;
    slots[10].neighbors[3] = 6;
    slots[10].neighbors[12] = 11;
    slots[11].neighbors[4] = 7;
    slots[11].neighbors[13] = 12;
    slots[12].neighbors[10] = 11;
    slots[12].neighbors[3] = 7;
    slots[12].neighbors[5] = 8;
    slots[12].neighbors[14] = 13;
    slots[13].neighbors[11] = 12;
    slots[13].neighbors[4] = 8;
    slots[14].neighbors[12] = 13;
    slots[14].neighbors[5] = 9;
}

