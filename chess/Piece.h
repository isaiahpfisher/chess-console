#include "header.h"

#pragma once
class Piece {
public:
	char type;
	string color; // color (WHITE, BLACK)
	string unicode = "";
	bool isAI = false;
	bool enPassant = false; // 
	bool hasMoved = false; // 
	int row; // 
	int col; // 
	Piece(); // default constructor
	Piece(char type, string color); // 
	bool isEmpty(); // check if Piece is empty
	bool isPieceInWay(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol); // 
	virtual string isValidMove(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) = 0; // 
	virtual string move(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) = 0; // 
	virtual bool isInCheck(Piece* grid[8][8], int startRow, int startCol, int endRow, int endCol) = 0; // 

};