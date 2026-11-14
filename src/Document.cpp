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

bool Document::is_word_char(char symbol) noexcept {
    return std::isalnum(static_cast<unsigned char>(symbol)) != 0;
}

bool Document::is_sentence_end(char symbol) noexcept {
    return symbol == '.' || symbol == '!' || symbol == '?';
}

bool Document::is_whole_word_at(std::size_t position, std::size_t length) const noexcept {
    bool left_ok = (position == 0) || !is_word_char(text_[position - 1]);
    std::size_t end_pos = position + length;
    bool right_ok = (end_pos >= text_.length()) || !is_word_char(text_[end_pos]);
    return left_ok && right_ok;
}

std::string Document::replace_words(const std::string& old_word, const std::string& new_word) {
    if (old_word.empty()) {
        return text_;
    }

    std::size_t position = 0;
    while ((position = text_.find(old_word, position)) != std::string::npos) {
        if (is_whole_word_at(position, old_word.length())) {
            text_.replace(position, old_word.length(), new_word);
            position += new_word.length();
        }
        else {
            position += 1;
        }
    }
    return text_;
}

std::string Document::find_phrase(const std::string& phrase) const {
    if (phrase.empty()) {
        return "Not found";
    }
    if (text_.find(phrase) != std::string::npos) {
        return "Found";
    }
    return "Not found";
}

void Document::capitalize_all_words() {
    bool in_word = false;
    for (char& symbol : text_) {
        if (is_word_char(symbol)) {
            if (!in_word) {
                symbol = static_cast<char>(std::toupper(static_cast<unsigned char>(symbol)));
                in_word = true;
            }
        }
        else {
            in_word = false;
        }
    }
}

std::string Document::capitalize_words(const std::string& target_word) {
    if (target_word.empty()) {
        capitalize_all_words();
        return text_;
    }

    std::size_t position = 0;
    while ((position = text_.find(target_word, position)) != std::string::npos) {
        if (is_whole_word_at(position, target_word.length())) {
            auto upper_char = std::toupper(static_cast<unsigned char>(text_[position]));
            text_[position] = static_cast<char>(upper_char);
        }
        position += target_word.length();
    }
    return text_;
}

std::string Document::capitalize_sentences(const std::string& delimiters) {
    std::string delims = delimiters.empty() ? ".!?" : delimiters;
    bool new_sentence = true;

    for (char& symbol : text_) {
        if (new_sentence && std::isalpha(static_cast<unsigned char>(symbol)) != 0) {
            symbol = static_cast<char>(std::toupper(static_cast<unsigned char>(symbol)));
            new_sentence = false;
        }
        if (delims.find(symbol) != std::string::npos) {
            new_sentence = true;
        }
    }
    return text_;
}

std::vector<std::string> Document::split_sentences(const std::string& raw_text) {
    std::vector<std::string> sentences;
    std::string current;

    for (std::size_t i = 0; i < raw_text.length(); ++i) {
        char symbol = raw_text[i];
        if (symbol == '\n' || symbol == '\r') {
            if (!current.empty() && current.back() != ' ') {
                current += ' ';
            }
            continue;
        }

        current += symbol;
        if (is_sentence_end(symbol)) {
            while (i + 1 < raw_text.length() && is_sentence_end(raw_text[i + 1])) {
                current += raw_text[++i];
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
    return sentences;
}

std::string Document::number_sentences(const std::string& separator) {
    std::string prefix_sep = separator.empty() ? ". " : separator;
    std::vector<std::string> sentences = split_sentences(text_);

    std::ostringstream stream;
    std::size_t index = 1;
    for (const auto& sentence : sentences) {
        if (index > 1) {
            stream << '\n';
        }
        stream << index << prefix_sep << sentence;
        ++index;
    }

    text_ = stream.str();
    return text_;
}