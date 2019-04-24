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

### Easy
Hints available.
### Normal
No hints, no timer.
### Hard
No hints, time limit.



## Game/Hardware Interface

Press any button to begin game with selected game mode.
After game, press any button to return to game select.

### LCD
Software call `void printLCD(int lcd, int line, char* msg, int len)`
### Buttons
Software call `int get_button_pressed()`. Software should call this in a loop while checking that the game is not otherwise ended.
### Timer
TIM2_IRQn at 1 Hz. IRQn increments a global variable for second counter. Then call `void updateClock()`. Software will determine call `printLCD()` to update time display. If in timed mode, will end game if ran out of time. If game is over due to winning or timer running out, software will call `endTimer()` which should disable the timer's interrupt.
### LEDs
Software call `void setLights(int* light)` - light is array of 15 ints. Lower 2 bits are the color. Bit 1 is green, bit 0 is red.

## Criteria for Success
### Minimally
Play game. It works.

### Better
Display optimal move. (winning move)

### Riiiiiiiiick
Calculate percentages - winning outcomes for each move. Modulate this with k in the optimization routine? Doable.
