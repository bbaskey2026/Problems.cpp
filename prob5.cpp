#include <iostream>
using namespace std;

int countOccurrences(string str, char ch) {
    int count = 0;
    for (char c : str) {
        if (c == ch) {
            count++;
        }
    }
    return count;
}

int main() {
    string str = "iloveprogramming";
    char ch = 'g';
    cout << "Occurrences of '" << ch << "': " << countOccurrences(str, ch) << endl;
    return 0;
}
