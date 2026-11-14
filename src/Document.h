#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

class Document {
public:
    explicit Document(std::string text = "");
    ~Document() noexcept = default;

    void set_text(const std::string& text);
    std::string get_text() const;

    std::string replace_words(const std::string& old_word, const std::string& new_word);
    std::string find_phrase(const std::string& phrase);
    std::string capitalize_words(const std::string& target_word);
    std::string capitalize_sentences(const std::string& delimiters);
    std::string number_sentences(const std::string& separator);

private:
    static bool is_word_char(char c) noexcept;

    std::string text_;
};

#endif 