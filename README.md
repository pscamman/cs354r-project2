# cs354r-project2

## To build and run:
1. ./buildit && ./BattingSimulator
2. Say 'y' to host, or if a host is already running say 'n' and enter the appropriate address (localhost works if host and client are on the same machine).
3. DO NOT CLICK ON THE BACKGROUND
4. Double click on 'Single Player' to start the game. A single click will not work right now! 'Multi Player' will currently do nothing.
5. Hold down left click to power up the bat like before. R serves a ball to the player like before. Hitting point blocks to the floor will now award points on the GUI, as will hitting the ogre head with a ball.

## Current progress for 3.2 milestone:
Currently we have the beginnings of our network functionality and most of our GUI functionality. We have also begun adding some AI features to improve the gameplay.

For now, we have not unified the GUI with the networking yet, so the networking features currently happen on the command line before the game opens. The game can either start a server or connect to a running server over the network, but for now it is still just a single player game. We are planning on teasing the server functionality into a dedicated server right now, and have figured out how to run the Ogre engine without actually rendering to the screen for that.

The current GUI features include a title screen and a score HUD. In the future we will implement a pause screen, an end condition screen, and a turn timer.

For now our AI is simply in the form of a moving target, but later we plan to add more interesting things (i.e. maybe something that can only be hit at certain times, or targets that provide bonuses such as extra turn time).
