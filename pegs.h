#include <stdbool.h>

enum slotState{Peg=0, Open=1, Legal=2, Optimal=3};

typedef struct _slot{
    int number;             // the ID of that slot
    bool[15] neighbors;     // whether each other pin is a valid jump spot
                            // NOT DIRECTLY ADJACENT
    enum slotState state;   // The state of the slot
}
