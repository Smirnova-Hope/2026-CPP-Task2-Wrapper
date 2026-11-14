#include "Document.h"
#include <cctype>
#include <sstream>
#include <utility>
#include <vector>

Document::Document(std::string text) : text_(std::move(text)) {}

void Document::set_text(const std::string& text) {
    text_ = text;
}

std::string Document::get_text() const {
    return text_;
}

bool Document::is_word_char(char c) noexcept {
    return std::isalnum(static_cast<unsigned char>(c)) != 0;
}

std::string Document::replace_words(const std::string& old_word, const std::string& new_word) {
    if (old_word.empty()) {
        return text_;
    }

    std::size_t pos = 0;
    while ((pos = text_.find(old_word, pos)) != std::string::npos) {
        bool left_ok = (pos == 0) || !is_word_char(text_[pos - 1]);
        std::size_t end_pos = pos + old_word.length();
        bool right_ok = (end_pos == text_.length()) || !is_word_char(text_[end_pos]);

        if (left_ok && right_ok) {
            text_.replace(pos, old_word.length(), new_word);
            pos += new_word.length();
        }
        else {
            pos += 1;
        }
    }
    return text_;
}

std::string Document::find_phrase(const std::string& phrase) {
    if (phrase.empty()) {
        return "Empty phrase provided.";
    }

    std::vector<std::size_t> positions;
    std::size_t pos = text_.find(phrase, 0);
    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = text_.find(phrase, pos + 1);
    }

    std::ostringstream oss;
    oss << "Phrase \"" << phrase << "\" found " << positions.size() << " time(s)";
    if (!positions.empty()) {
        oss << " at position(s): ";
        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << positions[i];
        }
    }
    return oss.str();
}

std::string Document::capitalize_words(const std::string& target_word) {
    if (target_word.empty()) {
        bool in_word = false;
        for (char& c : text_) {
            if (is_word_char(c)) {
                if (!in_word) {
                    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                    in_word = true;
                }
            }
            else {
                in_word = false;
            }
        }
    }
    else {
        std::size_t pos = 0;
        while ((pos = text_.find(target_word, pos)) != std::string::npos) {
            bool left_ok = (pos == 0) || !is_word_char(text_[pos - 1]);
            std::size_t end_pos = pos + target_word.length();
            bool right_ok = (end_pos == text_.length()) || !is_word_char(text_[end_pos]);

            if (left_ok && right_ok) {
                text_[pos] = static_cast<char>(std::toupper(static_cast<unsigned char>(text_[pos])));
            }
            pos += target_word.length();
        }
    }
    return text_;
}

std::string Document::capitalize_sentences(const std::string& delimiters) {
    std::string delims = delimiters.empty() ? ".!?" : delimiters;
    bool new_sentence = true;

    for (char& c : text_) {
        if (new_sentence && std::isalpha(static_cast<unsigned char>(c))) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            new_sentence = false;
        }

        if (delims.find(c) != std::string::npos) {
            new_sentence = true;
        }
    }
    return text_;
}

std::string Document::number_sentences(const std::string& separator) {
    std::string sep = separator.empty() ? ". " : separator;
    std::vector<std::string> sentences;
    std::string current;

    for (std::size_t i = 0; i < text_.length(); ++i) {
        char c = text_[i];
        if (c == '\n' || c == '\r') {
            if (!current.empty() && current.back() != ' ') {
                current += ' ';
            }
            continue;
        }

        current += c;
        if (c == '.' || c == '!' || c == '?') {
            while (i + 1 < text_.length() &&
                (text_[i + 1] == '.' || text_[i + 1] == '!' || text_[i + 1] == '?')) {
                current += text_[++i];
            }

            std::size_t start = current.find_first_not_of(" \t");
            if (start != std::string::npos) {
                sentences.push_back(current.substr(start));
            }
            current.clear();
        }
    }

    std::size_t start = current.find_first_not_of(" \t");
    if (start != std::string::npos) {
        sentences.push_back(current.substr(start));
    }

    std::ostringstream oss;
    for (std::size_t i = 0; i < sentences.size(); ++i) {
        if (i > 0) {
            oss << "\n";
        }
        oss << (i + 1) << sep << sentences[i];
    }

    text_ = oss.str();
    return text_;
}