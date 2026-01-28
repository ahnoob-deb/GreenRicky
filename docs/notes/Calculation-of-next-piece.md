The method for calculating the next piece in Tetris depends entirely on the game version (Guideline, NES, or TGM), ranging from a simple 7-bag shuffle to more complex pseudorandom number generators (PRNGs)
. 
Here is a breakdown of how the next piece is typically calculated:
Most modern versions (Tetris Friends, Tetris Effect) use the "7-bag" system, which ensures a fair distribution of pieces. 

    Method: The game creates a "bag" containing one of each of the 7 tetrominos (I, O, T, S, Z, J, L).
    Selection: The game randomly picks a piece from the bag.
    Replenishment: Once all 7 pieces have been used, a new bag of 7 is created and shuffled.
    Result: A player will never go more than 12 pieces without seeing an I-piece, and they might see a temporary sequence of 14 pieces (e.g., end of one bag, start of another) that seems random. 

2. Classic "Randomizer" System (NES)
The 1989 NES Tetris uses a different, more "brutal" algorithm. 

    Method: The game calls a random number generator (0–6).
    Bias: The algorithm compares the new piece to the last piece placed. If they are the same, it re-rolls once.
    Result: This makes it possible to have long droughts of specific pieces, such as 30+ pieces without an I-piece, resulting in a much more difficult game than modern systems. 

3. TGM System (Memory 4)
The Tetris The Grand Master (TGM) series uses a "Memory 4" system. 

    Method: The game keeps a history of the last four pieces.
    Selection: It attempts to pick a new piece, but if it is present in the history, it "re-rolls" up to 6 times to try to find a piece not in the history.
    Result: This significantly reduces the likelihood of seeing the same piece consecutively. 
    
 Technical Color Assignment
In development, each piece shape (matrix) is usually mapped to a specific color identifier: 

    A random index is chosen (0-6) based on the bag system.
    The corresponding piece shape is fetched from a library.
    The color is assigned based on the typeId. 

Tetris Guideline Colors: 
( according to Game Boy Color Longplay Tetris-DX )
( see : https://www.youtube.com/watch?v=DwZBJhbiz78 )

    I: Orange (light)
    O: Yellow
    T: Green (seagreen)
    S: Purple
    Z: Orange (dark)
    J: Cyanblue
    L: Red