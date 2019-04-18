# miniProject


It's the pin jumping game from Cracker Barrel. Jump pins to remove them.

## LCD:

|      |Before game | In game | After Game|
| ---- | ----------- | ------- | --------- |
| **LCD1** |Welcome screen | Selected slot #, move to slot number # | Call the player an idiot
| **LCD2** |game mode select | Time and pins remaining (hint?)| time spent, pins remaining



Slots:
Color to indicate if a pin is there or slot is empty.
When a slot with a pin is selected, change the color. The empty slots around it will also change color to indicate legal moves and recommended move.
## LEDs
|     | Red on | Red off|
|-----| ----| ------|
| Green on | Hint move (yellow) | Legal move (green) |
| Green off| Peg (red)  | Open (no light) |
