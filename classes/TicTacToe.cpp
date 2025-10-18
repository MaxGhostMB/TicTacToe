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

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

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

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x <_gameOptions.rowX; x++ ){
            _grid[y][x].initHolder(ImVec2(x*100, y*100) ,"square.png", x,y);
        }
    }

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (!holder) return false;

    if(holder->bit()) return false; 

    auto _player = PieceForPlayer(getCurrentPlayer()->playerNumber());
    if(_player) {
        auto _pos = holder->getPosition();
        _player->setPosition(_pos);
        holder->setBit(_player);
        return true;
    }
    return false; // replace with true if you complete a successful placement    
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
    int x = index % 3;
    int y = index / 3;

    Bit* _player = _grid[y][x].bit();

    if (_player) {
        return _player->getOwner();
    }
    return nullptr;
}

Player* TicTacToe::checkForWinner()
{   
    std::string _currentState = stateString();
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
        if (_currentState[_state[0]] != '0' &&
            _currentState[_state[1]] != '0' && 
            _currentState[_state[2]] != '0') {
                if (_currentState[_state[0]] == _currentState[_state[1]] &&
                    _currentState[_state[1]] == _currentState[_state[2]]) {
                        x = _state[0] % 3;
                        y = _state[0] / 3;
                        _winner = _grid[y][x].bit()->getOwner();
                        ConLog.print(Logger::INFO, "found a winner " + std::to_string(_winner->playerNumber()));
                        return _winner;
                    }
            }
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    std::string _currentState = stateString();
    for (int i = 0; i < 9; i++) {
        if (_currentState[i] == '0') {
            return false;
        }
    }
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
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for(int x = 0; x < _gameOptions.rowX; x++) {
            auto _bit = _grid[y][x].bit();
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
    for (int i = 0; i < 9; i++) {
        _playerNum = s[i] - '0';
        x = i % 3;
        y = i / 3;
        if(_playerNum) {
            _grid[y][x].setBit(PieceForPlayer(_playerNum - 1));
        } else {
            _grid[y][x].setBit(nullptr);
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // we will implement the AI in the next assignment!
}

