#ifndef QUIZDATA_H
#define QUIZDATA_H

struct QuizQuestion
{
    String question;
    String answer;
    uint8_t difficulty; // 1=easy, 2=medium, 3=hard
};

struct QuizSet
{
    QuizQuestion q1, q2, q3;
};

// Mapping table for 3-button color combinations to quiz set indices
// Based on your hash function: convertColorCodeTo3Digits()
struct ColorCodeMapping
{
    String colorCode; // Original 3-digit color code (111, 112, etc.)
    int hashedCode;   // Hashed 3-digit code (201, 224, etc.)
    int quizSetIndex; // Index into QUIZ_DATA array
};

static const ColorCodeMapping COLOR_TO_QUIZ_MAP[27] = {
    // Only 3-button combinations (111 to 333)
    {"111", 201, 0},  // Red-Red-Red
    {"112", 224, 1},  // Red-Red-Yellow
    {"113", 247, 2},  // Red-Red-Blue
    {"121", 270, 3},  // Red-Yellow-Red
    {"122", 293, 4},  // Red-Yellow-Yellow
    {"123", 316, 5},  // Red-Yellow-Blue
    {"131", 339, 6},  // Red-Blue-Red
    {"132", 362, 7},  // Red-Blue-Yellow
    {"133", 385, 8},  // Red-Blue-Blue
    {"211", 408, 9},  // Yellow-Red-Red
    {"212", 431, 10}, // Yellow-Red-Yellow
    {"213", 454, 11}, // Yellow-Red-Blue
    {"221", 477, 12}, // Yellow-Yellow-Red
    {"222", 500, 13}, // Yellow-Yellow-Yellow
    {"223", 523, 14}, // Yellow-Yellow-Blue
    {"231", 546, 15}, // Yellow-Blue-Red
    {"232", 569, 16}, // Yellow-Blue-Yellow
    {"233", 592, 17}, // Yellow-Blue-Blue
    {"311", 615, 18}, // Blue-Red-Red
    {"312", 638, 19}, // Blue-Red-Yellow
    {"313", 661, 20}, // Blue-Red-Blue
    {"321", 684, 21}, // Blue-Yellow-Red
    {"322", 707, 22}, // Blue-Yellow-Yellow
    {"323", 730, 23}, // Blue-Yellow-Blue
    {"331", 753, 24}, // Blue-Blue-Red
    {"332", 776, 25}, // Blue-Blue-Yellow
    {"333", 799, 26}  // Blue-Blue-Blue
};

// Function to get quiz set index from hashed code
int getQuizSetFromHashedCode(int hashedCode)
{
    for (int i = 0; i < 27; i++) // Only check 27 entries
    {
        if (COLOR_TO_QUIZ_MAP[i].hashedCode == hashedCode)
        {
            return COLOR_TO_QUIZ_MAP[i].quizSetIndex;
        }
    }
    return 0; // Default to first quiz set if not found
}

// Alternative function to get quiz set index from original color code
int getQuizSetFromColorCode(const String &colorCode)
{
    for (int i = 0; i < 27; i++) // Only check 27 entries
    {
        if (COLOR_TO_QUIZ_MAP[i].colorCode == colorCode)
        {
            return COLOR_TO_QUIZ_MAP[i].quizSetIndex;
        }
    }
    return 0; // Default to first quiz set if not found
}

static const QuizSet QUIZ_DATA[27] = {
    // 0 - Basic counting (111 → 201)
    {
        {"What is 1?", "1", 1},
        {"What is 2?", "2", 1},
        {"What is 3?", "3", 1}},

    // 1 - Simple multiplication and addition (112 → 224)
    {
        {"4 x 1 =", "4", 1},
        {"5 x 2 =", "10", 2},
        {"2 + 2 =", "4", 1}},

    // 2 - Subtraction and division (113 → 247)
    {
        {"8 - 1 =", "7", 1},
        {"8 / 2 =", "4", 2},
        {"9 - 0 =", "9", 1}},

    // 3 - Zero operations (121 → 247)
    {
        {"2 - 1 =", "1", 1},
        {"2 + 2 =", "4", 1},
        {"1 + 1 =", "2", 1}},

    // 4 - Basic operations with 1 (122 → 270)
    {
        {"1 - 0 =", "1", 1},
        {"2 - 1 =", "1", 1},
        {"1 x 1 =", "1", 1}},

    // 5 - Mixed operations (123 → 293)
    {
        {"3 + 2 =", "5", 1},
        {"7 - 5 =", "2", 1},
        {"2 + 5 =", "7", 1}},

    // 6 - More mixed operations (131 → 339)
    {
        {"6 - 3 =", "3", 1},
        {"3 - 1 =", "2", 1},
        {"6 + 0 =", "6", 1}},

    // 7 - Slightly harder (132 → 362)
    {
        {"10 - 1 =", "9", 1},
        {"12 - 3 =", "9", 2},
        {"1 + 7 =", "8", 1}},

    // 8 - Basic operations (133 → 385)
    {
        {"4 - 0 =", "4", 1},
        {"3 x 1 =", "3", 1},
        {"4 - 3 =", "1", 1}},

    // 9 - Mixed difficulty (211 → 408)
    {
        {"4 - 2 =", "2", 1},
        {"3 + 5 =", "8", 1},
        {"10 - 5 =", "5", 1}},

    // 10 - Addition practice (212 → 431)
    {
        {"7 + 5 =", "12", 1}, // Fixed the answer
        {"9 - 5 =", "4", 1},  // Fixed the answer
        {"7 + 9 =", "16", 2}  // Fixed the answer
    },

    // 11 - More practice (213 → 454)
    {
        {"8 - 4 =", "4", 1},
        {"4 + 2 =", "6", 1},
        {"6 - 3 =", "3", 1}},

    // 12 - Simple operations (221 → 477)
    {
        {"10 - 9 =", "1", 1},
        {"2 - 2 =", "0", 1},
        {"1 + 1 =", "2", 1}},

    // 13 - Practice set (222 → 500)
    {
        {"2 x 1 =", "2", 1},
        {"12 - 10 =", "2", 1},
        {"2 - 0 =", "2", 1}},

    // 14 - Division and multiplication (223 → 523)
    {
        {"16 - 8 =", "8", 2},
        {"15 / 3 =", "5", 2},
        {"8 x 1 =", "8", 1}},

    // 15 - More division (231 → 546)
    {
        {"18 / 3 =", "6", 2},
        {"7 - 6 =", "1", 1},
        {"14 / 2 =", "7", 2}},

    // 16 - Squares and basics (232 → 569)
    {
        {"1 + 0 =", "1", 1},
        {"2 x 2 =", "4", 1},
        {"3 x 3 =", "9", 2}},

    // 17 - Mixed operations (233 → 592)
    {
        {"7 - 4 =", "3", 1},
        {"3 + 4 =", "7", 1},
        {"7 - 7 =", "0", 1}},

    // 18 - Division practice (311 → 615)
    {
        {"7 - 0 =", "7", 1},
        {"21 / 3 =", "7", 2},
        {"16 / 4 =", "4", 2}},

    // 19 - Final basic set (312 → 638)
    {
        {"10 - 5 =", "5", 1},
        {"3 + 5 =", "8", 1},
        {"8 - 3 =", "5", 1} // Fixed the answer
    },

    // 20 - Larger numbers start (313 → 661)
    {
        {"5 + 5 =", "10", 1},
        {"5 x 5 =", "25", 2},
        {"15 x 10 =", "150", 3}},

    // 21 - Quiz set 21 (321 → 684)
    {
        {"10 + 2 =", "12", 1},
        {"200 + 33 =", "233", 3},
        {"300 + 11 =", "311", 3}},

    // 22 - Quiz set 22 (322 → 707)
    {
        {"100 - 1 =", "99", 2},
        {"350 + 1 =", "351", 3},
        {"50 x 2 =", "100", 2}},

    // 23 - Quiz set 23 (323 → 730)
    {
        {"280 + 4 =", "284", 3},
        {"400 + 75 =", "475", 3},
        {"200 + 50 =", "250", 3}},

    // 24 - Quiz set 24 (331 → 753)
    {
        {"50 + 5 =", "55", 1},
        {"120 + 1 =", "121", 2},
        {"250 + 6 =", "256", 3}},

    // 25 - Quiz set 25 (332 → 776)
    {
        {"20 - 2 =", "18", 1},
        {"70 + 2 =", "72", 2},
        {"400 + 25 =", "425", 3}},

    // 26 - Quiz set 26 (333 → 799)
    {
        {"70 + 7 =", "77", 2},
        {"110 + 2 =", "112", 2},
        {"300 + 15 =", "315", 3}}};

#endif // QUIZDATA_H