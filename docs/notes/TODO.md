*****************
TODO :
*****************

Rules :
*  snake_case for all variables and function-names
*  parameters begin with a "p_"


Overall :

*  higher structure of the game as a state maschine : DONE

*  update documentation for doxygen : TODO (PRIO 1!!!)
*  update comments in code : TODO
*  update source-folders structure : TODO
*  every single function with comments in head for doxygen : TODO
*  implement Game-Over-state correctly : some DONE, some TODO
*  implement a debug-module for deep bugfixing: TODO
*  main_loop : some things still TODO 
*  implement a level system : TODO
*  implement increasing difficulty : TODO
*  implement a scoring system : some DONE some TODO
*  implement HALL OF FAME : PLACEHOLDER there, more TODO
*  implement a more general time measurement for timing events like sounds- 
   and graphics-effects : TODO

*  get this running for Windows x64 : TODO
*  get this running for Windows x86 : TODO


*****************
DONE :
*****************

LOWER GAME LOGIC : 

*  draw the map : DONE
*  draw the Pieces : DONE
*  spawn the Pieces : DONE
*  move the Pieces : DONE
*  rotate the Pieces : DONE
*  implement the collision detection correctly : DONE
*  get the Pieces to pile up : DONE
*  Pieces now have colors : DONE
*  define all Pieces of a typical tetris game (7 Pieces) : DONE
*  map has to store color-values after park_piece() : DONE
*  imploding of a line : DONE
*  implement input handling : DONE
*  let there be a last move, when a Piece collides with other Pieces : DONE
*  manager_textures has to free() its data at leaving the game : DONE


ARCHITECTURE :

* split core_game.c

BUILD PROCESS :

*  define a good Makefile : DONE