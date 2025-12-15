#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>
#include <algorithm>

using namespace std;

/* ---------- Utility: Trim trailing whitespace ---------- */
void trim(string &s) {
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
}

/* ---------- Utility: Split sentence into words ---------- */
vector<string> splitWords(const string &s) {
    vector<string> words;
    string temp = "";

    for (char c : s) {
        if (c == ' ') {
            if (!temp.empty()) {
                words.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty())
        words.push_back(temp);

    return words;
}

/* ---------- Edit Distance (Levenshtein) ---------- */
int levenshteinDistance(const string &a, const string &b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({ dp[i - 1][j],
                                     dp[i][j - 1],
                                     dp[i - 1][j - 1] });
        }
    }
    return dp[n][m];
}

/* ---------- MAIN ---------- */
int main() {
    string reference =
        "Typing speed tests help improve accuracy and efficiency";

    string userInput;

    cout << "Type the following text:\n\n";
    cout << reference << "\n\n";
    cout << "Press ENTER when ready...";
    cin.get();

    time_t startTime = time(NULL);
    getline(cin, userInput);
    time_t endTime = time(NULL);

    trim(userInput);

    double timeTaken = difftime(endTime, startTime);

    vector<string> refWords = splitWords(reference);
    vector<string> userWords = splitWords(userInput);

    int minWords = min(refWords.size(), userWords.size());
    int totalErrors = 0;

    for (int i = 0; i < minWords; i++) {
        totalErrors += levenshteinDistance(userWords[i], refWords[i]);
    }

    // Extra or missing words
    totalErrors += abs((int)refWords.size() - (int)userWords.size());

    // Word count & WPM
    int wordsTyped = userWords.size();
    double wpm = (wordsTyped / timeTaken) * 60;

    // Accuracy (based on characters)
    int totalChars = reference.length();
    int correctChars = max(0, totalChars - totalErrors);
    double accuracy = ((double)correctChars / totalChars) * 100;

    cout << "\n------ Result ------\n";
    cout << "Time Taken   : " << timeTaken << " seconds\n";
    cout << "Words Typed  : " << wordsTyped << endl;
    cout << "WPM          : " << (int)wpm << endl;
    cout << "Errors       : " << totalErrors << endl;
    cout << "Accuracy     : " << accuracy << "%\n";
    cout << "--------------------\n";

    return 0;
}