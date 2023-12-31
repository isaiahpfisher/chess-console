#include "Bishop.h"
#include "Piece.h"

Bishop::Bishop(string color) {
	this->color = color;
	this->type = BISHOP;
	this->unicode = u8"\u265D";
}

string Bishop::isValidMove(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) {
	string checkResult;
	int rowDiff = abs(startRow - endRow);
	int colDiff = abs(startCol - endCol);

	if (rowDiff != colDiff) {
		checkResult = "Bishops can only move diagonally. Try Again.";
	}

	return checkResult;
}

string Bishop::move(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) {
	
	this->row = endRow;
	this->col = endCol;
	this->hasMoved = true;
	return "";
}

bool Bishop::isInCheck(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) {
	return false;
}