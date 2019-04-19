# miniProject


It's the pin jumping game from Cracker Barrel. Jump pins to remove them.

## LCD:

|      |Before game | In game | After Game|
| ---- | ----------- | ------- | --------- |
| **LCD1** |Welcome screen | Selected slot #, move to slot number # | Call the player an idiot
| **LCD2** |game mode select | Time and pins remaining (hint?)| time spent, pins remaining


## LEDs
|     | Red on | Red off|
|-----| ----| ------|
| Green on | Selected peg (yellow) | Legal move (green) |
| Green off| Peg (red)  | Open (no light) |

## Game/Hardware Interface

Interrupt handler for button press should pass the number of the peg slot pressed to slotSelect(int).
Numerous colors will call setColor(int, SlotColor), which will pass the number of the slot to change color and the color to change it to. See pegs.h for the SlotColor Enum.
