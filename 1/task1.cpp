#include <iostream>
#include <fstream>

struct word_count {
    std::string word;
    int count = 0;
};

int main() {
    std::ifstream stop_words_file("resources/stop_words_english.txt");
    std::ifstream text_file("resources/text_task1.txt");
    std::ofstream output_file("resources/task1_output.txt");
    std::string word;
    char* new_word_chars;
    std::string* string_buf;
    word_count* word_count_buf;
    const int n = 25;
    int j, k, m;
    const int initial_capacity = 25;
    int stop_words_capacity = initial_capacity;
    int stop_words_len = 0;
    std::string* stop_words = new std::string[stop_words_capacity];
    int word_count_capacity = initial_capacity;
    int word_count_len = 0;
    word_count* words_counter = new word_count[word_count_capacity];
    bool is_stop_word = false;
    bool is_word_counted = false;
    int word_count_index;
    word_count word_count_tmp;
    int to_print;

    //Load stop words
    read_stop_word:
    if (!stop_words_file.eof()) {
        stop_words_file >> word;

        //Reallocate memory if needed
        if (stop_words_len == stop_words_capacity) {
            stop_words_capacity *= 2;
            string_buf = new std::string[stop_words_capacity];

            j = 0;
            stop_words_loop:
            if (j < stop_words_len) {
                string_buf[j] = stop_words[j];
                j++;
                goto stop_words_loop;
            }

            delete[] stop_words;
            stop_words = string_buf;
        }

        stop_words[stop_words_len++] = word;

        goto read_stop_word;
    }

    //Read word by word from text
    read_word:
    if (!text_file.eof()) {
        text_file >> word;

        //Going to section where word is made lowercase
        goto to_lower_case;
        after_word_processed:

        //Check if word is a stop word
        if (stop_words_len > 0) {
            j = 0;
            stop_word_check:
            if (stop_words[j] == word) {
                is_stop_word = true;
            } else {
                j++;
                if (j < stop_words_len) {
                    goto stop_word_check;
                }
            }
        }

        //If a stop word then continue
        if (is_stop_word) {
            is_stop_word = false;
            goto read_word;
        }

        //Count words
        if (word_count_len > 0) {
            j = 0;
            count_words_loop:
            if (words_counter[j].word == word) {
                is_word_counted = true;
                word_count_index = j;
            } else {
                j++;
                if (j < word_count_len) {
                    goto count_words_loop;
                }
            }
        }

        // If word has already been counted
        if (is_word_counted) {
            words_counter[word_count_index].count++;
            is_word_counted = false;
        } else { // If word is new
            //Reallocate memory if needed
            if (word_count_len == word_count_capacity) {
                word_count_capacity *= 2;
                word_count_buf = new word_count[word_count_capacity];

                j = 0;
                word_count_loop:
                if (j < word_count_len) {
                    word_count_buf[j] = words_counter[j];
                    j++;
                    goto word_count_loop;
                }

                delete[] words_counter;
                words_counter = word_count_buf;
            }

            words_counter[word_count_len++] = {word, 1};
        }

        goto read_word;
    }

    // Insertion sort to sort counted words in descending order
    j = 0;
    insertion_sort_loop:
    if (j < word_count_len) {

        k = j - 1;
        insertion_sort_nested_loop:
        if (k >= 0) {
            if (words_counter[j].count < words_counter[k].count) {
                word_count_tmp = words_counter[j];
                m = j;
                insertion_sort_array_shift_loop:
                if (m > k + 1) {
                    words_counter[m] = words_counter[m - 1];
                    m--;
                    goto insertion_sort_array_shift_loop;
                }

                words_counter[k + 1] = word_count_tmp;
                j++;
                goto insertion_sort_loop;
            }

            k--;
            goto insertion_sort_nested_loop;
        } else {
            word_count_tmp = words_counter[j];
            m = j;
            goto insertion_sort_array_shift_loop;
        }
    }

    // Printing to output file
    to_print = word_count_len < n ? word_count_len : n;

    j = 0;
    print_loop:
    if (j < to_print) {
        output_file << words_counter[j].word << " - " << words_counter[j].count << std::endl;

        j++;
        goto print_loop;
    }

    return 0;

    //Making word lowercase
    to_lower_case:
    j = 0;
    new_word_chars = new char[word.length() + 1];

    lower_case_word_loop:
    if (j < word.length()) {
        char c = word[j];

        c = (c >= 'A' && c <= 'Z') ? (char) (c + 32) : c;

        new_word_chars[j] = c;

        j++;
        goto lower_case_word_loop;
    }
    new_word_chars[j] = '\0';

    word = std::string(new_word_chars);

    delete[] new_word_chars;
    goto after_word_processed;
}
