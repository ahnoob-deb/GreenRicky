TODO for version 0.9 alpha :

* at Gameover show a wall falling down, then show "Game Over"
* at quad-line-combo show "SUPERB".
* all printf-calls must be replaced by a log-function
* different patterns in the pieces like in the gameboy-color-version.
* all stats must be displayed with the new font, FPS with sdl-debug-printer.
* change piece-colors to Game Boy Color Tetris-DX-colorscheme +++> DONE
* calculation of type of next piece must become better 
  (Classic "Randomizer" System (NES) 
  --- see docs/notes/Calculation-of-next-piece.md) +++> DONE
* pressing enter in core game should drop the piece. +++> DONE
* remove ALL memory leaks  +++> DONE
* better selfmade font - letters from leadingedge?! +++> DONE
* add fullscreen-mode +++> DONE
* keyboard-delay is poor at the moment - make it better and fast! +++> DONE
* switch to fullscreen 800x600 +++> DONE
* central function for limiting the fps +++> DONE

BUGS :

* BUG #1 : If you type "abcdefghijklmnopqrst" in hall of fame, there is a segmention fault at "t", the 20th letter. +++> FIXED

TODO in new branch after 0.9 alpha :

* make it portable
* Definition of the piece-shapes has to be reworked.
* source-code is too large at the moment!
