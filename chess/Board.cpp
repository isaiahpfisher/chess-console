#include "Board.h"
#include "color.hpp"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Empty.h"

// initializes the grid of the board to have the starting positions of a chess game
Board::Board() {

	// Black First Row
	this->grid[0][0] = new Rook(BLACK);
	this->grid[0][1] = new Knight(BLACK);
	this->grid[0][2] = new Bishop(BLACK);
	this->grid[0][3] = new Queen(BLACK);
	this->grid[0][4] = new King(BLACK, 0, 4);
	this->blackKing = this->grid[0][4];
	this->grid[0][5] = new Bishop(BLACK);
	this->grid[0][6] = new Knight(BLACK);
	this->grid[0][7] = new Rook(BLACK);

	// Black Second Row
	this->grid[1][0] = new Pawn(BLACK, this->isAI);
	this->grid[1][1] = new Pawn(BLACK, this->isAI);
	this->grid[1][2] = new Pawn(BLACK, this->isAI);
	this->grid[1][3] = new Pawn(BLACK, this->isAI);
	this->grid[1][4] = new Pawn(BLACK, this->isAI);
	this->grid[1][5] = new Pawn(BLACK, this->isAI);
	this->grid[1][6] = new Pawn(BLACK, this->isAI);
	this->grid[1][7] = new Pawn(BLACK, this->isAI);

	// Empty Spaces
	for (int row = 2; row < 6; row++) {
		for (int col = 0; col < 8; col++) {
			this->grid[row][col] = new Empty();
		}
	}

	// White First Row
	this->grid[7][0] = new Rook(WHITE);
	this->grid[7][1] = new Knight(WHITE);
	this->grid[7][2] = new Bishop(WHITE);
	this->grid[7][3] = new Queen(WHITE);
	this->grid[7][4] = new King(WHITE, 7, 4);
	this->whiteKing = this->grid[7][4];
	this->grid[7][5] = new Bishop(WHITE);
	this->grid[7][6] = new Knight(WHITE);
	this->grid[7][7] = new Rook(WHITE);

	// White Second Row
	this->grid[6][0] = new Pawn(WHITE, false);
	this->grid[6][1] = new Pawn(WHITE, false);
	this->grid[6][2] = new Pawn(WHITE, false);
	this->grid[6][3] = new Pawn(WHITE, false);
	this->grid[6][4] = new Pawn(WHITE, false);
	this->grid[6][5] = new Pawn(WHITE, false);
	this->grid[6][6] = new Pawn(WHITE, false);
	this->grid[6][7] = new Pawn(WHITE, false);
}

// copy constructor for board
Board::Board(Board* game) {
	this->turnCount = game->turnCount;

	this->turnSinceLastTake = game->turnSinceLastTake;
	this->history = game->history;
	this->moveHistory = game->moveHistory;
	this->isAI = game->isAI;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Piece* piece = game->grid[row][col];
			string currentColor = piece->color;

			switch (piece->type) {
			case KING:
				this->grid[row][col] = new King(currentColor, row, col);
				(currentColor == WHITE ? this->whiteKing = this->grid[row][col] : this->blackKing = this->grid[row][col]);
				break;
			case QUEEN:
				this->grid[row][col] = new Queen(currentColor);
				break;
			case ROOK:
				this->grid[row][col] = new Rook(currentColor);
				break;
			case BISHOP:
				this->grid[row][col] = new Bishop(currentColor);
				break;
			case KNIGHT:
				this->grid[row][col] = new Knight(currentColor);
				break;
			case PAWN:
				this->grid[row][col] = new Pawn(currentColor, game->grid[row][col]->isAI);
				break;
			default:
				this->grid[row][col] = new Empty();
			}

			this->grid[row][col]->type = game->grid[row][col]->type;
			this->grid[row][col]->color = game->grid[row][col]->color;
			this->grid[row][col]->unicode = game->grid[row][col]->unicode;
			this->grid[row][col]->enPassant = game->grid[row][col]->enPassant;
			this->grid[row][col]->hasMoved = game->grid[row][col]->hasMoved;
			this->grid[row][col]->row = game->grid[row][col]->row;
			this->grid[row][col]->col = game->grid[row][col]->col;

			if (row == game->lastPieceMoved->row && col == game->lastPieceMoved->col) {
				this->lastPieceMoved = this->grid[row][col];
			}
		}
	}
}

// prints the board
void Board::print() {
	
	system("cls");

	cout << endl;

	cout << setw(54) << "" << "      Taken Pieces" << setw(5) << "" << "Move History" << endl;
	
	vector<MoveHistory> whiteTaken = this->getTakenPieces(WHITE);
	vector<MoveHistory> blackTaken = this->getTakenPieces(BLACK);

	for (int row = 0; row < 8; row++) {
		printLine(row, whiteTaken, blackTaken);
	}

	cout << endl;
	cout << "      ";
	for (char letter = 'A'; letter <= 'H'; letter++) {
		cout << "  " << letter << "   ";
	}
	cout << endl << endl;
}

// gets all taken pieces of given color
vector<MoveHistory> Board::getTakenPieces(string color) {
	vector<MoveHistory> takenPieces;
	for (int i = 0; i < this->moveHistory.size(); i++) {
		if (this->moveHistory[i].endColor == color) {
			takenPieces.push_back(this->moveHistory[i]);
		}
	}
	return takenPieces;
}

// translate x,y coordinates to a string for moveHistory
string Board::translateCoordinates(int row, int col) {
	string strCol(1, col + 'A');
	string strRow(1, 8 - row + '0');
	return strCol + strRow;
}

void Board::translateMoveHistory(MoveHistory move) {
	string start = translateCoordinates(move.startRow, move.startCol);
	string end = translateCoordinates(move.endRow, move.endCol);
	string startPiece = "(" + move.startUnicode + ")";
	string endPiece = (move.endType != EMPTY) ? "(" + move.endUnicode + ")" : "  ";
	(move.startColor == WHITE) ? cout << dye::light_red_on_black(startPiece) : cout << dye::light_purple_on_black(startPiece);
	cout << " " << start << " to " << end << " ";
	(move.endColor == WHITE) ? cout << dye::light_red_on_black(endPiece) : cout << dye::light_purple_on_black(endPiece);

	if (move.enPassant) {
		cout << " (En Passant)";
	}
	else if (move.castled) {
		cout << "  (Castled)";
	}
}

// prints a single row of the board
void Board::printLine(int row, vector<MoveHistory> whiteTaken, vector<MoveHistory> blackTaken) {
	for (int subRow = 0; subRow < CELL_SIZE / 2; subRow++) {

		if (subRow == (CELL_SIZE / 4)) {
			cout << "  " << (8 - row) << "   ";
		} else {
			cout << "      ";
		}

		for (int col = 0; col < 8; col++) {
			for (int subCol = 0; subCol < CELL_SIZE; subCol++) {
				if ((row + col) % 2 == 0) {
					if (!(*getPieceAtPosition(row, col)).isEmpty() && (subRow == (CELL_SIZE / 4) && subCol == CELL_SIZE / 2 - 1)) {
						if (getPieceAtPosition(row, col)->color == WHITE) {
							cout << dye::light_red_on_white(getPieceAtPosition(row, col)->unicode);
						} else {
							cout << dye::light_purple_on_white(getPieceAtPosition(row, col)->unicode);
						}
					} else {
						cout << dye::on_white(" ");
					}
				}
				else {
					if (!(*getPieceAtPosition(row, col)).isEmpty() && (subRow == (CELL_SIZE / 4) && subCol == CELL_SIZE / 2 - 1)) {
						if (getPieceAtPosition(row, col)->color == WHITE) {
							cout << dye::light_red_on_black(getPieceAtPosition(row, col)->unicode);
						} else {
							cout << dye::light_purple_on_black(getPieceAtPosition(row, col)->unicode);
						}
					} else {
						cout << dye::on_black(" ");
					}
				}
			}
		}

		cout << "         ";
		if (whiteTaken.size() > (row * CELL_SIZE / 2) + subRow) {
			cout << dye::light_red_on_black(whiteTaken[(row * CELL_SIZE / 2) + subRow].endUnicode);
		}
		else {
			cout << " ";
		}
		cout << "    ";
		if (blackTaken.size() > (row * CELL_SIZE / 2) + subRow) {
			cout << dye::light_purple_on_black(blackTaken[(row * CELL_SIZE / 2) + subRow].endUnicode);
		}
		else {
			cout << " ";
		}
		cout << setw(8) << "";
		if (this->moveHistory.size() > 0) {
			if (this->moveHistory.size() >= 24) {
				cout << setw(2) << left << (this->turnCount - 24 + (row * CELL_SIZE / 2) + subRow) << " ";
				this->translateMoveHistory(this->moveHistory[(this->turnCount - 24 + (row * CELL_SIZE / 2) + subRow) - 1]);
			}
			else if ((row * CELL_SIZE / 2) + subRow < this->moveHistory.size()) {
				cout << setw(2) << left << (this->turnCount - this->moveHistory.size() + (row * CELL_SIZE / 2) + subRow) << " ";
				this->translateMoveHistory(this->moveHistory[(this->turnCount - this->moveHistory.size() + (row * CELL_SIZE / 2) + subRow) - 1]);
			}
		}

		cout << endl;
	}
}

// returns the Piece at the given position
Piece* Board::getPieceAtPosition(int row, int col) {
	return this->grid[row][col];
}

// moves a piece on the board
void Board::move(int startRow, int startCol, int endRow, int endCol) {
	Piece* startPiece = this->getPieceAtPosition(startRow, startCol);
	Piece* endPiece = this->getPieceAtPosition(endRow, endCol);

	MoveHistory currentMove = { startRow, startCol, endRow, endCol, startPiece->type, startPiece->color, startPiece->unicode, endPiece->type, endPiece->color, endPiece->unicode};

	int direction = (startPiece->color == WHITE ? WHITE_DIRECTION : BLACK_DIRECTION);

	if (!grid[endRow][endCol]->isEmpty() || (((endRow - direction >= 0) && (endRow - direction <= 7)) && (grid[endRow - direction][endCol]->enPassant))) {
		this->turnSinceLastTake = 0;
		this->history.clear();
		this->history.push_back("base");
	}
	else {
		this->turnSinceLastTake++;
	}

	// make the move
	delete grid[endRow][endCol]; // THIS COULD BE THE SOURCE OF A BUG
	grid[endRow][endCol] = grid[startRow][startCol];
	grid[startRow][startCol] = new Empty();

	// do move-specific code and push move to move history
	string note = startPiece->move(this->grid, startRow, startCol, endRow, endCol);
	currentMove.enPassant = (note == "En Passant");
	currentMove.castled = (note == "Castled");


	//endPiece->type, endPiece->color, endPiece->unicode};

	if (currentMove.enPassant) {
		currentMove.endType = PAWN;
		currentMove.endUnicode = u8"\u265F";
		currentMove.endColor = (startPiece->color == WHITE ? BLACK : WHITE);
	}

	//this->moveHistory.insert(this->moveHistory.begin(), currentMove);
	this->moveHistory.push_back(currentMove);
	
	// reset enPassant of previous piece
	lastPieceMoved->enPassant = false;

	// set lastPieceMoved to current piece
	lastPieceMoved = this->grid[endRow][endCol];

	// when the move is over, save a snapshot of the board to history
	this->history.push_back(this->boardToString());
}

// Gets the input for moves
void Board::getInput() {
	string input;
	int startRow, startCol, endRow, endCol;
	cout << " >> " << (this->getCurrentTurn() == WHITE ? dye::light_red(WHITE) : dye::light_purple(BLACK))
		<< ": What's your next move? (ex. A3 B5  or  a3 b5)" << endl << " >> ";
	getline(cin, input);

	// makes sure input is long enough
	if (input.length() < 5) {
		print();
		printErrorMessage("Invalid input (" + input + "). Too short. Try Again.");
		getInput();
		return;
	}

	startCol = toupper(input[0]) - 'A';
	startRow = 8 - (input[1] - '0');
	endCol = toupper(input[3]) - 'A';
	endRow = 8 - (input[4] - '0');
	
	string checkResult = checkMove(this->getCurrentTurn(), input, startRow, startCol, endRow, endCol);
	if (checkResult == "") {
		move(startRow, startCol, endRow, endCol);
	}
	else {
		print();
		printErrorMessage(checkResult);
		getInput();
	}
}

// Checks if moves are valid
string Board::checkMove(string currentTurn, string input, int startRow, int startCol, int endRow, int endCol){
	
	// get pieces/spaces
	Piece* startPiece = this->getPieceAtPosition(startRow, startCol);
	Piece* endPiece = this->getPieceAtPosition(endRow, endCol);
	string checkResult; // What the error message will say, blank if valid

	// Checks if the move is on the board
	if (input != "-1" && (startCol < 0 || startCol > 7 || startRow < 0 || startRow > 7 || endCol < 0 || endCol > 7 || endRow < 0 || endRow > 7)) {
		checkResult = "Invalid Move (" + input + "). Try Again.";
	}

	// Check if startPiece is invalid
	else if (startPiece->isEmpty()) {
		checkResult = "No piece at first coordinate. Try Again.";
	}

	// Check if startPiece is from the right team
	else if (currentTurn != startPiece->color) {
		checkResult = "It's " + (currentTurn == WHITE ? WHITE : BLACK) + "'s turn. Try Again.";
	}

	// Checks if move is on top of same team
	else if (startPiece->color == endPiece->color) {
		checkResult = "Can't move on top of own piece. Try Again.";
	}
	// checking if piece is in the way
	else if (startPiece->isPieceInWay(this->grid, startRow, startCol, endRow, endCol)) {
		checkResult = "Can't jump over piece. Try Again.";
	}
	// checking if piece can do that move
	else if (checkResult == "") {
		checkResult = startPiece->isValidMove(this->grid, startRow, startCol, endRow, endCol);
	}
	// checking for check
	if (checkResult == "") {
		Piece* currentKing = (currentTurn == WHITE ? whiteKing : blackKing);
		string inCheckResult;
		if (currentKing->isInCheck(this->grid, -1, -1, -1, -1)) {
			inCheckResult = "You're king is in check. Try Again.";
		}
		else if (currentKing->isInCheck(this->grid, startRow, startCol, endRow, endCol)) {
			inCheckResult = "You can't put your king in check. Try Again.";
		}
		if (currentKing->isInCheck(this->grid, startRow, startCol, endRow, endCol)) {
			checkResult = inCheckResult;
		}
	}

	return checkResult;
}

// Prints the error message is wrong move or does the move is valid
void Board::printErrorMessage(string error) {
	cout << " >> " << dye::white_on_light_red(" " + error + " ") << endl;
}

// returns the color of whatever team's turn it is
string Board::getCurrentTurn() {
	return (turnCount % 2 == WHITE_TURN ? WHITE : BLACK);
}

// 
bool Board::isOver() {

	// check for checkmate
	if (this->isCheckmate()) { return true; }

	// 50+ moves since a piece was taken
	if (this->turnSinceLastTake >= 50) { return true; }

	// check for stalemate
	if (this->isStalemate()) { return true; }

	// check for three-repeated moves
	if (this->isThirdRepetition()) { return true; }

	// check for insufficient material
	if (this->isInsufficientMaterial()) { return true; }



	return false;
}

// 
bool Board::isMate() {
	string currentColor = this->getCurrentTurn();
	Piece* currentKing = (currentColor == WHITE ? whiteKing : blackKing);

	// check for any possible moves that won't make the king in check
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Piece* piece = this->getPieceAtPosition(row, col);
			if (!piece->isEmpty() && piece->color == currentColor) {
				for (int subRow = 0; subRow < 8; subRow++) {
					for (int subCol = 0; subCol < 8; subCol++) {
						if ((this->checkMove(currentColor, "-1", row, col, subRow, subCol) == "") && !currentKing->isInCheck(this->grid, row, col, subRow, subCol)) {
							return false; // return false at first sign of a move that wouldn't put the king in check
						}
					}
				}
			}
		}
	}
	return true;
}

// 
bool Board::isStalemate() {
	Piece* currentKing = (this->getCurrentTurn() == WHITE ? whiteKing : blackKing);

	// return false if king is in check at start of turn
	if (currentKing->isInCheck(this->grid, -1, -1, -1, -1)) { return false; }

	// 
	return isMate();
}

// 
bool Board::isCheckmate() {
	Piece* currentKing = (this->getCurrentTurn() == WHITE ? whiteKing : blackKing);

	// return false if king isn't in check at start of turn
	if (!currentKing->isInCheck(this->grid, -1, -1, -1, -1)) { return false; }

	// 
	return isMate();
}

// 
bool Board::isThirdRepetition() {
	string board = this->history[this->history.size() - 1];
	int matchCount = 0;
	for (int i = 0; i < this->history.size(); i++) {
		if (this->history[i] == board) { matchCount++; }
	}
	return (matchCount >= 3);
}

// 
string Board::boardToString() {
	string board;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			string symbol = this->grid[row][col]->color.substr(0, 1) + this->grid[row][col]->type;
			board += symbol;
		}
	}
	return board;
}

// 
bool Board::isInsufficientMaterial() {
	int knights = 0;
	int whiteBishopsOnWhite = 0;
	int whiteBishopsOnBlack = 0;
	int blackBishopsOnWhite = 0;
	int blackBishopsOnBlack = 0;


	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Piece* piece = this->grid[row][col];

			if (piece->type == QUEEN || piece->type == ROOK || piece->type == PAWN) { return false; }
			if (piece->type == KNIGHT) { knights++; }
			if (piece->type == BISHOP) {
				if ((row + col) % 2 == 0) {
					{ piece->color == WHITE ? whiteBishopsOnWhite++ : blackBishopsOnWhite++; }
				}
				else {
					{ piece->color == WHITE ? whiteBishopsOnBlack++ : blackBishopsOnBlack++; }
				}
			}
		}
	}

	// just two kings
	if ((knights + whiteBishopsOnWhite + whiteBishopsOnBlack + blackBishopsOnWhite + blackBishopsOnBlack) == 0) { return true; }

	// one king and bishop versus one king
	if ((knights == 0)
		&& (((whiteBishopsOnWhite + whiteBishopsOnBlack == 1) && (blackBishopsOnWhite + blackBishopsOnBlack == 0))
			|| (((whiteBishopsOnWhite + whiteBishopsOnBlack == 0) && (blackBishopsOnWhite + blackBishopsOnBlack == 1))))) { return true; }

	// one king and knight versus one king
	if ((whiteBishopsOnWhite + whiteBishopsOnBlack + blackBishopsOnWhite + blackBishopsOnBlack == 0) && (knights == 1)) { return true; }

	// one king and bishop vs one king and bishop (bishops on same color)
	if ((knights == 0)
		&& (((whiteBishopsOnBlack + blackBishopsOnBlack == 0) && (whiteBishopsOnWhite + blackBishopsOnWhite == 2))
			|| ((whiteBishopsOnBlack + blackBishopsOnBlack == 2) && (whiteBishopsOnWhite + blackBishopsOnWhite == 0)))) { return true; }
	

	return false;
}

// 
bool Board::gameEndMenu() {
	char userChoice;
	bool playAgain;
	this->print();
	cout << " >> Game over ";
	if (!(this->isStalemate() || this->isThirdRepetition() || this->isInsufficientMaterial() || this->turnSinceLastTake >= 50)) {
		cout << (this->getCurrentTurn() == WHITE ? dye::light_red(WHITE) : dye::light_purple(BLACK));
		cout << " lost by: Checkmate." << endl;
	}
	else {
		cout << dye::light_red("Tied");
		cout << " by: ";
		if (this->isStalemate()) {
			cout << "Stalemate." << endl;
		}
		else if (this->isThirdRepetition()) {
			cout << "Third Repitition" << endl;
		}
		else if (this->turnSinceLastTake >= 50) {
			cout << "50 Moves without takeing a piece." << endl;
		}
		else if (this->isInsufficientMaterial()) {
			cout << "Insufficient Materials." << endl;
		}
	}

	cout << " >> Would you like to play again? (Y for yes N for no)." << endl;

	cout << " >> ";
	cin >> userChoice;
	userChoice = toupper(userChoice);
	cin.ignore();

	if (userChoice == 'Y') {
		playAgain = true;
		system("cls");
	}
	else {
		playAgain = false;
	}

	return playAgain;
}

// 
void Board::titleScreen() {
	char userChoice;
	cout << endl << endl << endl;
	cout << " >> Welcome to Chess." << endl;
	cout << " >> This was made by:" << endl;
	cout << " >> " << dye::light_green("Isaiah and Jaxen.") << endl << endl;

	cout << " >> If you would like to play against an AI type " << dye::light_green("Y") << ", or if you would like to play with a friend type " << dye::light_red("N") << "." << endl;
	cout << " >> ";
	cin >> userChoice;
	userChoice = toupper(userChoice);
	cin.ignore();

	switch (userChoice) {
	case ('Y'):
		this->isAI = true;
		break;
	case ('N'):
		this->isAI = false;
		break;
	default:
		system("cls");
		cout << "You did not type an Y or an N. Try Again." << endl;
		this->titleScreen();
		break;
	}
}

//
void Board::doComputerMove() {
	string color = getCurrentTurn();
	Piece* currentKing = (color == WHITE ? whiteKing : blackKing);

	int startRow, startCol, endRow, endCol;
	double maxPosition = -1000;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Piece* piece = this->getPieceAtPosition(row, col);
			if (piece->color == color) {
				for (int subRow = 0; subRow < 8; subRow++) {
					for (int subCol = 0; subCol < 8; subCol++) {
						if ((this->checkMove(color, "-1", row, col, subRow, subCol) == "") && !currentKing->isInCheck(this->grid, row, col, subRow, subCol)) {
							Board simulation(this);
							simulation.move(row, col, subRow, subCol);
							double position = simulation.evaluateBoard(BLACK);
							if (position > maxPosition) {
								maxPosition = position;
								startRow = row;
								startCol = col;
								endRow = subRow;
								endCol = subCol;
							}
						}
					}
				}
			}
		}
	}

	this->move(startRow, startCol, endRow, endCol);
}

//
double Board::evaluateBoard(string color) {
	string enemyColor = (color == WHITE ? BLACK : WHITE);
	Piece* currentKing = (color == WHITE ? whiteKing : blackKing);
	Piece* enemyKing = (color == WHITE ? blackKing : whiteKing);
	double score = 0;
	score += (8 * (enemyKing->isInCheck(this->grid, -1, -1, -1, -1)));
	score += (9 * (this->countPieces(QUEEN, color) - this->countPieces(QUEEN, enemyColor)));
	score += (5 * (this->countPieces(ROOK, color) - this->countPieces(ROOK, enemyColor)));
	score += (3 * (this->countPieces(BISHOP, color) - this->countPieces(BISHOP, enemyColor)));
	score += (3 * (this->countPieces(KNIGHT, color) - this->countPieces(KNIGHT, enemyColor)));
	score += (1 * (this->countPieces(PAWN, color) - this->countPieces(PAWN, enemyColor)));
	score -= (0.5 * (this->countBlockedPawns(color) - this->countBlockedPawns(enemyColor)));
	score -= (0.5 * (this->countIsolatedPawns(color) - this->countIsolatedPawns(enemyColor)));
	score -= (0.5 * (this->countDoubledPawns(color) - this->countDoubledPawns(enemyColor)));
	score += (0.1 * (this->countTotalLegalMoves(color) - this->countTotalLegalMoves(enemyColor)));

	return score;
}

//
int Board::countPieces(char type, string color) {
	int count = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->grid[row][col]->type == type && this->grid[row][col]->color == color) {
				count++;
			}
		}
	}
	return count;
}

//
int Board::countDoubledPawns(string color) {
	int totalCount = 0;
	for (int col = 0; col < 8; col++) {
		int count = 0;
		for (int row = 0; row < 8; row++) {
			if (this->grid[row][col]->type == PAWN && this->grid[row][col]->color == color) {
				count++;
			}
		}
		if (count > 0) {
			totalCount += count;
		}
	}
	return totalCount;
}

//
int Board::countIsolatedPawns(string color) {
	int totalCount = 0;
	for (int col = 0; col < 8; col++) {
		int prevCount = 0;
		int currCount = 0;
		int nextCount = 0;
		for (int row = 0; row < 8; row++) {
			if (col > 0 && this->grid[row][col - 1]->type == PAWN && this->grid[row][col - 1]->color == color) {
				prevCount++;
			}
			if (this->grid[row][col]->type == PAWN && this->grid[row][col]->color == color) {
				currCount++;
			}
			if (col < 7 && this->grid[row][col + 1]->type == PAWN && this->grid[row][col + 1]->color == color) {
				prevCount++;
			}
		}
		if (currCount > 0 && prevCount == 0 && nextCount == 0) {
			totalCount += currCount;
		}
	}
	return totalCount;
}

//
int Board::countBlockedPawns(string color) {
	int direction = (color == WHITE ? WHITE_DIRECTION : BLACK_DIRECTION);
	string enemyColor = (color == WHITE ? BLACK : WHITE);
	int totalCount = 0;
	for (int row = 0; row < 8; row++) {
		int count = 0;
		for (int col = 0; col < 8; col++) {
			if (this->grid[row][col]->type == PAWN && this->grid[row][col]->color == color) {
				if ((color == WHITE && row == 0) || (color == BLACK && row == 7)) {
					totalCount++;
				}
				else if (!this->grid[row + direction][col]->isEmpty()) {
					if ((col == 0 || this->grid[row + direction][col - 1]->color != enemyColor) && (col == 7 || this->grid[row + direction][col + 1]->color != enemyColor)) {
						totalCount++;
					}
				}
			}
		}
	}
	return totalCount;
}

//
int Board::countTotalLegalMoves(string color) {
	int totalMoves = 0;
	Piece* currentKing = (color == WHITE ? whiteKing : blackKing);

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Piece* piece = this->getPieceAtPosition(row, col);
			if (piece->color == color) {
				for (int subRow = 0; subRow < 8; subRow++) {
					for (int subCol = 0; subCol < 8; subCol++) {
						if ((this->checkMove(color, "-1", row, col, subRow, subCol) == "") && !currentKing->isInCheck(this->grid, row, col, subRow, subCol)) {
							totalMoves++;
						}
					}
				}
			}
		}
	}
	return totalMoves;
}