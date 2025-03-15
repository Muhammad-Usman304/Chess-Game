namespace CHESS
{
    const wchar_t whiteKing = L'\u2654';
    const wchar_t whiteQueen = L'\u2655';
    const wchar_t whiteRook = L'\u2656';
    const wchar_t whiteBishop = L'\u2657';
    const wchar_t whiteKnight = L'\u2658';
    const wchar_t whitePawn = L'\u2659';

    const wchar_t blackKing = L'\u265A';
    const wchar_t blackQueen = L'\u265B';
    const wchar_t blackRook = L'\u265C';
    const wchar_t blackBishop = L'\u265D';
    const wchar_t blackKnight = L'\u265E';
    const wchar_t blackPawn = L'\u265F';

    // Unicode box-drawing characters
    const wchar_t topLeft = L'\u250C';
    const wchar_t topRight = L'\u2510';
    const wchar_t bottomLeft = L'\u2514';
    const wchar_t bottomRight = L'\u2518';
    const wchar_t topJoin = L'\u252C';
    const wchar_t bottomJoin = L'\u2534';
    const wchar_t leftJoin = L'\u251C';
    const wchar_t rightJoin = L'\u2524';
    const wchar_t crossJoin = L'\u253C';
    const wchar_t horizontal = L'\u2500';
    const wchar_t vertical = L'\u2502';

    // Change the board to wchar_t to handle Unicode characters properly
    wchar_t board[8][8];

    void initializeBoard() {
        // Set up white pieces
        board[0][0] = board[0][7] = blackRook;
        board[0][1] = board[0][6] = blackKnight;
        board[0][2] = board[0][5] = blackBishop;
        board[0][3] = blackQueen;
        board[0][4] = blackKing;
        for (int i = 0; i < 8; ++i) board[1][i] = blackPawn;

        // Set up black pieces
        board[7][0] = board[7][7] = whiteRook;
        board[7][1] = board[7][6] = whiteKnight;
        board[7][2] = board[7][5] = whiteBishop;
        board[7][3] = whiteQueen;
        board[7][3] = whiteQueen;
        board[7][4] = whiteKing;
        for (int i = 0; i < 8; ++i) board[6][i] = whitePawn;

        // Empty spaces
        for (int i = 2; i < 6; ++i)
            for (int j = 0; j < 8; ++j)
                board[i][j] = L' ';  // Empty space
    }

}