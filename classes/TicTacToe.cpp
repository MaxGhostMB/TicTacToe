#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER    = 1;      // index of the AI player (O)
const int HUMAN_PLAYER = -1;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner

Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}


bool TicTacToe::gameHasAI() {
    if (_gameOptions.AIPlaying) {
        return true;
    }
    return false;
}


//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    // this section loops through and loads the emtpy squares into a 3x3 grid
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x <_gameOptions.rowX; x++ ){
            _grid[y][x].initHolder(ImVec2(x*100, y*100) ,"square.png", x, y);
        }
    }

    // if (TicTacToe::gameHasAI()) {
    //     // _gameOptions.AIPlaying = true;
    //     setAIPlayer(AI_PLAYER);
    // }


    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // Checks if a holder exists
    if (!holder) return false;

    // Checks if a holder is occupide 
    if(holder->bit()) return false; 

    // if not occupide, it finds the current player then places that player on the board
    auto _player = PieceForPlayer(getCurrentPlayer()->playerNumber());
    if(_player) {
        auto _pos = holder->getPosition();
        _player->setPosition(_pos);
        holder->setBit(_player);
        return true;
    }
    return false; 
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // loops through and deletes each bit
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x <_gameOptions.rowX; x++ ){
            _grid[y][x].destroyBit(); 
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{   
    // finds the potion in rows of length 3
    int x = index % 3;
    int y = index / 3;

    // gets the bit at that location
    Bit* _player = _grid[y][x].bit();

    // returns the player at that location
    if (_player) {
        return _player->getOwner();
    }
    return nullptr;
}

Player* TicTacToe::checkForWinner()
{   
    std::string _currentState = stateString();

    // each win state 
    int _winStates[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };
    int x, y;
    Player * _winner;
    for (auto _state : _winStates) {
        // chechs if there is a 0 in each of the 3 locations in the statestring
        // if so there cant be a win state and returns a nullptr
        if (_currentState[_state[0]] != '0' &&
            _currentState[_state[1]] != '0' && 
            _currentState[_state[2]] != '0') {
                // checks if all 3 locations have the same value
                if (_currentState[_state[0]] == _currentState[_state[1]] &&
                    _currentState[_state[1]] == _currentState[_state[2]]) {
                        // finds the index on the grid and retreves the owner
                        x = _state[0] % 3;
                        y = _state[0] / 3;
                        _winner = _grid[y][x].bit()->getOwner();
                        // retuns the winner and logs it
                        ConLog.printLog("found a winner " + std::to_string(_winner->playerNumber() + 1));
                        return _winner;
                    }
            }
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // runs through and finds if a '0' exists if so there is no draws yet
    std::string _currentState = stateString();
    for (int i = 0; i < 9; i++) {
        if (_currentState[i] == '0') {
            return false;
        }
    }
    // logs if there is a draw
    ConLog.printLog("Draw found");
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{   
    std::string _state;
    // loops through the board
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for(int x = 0; x < _gameOptions.rowX; x++) {
            // gets the bit at that location
            auto _bit = _grid[y][x].bit();
            // if a bit is found it retreves the player +1 and sets it to the state string
            // else it adds a zero
            if (_bit) {
                _state = _state + std::to_string(_bit->getOwner()->playerNumber() + 1);
            } else {
                _state = _state + "0";
            }
        }
    }
    return _state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{   
    int _playerNum;
    int x, y;

    // loops through the string
    for (int i = 0; i < 9; i++) {
        _playerNum = s[i] - '0';
        x = i % 3;
        y = i / 3;
        // sees if theres a player and if so gets a holder, sets up a bit and uses the same
        // logic as actionForEmptyHolder to set a bit 
        if(_playerNum != 0) {
            BitHolder* _holder = &getHolderAt(x , y);
            Bit* _player = PieceForPlayer(_playerNum - 1);
            auto _pos = _holder->getPosition();
            _player->setPosition(_pos);
            _holder->setBit(_player);
        } else {
            _grid[y][x].setBit(nullptr);
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() {
	int bestMove = -1000;
	int bestSquare = -1;
	std::string state = stateString();
	
	for (int i = 0; i < 9; i++) {
		if (state[i] == '0') {
			state[i] = '2';
            // ConLog.printWarning("we are at state: " + state + "\n player: " + std::to_string(HUMAN_PLAYER));
			int negamax_ = -TicTacToe::negamax(state, 0, HUMAN_PLAYER);
			if (negamax_ > bestMove) {
				bestMove = negamax_;
				bestSquare = i;
			}
			state[i] = '0';
		}
	}
	if (bestSquare != -1) {
		int xcol = bestSquare % 3;
		int ycol = bestSquare / 3;
		BitHolder *holder = &getHolderAt(xcol , ycol);
        actionForEmptyHolder(holder);
		endTurn();
	}
 }

bool TicTacToe::AIBoardFull(std::string state) {
	if (state.find('0') == std::string::npos) {
		return true;
	}
	return false;
}

int TicTacToe::AIWinner(std::string state) {
	// check all win conditions for Tic Tac Toe
	// we don't care who won in this function, we just care that one exists
    std::string _currentState = state;

    // each win state 
    int _winStates[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };
    for (auto _state : _winStates) {
        // chechs if there is a 0 in each of the 3 locations in the statestring
        // if so there cant be a win state and returns a nullptr
        if (_currentState[_state[0]] != '0' &&
            _currentState[_state[1]] != '0' && 
            _currentState[_state[2]] != '0') {
                // checks if all 3 locations have the same value
                if (_currentState[_state[0]] == _currentState[_state[1]] &&
                    _currentState[_state[1]] == _currentState[_state[2]]) {
                        return _currentState[_state[0]];
                    }
            }
    }
    return 0;
}

int TicTacToe::negamax(std::string state, int depth, int playerColor) {
	int bestVal = -1000;
	int boardWinner = AIWinner(state);
	if (boardWinner) {
		// we are returning to the recursive version above
		return -boardWinner;
	}
	bool boardFull = AIBoardFull(state);
	if (boardFull) {
		// draw
		return 0;	
	}
	
	for (int i = 0; i < 9; i++) {
		if (state[i] == '0') {
			state[i] = playerColor == HUMAN_PLAYER ? '1': '2';
			int result = -negamax(state, depth + 1, -playerColor);
			if (result > bestVal) {
				bestVal = result;
			}
			state[i] = '0';
		}
	}
	return bestVal;
}
