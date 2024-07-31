# Connect Four Robot
Summer Internship Project - Making a Robot Play Connect Four


## System Overview
The system is split into three main areas:
- Computer Vision - Tracks the state of the game
- Solving Algorithm - Determines the next move to play
- Robot - Enacts the move

System is running on a Raspberry Pi 5 with the Camera Module 3. <br>
OpenCV is used for the Computer Vision. <br>

# How the Algorithm Works
This algorithm was adapted from the AI Solver created by Pascal Pons - see his GitHub for a detailled tutorial and explaination of the different stages of the algorithm. Below is a breif desciption of how it works. 

## Algorithm Descrpition
Notation: There are multiple ways to notate a game of connect four. This algorithm uses several.
- Move Sequence: An array of column numbers that depicts order that the columns are played.
- Bitmap: The slots are numbered from 1 to 49 (starting in the bottom-left corner and moving up - a spacing state is used above each column). Two 64-bit numbers are used to represent the game - If a slot has a red counter in it, the corresponding bitmap slot with have a 1; else the rest of the bitmap is filled with zeros. Another number is used for the yellow counters.
- Q-bitmap: Similar to the bitmap except it is a number with 42 elements where a 0 represents an empty slot; 1 represents a red counter and a 2 represents a yellow counter. NB: There are no spacing states in this notation
- Matrix: A 7x6 matrix where the slots align with the real world board. Again 0 = empty, 1 = red, 2 = yellow.

Scoring: Each possible game state is scored based on the number of moves remaining that a player or win or lose with. A score of +5 means the current player will win with 5 moves remaining; a score of -15 means that the current player will lose with 15 moves remaining. 

Searching: The algorithm creates a large decision tree that seraches each possible move that can be played from a given state and the subsequent moves from each of the new calculated states.

MinMax: A minmax algorithm is used to create the decision tree and find the best or worst score from a given branch

Optimisations: The algorithm is heavily optimised else it would have to serach an obscene number of states. The algorithm is optimised using the following statergies: Alhpa-Beta Pruning; Transposition Tables (stop researching found states), Bitmaps (identify repeated states), Move ordering (prioritse winning moves)

Look-Up Tables: Look up tables were created as the algorithm was too slow when the move count is low due to the larger number of computations stemming from more possible moves.


# How the Computer Vision Works
Aim: Identify the State of the board <br> 

### Method: 
- Apply mask to image to isolate the board (Blue mask)
- Detect contours (Sudden changes in pixel values = edges)
- Eliminate incorrect Contours
  - Delete smallest and largest (Noise and the board outline)
  - Delete contours outside of game border 
  - Delete non-circular contours
  - Delete contours that are too close together
- Check if 42 circles found
  - If too few: Figure out where other circles should be (ICP algorithm)
  - Or iterative algorithm that shifts the range values based on distance to game***

When 42 Circles Found:

- Use fitting to find circle around contours - provides centre point
- Detect colours within circles – Average of multiple points?
- Produce a game state array 
- Obvious error check (colour on top of blank)
<br>

# Useful links
[Setting up C++ in VS Code on Raspberry Pi (Linux)](https://code.visualstudio.com/docs/cpp/config-linux) <br>
[Install OpenCV on Raspberry Pi](https://qengineering.eu/install%20opencv%20on%20raspberry%20pi%205.html) <br>
[Connect Four Algorithm Tutorial](http://blog.gamesolver.org/) <br>
[Connect Four Solver - Python](https://github.com/lhorrell99/connect-4-solver?tab=readme-ov-file) <br>
[OpenCV API](https://docs.opencv.org/2.4/index.html) <br>
