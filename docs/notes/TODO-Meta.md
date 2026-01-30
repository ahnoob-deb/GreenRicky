TODO for version 0.9 alpha :

**************************************************
OVERALL :
**************************************************
TODO PRIO 1 :
**************************************************
* update documentation for doxygen : TODO 
* update comments in code : TODO 
* document the project. : TODO
* every single function with comments in head for doxygen : TODO

* implement a debug-module for logging and deep bugfixing. : TODO
* all printf-calls must be replaced by a log-function in the debug module. : TODO
**************************************************
TODO PRIO 2 :
**************************************************
* main_loop : some things still : TODO :
  * at Gameover show a wall is falling down, then show "GAME OVER" : TODO
  * at quad-line-combo show "SUPERB". : TODO

* Next-Piece-Calculation :
   * Implement the "7-bag"-system for next piece calculation and make it
   * Make the Classic-"Randomizer"-System (NES) in options

* all Eclipse-Warning have to be fixed. : TODO

* different patterns in the pieces similar to the gameboy-color-version. : TODO
* implement Game-Over-state correctly : TODO
* implement a more general time measurement for timing events like sounds- 
  and graphics-effects : TODO

**************************************************
ARCHITECTURE :
**************************************************
* split Game Core even more for smaller source-files : TODO

**************************************************
BUILD PROCESS :
**************************************************
* make it buildable without changes in code
  on varius platforms. Even Smartphone. : TODO

**************************************************
DONE :
**************************************************
* all stats must be displayed with the new font +++> DONE
* FPS Couner with new font. : +++> DONE
* create a file for tracking bugs : +++> DONE
* merge both TODO-files. : +++> DONE
* change piece-colors to Game Boy Color Tetris-DX-colorscheme +++> DONE
* calculation of type of next piece must become better  +++> DONE :
   * (currently : Classic "Randomizer" System (NES))
   * see docs/notes/Calculation-of-next-piece.md
* remove ALL memory leaks  +++> DONE
* better selfmade font +++> DONE
* add fullscreen-mode +++> DONE
* keyboard-delay is poor at the moment - make it much better and fast! +++> DONE
* switch to fullscreen 800x600 +++> DONE
* central function for limiting the fps +++> DONE
* implement a level system +++> DONE
* implement increasing difficulty +++> DONE
* implement a scoring system +++> DONE
* implement HALL OF FAME +++> DONE
* update source-folders structure. : +++> DONE
* higher structure of the game as a state maschine. +++> DONE
* draw the map +++> DONE
* draw the Pieces +++> DONE
* spawn the Pieces +++> DONE
* move the Pieces +++> DONE
* rotate the Pieces +++> DONE
* pressing enter in core game should drop the piece. +++> DONE
* implement the collision detection correctly +++> DONE but test it even more
* get the Pieces to pile up +++> DONE
* Pieces now have colors +++> DONE
* define all Pieces of a typical tetris game (7 Pieces) +++> DONE
* map has to store color-values after cg_park_piece() +++> DONE
* implement imploding of a line +++> DONE
* implement input handling +++> DONE
* let there be a last move, when a Piece collides with other Pieces +++> DONE
* manager_textures has to free() its data when leaving the game +++> DONE
* define a good Makefile +++> DONE

**************************************************
WHEN THIS IS ALL DONE, WE WILL HAVE 0.9 alpha
**************************************************
TODO in NEW BRANCH(!!!) after 0.9 alpha :

* Definition of the piece-shapes-data has to be reworked.
* source-code is too large at the moment!
* make it portable :
  * get this running for Windows x64 : TODO
  * get this running for Windows x86 : TODO
  * get this running for Android Smartphones. : TODO

**************************************************
TODO LATER :
**************************************************
* install a local bug tracking system.