//// filepath: c:\Users\OEM\Documents\GitHub\BS2\src\quizAnswers.h
#ifndef QUIZANSWERS_H
#define QUIZANSWERS_H

// We'll store three "random" values for each challenge code.
struct ChallengeValues
{
    String value1;
    String value2;
    String value3;
};

// 50 total entries
static const ChallengeValues quizAnswers[50] = {
    // 0..19 => Simple single-digit values (3 each)
    {"1", "2", "3"},  // 0
    {"4", "10", "4"}, // 1
    {"7", "4", "9"},  // 2
    {"0", "4", "2"},  // 3
    {"1", "1", "1"},  // 4
    {"5", "2", "7"},  // 5
    {"3", "0", "6"},  // 6
    {"9", "9", "8"},  // 7
    {"4", "3", "1"},  // 8
    {"2", "8", "5"},  // 9
    {"7", "5", "9"},  // 10
    {"4", "6", "3"},  // 11
    {"1", "0", "2"},  // 12
    {"2", "2", "2"},  // 13
    {"8", "5", "8"},  // 14
    {"6", "1", "7"},  // 15
    {"1", "4", "9"},  // 16
    {"3", "7", "0"},  // 17
    {"7", "7", "4"},  // 18
    {"5", "8", "3"},  // 19

    // 20..29 => Integer values between 10 and 500 (3 each)
    {"10", "25", "150"},   // 20
    {"12", "233", "311"},  // 21
    {"99", "351", "100"},  // 22
    {"284", "475", "250"}, // 23
    {"55", "121", "256"},  // 24
    {"18", "72", "425"},   // 25
    {"77", "112", "315"},  // 26
    {"104", "389", "300"}, // 27
    {"17", "250", "350"},  // 28
    {"499", "275", "79"},  // 29

    // 30..49 => Random objects (3 each)
    {"horse", "chair", "spoon"},   // 30
    {"kiln", "truck", "socks"},    // 31
    {"lamp", "ice", "book"},       // 32
    {"desk", "pizza", "hammer"},   // 33
    {"bird", "cookie", "phone"},   // 34
    {"robot", "bottle", "cactus"}, // 35
    {"cap", "drone", "rocket"},    // 36
    {"tea", "crane", "floor"},     // 37
    {"carrot", "shell", "mug"},    // 38
    {"paper", "wire", "kettle"},   // 39
    {"house", "sugar", "toy"},     // 40
    {"mouse", "blanket", "train"}, // 41
    {"door", "frame", "puppy"},    // 42
    {"stone", "piano", "pillow"},  // 43
    {"boat", "light", "cheese"},   // 44
    {"soap", "stool", "banana"},   // 45
    {"watch", "grass", "cloud"},   // 46
    {"apple", "cable", "candle"},  // 47
    {"radio", "visor", "headset"}, // 48
    {"chalk", "scarf", "box"}      // 49
};

#endif // QUIZANSWERS_H