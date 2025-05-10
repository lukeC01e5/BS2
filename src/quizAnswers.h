//// filepath: c:\Users\OEM\Documents\GitHub\gameUnit\src\quizAnswers.h
#ifndef QUIZANSWERS_H
#define QUIZANSWERS_H

// We'll store four "random" values for each challenge code.
struct ChallengeValues
{
    String value1;
    String value2;
    String value3;
    String value4;
};

// Ten entries, indexed 0..9
static const ChallengeValues quizAnswers[10] = {
    {"42", "cats", "7", "peanut"},     // Code = 0
    {"5", "blue", "apple", "lion"},    // Code = 1
    {"67", "dog", "tree", "orange"},   // Code = 2
    {"100", "red", "Hawk", "zebra"},   // Code = 3
    {"999", "biscuit", "88", "cup"},   // Code = 4
    {"123", "fish", "delta", "bird"},  // Code = 5
    {"13", "car", "200", "boat"},      // Code = 6
    {"777", "xyz", "500", "table"},    // Code = 7
    {"21", "toast", "horse", "chair"}, // Code = 8
    {"0", "robot", "shrimp", "milk"}   // Code = 9
};

#endif // QUIZANSWERS_H