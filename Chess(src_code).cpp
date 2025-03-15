#include <iostream>
#include <io.h>  //Provides the necessary functions e.g _setmode()
#include <fcntl.h>  // Specifies that the output should be in UTF-8 format.
#include "chess.h"
#include <iomanip>
#include <string>
#include <ctime>
#include<cstdlib>
#include <vector>

using namespace std;
using namespace CHESS;

//============Draw chess board===========================
void drawBoard(vector<wstring>* gameHistory)
{
    _setmode(_fileno(stdout), _O_U16TEXT);  // Enable UTF-8 output mode
    wcout << "\t\t" << "============== CHESS ==============\n";
    int size = gameHistory->size();
    wcout << "\t\t";
    for (int i = size - 3; i < size && size != 0; i++)
    {
        if (i < 0) i = 0;
        wcout << i + 1 << L"." << gameHistory->at(i) << L"  ";
    }
    wcout << endl;
    wcout << "\t\t" << "  a   b   c   d   e   f   g   h  \n";

    // Draw top border
    wcout << "\t" << setw(8);
    wcout << L"  " << topLeft;
    for (int i = 0; i < 8; ++i) {
        wcout << horizontal << horizontal << horizontal;
        if (i < 7) wcout << topJoin;
    }
    wcout << topRight << endl;

    // Draw the chessboard rows
    for (int i = 0; i < 8; ++i) {
        // Display row number and pieces
        wcout << L"\t " << setw(7) << (8 - i) << vertical;

        for (int j = 0; j < 8; ++j) {
            wcout << L" " << board[i][j] << L" " << vertical;
        }
        wcout << endl;

        // Draw horizontal lines between rows
        if (i < 7) {
            wcout << setw(17) << leftJoin;
            for (int j = 0; j < 8; ++j) {
                wcout << horizontal << horizontal << horizontal;
                if (j < 7) wcout << crossJoin;
            }
            wcout << rightJoin << endl;
        }
    }

    // Draw bottom border
    wcout << setw(17) << bottomLeft;
    for (int i = 0; i < 8; ++i) {
        wcout << horizontal << horizontal << horizontal;
        if (i < 7) wcout << bottomJoin;
    }
    wcout << bottomRight << endl;
}
//============   Straight line Movement validity Calculation (Queen,Rook)=========================
bool moveValidityDiagonal(int initialRow, int initialCol, int targetRow, int targetCol)
{
    int Colbox, Rowbox;
    if (initialCol < targetCol) Colbox = 1;
    else Colbox = -1;
    if (initialRow < targetRow) Rowbox = 1;
    else Rowbox = -1;

    int CurrentRow = initialRow + Rowbox;
    int CurrentCol = initialCol + Colbox;
    while (CurrentRow != targetRow && CurrentCol != targetCol)
    {
        if (board[CurrentRow][CurrentCol] != L' ')
            return false;
        CurrentRow += Rowbox;
        CurrentCol += Colbox;
    }
    return true;
}

//============   Diagonal line Movement validity Calculation (Queen,Bishop)=========================
bool moveValidityStraight(int initialRow, int initialCol, int targetRow, int targetCol)
{
    int Colbox, Rowbox;
    if (initialCol < targetCol) Colbox = -1;
    else Colbox = 1;
    if (initialRow < targetRow) Rowbox = -1;
    else Rowbox = 1;
    int CurrentRow = targetRow + Rowbox;
    int CurrentCol = targetCol + Colbox;
    if (initialRow == targetRow)
    {
        while (CurrentCol != initialCol)
        {
            if (board[initialRow][CurrentCol] != L' ')
                return false;
            CurrentCol += Colbox;
        }
    }
    if (initialCol == targetCol)
    {
        while (CurrentRow != initialRow)
        {
            if (board[CurrentRow][initialCol] != L' ')
                return false;
            CurrentRow += Rowbox;
        }
    }
    return true;
}

//============   Check Calculator =========================
bool checkCalculator(string opponent)
{
    int kingPosRow, kingPosCol;
    int tempR, tempC;
    bool check;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (opponent == "black")
            {
                if (board[r][c] == blackKing)
                {
                    kingPosRow = r;
                    kingPosCol = c;
                    break;
                }
            }
            if (opponent == "white")
            {
                if (board[r][c] == whiteKing)
                {
                    kingPosRow = r;
                    kingPosCol = c;
                    break;
                }
            }
        }
    }
    if ((board[kingPosRow + 1][kingPosCol + 1] == whitePawn || board[kingPosRow + 1][kingPosCol - 1] == whitePawn && opponent == "black") ||
        (board[kingPosRow - 1][kingPosCol + 1] == blackPawn || board[kingPosRow - 1][kingPosCol - 1] == blackPawn && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow - 2 >= 0 && kingPosRow - 2 < 8 && kingPosCol + 1 >= 0 && kingPosCol + 1 < 8 && board[kingPosRow - 2][kingPosCol + 1] == whiteKnight && opponent == "black") ||
        (kingPosRow - 2 >= 0 && kingPosRow - 2 < 8 && kingPosCol + 1 >= 0 && kingPosCol + 1 < 8 && board[kingPosRow - 2][kingPosCol + 1] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow - 2 >= 0 && kingPosRow - 2 < 8 && kingPosCol - 1 >= 0 && kingPosCol - 1 < 8 && board[kingPosRow - 2][kingPosCol - 1] == whiteKnight && opponent == "black") ||
        (kingPosRow - 2 >= 0 && kingPosRow - 2 < 8 && kingPosCol - 1 >= 0 && kingPosCol - 1 < 8 && board[kingPosRow - 2][kingPosCol - 1] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow + 2 >= 0 && kingPosRow + 2 < 8 && kingPosCol - 1 >= 0 && kingPosCol - 1 < 8 && board[kingPosRow + 2][kingPosCol - 1] == whiteKnight && opponent == "black") ||
        (kingPosRow + 2 >= 0 && kingPosRow + 2 < 8 && kingPosCol - 1 >= 0 && kingPosCol - 1 < 8 && board[kingPosRow + 2][kingPosCol - 1] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow + 2 >= 0 && kingPosRow + 2 < 8 && kingPosCol + 1 >= 0 && kingPosCol + 1 < 8 && board[kingPosRow + 2][kingPosCol + 1] == whiteKnight && opponent == "black") ||
        (kingPosRow + 2 >= 0 && kingPosRow + 2 < 8 && kingPosCol + 1 >= 0 && kingPosCol + 1 < 8 && board[kingPosRow + 2][kingPosCol + 1] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow - 1 >= 0 && kingPosRow - 1 < 8 && kingPosCol + 2 >= 0 && kingPosCol + 2 < 8 && board[kingPosRow - 1][kingPosCol + 2] == whiteKnight && opponent == "black") ||
        (kingPosRow - 1 >= 0 && kingPosRow - 1 < 8 && kingPosCol + 2 >= 0 && kingPosCol + 2 < 8 && board[kingPosRow - 1][kingPosCol + 2] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow + 1 >= 0 && kingPosRow + 1 < 8 && kingPosCol + 2 >= 0 && kingPosCol + 2 < 8 && board[kingPosRow + 1][kingPosCol + 2] == whiteKnight && opponent == "black") ||
        (kingPosRow + 1 >= 0 && kingPosRow + 1 < 8 && kingPosCol + 2 >= 0 && kingPosCol + 2 < 8 && board[kingPosRow + 1][kingPosCol + 2] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow + 1 >= 0 && kingPosRow + 1 < 8 && kingPosCol - 2 >= 0 && kingPosCol - 2 < 8 && board[kingPosRow + 1][kingPosCol - 2] == whiteKnight && opponent == "black") ||
        (kingPosRow + 1 >= 0 && kingPosRow + 1 < 8 && kingPosCol - 2 >= 0 && kingPosCol - 2 < 8 && board[kingPosRow + 1][kingPosCol - 2] == blackKnight && opponent == "white"))
    {
        return true;
    }
    if ((kingPosRow - 1 >= 0 && kingPosRow - 1 < 8 && kingPosCol - 2 >= 0 && kingPosCol - 2 < 8 && board[kingPosRow - 1][kingPosCol - 2] == whiteKnight && opponent == "black") ||
        (kingPosRow - 1 >= 0 && kingPosRow - 1 < 8 && kingPosCol - 2 >= 0 && kingPosCol - 2 < 8 && board[kingPosRow - 1][kingPosCol - 2] == blackKnight && opponent == "white"))
    {
        return true;
    }
    for (int i = 0; i < 8; i++)
    {
        if (((board[i][kingPosCol] == whiteQueen || board[i][kingPosCol] == whiteRook) && opponent == "black") ||
            ((board[i][kingPosCol] == blackQueen || board[i][kingPosCol] == blackRook) && opponent == "white"))
        {
            tempR = i;
            tempC = kingPosCol;
            check = moveValidityStraight(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
        if (((board[kingPosRow][i] == whiteQueen || board[kingPosRow][i] == whiteRook) && opponent == "black") ||
            ((board[kingPosRow][i] == blackQueen || board[kingPosRow][i] == blackRook) && opponent == "white"))
        {
            tempR = kingPosRow;
            tempC = i;
            check = moveValidityStraight(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
        if ((kingPosRow + i >= 0 && kingPosRow + i < 8 && kingPosCol + i >= 0 && kingPosCol + i < 8 && (board[kingPosRow + i][kingPosCol + i] == whiteQueen || board[kingPosRow + i][kingPosCol + i] == whiteBishop) && opponent == "black") ||
            (kingPosRow + i >= 0 && kingPosRow + i < 8 && kingPosCol + i >= 0 && kingPosCol + i < 8 && (board[kingPosRow + i][kingPosCol + i] == blackQueen || board[kingPosRow + i][kingPosCol + i] == blackBishop) && opponent == "white"))
        {
            tempR = kingPosRow + i;
            tempC = kingPosCol + i;
            check = moveValidityDiagonal(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
        if ((kingPosRow + i >= 0 && kingPosRow + i < 8 && kingPosCol - i >= 0 && kingPosCol - i < 8 && (board[kingPosRow + i][kingPosCol - i] == whiteQueen || board[kingPosRow + i][kingPosCol - i] == whiteBishop) && opponent == "black") ||
            (kingPosRow + i >= 0 && kingPosRow + i < 8 && kingPosCol - i >= 0 && kingPosCol - i < 8 && (board[kingPosRow + i][kingPosCol - i] == blackQueen || board[kingPosRow + i][kingPosCol - i] == blackBishop) && opponent == "white"))
        {
            tempR = kingPosRow + i;
            tempC = kingPosCol - i;
            check = moveValidityDiagonal(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
        if ((kingPosRow - i >= 0 && kingPosRow - i < 8 && kingPosCol - i >= 0 && kingPosCol - i < 8 && (board[kingPosRow - i][kingPosCol - i] == whiteQueen || board[kingPosRow - i][kingPosCol - i] == whiteBishop) && opponent == "black") ||
            (kingPosRow - i >= 0 && kingPosRow - i < 8 && kingPosCol - i >= 0 && kingPosCol - i < 8 && (board[kingPosRow - i][kingPosCol - i] == blackQueen || board[kingPosRow - i][kingPosCol - i] == blackBishop) && opponent == "white"))
        {
            tempR = kingPosRow - i;
            tempC = kingPosCol - i;
            check = moveValidityDiagonal(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
        if ((kingPosRow - i >= 0 && kingPosRow - i < 8 && kingPosCol + i >= 0 && kingPosCol + i < 8 && (board[kingPosRow - i][kingPosCol + i] == whiteQueen || board[kingPosRow - i][kingPosCol + i] == whiteBishop) && opponent == "black") ||
            (kingPosRow - i >= 0 && kingPosRow - i < 8 && kingPosCol + i >= 0 && kingPosCol + i < 8 && (board[kingPosRow - i][kingPosCol + i] == blackQueen || board[kingPosRow - i][kingPosCol + i] == blackBishop) && opponent == "white"))
        {
            tempR = kingPosRow - i;
            tempC = kingPosCol + i;
            check = moveValidityDiagonal(kingPosRow, kingPosCol, tempR, tempC);
            if (check) return true;
        }
    }
    return false;
}
bool moveAssignment(string input, string player, int& coordC, int& coordR)
{
    if (input.length() == 3 || input.length() == 4 && input[3] == '+')
    {
        coordC = input[1] - 'a';
        coordR = '8' - input[2];
    }
    if (input.length() == 4 && input[1] == 'x' || input.length() == 5)
    {
        coordC = input[2] - 'a';
        coordR = '8' - input[3];
    }
    if (input.length() == 3 && board[coordR][coordC] != L' ') return false;
    if (player == "white")
    {
        if ((input.length() == 4 && input[1] == 'x') && (board[coordR][coordC] == blackKing || (board[coordR][coordC] != blackBishop &&
            board[coordR][coordC] != blackRook && board[coordR][coordC] != blackQueen && board[coordR][coordC] != blackPawn &&
            board[coordR][coordC] != blackKnight)))
            return false;
    }
    if (player == "black")
    {
        if ((input.length() == 4 && input[1] == 'x') && (board[coordR][coordC] == whiteKing || (board[coordR][coordC] != whiteBishop &&
            board[coordR][coordC] != whiteRook && board[coordR][coordC] != whiteQueen && board[coordR][coordC] != whitePawn &&
            board[coordR][coordC] != whiteKnight)))
            return false;
    }
    return true;
}

//============   White Pawn Movement=========================
bool whitePawnmove(string input, bool* firstmove, bool en_passant, int* prev_location_of_white, int last_move_of_black, vector<wchar_t>* captures)
{
    int coordC, coordR;
    bool retval = false;
    if (input.length() == 2 || input.length() == 3)
    {
        coordC = input[0] - 'a';
        coordR = '8' - input[1];
        if (board[coordR + 2][coordC] == whitePawn && coordR + 4 == 8 && board[coordR + 1][coordC] == L' ' && board[coordR][coordC] == L' ')
        {
            board[coordR][coordC] = board[coordR + 2][coordC];
            board[coordR + 2][coordC] = L' ';
            bool result = checkCalculator("white");
            if (result)
            {
                board[coordR + 2][coordC] = board[coordR][coordC];
                board[coordR][coordC] = L' ';
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
            firstmove[0] = true;
            prev_location_of_white[0] = coordC;
            return true;
        }
        if (board[coordR + 1][coordC] == whitePawn && board[coordR][coordC] == L' ')
        {
            board[coordR][coordC] = board[coordR + 1][coordC];
            board[coordR + 1][coordC] = L' ';
            bool result = checkCalculator("white");
            if (result)
            {
                board[coordR + 1][coordC] = board[coordR][coordC];
                board[coordR][coordC] = L' ';
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
            if (coordR == 0)
            {
                wcout << "\t\tPawn Promotion:\n";
                wcout << "\t\t 1 for " << whiteQueen << "\n\t\t 2 for " << whiteRook << "\n\t\t 3 for " << whiteBishop << "\n\t\t 4 for " << whiteKnight << endl;
                int choice;
                wcout << "\t\tEnter choice: "; cin >> choice;
                if (choice == 1)board[coordR][coordC] = whiteQueen;
                else if (choice == 2)board[coordR][coordC] = whiteRook;
                else if (choice == 3)board[coordR][coordC] = whiteBishop;
                else if (choice == 4)board[coordR][coordC] = whiteKnight;
            }
            return true;
        }
    }
    if (input.length() == 4 && input[1] == 'x' || input.length() == 5)
    {
        coordC = input[2] - 'a';
        coordR = '8' - input[3];
        int ScoordC = input[0] - 'a';
        if (coordC + 1 != ScoordC && coordC - 1 != ScoordC)
        {
            return false;
        }
        if (en_passant && last_move_of_black == coordC)
        {
            if (board[coordR + 1][ScoordC] == whitePawn && board[coordR + 1][coordC] == blackPawn)
            {
                wchar_t piece = board[coordR + 1][coordC];
                board[coordR][coordC] = board[coordR + 1][ScoordC];
                board[coordR + 1][ScoordC] = board[coordR + 1][coordC] = L' ';
                bool result = checkCalculator("white");
                if (result)
                {
                    board[coordR + 1][ScoordC] = board[coordR][coordC];
                    board[coordR][coordC] = L' ';
                    board[coordR + 1][coordC] = piece;
                    wcout << "\t\tKing is not Safe!!!!!!\n";
                    return false;
                }
                captures->push_back(piece);
                return true;
            }
        }
        if (board[coordR][coordC] == blackBishop || board[coordR][coordC] == blackRook || board[coordR][coordC] == blackQueen || board[coordR][coordC] == blackPawn || board[coordR][coordC] == blackKnight)
        {
            if (board[coordR + 1][ScoordC] == whitePawn)
            {
                wchar_t piece = board[coordR][coordC];
                board[coordR][coordC] = board[coordR + 1][ScoordC];
                board[coordR + 1][ScoordC] = L' ';
                bool result = checkCalculator("white");
                if (result)
                {
                    board[coordR + 1][ScoordC] = board[coordR][coordC];
                    board[coordR][coordC] = piece;
                    wcout << "\t\tKing is not Safe!!!!!!\n";
                    return false;
                }
                if (coordR == 0)
                {
                    wcout << "\t\tPawn Promotion:\n";
                    wcout << "\t\t 1 for " << whiteQueen << "\n\t\t 2 for " << whiteRook << "\n\t\t 3 for " << whiteBishop << "\n\t\t 4 for " << whiteKnight << endl;
                    int choice;
                    wcout << "\t\tEnter choice: "; cin >> choice;
                    if (choice == 1)board[coordR][coordC] = whiteQueen;
                    else if (choice == 2)board[coordR][coordC] = whiteRook;
                    else if (choice == 3)board[coordR][coordC] = whiteBishop;
                    else if (choice == 4)board[coordR][coordC] = whiteKnight;
                }
                captures->push_back(piece);
                return true;
            }
        }
    }
    return retval;
}
//============   Black Pawn Movement=========================
bool blackPawnmove(string input, bool* firstmove, bool en_passant, int* prev_location_of_black, int last_move_of_white, vector<wchar_t>* captures)
{
    int coordC, coordR;
    if (input.length() == 2 || input.length() == 3)
    {
        coordC = input[0] - 'a';
        coordR = '8' - input[1];
        if (board[coordR - 2][coordC] == blackPawn && coordR - 3 == 0 && board[coordR - 1][coordC] == L' ' && board[coordR][coordC] == L' ')
        {
            board[coordR][coordC] = board[coordR - 2][coordC];
            board[coordR - 2][coordC] = L' ';
            bool result = checkCalculator("black");
            if (result)
            {
                board[coordR - 2][coordC] = board[coordR][coordC];
                board[coordR][coordC] = L' ';
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
            firstmove[0] = true;
            prev_location_of_black[0] = coordC;
            return true;
        }
        if (board[coordR - 1][coordC] == blackPawn && board[coordR][coordC] == L' ')
        {
            board[coordR][coordC] = board[coordR - 1][coordC];
            board[coordR - 1][coordC] = L' ';
            bool result = checkCalculator("black");
            if (result)
            {
                board[coordR - 1][coordC] = board[coordR][coordC];
                board[coordR][coordC] = L' ';
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
            if (coordR == 7)
            {
                wcout << "\t\tPawn Promotion:\n";
                wcout << "\t\t 1 for " << blackQueen << "\n\t\t 2 for " << blackRook << "\n\t\t 3 for " << blackBishop << "\n\t\t 4 for " << blackKnight << endl;
                int choice;
                wcout << "\t\tEnter choice: "; cin >> choice;
                if (choice == 1)board[coordR][coordC] = blackQueen;
                else if (choice == 2)board[coordR][coordC] = blackRook;
                else if (choice == 3)board[coordR][coordC] = blackBishop;
                else if (choice == 4)board[coordR][coordC] = blackKnight;
            }
            return true;
        }
    }
    if (input.length() == 4 && input[1] == 'x' || input.length() == 5)
    {
        coordC = input[2] - 'a';
        coordR = '8' - input[3];
        int ScoordC = input[0] - 'a';
        if (coordC + 1 != ScoordC && coordC - 1 != ScoordC)
        {
            return false;
        }
        if (en_passant && last_move_of_white == coordC)
        {
            if (board[coordR - 1][ScoordC] == blackPawn && board[coordR - 1][coordC] == whitePawn)
            {
                wchar_t piece = board[coordR - 1][coordC];
                board[coordR][coordC] = board[coordR - 1][ScoordC];
                board[coordR - 1][ScoordC] = board[coordR - 1][coordC] = L' ';
                bool result = checkCalculator("black");
                if (result)
                {
                    board[coordR + 1][ScoordC] = board[coordR][coordC];
                    board[coordR][coordC] = L' ';
                    board[coordR - 1][coordC] = piece;
                    wcout << "\t\tKing is not Safe!!!!!!\n";
                    return false;
                }
                captures->push_back(piece);
                return true;
            }
        }
        if (board[coordR][coordC] == whiteBishop || board[coordR][coordC] == whiteRook || board[coordR][coordC] == whiteQueen || board[coordR][coordC] == whitePawn || board[coordR][coordC] == whiteKnight)
        {
            if (board[coordR - 1][ScoordC] == blackPawn)
            {
                wchar_t piece = board[coordR][coordC];
                board[coordR][coordC] = board[coordR - 1][ScoordC];
                board[coordR - 1][ScoordC] = L' ';
                bool result = checkCalculator("black");
                if (result)
                {
                    board[coordR - 1][ScoordC] = board[coordR][coordC];
                    board[coordR][coordC] = piece;
                    wcout << "\t\tKing is not Safe!!!!!!\n";
                    return false;
                }
                if (coordR == 7)
                {
                    wcout << "\t\tPawn Promotion:\n";
                    wcout << "\t\t 1 for " << blackQueen << "\n\t\t 2 for " << blackRook << "\n\t\t 3 for " << blackBishop << "\n\t\t 4 for " << blackKnight << endl;
                    int choice;
                    wcout << "\t\tEnter choice: "; cin >> choice;
                    if (choice == 1)board[coordR][coordC] = blackQueen;
                    else if (choice == 2)board[coordR][coordC] = blackRook;
                    else if (choice == 3)board[coordR][coordC] = blackBishop;
                    else if (choice == 4)board[coordR][coordC] = blackKnight;
                }
                captures->push_back(piece);
                return true;
            }
        }

    }
    return false;
}

//============   Queen Movement=========================
bool Queenmove(string input, string player, vector<wchar_t>* captures)
{
    int initialposRow = -1, initialposCol = -1, tempR, tempC;
    int queenCounter = 0;
    int coordR, coordC;
    bool legal = moveAssignment(input, player, coordC, coordR);
    if (!legal) return false;

    for (int i = 0; i < 8; i++)
    {
        if ((board[i][coordC] == (player == "white" ? whiteQueen : blackQueen)))
        {
            tempR = i;
            tempC = coordC;
            if (moveValidityStraight(tempR, tempC, coordR, coordC))
            {
                queenCounter++;
                initialposRow = tempR;
                initialposCol = tempC;
            }
        }
        if ((board[coordR][i] == (player == "white" ? whiteQueen : blackQueen)))
        {
            tempR = coordR;
            tempC = i;
            if (moveValidityStraight(tempR, tempC, coordR, coordC))
            {
                queenCounter++;
                initialposRow = tempR;
                initialposCol = tempC;
            }
        }
        for (int j = -1; j <= 1; j += 2)
        {
            for (int k = -1; k <= 1; k += 2)
            {
                if (coordR + i * j >= 0 && coordR + i * j < 8 && coordC + i * k >= 0 && coordC + i * k < 8 &&
                    board[coordR + i * j][coordC + i * k] == (player == "white" ? whiteQueen : blackQueen))
                {
                    tempR = coordR + i * j;
                    tempC = coordC + i * k;
                    if (moveValidityStraight(tempR, tempC, coordR, coordC))
                    {
                        queenCounter++;
                        initialposRow = tempR;
                        initialposCol = tempC;
                    }
                }
            }
        }
    }

    if (queenCounter == 1)
    {
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board[initialposRow][initialposCol];
        board[initialposRow][initialposCol] = L' ';
        if (checkCalculator(player))
        {
            board[initialposRow][initialposCol] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    if (queenCounter > 1)
    {
        string coord;
        do
        {
            wcout << "\t\tEnter Coordinate of Queen you want to move: ";
            cin >> coord;
        } while (board['8' - coord[1]][coord[0] - 'a'] != (player == "white" ? whiteQueen : blackQueen));
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board['8' - coord[1]][coord[0] - 'a'];
        board['8' - coord[1]][coord[0] - 'a'] = L' ';
        if (checkCalculator(player))
        {
            board['8' - coord[1]][coord[0] - 'a'] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    return false;
}

//============   Rook Movement=========================
bool Rookmove(string input, string player, vector<wchar_t>* captures)
{
    int initialposRow = -1, initialposCol = -1;
    int rookCounter = 0;
    int coordR, coordC;
    bool legal = moveAssignment(input, player, coordC, coordR);
    if (!legal) return false;

    for (int i = 0; i < 8; i++)
    {
        if (board[i][coordC] == (player == "white" ? whiteRook : blackRook))
        {
            if (moveValidityStraight(i, coordC, coordR, coordC))
            {
                rookCounter++;
                initialposRow = i;
                initialposCol = coordC;
            }
        }
        if (board[coordR][i] == (player == "white" ? whiteRook : blackRook))
        {
            if (moveValidityStraight(coordR, i, coordR, coordC))
            {
                rookCounter++;
                initialposRow = coordR;
                initialposCol = i;
            }
        }
    }

    if (rookCounter == 1)
    {
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board[initialposRow][initialposCol];
        board[initialposRow][initialposCol] = L' ';
        if (checkCalculator(player))
        {
            board[initialposRow][initialposCol] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    if (rookCounter > 1)
    {
        string coord;
        do
        {
            wcout << "\t\tEnter Coordinate of Rook you want to move: ";
            cin >> coord;
        } while (board['8' - coord[1]][coord[0] - 'a'] != (player == "white" ? whiteRook : blackRook));
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board['8' - coord[1]][coord[0] - 'a'];
        board['8' - coord[1]][coord[0] - 'a'] = L' ';
        if (checkCalculator(player))
        {
            board['8' - coord[1]][coord[0] - 'a'] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    return false;
}

//============   Bishop Movement=========================
bool Bishopmove(string input, string player, vector<wchar_t>* captures) {
    int initialposRow = -1, initialposCol = -1;
    int bishopCounter = 0;
    int coordR, coordC;
    bool legal = moveAssignment(input, player, coordC, coordR);
    if (!legal) return false;

    wchar_t bishop = (player == "white") ? whiteBishop : blackBishop;

    for (int i = 1; i < 8; i++)
    {
        for (int j = -1; j <= 1; j += 2)
        {
            for (int k = -1; k <= 1; k += 2)
            {
                int tempR = coordR + i * j;
                int tempC = coordC + i * k;

                if (tempR >= 0 && tempR < 8 && tempC >= 0 && tempC < 8 && board[tempR][tempC] == bishop)
                {
                    if (moveValidityDiagonal(tempR, tempC, coordR, coordC))
                    {
                        bishopCounter++;
                        initialposRow = tempR;
                        initialposCol = tempC;
                    }
                }
            }
        }
    }

    if (bishopCounter == 1)
    {
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board[initialposRow][initialposCol];
        board[initialposRow][initialposCol] = L' ';
        if (checkCalculator(player))
        {
            board[initialposRow][initialposCol] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    if (bishopCounter > 1)
    {
        string coord;
        do
        {
            wcout << "\t\tEnter Coordinate of Bishop you want to move: ";
            cin >> coord;
        } while (board['8' - coord[1]][coord[0] - 'a'] != bishop);
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board['8' - coord[1]][coord[0] - 'a'];
        board['8' - coord[1]][coord[0] - 'a'] = L' ';
        if (checkCalculator(player))
        {
            board['8' - coord[1]][coord[0] - 'a'] = board[coordR][coordC];
            board[coordR][coordC] = L' ';
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }

    return false;
}

//============   Knight Movement=========================
bool Knightmove(string input, string player, vector<wchar_t>* captures)
{
    int coordR, coordC;
    int initialPosRow = -1, initialPosCol = -1;
    bool legal = moveAssignment(input, player, coordC, coordR);
    if (!legal) return false;
    int Knights = 0;
    if ((coordR - 2 >= 0 && coordR - 2 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR - 2][coordC + 1] == whiteKnight && player == "white") ||
        (coordR - 2 >= 0 && coordR - 2 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR - 2][coordC + 1] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC + 1;
        initialPosRow = coordR - 2;
    }
    if ((coordR - 2 >= 0 && coordR - 2 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR - 2][coordC - 1] == whiteKnight && player == "white") ||
        (coordR - 2 >= 0 && coordR - 2 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR - 2][coordC - 1] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC - 1;
        initialPosRow = coordR - 2;
    }
    if ((coordR + 2 >= 0 && coordR + 2 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR + 2][coordC - 1] == whiteKnight && player == "white") ||
        (coordR + 2 >= 0 && coordR + 2 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR + 2][coordC - 1] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC - 1;
        initialPosRow = coordR + 2;
    }
    if ((coordR + 2 >= 0 && coordR + 2 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR + 2][coordC + 1] == whiteKnight && player == "white") ||
        (coordR + 2 >= 0 && coordR + 2 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR + 2][coordC + 1] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC + 1;
        initialPosRow = coordR + 2;
    }
    if ((coordR - 1 >= 0 && coordR - 1 < 8 && coordC + 2 >= 0 && coordC + 2 < 8 && board[coordR - 1][coordC + 2] == whiteKnight && player == "white") ||
        (coordR - 1 >= 0 && coordR - 1 < 8 && coordC + 2 >= 0 && coordC + 2 < 8 && board[coordR - 1][coordC + 2] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC + 2;
        initialPosRow = coordR - 1;
    }
    if ((coordR + 1 >= 0 && coordR + 1 < 8 && coordC + 2 >= 0 && coordC + 2 < 8 && board[coordR + 1][coordC + 2] == whiteKnight && player == "white") ||
        (coordR + 1 >= 0 && coordR + 1 < 8 && coordC + 2 >= 0 && coordC + 2 < 8 && board[coordR + 1][coordC + 2] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC + 2;
        initialPosRow = coordR + 1;
    }
    if ((coordR + 1 >= 0 && coordR + 1 < 8 && coordC - 2 >= 0 && coordC - 2 < 8 && board[coordR + 1][coordC - 2] == whiteKnight && player == "white") ||
        (coordR + 1 >= 0 && coordR + 1 < 8 && coordC - 2 >= 0 && coordC - 2 < 8 && board[coordR + 1][coordC - 2] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC - 2;
        initialPosRow = coordR + 1;
    }
    if ((coordR - 1 >= 0 && coordR - 1 < 8 && coordC - 2 >= 0 && coordC - 2 < 8 && board[coordR - 1][coordC - 2] == whiteKnight && player == "white") ||
        (coordR - 1 >= 0 && coordR - 1 < 8 && coordC - 2 >= 0 && coordC - 2 < 8 && board[coordR - 1][coordC - 2] == blackKnight && player == "black"))
    {
        Knights++;
        initialPosCol = coordC - 2;
        initialPosRow = coordR - 1;
    }
    if (Knights == 1)
    {
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board[initialPosRow][initialPosCol];
        board[initialPosRow][initialPosCol] = L' ';
        if (checkCalculator(player))
        {
            board[initialPosRow][initialPosCol] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }
    if (Knights > 1)
    {
        string coord;
        do
        {
            wcout << "\t\tEnter Coordinate of Knight you want to move: ";
            cin >> coord;
        } while (!(board['8' - coord[1]][coord[0] - 'a'] == whiteKnight && player == "white") && !(board['8' - coord[1]][coord[0] - 'a'] == blackKnight && player == "black"));
        board[coordR][coordC] = board['8' - coord[1]][coord[0] - 'a'];
        board['8' - coord[1]][coord[0] - 'a'] = L' ';
        wchar_t piece = board[coordR][coordC];
        if (checkCalculator(player))
        {
            board['8' - coord[1]][coord[0] - 'a'] = board[coordR][coordC];
            board[coordR][coordC] = piece;
            wcout << "\t\tKing is not Safe!!!!!!\n";
            return false;
        }
        if (piece != L' ')
        {
            captures->push_back(piece);
        }
        return true;
    }
    return false;
}

//============   King Movement=========================
bool Kingmove(string input, string player)
{
    int coordR, coordC;
    int initialPosRow = -1, initialPosCol = -1;
    bool legal = moveAssignment(input, player, coordC, coordR);
    if (!legal) return false;
    if ((coordR + 1 >= 0 && coordR + 1 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR + 1][coordC + 1] == whiteKing && player == "white") ||
        (coordR + 1 >= 0 && coordR + 1 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR + 1][coordC + 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR + 1;
        initialPosCol = coordC + 1;
    }
    else if ((coordR + 1 >= 0 && coordR + 1 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR + 1][coordC - 1] == whiteKing && player == "white") ||
        (coordR + 1 >= 0 && coordR + 1 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR + 1][coordC - 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR + 1;
        initialPosCol = coordC - 1;
    }
    else if ((coordR - 1 >= 0 && coordR - 1 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR - 1][coordC + 1] == whiteKing && player == "white") ||
        (coordR - 1 >= 0 && coordR - 1 < 8 && coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR - 1][coordC + 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR - 1;
        initialPosCol = coordC + 1;
    }
    else if ((coordR - 1 >= 0 && coordR - 1 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR - 1][coordC - 1] == whiteKing && player == "white") ||
        (coordR - 1 >= 0 && coordR - 1 < 8 && coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR - 1][coordC - 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR - 1;
        initialPosCol = coordC - 1;
    }
    else if ((coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR][coordC + 1] == whiteKing && player == "white") ||
        (coordC + 1 >= 0 && coordC + 1 < 8 && board[coordR][coordC + 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR;
        initialPosCol = coordC + 1;
    }
    else if ((coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR][coordC - 1] == whiteKing && player == "white") ||
        (coordC - 1 >= 0 && coordC - 1 < 8 && board[coordR][coordC - 1] == blackKing && player == "black"))
    {
        initialPosRow = coordR;
        initialPosCol = coordC - 1;
    }
    else if ((coordR + 1 >= 0 && coordR + 1 < 8 && board[coordR + 1][coordC] == whiteKing && player == "white") ||
        (coordR + 1 >= 0 && coordR + 1 < 8 && board[coordR + 1][coordC] == blackKing && player == "black"))
    {
        initialPosRow = coordR + 1;
        initialPosCol = coordC;
    }
    else if ((coordR - 1 >= 0 && coordR - 1 < 8 && board[coordR - 1][coordC] == whiteKing && player == "white") ||
        (coordR - 1 >= 0 && coordR - 1 < 8 && board[coordR - 1][coordC] == blackKing && player == "black"))
    {
        initialPosRow = coordR - 1;
        initialPosCol = coordC;
    }
    if ((board[7][4] == whiteKing && coordC == 6 && coordR == 7 && moveValidityStraight(7, 5, 7, 7) && board[7][7] == whiteRook && player == "white") ||
        (board[0][4] == blackKing && coordC == 6 && coordR == 0 && moveValidityStraight(0, 5, 0, 7) && board[0][7] == blackRook && player == "black"))
    {
        bool check = true;
        for (int i = 1; i < 7; i++)
        {
            if ((((board[7 - i][5] == blackRook || board[7 - i][5] == blackQueen) && player == "black") ||
                ((board[7 - i][5] == whiteRook || board[7 - i][5] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8)
            {
                if (moveValidityStraight(7 - i, 5, 7, 5)) check = false;
            }
            if ((((board[7 - i][6] == blackRook || board[7 - i][6] == blackQueen) && player == "black") ||
                ((board[7 - i][6] == whiteRook || board[7 - i][6] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8)
            {
                if (moveValidityStraight(7 - i, 6, 7, 6)) check = false;
            }
            if ((((board[7 - i][6 - i] == blackBishop || board[7 - i][6 - i] == blackQueen) && player == "black") ||
                ((board[7 - i][6 - i] == whiteBishop || board[7 - i][6 - i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 6 - i >= 0 && 6 - i < 8)
            {
                if (moveValidityDiagonal(7 - i, 6 - i, 7, 6)) check = false;
            }if ((((board[7 - i][5 - i] == blackBishop || board[7 - i][5 - i] == blackQueen) && player == "black") ||
                ((board[7 - i][5 - i] == whiteBishop || board[7 - i][5 - i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 5 - i >= 0 && 5 - i < 8)
            {
                if (moveValidityDiagonal(7 - i, 5 - i, 7, 5)) check = false;
            }
            if ((((board[7 - i][6 + i] == blackBishop || board[7 - i][6 + i] == blackQueen) && player == "black") ||
                ((board[7 - i][6 + i] == whiteBishop || board[7 - i][6 + i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 6 + i >= 0 && 6 + i < 8)
            {
                if (moveValidityDiagonal(7 - i, 6 + i, 7, 6)) check = false;
            }
            if ((((board[7 - i][5 + i] == blackBishop || board[7 - i][5 + i] == blackQueen) && player == "black") ||
                ((board[7 - i][5 + i] == whiteBishop || board[7 - i][5 + i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 5 + i >= 0 && 5 + i < 8)
            {
                if (moveValidityDiagonal(7 - i, 5 + i, 7, 5)) check = false;
            }
        }
        if (player == "white" && check)
        {
            board[7][6] = whiteKing;
            board[7][5] = whiteRook;
            board[7][4] = L' ';
            board[7][7] = L' ';
            return true;
        }
        if (player == "black" && check)
        {
            board[0][6] = blackKing;
            board[0][5] = blackRook;
            board[0][4] = L' ';
            board[0][7] = L' ';
            return true;
        }
        return false;
    }
    if ((board[7][4] == whiteKing && coordC == 2 && coordR == 7 && moveValidityStraight(7, 4, 7, 0) && board[7][0] == whiteRook && player == "white") ||
        (board[0][4] == blackKing && coordC == 2 && coordR == 0 && moveValidityStraight(0, 4, 0, 0) && board[0][0] == blackRook && player == "black"))
    {
        bool check = true;
        for (int i = 1; i < 7; i++)
        {
            if ((((board[7 - i][3] == blackRook || board[7 - i][3] == blackQueen) && player == "black") ||
                ((board[7 - i][3] == whiteRook || board[7 - i][3] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8)
            {
                if (moveValidityStraight(7 - i, 3, 7, 3)) check = false;
            }
            if ((((board[7 - i][2] == blackRook || board[7 - i][2] == blackQueen) && player == "black") ||
                ((board[7 - i][2] == whiteRook || board[7 - i][2] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8)
            {
                if (moveValidityStraight(7 - i, 2, 7, 2)) check = false;
            }
            if ((((board[7 - i][3 - i] == blackBishop || board[7 - i][3 - i] == blackQueen) && player == "black") ||
                ((board[7 - i][3 - i] == whiteBishop || board[7 - i][3 - i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 3 - i >= 0 && 3 - i < 8)
            {
                if (moveValidityDiagonal(7 - i, 3 - i, 7, 3)) check = false;
            }if ((((board[7 - i][2 - i] == blackBishop || board[7 - i][2 - i] == blackQueen) && player == "black") ||
                ((board[7 - i][2 - i] == whiteBishop || board[7 - i][2 - i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 2 - i >= 0 && 2 - i < 8)
            {
                if (moveValidityDiagonal(7 - i, 2 - i, 7, 2)) check = false;
            }
            if ((((board[7 - i][3 + i] == blackBishop || board[7 - i][3 + i] == blackQueen) && player == "black") ||
                ((board[7 - i][3 + i] == whiteBishop || board[7 - i][3 + i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 3 + i >= 0 && 3 + i < 8)
            {
                if (moveValidityDiagonal(7 - i, 3 + i, 7, 3)) check = false;
            }
            if ((((board[7 - i][2 + i] == blackBishop || board[7 - i][2 + i] == blackQueen) && player == "black") ||
                ((board[7 - i][2 + i] == whiteBishop || board[7 - i][2 + i] == whiteQueen) && player == "white")) && 7 - i >= 0 && 7 - i < 8 && 2 + i >= 0 && 2 + i < 8)
            {
                if (moveValidityDiagonal(7 - i, 2 + i, 7, 2)) check = false;
            }
        }
        if (player == "white")
        {
            board[7][2] = whiteKing;
            board[7][3] = whiteRook;
            board[7][4] = L' ';
            board[7][0] = L' ';
            return true;
        }
        if (player == "black")
        {
            board[0][2] = blackKing;
            board[0][3] = blackRook;
            board[0][4] = L' ';
            board[0][0] = L' ';
            return true;
        }
        return false;
    }
    if (initialPosRow > -1 && initialPosCol > -1)
    {
        wchar_t piece = board[coordR][coordC];
        board[coordR][coordC] = board[initialPosRow][initialPosCol];
        board[initialPosRow][initialPosCol] = L' ';
        if (player == "black")
        {
            bool result = checkCalculator("black");
            if (result)
            {
                board[initialPosRow][initialPosCol] = board[coordR][coordC];
                board[coordR][coordC] = piece;
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
        }
        else if (player == "white")
        {
            bool result = checkCalculator("white");
            if (result)
            {
                board[initialPosRow][initialPosCol] = board[coordR][coordC];
                board[coordR][coordC] = piece;
                wcout << "\t\tKing is not Safe!!!!!!\n";
                return false;
            }
        }
        return true;
    }
    return false;
}

bool checkCaptureValidity(int pieceRow, int pieceCol, int targetRow, int targetCol, bool* target, string player, bool diagonal)
{
    bool aftermove, checkmate = false, check;
    if (diagonal)
    {
        check = moveValidityDiagonal(pieceRow, pieceCol, targetRow, targetCol);
    }
    else
    {
        check = moveValidityStraight(pieceRow, pieceCol, targetRow, targetCol);
    }
    wchar_t piece = board[pieceRow][pieceCol];
    string Cplayer = (player == "white" ? "black" : "white");
    if (check)
    {
        board[pieceRow][pieceCol] = board[targetRow][targetCol];
        board[targetRow][targetCol] = L' ';
        aftermove = checkCalculator(Cplayer);
        board[targetRow][targetCol] = board[pieceRow][pieceCol];
        board[pieceRow][pieceCol] = piece;
        if (aftermove) checkmate = true;
        else checkmate = false;
        target[0] = false;
    }
    return checkmate;
}

bool checkBlockValidity(int KingPosRow, int KingPosCol, int targetRow, int targetCol, int pieceRow, int pieceCol, bool diagonal)
{
    bool validityCheck;
    if (diagonal)
    {
        validityCheck = moveValidityDiagonal(targetRow, targetCol, pieceRow, pieceCol);
    }
    else
    {
        validityCheck = moveValidityStraight(targetRow, targetCol, pieceRow, pieceCol);
    }
    if (validityCheck)
    {
        wchar_t piece = board[targetRow][targetCol];
        board[targetRow][targetCol] = board[pieceRow][pieceCol];
        board[pieceRow][pieceCol] = L' ';
        bool result = false;
        if (board[KingPosRow][KingPosCol] == blackKing) result = checkCalculator("black");
        if (board[KingPosRow][KingPosCol] == whiteKing) result = checkCalculator("white");
        board[pieceRow][pieceCol] = board[targetRow][targetCol];
        board[targetRow][targetCol] = piece;
        if (!result)
        {
            return false;
        }
    }
    return true;
}

bool checkKnightValidity(int targetRow, int targetCol, int KingPosRow, int KingPosCol)
{
    if ((((board[targetRow - 2][targetCol + 1] == blackKnight || board[targetRow - 2][targetCol - 1] == blackKnight) && board[KingPosRow][KingPosCol] == blackKing) ||
        ((board[targetRow - 2][targetCol + 1] == whiteKnight || board[targetRow - 2][targetCol - 1] == whiteKnight) && board[KingPosRow][KingPosCol] == whiteKing)) &&
        targetRow - 2 >= 0 && targetRow - 2 < 8 && ((targetCol + 1 >= 0 && targetCol + 1 < 8) || (targetCol - 1 >= 0 && targetCol - 1 < 8)))
    {
        return false;
    }
    if ((((board[targetRow + 2][targetCol + 1] == blackKnight || board[targetRow + 2][targetCol - 1] == blackKnight) && board[KingPosRow][KingPosCol] == blackKing) ||
        ((board[targetRow + 2][targetCol + 1] == whiteKnight || board[targetRow + 2][targetCol - 1] == whiteKnight) && board[KingPosRow][KingPosCol] == whiteKing)) &&
        targetRow - 2 >= 0 && targetRow - 2 < 8 && ((targetCol + 1 >= 0 && targetCol + 1 < 8) || (targetCol - 1 >= 0 && targetCol - 1 < 8)))
    {
        return false;
    }
    if ((((board[targetRow + 1][targetCol - 2] == blackKnight || board[targetRow - 1][targetCol - 2] == blackKnight) && board[KingPosRow][KingPosCol] == blackKing) ||
        ((board[targetRow + 1][targetCol - 2] == whiteKnight || board[targetRow - 1][targetCol - 2] == whiteKnight) && board[KingPosRow][KingPosCol] == whiteKing)) &&
        ((targetRow - 1 >= 0 && targetRow - 1 < 8) || (targetRow + 1 >= 0 && targetRow + 1 < 8)) && targetCol - 2 >= 0 && targetCol - 2 < 8)
    {
        return false;
    }
    if ((((board[targetRow + 1][targetCol + 2] == blackKnight || board[targetRow - 1][targetCol + 2] == blackKnight) && board[KingPosRow][KingPosCol] == blackKing) ||
        ((board[targetRow + 1][targetCol + 2] == whiteKnight || board[targetRow - 1][targetCol + 2] == whiteKnight) && board[KingPosRow][KingPosCol] == whiteKing)) &&
        ((targetRow - 1 >= 0 && targetRow - 1 < 8) || (targetRow + 1 >= 0 && targetRow + 1 < 8)) && targetCol + 2 >= 0 && targetCol + 2 < 8)
    {
        return false;
    }
    return true;
}

bool checkKingValidity(string move, int row, int col, int KingPosRow, int KingPosCol, string KingPlayer, wchar_t King)
{
    move += col; move += row;
    wchar_t piece = board['8' - row][col - 'a'];
    bool kingMove = Kingmove(move, KingPlayer);
    if (kingMove)
    {
        board[KingPosRow][KingPosCol] = King;
        board['8' - row][col - 'a'] = piece;
    }
    move = "Kx";
    move += col; move += row;
    if (!kingMove)
    {
        kingMove = Kingmove(move, KingPlayer);
        board[KingPosRow][KingPosCol] = King;
        board['8' - row][col - 'a'] = piece;
    }
    return kingMove;
}

//============   Checkmate Calculation=========================
bool checkmate(string input, string player)
{
    int opponentRow, opponentCol;
    int tempR, tempC;
    bool check;
    bool checkmate = true;
    bool target = true;
    bool aftermove;
    if (input.length() == 2 || (input.length() == 3 && input[2] == '+'))
    {
        opponentRow = '8' - input[1];
        opponentCol = input[0] - 'a';
    }
    else if (input.length() == 3 || (input.length() == 4 && input[3] == '+'))
    {
        opponentRow = '8' - input[2];
        opponentCol = input[1] - 'a';
    }
    else if ((input.length() == 4 && input[1] == 'x') || (input.length() == 5 && input[4] == '+'))
    {
        opponentRow = '8' - input[3];
        opponentCol = input[2] - 'a';
    }
    if ((board[opponentRow - 1][opponentCol + 1] == blackPawn || board[opponentRow - 1][opponentCol - 1] == blackPawn) && player == "white")
    {
        return false;
    }
    if ((board[opponentRow + 1][opponentCol + 1] == whitePawn || board[opponentRow + 1][opponentCol - 1] == whitePawn) && player == "black")
    {
        return false;
    }
    for (int i = 0; i < 8; i++)
    {
        if (((board[i][opponentCol] == blackQueen || board[i][opponentCol] == blackRook) && player == "white") ||
            ((board[i][opponentCol] == whiteQueen || board[i][opponentCol] == whiteRook) && player == "black"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, i, opponentCol, &target, player, false);
        }
        if (((board[opponentRow][i] == blackQueen || board[opponentRow][i] == blackRook) && player == "white") ||
            ((board[opponentRow][i] == whiteQueen || board[opponentRow][i] == whiteRook) && player == "black"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, opponentRow, i, &target, player, false);
        }
        if ((opponentRow + i >= 0 && opponentRow + i < 8 && opponentCol + i >= 0 && opponentCol + i < 8 && (board[opponentRow + i][opponentCol + i] == whiteQueen || board[opponentRow + i][opponentCol + i] == whiteBishop) && player == "black") ||
            (opponentRow + i >= 0 && opponentRow + i < 8 && opponentCol + i >= 0 && opponentCol + i < 8 && (board[opponentRow + i][opponentCol + i] == blackQueen || board[opponentRow + i][opponentCol + i] == blackBishop) && player == "white"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, opponentRow + i, opponentCol + i, &target, player, true);
        }
        if ((opponentRow + i >= 0 && opponentRow + i < 8 && opponentCol - i >= 0 && opponentCol - i < 8 && (board[opponentRow + i][opponentCol - i] == whiteQueen || board[opponentRow + i][opponentCol - i] == whiteBishop) && player == "black") ||
            (opponentRow + i >= 0 && opponentRow + i < 8 && opponentCol - i >= 0 && opponentCol - i < 8 && (board[opponentRow + i][opponentCol - i] == blackQueen || board[opponentRow + i][opponentCol - i] == blackBishop) && player == "white"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, opponentRow + i, opponentCol - i, &target, player, true);
        }
        if ((opponentRow - i >= 0 && opponentRow - i < 8 && opponentCol - i >= 0 && opponentCol - i < 8 && (board[opponentRow - i][opponentCol - i] == whiteQueen || board[opponentRow - i][opponentCol - i] == whiteBishop) && player == "black") ||
            (opponentRow - i >= 0 && opponentRow - i < 8 && opponentCol - i >= 0 && opponentCol - i < 8 && (board[opponentRow - i][opponentCol - i] == blackQueen || board[opponentRow - i][opponentCol - i] == blackBishop) && player == "white"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, opponentRow - i, opponentCol - i, &target, player, true);
        }
        if ((opponentRow - i >= 0 && opponentRow - i < 8 && opponentCol + i >= 0 && opponentCol + i < 8 && (board[opponentRow - i][opponentCol + i] == whiteQueen || board[opponentRow - i][opponentCol + i] == whiteBishop) && player == "black") ||
            (opponentRow - i >= 0 && opponentRow - i < 8 && opponentCol + i >= 0 && opponentCol + i < 8 && (board[opponentRow - i][opponentCol + i] == blackQueen || board[opponentRow - i][opponentCol + i] == blackBishop) && player == "white"))
        {
            checkmate = checkCaptureValidity(opponentRow, opponentCol, opponentRow - i, opponentCol + i, &target, player, true);
        }
    }

    if (target) checkmate = true;
    int KingPosRow, KingPosCol;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (player == "white" && board[r][c] == blackKing)
            {
                KingPosRow = r;
                KingPosCol = c;
            }
            if (player == "black" && board[r][c] == whiteKing)
            {
                KingPosRow = r;
                KingPosCol = c;
            }
        }
    }
    for (int k = 1; k < 8; k++)
    {
        for (int j = 1; j < 8; j++)
        {
            if (opponentCol == KingPosCol && KingPosRow < opponentRow)
            {
                if ((((board[(KingPosRow + k) + j][KingPosCol + j] == blackQueen || board[(KingPosRow + k) + j][KingPosCol + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k) + j][KingPosCol + j] == whiteQueen || board[(KingPosRow + k) + j][KingPosCol + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + k) + j >= 0 && (KingPosRow + k) + j < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, (KingPosRow + k) + j, KingPosCol + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k) + j][KingPosCol - j] == blackQueen || board[(KingPosRow + k) + j][KingPosCol - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k) + j][KingPosCol - j] == whiteQueen || board[(KingPosRow + k) + j][KingPosCol - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + k) + j >= 0 && (KingPosRow + k) + j < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, (KingPosRow + k) + j, KingPosCol - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k) - j][KingPosCol + j] == blackQueen || board[(KingPosRow + k) - j][KingPosCol + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k) - j][KingPosCol + j] == whiteQueen || board[(KingPosRow + k) - j][KingPosCol + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + k) - j >= 0 && (KingPosRow + k) - j < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, (KingPosRow + k) - j, KingPosCol + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k) - j][KingPosCol - j] == blackQueen || board[(KingPosRow + k) - j][KingPosCol - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k) - j][KingPosCol - j] == whiteQueen || board[(KingPosRow + k) - j][KingPosCol - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + k) - j >= 0 && (KingPosRow + k) - j < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, (KingPosRow + k) - j, KingPosCol - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k)][KingPosCol + j] == blackQueen || board[(KingPosRow + k)][KingPosCol + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k)][KingPosCol + j] == whiteQueen || board[(KingPosRow + k)][KingPosCol + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow + k >= 0 && KingPosRow + k < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, KingPosRow + k, KingPosCol + j, false))
                    {
                        return false;
                    }
                }
                if (((board[(KingPosRow + k)][KingPosCol - j] == blackQueen || board[(KingPosRow + k)][KingPosCol - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k)][KingPosCol - j] == whiteQueen || board[(KingPosRow + k)][KingPosCol - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing)
                    && KingPosRow + k >= 0 && KingPosRow + k < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + k, KingPosCol, KingPosRow + k, KingPosCol - j, false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow + k, KingPosCol, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
            if (opponentCol == KingPosCol && KingPosRow > opponentRow)
            {
                if ((((board[(KingPosRow - k) + j][KingPosCol + j] == blackQueen || board[(KingPosRow - k) + j][KingPosCol + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - k) + j][KingPosCol + j] == whiteQueen || board[(KingPosRow - k) + j][KingPosCol + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - k) + j >= 0 && (KingPosRow - k) + j < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, (KingPosRow - k) + j, KingPosCol + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - k) + j][KingPosCol - j] == blackQueen || board[(KingPosRow - k) + j][KingPosCol - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - k) + j][KingPosCol - j] == whiteQueen || board[(KingPosRow - k) + j][KingPosCol - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - k) + j >= 0 && (KingPosRow - k) + j < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, (KingPosRow - k) + j, KingPosCol - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - k) - j][KingPosCol + j] == blackQueen || board[(KingPosRow - k) - j][KingPosCol + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - k) - j][KingPosCol + j] == whiteQueen || board[(KingPosRow - k) - j][KingPosCol + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - k) - j >= 0 && (KingPosRow - k) - j < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, (KingPosRow - k) - j, KingPosCol + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - k) - j][KingPosCol - j] == blackQueen || board[(KingPosRow - k) - j][KingPosCol - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - k) - j][KingPosCol - j] == whiteQueen || board[(KingPosRow - k) - j][KingPosCol - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - k) - j >= 0 && (KingPosRow - k) - j < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, (KingPosRow - k) - j, KingPosCol - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k)][KingPosCol + j] == blackQueen || board[(KingPosRow + k)][KingPosCol + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k)][KingPosCol + j] == whiteQueen || board[(KingPosRow + k)][KingPosCol + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow + k >= 0 && KingPosRow + k < 8 && KingPosCol + j >= 0 && KingPosCol + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, KingPosRow + k, KingPosCol + j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + k)][KingPosCol - j] == blackQueen || board[(KingPosRow + k)][KingPosCol - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + k)][KingPosCol - j] == whiteQueen || board[(KingPosRow + k)][KingPosCol - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow + k >= 0 && KingPosRow + k < 8 && KingPosCol - j >= 0 && KingPosCol - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - k, KingPosCol, KingPosRow + k, KingPosCol - j, false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow - k, KingPosCol, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
        }
    }
    for (int k = 1; k < opponentCol; k++)
    {
        for (int j = 1; j < 8; j++)
        {
            if (opponentCol > KingPosCol && KingPosRow == opponentRow)
            {
                if ((((board[(KingPosRow)+j][(KingPosCol + k) + j] == blackQueen || board[(KingPosRow)+j][(KingPosCol + k) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)+j][(KingPosCol + k) + j] == whiteQueen || board[(KingPosRow)+j][(KingPosCol + k) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)+j >= 0 && (KingPosRow)+j < 8 && (KingPosCol + k) + j >= 0 && (KingPosCol + k) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, (KingPosRow)+j, (KingPosCol + k) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)-j][(KingPosCol + k) + j] == blackQueen || board[(KingPosRow)-j][(KingPosCol + k) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)-j][(KingPosCol + k) + j] == whiteQueen || board[(KingPosRow)-j][(KingPosCol + k) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)-j >= 0 && (KingPosRow)-j < 8 && (KingPosCol + k) + j >= 0 && (KingPosCol + k) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, (KingPosRow)-j, (KingPosCol + k) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)+j][(KingPosCol + k) - j] == blackQueen || board[(KingPosRow)+j][(KingPosCol + k) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)+j][(KingPosCol + k) - j] == whiteQueen || board[(KingPosRow)+j][(KingPosCol + k) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)+j >= 0 && (KingPosRow)+j < 8 && (KingPosCol + k) - j >= 0 && (KingPosCol + k) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, (KingPosRow)+j, (KingPosCol + k) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)-j][(KingPosCol + k) - j] == blackQueen || board[(KingPosRow)-j][(KingPosCol + k) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)-j][(KingPosCol + k) - j] == whiteQueen || board[(KingPosRow)-j][(KingPosCol + k) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)-j >= 0 && (KingPosRow)-j < 8 && (KingPosCol + k) - j >= 0 && (KingPosCol + k) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, (KingPosRow)-j, (KingPosCol + k) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + j)][KingPosCol + k] == blackQueen || board[(KingPosRow + j)][KingPosCol + k] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + j)][KingPosCol + k] == whiteQueen || board[(KingPosRow + j)][KingPosCol + k] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow + j >= 0 && KingPosRow + j < 8 && KingPosCol + k >= 0 && KingPosCol + k < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, KingPosRow + j, KingPosCol + k, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - j)][KingPosCol + k] == blackQueen || board[(KingPosRow - j)][KingPosCol + k] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - j)][KingPosCol + k] == whiteQueen || board[(KingPosRow - j)][KingPosCol + k] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow - j >= 0 && KingPosRow - j < 8 && KingPosCol + k >= 0 && KingPosCol + k < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol + k, KingPosRow - j, KingPosCol + k, false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow, KingPosCol + k, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
            if (opponentCol < KingPosCol && KingPosRow == opponentRow)
            {
                if ((((board[(KingPosRow)+j][(KingPosCol - k) - j] == blackQueen || board[(KingPosRow)+j][(KingPosCol - k) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)+j][(KingPosCol - k) - j] == whiteQueen || board[(KingPosRow)+j][(KingPosCol - k) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)+j >= 0 && (KingPosRow)+j < 8 && (KingPosCol - k) - j >= 0 && (KingPosCol - k) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, (KingPosRow)+j, (KingPosCol - k) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)+j][(KingPosCol - k) + j] == blackQueen || board[(KingPosRow)+j][(KingPosCol - k) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)+j][(KingPosCol - k) + j] == whiteQueen || board[(KingPosRow)+j][(KingPosCol - k) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)-j >= 0 && (KingPosRow)-j < 8 && (KingPosCol + k) + j >= 0 && (KingPosCol + k) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, (KingPosRow)+j, (KingPosCol - k) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)-j][(KingPosCol - k) - j] == blackQueen || board[(KingPosRow)-j][(KingPosCol - k) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)-j][(KingPosCol - k) - j] == whiteQueen || board[(KingPosRow)-j][(KingPosCol - k) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)-j >= 0 && (KingPosRow)-j < 8 && (KingPosCol - k) - j >= 0 && (KingPosCol - k) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, (KingPosRow)-j, (KingPosCol - k) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow)-j][(KingPosCol - k) + j] == blackQueen || board[(KingPosRow)-j][(KingPosCol - k) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow)-j][(KingPosCol - k) + j] == whiteQueen || board[(KingPosRow)-j][(KingPosCol - k) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow)-j >= 0 && (KingPosRow)-j < 8 && (KingPosCol - k) + j >= 0 && (KingPosCol - k) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, (KingPosRow)-j, (KingPosCol - k) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + j)][KingPosCol - k] == blackQueen || board[(KingPosRow + j)][KingPosCol - k] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + j)][KingPosCol - k] == whiteQueen || board[(KingPosRow + j)][KingPosCol - k] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow + j >= 0 && KingPosRow + j < 8 && KingPosCol - k >= 0 && KingPosCol - k < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, KingPosRow + j, KingPosCol - k, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - j)][KingPosCol - k] == blackQueen || board[(KingPosRow - j)][KingPosCol - k] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - j)][KingPosCol - k] == whiteQueen || board[(KingPosRow - j)][KingPosCol - k] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && KingPosRow - j >= 0 && KingPosRow - j < 8 && KingPosCol - k >= 0 && KingPosCol - k < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow, KingPosCol - k, KingPosRow - j, KingPosCol - k, false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow, KingPosCol - k, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
        }
    }
    for (int r = 1, c = 1; r < 8 && c < 8; r++, c++)
    {
        for (int j = 1; j < 8; j++)
        {
            if (opponentCol > KingPosCol && opponentRow > KingPosRow)
            {
                if (((board[((KingPosRow + r) + j) - 1][(KingPosCol + c) + j] == blackPawn && board[KingPosRow][KingPosCol] == blackKing)) &&
                    ((KingPosRow + r) + j) - 1 >= 0 && ((KingPosRow + r) + j) - 1 < 8 && (KingPosCol + c) + j > KingPosCol && (KingPosCol + c) + j < opponentCol)
                {
                    return false;
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol + c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol + c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (board[((KingPosRow + r) + j) - 1][(KingPosCol + c) + j] == blackPawn)
                    {
                        return false;
                    }
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) + j, (KingPosCol + c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol + c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol + c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) + j, (KingPosCol + c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol + c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol + c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) - j, (KingPosCol + c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol + c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol + c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) - j, (KingPosCol + c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r)][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow + r)][(KingPosCol + c) + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r)][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow + r)][(KingPosCol + c) + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) >= 0 && (KingPosRow + r) < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r), (KingPosCol + c) + j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r)][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow + r)][(KingPosCol + c) - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r)][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow + r)][(KingPosCol + c) - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) >= 0 && (KingPosRow + r) < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r), (KingPosCol + c) - j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol + c)] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol + c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol + c)] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol + c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol + c) >= 0 && (KingPosCol + c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) + j, (KingPosCol + c), false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol + c)] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol + c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol + c)] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol + c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol + c) >= 0 && (KingPosCol + c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol + c, (KingPosRow + r) - j, (KingPosCol + c), false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow + r, KingPosCol + c, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
            if (opponentCol < KingPosCol && opponentRow > KingPosRow)
            {
                if ((board[((KingPosRow + r) + j) - 1][(KingPosCol - c) + j] == blackPawn && board[KingPosRow][KingPosCol] == blackKing) &&
                    ((KingPosRow + r) + j) - 1 >= 0 && ((KingPosRow + r) + j) - 1 < 8 && (KingPosCol - c) + j < KingPosCol && (KingPosCol - c) + j > opponentCol)
                {
                    return false;
                }
                if ((board[((KingPosRow + r) + j) - 1][(KingPosCol - c) + j] == blackPawn && board[KingPosRow][KingPosCol] == blackKing) ||
                    (board[((KingPosRow + r) + j) + 1][(KingPosCol - c) + j] == whitePawn && board[KingPosRow][KingPosCol] == whiteKing))
                {
                    return false;
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol - c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol - c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) + j, (KingPosCol - c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol - c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol - c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) + j, (KingPosCol - c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol - c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol - c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) - j, (KingPosCol - c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol - c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol - c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) - j, (KingPosCol - c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r)][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow + r)][(KingPosCol - c) + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r)][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow + r)][(KingPosCol - c) + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) >= 0 && (KingPosRow + r) < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r), (KingPosCol - c) + j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r)][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow + r)][(KingPosCol - c) - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r)][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow + r)][(KingPosCol - c) - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) >= 0 && (KingPosRow + r) < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r), (KingPosCol - c) - j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) + j][(KingPosCol - c)] == blackQueen || board[(KingPosRow + r) + j][(KingPosCol - c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) + j][(KingPosCol - c)] == whiteQueen || board[(KingPosRow + r) + j][(KingPosCol - c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) + j >= 0 && (KingPosRow + r) + j < 8 && (KingPosCol - c) >= 0 && (KingPosCol - c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) + j, (KingPosCol - c), false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow + r) - j][(KingPosCol - c)] == blackQueen || board[(KingPosRow + r) - j][(KingPosCol - c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow + r) - j][(KingPosCol - c)] == whiteQueen || board[(KingPosRow + r) - j][(KingPosCol - c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow + r) - j >= 0 && (KingPosRow + r) - j < 8 && (KingPosCol - c) >= 0 && (KingPosCol - c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow + r, KingPosCol - c, (KingPosRow + r) - j, (KingPosCol - c), false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow + r, KingPosCol - c, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
            if (opponentCol < KingPosCol && opponentRow < KingPosRow)
            {
                if ((board[((KingPosRow - r) + j) + 1][(KingPosCol - c) + j] == whitePawn && board[KingPosRow][KingPosCol] == whiteKing))
                {
                    return false;
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol - c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol - c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) + j, (KingPosCol - c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol - c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol - c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) + j, (KingPosCol - c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol - c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol - c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) - j, (KingPosCol - c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol - c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol - c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) - j, (KingPosCol - c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r)][(KingPosCol - c) + j] == blackQueen || board[(KingPosRow - r)][(KingPosCol - c) + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r)][(KingPosCol - c) + j] == whiteQueen || board[(KingPosRow - r)][(KingPosCol - c) + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) >= 0 && (KingPosRow - r) < 8 && (KingPosCol - c) + j >= 0 && (KingPosCol - c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r), (KingPosCol - c) + j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r)][(KingPosCol - c) - j] == blackQueen || board[(KingPosRow - r)][(KingPosCol - c) - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r)][(KingPosCol - c) - j] == whiteQueen || board[(KingPosRow - r)][(KingPosCol - c) - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) >= 0 && (KingPosRow - r) < 8 && (KingPosCol - c) - j >= 0 && (KingPosCol - c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r), (KingPosCol - c) - j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol - c)] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol - c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol - c)] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol - c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol - c) >= 0 && (KingPosCol - c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) + j, (KingPosCol - c), false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol - c)] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol - c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol - c)] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol - c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol - c) >= 0 && (KingPosCol - c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol - c, (KingPosRow - r) - j, (KingPosCol - c), false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow - r, KingPosCol - c, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
            if (opponentCol > KingPosCol && opponentRow < KingPosRow)
            {
                if ((board[((KingPosRow - r) + j) + 1][(KingPosCol + c) + j] == whitePawn && board[KingPosRow][KingPosCol] == whiteKing))
                {
                    return false;
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol + c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol + c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) + j, (KingPosCol + c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol + c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol + c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) + j, (KingPosCol + c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol + c) + j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol + c) + j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) - j, (KingPosCol + c) + j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol + c) - j] == blackBishop) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol + c) - j] == whiteBishop) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) - j, (KingPosCol + c) - j, true))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r)][(KingPosCol + c) + j] == blackQueen || board[(KingPosRow - r)][(KingPosCol + c) + j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r)][(KingPosCol + c) + j] == whiteQueen || board[(KingPosRow - r)][(KingPosCol + c) + j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) >= 0 && (KingPosRow - r) < 8 && (KingPosCol + c) + j >= 0 && (KingPosCol + c) + j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r), (KingPosCol + c) + j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r)][(KingPosCol + c) - j] == blackQueen || board[(KingPosRow - r)][(KingPosCol + c) - j] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r)][(KingPosCol + c) - j] == whiteQueen || board[(KingPosRow - r)][(KingPosCol + c) - j] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) >= 0 && (KingPosRow - r) < 8 && (KingPosCol + c) - j >= 0 && (KingPosCol + c) - j < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r), (KingPosCol + c) - j, false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) + j][(KingPosCol + c)] == blackQueen || board[(KingPosRow - r) + j][(KingPosCol + c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) + j][(KingPosCol + c)] == whiteQueen || board[(KingPosRow - r) + j][(KingPosCol + c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) + j >= 0 && (KingPosRow - r) + j < 8 && (KingPosCol + c) >= 0 && (KingPosCol + c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) + j, (KingPosCol + c), false))
                    {
                        return false;
                    }
                }
                if ((((board[(KingPosRow - r) - j][(KingPosCol + c)] == blackQueen || board[(KingPosRow - r) - j][(KingPosCol + c)] == blackRook) && board[KingPosRow][KingPosCol] == blackKing) ||
                    ((board[(KingPosRow - r) - j][(KingPosCol + c)] == whiteQueen || board[(KingPosRow - r) - j][(KingPosCol + c)] == whiteRook) && board[KingPosRow][KingPosCol] == whiteKing))
                    && (KingPosRow - r) - j >= 0 && (KingPosRow - r) - j < 8 && (KingPosCol + c) >= 0 && (KingPosCol + c) < 8)
                {
                    if (!checkBlockValidity(KingPosRow, KingPosCol, KingPosRow - r, KingPosCol + c, (KingPosRow - r) - j, (KingPosCol + c), false))
                    {
                        return false;
                    }
                }
                if (!checkKnightValidity(KingPosRow - r, KingPosCol + c, KingPosRow, KingPosCol))
                {
                    return false;
                }
            }
        }
    }

    if (!checkKnightValidity(opponentRow, opponentCol, KingPosRow, KingPosCol))
    {
        return false;
    }
    bool kingMove1, kingMove2, kingMove3, kingMove4, kingMove5, kingMove6, kingMove7, kingMove8;
    wchar_t King = (board[KingPosRow][KingPosCol] == blackKing ? blackKing : whiteKing);
    string KingPlayer = (board[KingPosRow][KingPosCol] == blackKing ? "black" : "white");
    if (checkmate)
    {
        char col = KingPosCol + 'a';
        char row = '8' - KingPosRow;
        string move = "K";
        if (row + 1 >= '0' && row + 1 < '8' && col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1; row += 1;
            kingMove1 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove1 = false;
        if (row + 1 >= '0' && row + 1 < '8' && col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1; row += 1;
            kingMove2 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove2 = false;
        if (row - 1 >= '0' && row - 1 < '8' && col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1; row -= 1;
            kingMove3 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove3 = false;
        if (row - 1 >= '0' && row - 1 < '8' && col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1; row -= 1;
            kingMove4 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove4 = false;
        if (col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1;
            kingMove5 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove5 = false;
        if (col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1;
            kingMove6 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove6 = false;
        if (row + 1 >= '0' && row + 1 < '8')
        {
            row += 1;
            kingMove7 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove7 = false;
        if (row - 1 >= '0' && row - 1 < '8')
        {
            row -= 1;
            move += col; move += row;
            kingMove8 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove8 = false;
        if (!kingMove1 && !kingMove2 && !kingMove3 && !kingMove4 && !kingMove5 && !kingMove6 && !kingMove7 && !kingMove8)
            checkmate = true;
        else
            checkmate = false;
    }
    return checkmate;
}

bool checkDraw(string player)
{
    int KingPosRow, KingPosCol;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (player == "white")
            {
                if (board[r][c] == blackKing)
                {
                    KingPosRow = r;
                    KingPosCol = c;
                }
                if (board[r][c] == blackBishop || board[r][c] == blackKnight || board[r][c] == blackQueen || board[r][c] == blackRook || board[r][c] == blackPawn)
                    return false;
            }
            if (player == "black")
            {
                if (board[r][c] == whiteKing)
                {
                    KingPosRow = r;
                    KingPosCol = c;
                }
                if (board[r][c] == whiteBishop || board[r][c] == whiteKnight || board[r][c] == whiteQueen || board[r][c] == whiteRook || board[r][c] == whitePawn)
                    return false;
            }
        }
    }
    bool kingMove1, kingMove2, kingMove3, kingMove4, kingMove5, kingMove6, kingMove7, kingMove8;
    bool draw = true;
    wchar_t King = (board[KingPosRow][KingPosCol] == blackKing ? blackKing : whiteKing);
    string KingPlayer = (board[KingPosRow][KingPosCol] == blackKing ? "black" : "white");
    if (draw)
    {
        char col = KingPosCol + 'a';
        char row = '8' - KingPosRow;
        string move = "K";
        if (row + 1 >= '0' && row + 1 < '8' && col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1; row += 1;
            kingMove1 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove1 = false;
        if (row + 1 >= '0' && row + 1 < '8' && col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1; row += 1;
            kingMove2 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove2 = false;
        if (row - 1 >= '0' && row - 1 < '8' && col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1; row -= 1;
            kingMove3 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove3 = false;
        if (row - 1 >= '0' && row - 1 < '8' && col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1; row -= 1;
            kingMove4 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove4 = false;
        if (col + 1 >= 'a' && col + 1 < 'h')
        {
            col += 1;
            kingMove5 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove5 = false;
        if (col - 1 >= 'a' && col - 1 < 'h')
        {
            col -= 1;
            kingMove6 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove6 = false;
        if (row + 1 >= '0' && row + 1 < '8')
        {
            row += 1;
            kingMove7 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove7 = false;
        if (row - 1 >= '0' && row - 1 < '8')
        {
            row -= 1;
            move += col; move += row;
            kingMove8 = checkKingValidity(move, row, col, KingPosRow, KingPosCol, KingPlayer, King);
            col = KingPosCol + 'a';
            row = '8' - KingPosRow;
        }
        else kingMove8 = false;
        if (!kingMove1 && !kingMove2 && !kingMove3 && !kingMove4 && !kingMove5 && !kingMove6 && !kingMove7 && !kingMove8)
            draw = true;
        else
            draw = false;
    }
    return draw;
}

//============   Both PLayers Movement =========================
wstring moveHistory(wstring move, char piece, bool checkmate, bool check, string player)
{
    if (move[move.length() - 1] != '+' || move[move.length() - 1] != '#')
    {
        if (checkmate) move += L"#";
        else if (check) move += L"+";
    }
    wstring piece_to_add;
    switch (piece)
    {
    case 'Q':
        piece_to_add = (player == "white" ? whiteQueen : blackQueen);
        break;
    case 'R':
        piece_to_add = (player == "white" ? whiteRook : blackRook);
        break;
    case 'B':
        piece_to_add = (player == "white" ? whiteBishop : blackBishop);
        break;
    case 'N':
        piece_to_add = (player == "white" ? whiteKnight : blackKnight);
        break;
    case 'K':
        piece_to_add = (player == "white" ? whiteKing : blackKing);
        break;
    default:
        return move;
        break;
    }
    move = piece_to_add + L" " + move;
    return move;
}
void movement()
{
    bool blackCheckCalculate = false;
    bool whiteCheckCalculate = false;
    string whitemove;
    string blackmove;
    bool checkmateCalculate = false;
    bool drawCheck = false;
    bool blackPawnfirst = false;
    bool whitePawnfirst = false;
    int blackPawnCol_enpassant = 0;
    int whitePawnCol_enpassant = 0;
    int movesCounter = 1;
    vector<wchar_t> whiteCaptures;
    vector<wchar_t> blackCaptures;
    vector<wstring> gameHistory;
    wstring whiteHistory, blackHistory;
    drawBoard(&gameHistory);
    //=================================white move=================================
    while (true)
    {
        whiteHistory = blackHistory = L"";
        bool validWhiteMove = false;
        whitePawnfirst = false;
        wcout << "Pieces you have captured: ";
        for (wchar_t pieces : whiteCaptures)
        {
            wcout << pieces << " ";
        }
        wcout << endl;
        while (!validWhiteMove)
        {
            wcout << "Player:1(white) enter your move e.g(e4,exd4,Qf7,Bf4+): ";
            cin >> whitemove;

            if (whitemove.length() == 2 || (whitemove.length() == 4 && whitemove[1] == 'x' && whitemove[0] != 'Q' && whitemove[0] != 'R' && whitemove[0] != 'B' && whitemove[0] != 'N' && whitemove[0] != 'K') ||
                (whitemove.length() == 5 && whitemove[1] == 'x' && whitemove[0] != 'Q' && whitemove[0] != 'R' && whitemove[0] != 'B' && whitemove[0] != 'N' && whitemove[0] != 'K' && whitemove[4] == '+') ||
                (whitemove.length() == 3 && whitemove[2] == '+'))
            {

                bool check = whitePawnmove(whitemove, &whitePawnfirst, blackPawnfirst, &whitePawnCol_enpassant, blackPawnCol_enpassant, &whiteCaptures);
                wstring temp(whitemove.begin(), whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else if ((whitemove.length() == 3 && whitemove[0] == 'Q') || (whitemove.length() == 4 && whitemove[0] == 'Q' && whitemove[1] == 'x') ||
                (whitemove.length() == 4 && whitemove[0] == 'Q' && whitemove[3] == '+') || (whitemove.length() == 5 && whitemove[0] == 'Q' && whitemove[1] == 'x' && whitemove[4] == '+'))
            {
                bool check = Queenmove(whitemove, "white", &whiteCaptures);
                wstring temp(whitemove.begin() + 1, whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else if ((whitemove.length() == 3 && whitemove[0] == 'B') || (whitemove.length() == 4 && whitemove[0] == 'B' && whitemove[1] == 'x') ||
                (whitemove.length() == 4 && whitemove[0] == 'B' && whitemove[3] == '+') || (whitemove.length() == 5 && whitemove[0] == 'B' && whitemove[1] == 'x' && whitemove[4] == '+'))
            {
                bool check = Bishopmove(whitemove, "white", &whiteCaptures);
                wstring temp(whitemove.begin() + 1, whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else if ((whitemove.length() == 3 && whitemove[0] == 'R') || (whitemove.length() == 4 && whitemove[0] == 'R' && whitemove[1] == 'x') ||
                (whitemove.length() == 4 && whitemove[0] == 'R' && whitemove[3] == '+') || (whitemove.length() == 5 && whitemove[0] == 'R' && whitemove[1] == 'x' && whitemove[4] == '+'))
            {
                bool check = Rookmove(whitemove, "white", &whiteCaptures);
                wstring temp(whitemove.begin() + 1, whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else if ((whitemove.length() == 3 && whitemove[0] == 'N') || (whitemove.length() == 4 && whitemove[0] == 'N' && whitemove[1] == 'x') ||
                (whitemove.length() == 4 && whitemove[0] == 'N' && whitemove[3] == '+') || (whitemove.length() == 5 && whitemove[0] == 'N' && whitemove[1] == 'x' && whitemove[4] == '+'))
            {
                bool check = Knightmove(whitemove, "white", &whiteCaptures);
                wstring temp(whitemove.begin() + 1, whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else if ((whitemove.length() == 3 && whitemove[0] == 'K') || (whitemove.length() == 4 && whitemove[0] == 'K' && whitemove[1] == 'x') ||
                (whitemove.length() == 4 && whitemove[0] == 'K' && whitemove[3] == '+') || (whitemove.length() == 5 && whitemove[0] == 'K' && whitemove[1] == 'x' && whitemove[4] == '+'))
            {
                bool check = Kingmove(whitemove, "white");
                wstring temp(whitemove.begin() + 1, whitemove.end());
                blackCheckCalculate = checkCalculator("black");
                if (blackCheckCalculate) checkmateCalculate = checkmate(whitemove, "white");
                if (check && !blackCheckCalculate) drawCheck = checkDraw("white");
                if (check)
                    validWhiteMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    whiteHistory = moveHistory(temp, whitemove[0], checkmateCalculate, blackCheckCalculate, "white");
                }
            }
            else
            {
                wcout << "\tInvalid Move! Enter again....\n";
            }
        }
        gameHistory.push_back(whiteHistory);
        if (checkmateCalculate)
        {
            system("CLS");
            drawBoard(&gameHistory);
            wcout << "\tWhite Won!!!";
            break;
        }
        if (drawCheck)
        {
            system("CLS");
            drawBoard(&gameHistory);
            wcout << "\tSettlemate/Draw!!!\n";
            break;
        }
        system("CLS");
        drawBoard(&gameHistory);
        if (blackCheckCalculate)
        {
            wcout << "\tCheck on Black... Save your King!!!\n";
        }

        //=================================black move============================
        bool validBlackMove = false;
        blackPawnfirst = false;
        wcout << "Pieces you have captured: ";
        for (wchar_t pieces : blackCaptures)
        {
            wcout << pieces << " ";
        }
        wcout << endl;
        while (!validBlackMove)
        {
            wcout << "Player:2(black)enter your move e.g(e4, exd4, Qf7, Bf4+ ) : ";
            cin >> blackmove;
            if (blackmove.length() == 2 || (blackmove.length() == 4 && blackmove[1] == 'x' && blackmove[0] != 'Q' && blackmove[0] != 'R' && blackmove[0] != 'B' && blackmove[0] != 'N' && blackmove[0] != 'K') ||
                (blackmove.length() == 5 && blackmove[1] == 'x' && blackmove[0] != 'Q' && blackmove[0] != 'R' && blackmove[0] != 'B' && blackmove[0] != 'N' && blackmove[0] != 'K' && blackmove[4] == '+') ||
                (blackmove.length() == 3 && blackmove[2] == '+'))
            {
                bool check = blackPawnmove(blackmove, &blackPawnfirst, whitePawnfirst, &blackPawnCol_enpassant, whitePawnCol_enpassant, &blackCaptures);
                wstring temp(blackmove.begin(), blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else if ((blackmove.length() == 3 && blackmove[0] == 'Q') || (blackmove.length() == 4 && blackmove[0] == 'Q' && blackmove[1] == 'x') ||
                (blackmove.length() == 4 && blackmove[0] == 'Q' && blackmove[3] == '+') || (blackmove.length() == 5 && blackmove[0] == 'Q' && blackmove[1] == 'x' && blackmove[4] == '+'))
            {
                bool check = Queenmove(blackmove, "black", &blackCaptures);
                wstring temp(blackmove.begin() + 1, blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else if ((blackmove.length() == 3 && blackmove[0] == 'B') || (blackmove.length() == 4 && blackmove[0] == 'B' && blackmove[1] == 'x') ||
                (blackmove.length() == 4 && blackmove[0] == 'B' && blackmove[3] == '+') || (blackmove.length() == 5 && blackmove[0] == 'B' && blackmove[1] == 'x' && blackmove[4] == '+'))
            {
                bool check = Bishopmove(blackmove, "black", &blackCaptures);
                wstring temp(blackmove.begin() + 1, blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else if ((blackmove.length() == 3 && blackmove[0] == 'R') || (blackmove.length() == 4 && blackmove[0] == 'R' && blackmove[1] == 'x') ||
                (blackmove.length() == 4 && blackmove[0] == 'R' && blackmove[3] == '+') || (blackmove.length() == 5 && blackmove[0] == 'R' && blackmove[1] == 'x' && blackmove[4] == '+'))
            {
                bool check = Rookmove(blackmove, "black", &blackCaptures);
                wstring temp(blackmove.begin() + 1, blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else if ((blackmove.length() == 3 && blackmove[0] == 'N') || (blackmove.length() == 4 && blackmove[0] == 'N' && blackmove[1] == 'x') ||
                (blackmove.length() == 4 && blackmove[0] == 'N' && blackmove[3] == '+') || (blackmove.length() == 5 && blackmove[0] == 'N' && blackmove[1] == 'x' && blackmove[4] == '+'))
            {
                bool check = Knightmove(blackmove, "black", &blackCaptures);
                wstring temp(blackmove.begin() + 1, blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else if ((blackmove.length() == 3 && blackmove[0] == 'K') || (blackmove.length() == 4 && blackmove[0] == 'K' && blackmove[1] == 'x') ||
                (blackmove.length() == 4 && blackmove[0] == 'K' && blackmove[3] == '+') || (blackmove.length() == 5 && blackmove[0] == 'K' && blackmove[1] == 'x' && blackmove[4] == '+'))
            {
                bool check = Kingmove(blackmove, "black");
                wstring temp(blackmove.begin() + 1, blackmove.end());
                whiteCheckCalculate = checkCalculator("white");
                if (whiteCheckCalculate) checkmateCalculate = checkmate(blackmove, "black");
                if (check && !whiteCheckCalculate) drawCheck = checkDraw("black");
                if (check)
                    validBlackMove = true;
                else
                    wcout << "\tInvalid Move! Enter again....\n";
                if (validWhiteMove)
                {
                    blackHistory = moveHistory(temp, blackmove[0], checkmateCalculate, whiteCheckCalculate, "black");
                }
            }
            else
            {
                wcout << "\tInvalid Move! Enter again....\n";
            }
        }
        gameHistory.at(movesCounter - 1) = gameHistory[movesCounter - 1] + L" " + blackHistory;
        if (checkmateCalculate)
        {
            system("CLS");
            drawBoard(&gameHistory);
            wcout << "\tBlack Won!!!!";
            break;
        }
        if (drawCheck)
        {
            system("CLS");
            drawBoard(&gameHistory);
            wcout << "\tSettlemate/Draw!!!\n";
            break;
        }
        system("CLS");
        drawBoard(&gameHistory);

        if (whiteCheckCalculate) {
            wcout << "\tCheck on White... Save your King!!!\n";
        }
        movesCounter++;
        if (movesCounter > 50)
        {
            wcout << "\tGame Draw..Because of 50 moves rule..";
            break;
        }
    }
}
void main()
{
    system("cls");
    initializeBoard();
    movement();
}