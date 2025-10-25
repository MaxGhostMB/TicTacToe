This is a addition to the original TicTacToe assignment of an AI to play against. the AI is always player 2,
the way the AI works:
    - it recursivaly looks for an end state of the game
    - when it detemines an end state it updates the value and we are returning it to the recursive version above
    - there are 3 end states, the Human wins, the AI winning.
        - the AI will always those Draw over the other player winning 
        - it returns 0 if the game ends in a Draw and it returns the winner if a win is found
    - the player switches through each recurive level and in each level the AI assumes the best move for that player 
this version automatically plays with AI over being able to choose to play with one.

