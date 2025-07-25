//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\quizQuestions.h
#ifndef QUIZQUESTIONS_H
#define QUIZQUESTIONS_H

// We'll store three question prompts for 50 possible challenge codes.
struct ChallengeQsheet
{
    String q1;
    String q2;
    String q3;
};

static const ChallengeQsheet questionSheet[50] = {
    // 0 => quizAnswers[0] = {"1","2","3"}
    {"What is 1?", "What is 2?", "What is 3?"},

    // 1 => quizAnswers[1] = {"4","5","6"}
    {"4 x 1 =", "5 x 2 =", "2 + 2 ="},

    // 2 => quizAnswers[2] = {"7","8","9"}
    {"8 - 1 =", "8 / 2 =", "9 - 0 ="},

    // 3 => quizAnswers[3] = {"0","4","2"}
    {"1 - 1 =", "2 + 2 =", "1 + 1 ="},

    // 4 => quizAnswers[4] = {"1","1","1"}
    {"1 - 0 =", "2 - 1 =", "1 x 1 ="},

    // 5 => quizAnswers[5] = {"5","2","7"}
    {"3 + 2 =", "7 - 5 =", "2 + 5 ="},

    // 6 => quizAnswers[6] = {"3","0","6"}
    {"6 - 3 =", "3 - 3 =", "6 + 0 ="},

    // 7 => quizAnswers[7] = {"9","9","8"}
    {"10 -1 =", "12 - 3 =", "1 + 7 ="},

    // 8 => quizAnswers[8] = {"4","3","1"}
    {"4 - 0 =", "3 x 1 =", "4 - 3 ="},

    // 9 => quizAnswers[9] = {"2","8","5"}
    {"4 - 2 =", "3 + 5 =", "10 - 5 ="},

    // 10 => quizAnswers[10] = {"7","5","9"}
    {"7 + 5 =", "9 - 5 =", "7 + 9 ="},

    // 11 => quizAnswers[11] = {"4","6","3"}
    {"8 - 4 =", "4 + 2 =", "6 - 3 ="},

    // 12 => quizAnswers[12] = {"1","0","2"}
    {"10 - 9 =", "2 - 2 =", "1 + 1 ="},

    // 13 => quizAnswers[13] = {"2","2","2"}
    {"2 x 1 =", "12 -10 =", "2 - 0 ="},

    // 14 => quizAnswers[14] = {"8","5","8"}
    {"16 - 8 =", "15 / 3 =", "8 x 1 ="},

    // 15 => quizAnswers[15] = {"6","1","7"}
    {"18 / 3 =", "7 - 6 =", "14 / 2 ="},

    // 16 => quizAnswers[16] = {"1","4","9"}
    {"1 + 0 =", "2 x 2 =", "3 x 3 ="},

    // 17 => quizAnswers[17] = {"3","7","0"}
    {"7 - 4 =", "3 + 4 =", "7 - 7 ="},

    // 18 => quizAnswers[18] = {"7","7","4"}
    {"7 - 0 =", "21 / 3 =", "16 / 4 ="},

    // 19 => quizAnswers[19] = {"5","8","3"}
    {"10 - 5 =", "3 + 5 =", "8 - 3 ="},

    // 20 => quizAnswers[20] = {"10","25","150"}
    {"5 + 5 =", "5 x 5 =", "15 x 10 ="},

    // For 21..49, you can either leave them blank or fill with placeholders
    {"", "", ""}, // 21
    {"", "", ""}, // 22
    // ...
    {"", "", ""} // 49
};

#endif // QUIZQUESTIONS_H