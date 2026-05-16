#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    explicit spell_checker(const std::string& word_list_path)
    {
        std::ifstream file(word_list_path);
        std::string word;
        while (std::getline(file, word)) {
            if (!word.empty()) {
                words.insert(word);
            }
        }
    }
    [[nodiscard]] bool is_correct(const std::string& word) const
    {
        std::string lower = word;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });

        std::erase_if(lower, [](const unsigned char c) { return !std::isalpha(c); });

        if (lower.empty()) {
            return true;
        }
        return words.count(lower) > 0;
    }

    [[nodiscard]] std::vector<std::string> suggestions(const std::string& word, int max = 5) const
    {
        std::string lower = word;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
        std::erase_if(lower, [](const unsigned char c) { return !std::isalpha(c); });

        std::vector<std::string> result;

        // Prefix matches first
        for (const auto& w : words) {
            if (static_cast<int>(result.size()) >= max) {
                break;
            }
            if (w.size() >= 2 && lower.size() >= 2 && w.substr(0, 2) == lower.substr(0, 2)) {
                result.push_back(w);
            }
        }

        // Fill remaining with edit-distance-1 candidates
        if (static_cast<int>(result.size()) < max) {
            for (const auto& w : words) {
                if (static_cast<int>(result.size()) >= max) {
                    break;
                }
                if (edit_distance(lower, w) == 1) {
                    if (std::find(result.begin(), result.end(), w) == result.end()) {
                        result.push_back(w);
                    }
                }
            }
        }

        return result;
    }

    [[nodiscard]] bool loaded() const { return !words.empty(); }

private:
    std::set<std::string> words;

    [[nodiscard]] static int edit_distance(const std::string& a, const std::string& b)
    {
        const std::size_t m = a.size();
        const std::size_t n = b.size();
        if (m > 3 || n > 3) {
            // Only check short words for performance
            if (std::abs(static_cast<int>(m) - static_cast<int>(n)) > 1) {
                return 99;
            }
        }
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        for (std::size_t i = 0; i <= m; ++i) {
            dp[i][0] = static_cast<int>(i);
        }
        for (std::size_t j = 0; j <= n; ++j) {
            dp[0][j] = static_cast<int>(j);
        }
        for (std::size_t i = 1; i <= m; ++i) {
            for (std::size_t j = 1; j <= n; ++j) {
                if (a[i - 1] == b[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
                }
            }
        }

    }
};

#endif // SPELL_CHECKER_H