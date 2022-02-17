#include <iostream>
#include <fstream>
#include <string>

const int initial_capacity = 25;

// Node for linked list
struct node {
    int value = -1;
    node* next = nullptr;
};

// Struct for counting specific word
struct word_page {
    std::string word;
    node* page_linked_list = nullptr;
};

int main() {
    std::ifstream text_file("resources/text_task2.txt");
    std::ofstream output_file("resources/task2_output.txt");
    std::string text_line;
    std::string word;
    char* new_word_chars;
    char* lower_case_buf;
    const int page_len = 45;
    int current_page = 0;
    int current_line = 0;
    int j, k, m, i;
    word_page* word_page_buf;
    int start_word, end_word;
    bool enable_start_word;
    int word_page_capacity = initial_capacity;
    int word_page_len = 0;
    word_page* word_page_count = new word_page[word_page_capacity];
    word_page word_page_tmp;
    bool is_detected = false;
    int detected_index;
    int compare_flag = 0;
    std::string s1;
    std::string s2;
    int process_word_index = 1;

    // Read line by line
    read_line:
    if (!text_file.eof()) {
        // Calculate current line
        current_line = current_line + 1 > page_len ? 1 : current_line + 1;
        // Calculate current page
        if (current_line == 1) {
            current_page++;
        }

        std::getline(text_file, text_line);

        j = 0;
        start_word = -1;
        enable_start_word = true;

        // Read char by char and create words from them
        line_index_loop:
        if (j < text_line.length()) {
            if (enable_start_word && ((text_line[j] >= 'A' && text_line[j] <= 'Z') ||
                                    (text_line[j] >= 'a' && text_line[j] <= 'z'))) {
                start_word = j;
                enable_start_word = false;
            }

            if (text_line[j] == ' ' && !enable_start_word) {
                end_word = j;
                new_word_chars = new char[end_word - start_word + 1];
                k = start_word;
                create_word_loop:
                if (k < end_word) {
                    new_word_chars[k - start_word] = text_line[k];
                    k++;
                    goto create_word_loop;
                }

                new_word_chars[end_word - start_word] = '\0';
                word = std::string(new_word_chars);
                process_word_index = 1;
                goto to_lower_case;
                after_word_processed_1:

                // Check if a word has already been counted
                if (word_page_len > 0) {
                    k = 0;
                    word_page_check_loop:
                    if (word_page_count[k].word == word) {
                        detected_index = k;
                        is_detected = true;
                    } else {
                        k++;
                        if (k < word_page_len) {
                            goto word_page_check_loop;
                        }
                    }
                }

                // If counted then just add a new page
                if (is_detected) {
                    node* page_linked_list_node = word_page_count[detected_index].page_linked_list;

                    last_node_loop:
                    if (page_linked_list_node->next != nullptr) {
                        page_linked_list_node = page_linked_list_node->next;
                        goto last_node_loop;
                    }

                    node* new_node = new node;
                    new_node->value = current_page;
                    page_linked_list_node->next = new_node;
                    is_detected = false;
                } else { // If not then add new word to word_page_count and create a new linked list for it
                    if (word_page_len == word_page_capacity) {
                        word_page_capacity *= 2;
                        word_page_buf = new word_page[word_page_capacity];

                        k = 0;
                        word_page_loop:
                        if (k < word_page_len) {
                            word_page_buf[k] = word_page_count[k];
                            k++;
                            goto word_page_loop;
                        }

                        delete[] word_page_count;
                        word_page_count = word_page_buf;
                    }

                    node* new_node = new node;
                    new_node->value = current_page;

                    word_page_count[word_page_len++] = {word, new_node};
                }

                delete[] new_word_chars;
                enable_start_word = true;
            }

            j++;
            goto line_index_loop;
        }

        // In the end process tha last word of each line
        if (!enable_start_word) {
            end_word = j;
            new_word_chars = new char[end_word - start_word + 1];
            k = start_word;
            create_word_loop_2:
            if (k < end_word) {
                new_word_chars[k - start_word] = text_line[k];
                k++;
                goto create_word_loop_2;
            }

            new_word_chars[end_word - start_word] = '\0';
            word = std::string(new_word_chars);
            process_word_index = 2;
            goto to_lower_case;
            after_word_processed_2:

            if (word_page_len > 0) {
                k = 0;
                word_page_check_loop_2:
                if (word_page_count[k].word == word) {
                    detected_index = k;
                    is_detected = true;
                } else {
                    k++;
                    if (k < word_page_len) {
                        goto word_page_check_loop_2;
                    }
                }
            }

            if (is_detected) {
                node* page_linked_list_node = word_page_count[detected_index].page_linked_list;

                last_node_loop_2:
                if (page_linked_list_node->next != nullptr) {
                    page_linked_list_node = page_linked_list_node->next;
                    goto last_node_loop_2;
                }

                node* new_node = new node;
                new_node->value = current_page;
                page_linked_list_node->next = new_node;
                is_detected = false;
            } else {
                if (word_page_len == word_page_capacity) {
                    word_page_capacity *= 2;
                    word_page_buf = new word_page[word_page_capacity];

                    k = 0;
                    word_page_loop_2:
                    if (k < word_page_len) {
                        word_page_buf[k] = word_page_count[k];
                        k++;
                        goto word_page_loop_2;
                    }

                    delete[] word_page_count;
                    word_page_count = word_page_buf;
                }

                node* new_node = new node;
                new_node->value = current_page;

                word_page_count[word_page_len++] = {word, new_node};
            }

            delete[] new_word_chars;
        }

        goto read_line;
    }

    // Insertion sort to sort output in alphabetical order
    j = 0;
    insertion_sort_loop:
    if (j < word_page_len) {

        k = j - 1;
        insertion_sort_nested_loop:
        if (k >= 0) {
            s1 = word_page_count[j].word;
            s2 = word_page_count[k].word;
            goto compare_strings;
            after_strings_compared:
            if (compare_flag == 1) {
                word_page_tmp = word_page_count[j];
                m = j;
                insertion_sort_array_shift_loop:
                if (m > k + 1) {
                    word_page_count[m] = word_page_count[m - 1];
                    m--;
                    goto insertion_sort_array_shift_loop;
                }

                word_page_count[k + 1] = word_page_tmp;
                j++;
                goto insertion_sort_loop;
            }

            k--;
            goto insertion_sort_nested_loop;
        } else {
            word_page_tmp = word_page_count[j];
            m = j;
            goto insertion_sort_array_shift_loop;
        }
    }

    // Print the result
    j = 0;
    print_loop:
    if (j < word_page_len) {
        k = 0;

        node* ll = word_page_count[j].page_linked_list;
        
        ll_count_loop:
        if (ll != nullptr) {
            k++;
            ll = ll->next;
            goto ll_count_loop;
        }

        ll = word_page_count[j].page_linked_list;

        if (k <= 100) {
            output_file << word_page_count[j].word << " - ";
            
            ll_print_loop:
            if (ll != nullptr) {
                output_file << ll->value;

                if (ll->next != nullptr) {
                    output_file << ", ";
                }

                ll = ll->next;
                goto ll_print_loop;
            }

            output_file << std::endl;
        }

        j++;
        goto print_loop;
    }

    return 0;

    //Making word lowercase
    to_lower_case:
    k = 0;
    lower_case_buf = new char[word.length() + 1];

    lower_case_word_loop:
    if (k < word.length()) {
        char c = word[k];

        c = (c >= 'A' && c <= 'Z') ? (char) (c + 32) : c;

        lower_case_buf[k] = c;

        k++;
        goto lower_case_word_loop;
    }
    lower_case_buf[k] = '\0';

    word = std::string(lower_case_buf);

    delete[] lower_case_buf;

    if (process_word_index == 1) {
        goto after_word_processed_1;
    } else if (process_word_index == 2) {
        goto after_word_processed_2;
    } else {
        return -1;
    }

    // Compares strings in s1 and s2 and sets compare_flag to -1, 0 or 1
    compare_strings:
    i = 0;
    compare_strings_loop:
    if (s1.length() == i && s2.length() == i) {
        compare_flag = 0;
        goto after_strings_compared;
    }

    if (s1.length() == i) {
        compare_flag = -1;
        goto after_strings_compared;
    }

    if (s2.length() == i) {
        compare_flag = 1;
        goto after_strings_compared;
    }

    if (s1[i] < s2[i]) {
        compare_flag = -1;
        goto after_strings_compared;
    } else if (s1[i] > s2[i]) {
        compare_flag = 1;
        goto after_strings_compared;
    }

    i++;
    goto compare_strings_loop;
}

