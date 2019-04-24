# miniProject


It's the pin jumping game from Cracker Barrel. Jump pins to remove them.

## LCD:

|      |Before game | In game | After Game|
| ---- | ----------- | ------- | --------- |
| **LCD2** | Team24!!!!! | Selected slot # | Call the player an idiot
|          | Turn dial to select game mode - Press button to continue | Hint | High Score (pins, time)
| **LCD1** | Current selected game mode | Time spent/remaining| time spent/remaining 
|          | description of game mode   | Pins remaining | pins remaining

## LEDs
|     | Red on | Red off|
|-----| ----| ------|
| Green on | Selected peg (yellow) | Legal move (green) |
| Green off| Peg (red)  | Open (no light) |

## Game Modes
Easy - hints
Normal - no hints, no time limit
Hard - time limit

## Game/Hardware Interface

Interrupt handler for button press should pass the number of the peg slot pressed to slotSelect(int).
Numerous colors will call setColor(int, SlotColor), which will pass the number of the slot to change color and the color to change it to. See pegs.h for the SlotColor Enum.

LCD:


## Criteria for Success
Criteria for success: Successfully have a working game with the LCD screen
> displaying accurate results.
Solver and hint system
Stats, mode select, instructions
