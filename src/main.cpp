#include <iostream>
#include "Wrapper.h"
#include "Engine.h"
#include "Document.h"

int main() {
    Document doc("hello world. abc def! hello test. qwe rty?");

    std::cout << doc.get_text() << std::endl;

    Wrapper replace_w(&doc, &Document::replace_words, { {"old_word", "test"}, {"new_word", "123"} });
    Wrapper find_w(&doc, &Document::find_phrase, { {"phrase", "hello"} });
    Wrapper cap_sent_w(&doc, &Document::capitalize_sentences, { {"delimiters", ".!?"} });
    Wrapper cap_words_w(&doc, &Document::capitalize_words, { {"target_word", ""} });
    Wrapper num_sent_w(&doc, &Document::number_sentences, { {"separator", ". "} });

    Engine engine;
    engine.register_command(&find_w, "find");
    engine.register_command(&replace_w, "replace");
    engine.register_command(&cap_sent_w, "cap_sent");
    engine.register_command(&cap_words_w, "cap_words");
    engine.register_command(&num_sent_w, "num_sent");

    std::cout << engine.execute("find") << std::endl;
    std::cout << engine.execute("find", { {"phrase", "abc def"} }) << std::endl;

    std::cout << engine.execute("replace") << std::endl;
    std::cout << engine.execute("replace", { {"old_word", "qwe"}, {"new_word", "zzz"} }) << std::endl;

    std::cout << engine.execute("cap_sent") << std::endl;
    std::cout << engine.execute("cap_words") << std::endl;

    std::cout << engine.execute("num_sent", { {"separator", ") "} }) << std::endl;

    return 0;
}