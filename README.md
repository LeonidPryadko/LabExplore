
# LabExplore - a text mode labyrinth game 

## How to run it

The program is terminal-based.  It compiles with `gcc` under linux,
but it should run on any computer.

At the start, the program asks for the labyrinth size (it is always a
square), the number of players, and their names.  Then it asks in turn
for each players to move.  Use arrow keys (`4`, `8`, `6`, and `4`),
respectively, to move `left`, `up`, `right`, or `down`, `5` to shoot,
`1` to build the wall, and `0` to break it.

To quit the game at any point, just press `Control-C`.  After that,
the game exits and cannot be restarted.

## Rules of the game 

The game play happens inside a labyrinth.  The goal is to find and
successfully carry out the *True Treasure*.  There is only one *True
Treasure*, but there may be one or more *False Treasures* which cannot
be recognized as such until you take it outside (to the **Exit**
square).

Originally the players are placed randomly inside of the labyrinth
without any supplies or knowledge of the layout.  Each Player has
three moves, which can be used to step in any of the four directions,
shoot, and also build or break walls (when in possession of the
supplies required).  It is dark in the labyrinth, and the only
information one gets are the responses of the game master to the
players' moves.  A successful player would use this to construct the
map, optionally kill out the opponents, and carry out the treasure.

A killed player has to wait three full rounds (that is, skip three
turns of three moves each).

The game squares: 

 - `[A]` **Arsenal** square.  When you are in the **Arsenal**, your
   supplies are automatically topped off.  There is only one
   **Arsenal** square in the labyrinth.
 - `[F]` **Forest** square.  Whenever you come to the **Forest**
   square carrying a treasure, robbers always come, steal it, and
   re-hide in a random labyrinth square.  There is only one **Forest**
   square in the labyrinth.
 - `[E]` **Exit** square.  You win when you come to the **Exit**
   square carrying the *True Treasure*.  If you come to the **Exit**
   carrying a *False Treasure*, you immediately see it as such and the
   Treasure is automatically discarded.  There is only one **Exit**
   square in the labyrinth.
 - `[L]` Dry **Land**.  This is just a regular square without any
   special function.
 - `[D]` **Delta** square.  Each river has a **Delta** where it flows,
   although a river which only consists of a **Delta** square is also
   allowed.
 - `[R]` **River** square.  Generally, a river is a chain of
   neighboring squares ending in a **Delta**.  When you step on a
   **River** square, you are automatically carried downstream one step
   (unless traveling upstream).  Thus, when you move downstream one
   step, you end up two steps down.
 - `[W]` **Wormhole** (Teleport).  When you step on a **Wormhole**
   square, you are automatically teleported to the **Wormhole** it
   leads to.  Generally, wormholes are connected in cycles, e.g.,
   `A->B->C->A`; a cycle can include as few as one **Wormhole**, in
   which case it is called a cyclic **Wormhole**.  A cyclic
   **Wormhole** leads to itself, i.e., it acts as dry **Land**, except
   this fact is not announced to the players.
 
In addition, labyrinth has walls which may separate neighboring
squares.  An outside wall cannot be destroyed.  A wall cannot be built
blocking the flow of a river.  Also, the **Exit** square cannot be
completely blocked by four walls.

## Comparison with the original paper-and-pencil game 

Several versions of the original game are described
[here](https://en.wikipedia.org/wiki/Labyrinth_(paper-and-pencil_game))
and [here](https://cyclowiki.org/wiki/Лабиринт_(игра_на_бумаге)) (in
Russian).

The program has two major simplifications compared to the original
paper-and-pencil game we used to play:

* In the original game, the **Arsenal** would provide three kinds of
   supplies: 
   
    - Three `bullet`s; a bullet can be used to shoot.
    - Three `bomb`s; a bomb can be used to break an internal wall.
    - Three `concrete sac`s used to build walls.
	
  Due to memory limitations of **Sharp PC-1211**, in the original
  BASIC program these were replaced by six universal supply `unit`s
  which could be used for any of these tasks.  Although it is easy to
  change, the same currently remains in the program.
   
* In the original game, the **Exit** would be in a side wall, and one
  could actually come outside and walk around in the field.  One could
  even turn around the corner of the labyrinth (e.g., to hide from a
  fellow player intent on shooting).  To simplify the BASIC program,
  this was replaced by an **Exit** square.

## History

I first encountered this game in 1982 when I was on a physics Olympiad
back in the USSR.  We played it a lot during the first year in
college.  The biggest drawback was that a leader or *game master* was
required who could not play.

Couple years later I discovered myself an owner of a **Sharp PC-1211**
pocket computer, a rather sophisticated programmable calculator with a
QWERTY keyboard.  A year or so later I wrote a BASIC program which
could replace a *game master*, namely, generate a labyrinth and
navigate the players during the game.  While the calculator had only
1424 available program steps (shared with an array of variables), only
minor simplifications have been done compared to the original version
of the game.

Around 1992 (when I got hold of an actual PC, that is, a Personal
Computer with a powerful `286` processor and whopping 4Mb of RAM) I
rewrote the program in C.  I found the code recently and cleaned it up
just a bit to compile without warnings.

## Disclaimer 

The original code was written in BASIC, and it was one of the first
programs I wrote.  The C version of the program was transcribed from
basic (and the original version even used multiplication in place of
logical AND), and it inherited many of the style features.
Nevertheless, it works.  If you wish, you can even compile it under
`termux` and run on your phone.
