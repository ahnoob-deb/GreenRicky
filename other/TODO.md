TODO for version 0.9 alpha :

* all printf-calls must be replaced by a log-function
* calculation of type of next piece must become better
* remove ALL memory leaks  +++> DONE
* better selfmade font - letters from leadingedge?! +++> DONE

TODO in new branch after 0.9 alpha :

* Definition of the piece-shapes has to be reworked. 
* source-code is too large at the moment :

scripts/ : count-lines.sh :

(24.01.2026)

command : find ../source -type f | xargs wc -l

result : 

   29 ../source/Includes/M_Game_Main_Menu.h
  165 ../source/Includes/D_Common.h
   41 ../source/Includes/S_SDL3_Rendering.h
   16 ../source/Includes/S_Logging.h
   96 ../source/Includes/S_Game_Objects.h
    9 ../source/Includes/D_fonts.h
   29 ../source/Includes/R_Resources.h
   22 ../source/Includes/S_Shape_Manager.h
    9 ../source/Includes/M_Game_Framework.h
   17 ../source/Includes/D_HOF_Data.h
   53 ../source/Includes/S_Texture_Manager.h
    6 ../source/Includes/S_Quit.h
   23 ../source/Includes/M_Game_Main.h
    8 ../source/Includes/M_Game_Hall_of_Fame.h
  283 ../source/Impl/Resources/S_Texture_Manager.c
  173 ../source/Impl/Resources/S_Shape_Manager.c
  303 ../source/Impl/Resources/R_Resources.c
  411 ../source/Impl/Game/M_Game_Hall_of_Fame.c
  975 ../source/Impl/Game/M_Game_Main.c      -> way too large -> rework code
  510 ../source/Impl/Game/S_Game_Objects.c   -> way too large -> rework code
  156 ../source/Impl/Game/M_Game_Main_Menu.c
  176 ../source/Impl/Framework/M_Game_Framework.c
   41 ../source/Impl/Framework/S_Quit.c
   11 ../source/Impl/main.c
  292 ../source/Impl/Rendering/S_SDL3_Rendering.c
 3854 insgesamt   -> try to reduce to 2000 lines without comments


