#include "Rook.h"
#include "Piece.h"

Rook::Rook(string color) {
	this->color = color;
	this->type = ROOK;
}

string Rook::isValidMove(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) {
	string checkResult;
	
	if (startCol != endCol && startRow != endRow) {
		checkResult = "Rook can't move diagonally. Try again.";
	}

	return checkResult;
}

string Rook::move(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) {
	return "";
}