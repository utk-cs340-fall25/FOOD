//HelloWorld+.cpp

#include <iostream>
#include <map>
#include <string>
#include <cctype>

using namespace std;

// Helper: build frequency map of letters in a string
map<char, int> build_freq(const string &s) {
    map<char, int> freq;
    for (char c : s) {
        if (isalpha(c)) {
            freq[toupper(c)]++;
        }
    }
    return freq;
}

int main() {
    string target = "HELLO WORLD";
    map<char, int> target_freq = build_freq(target);

    cout << "Enter a string to check if it is an anagram of 'Hello World':\n";
    string input;
    getline(cin, input);

    map<char, int> input_freq = build_freq(input);

    // Check if input matches
    bool is_anagram = (input_freq == target_freq);

    if (is_anagram) {
        cout << "That’s an anagram of 'Hello World'." << endl;
    } else {
        cout << "Not an anagram of 'Hello World'." << endl;

        // Check missing letters
        cout << "Missing letters: ";
        bool none_missing = true;
        for (auto &[ch, count] : target_freq) {
            if (input_freq[ch] < count) {
                cout << ch << "(" << (count - input_freq[ch]) << ") ";
                none_missing = false;
            }
        }
        if (none_missing) cout << "None";
        cout << endl;

        // Check extra letters
        cout << "Extra letters: ";
        bool none_extra = true;
        for (auto &[ch, count] : input_freq) {
            if (target_freq[ch] < count) {
                cout << ch << "(" << (count - target_freq[ch]) << ") ";
                none_extra = false;
            }
        }
        if (none_extra) cout << "None";
        cout << endl;
    }

    return 0;
}
