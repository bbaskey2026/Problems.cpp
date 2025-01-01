#include <iostream>
using namespace std;

bool isPalindrome(int num) {
    int original = num, reversed = 0, remainder;
    while (num != 0) {
        remainder = num % 10;
        reversed = reversed * 10 + remainder;
        num /= 10;
    }
    return original == reversed;
}

int largestPalindrome(int n) {
    for (int i = n - 1; i >= 0; i--) {
        if (isPalindrome(i)) {
            return i;
        }
    }
    return -1;
}

int main() {
    int n = 100;
    cout << "Largest palindrome less than " << n << " is: " << largestPalindrome(n) << endl;
    return 0;
}
