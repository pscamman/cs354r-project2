# cs354r-project2

To build and run:
./buildit && ./BattingSimulator

## Current progress for 3.2 milestone:
Currently we have the beginnings of our network functionality and most of our GUI functionality. We have also begun adding some AI features to improve the gameplay.

For now, we have not unified the GUI with the networking yet, so the networking features currently happen on the command line before the game opens. The game can either start a server or connect to a running server over the network, but for now it is still just a single player game. We are planning on teasing the server functionality into a dedicated server right now, and have figured out how to run the Ogre engine without actually rendering to the screen for that.

The current GUI features include a title screen and a score HUD. In the future we will implement a pause screen, an end condition screen, and a turn timer.

For now our AI is simply in the form of a moving target, but later we plan to add more interesting things (i.e. maybe something that can only be hit at certain times, or targets that provide bonuses such as extra turn time).
