// "Copyright 2025 Artem Denisov 33824B1PR2"
#include <windows.h>
#include <iostream>
#include "TVector.h"

template <class T> void info(const TVector<T>& mass) {
    std::cout << "* Size is " << mass.size() << std::endl;
    std::cout << "* Capacity is " << mass.capacity() << std::endl;
    std::cout << "* Deleted is " << mass.deleted_count() << std::endl;
    mass.service_print();
}

void set_color(int text_color, int bg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | text_color);
}

namespace TestSystem {
int count_success = 0, count_failed = 0;

void start_test(bool(*test)(), const char* name_of_test) {
    set_color(2, 0);
    std::cout << "[ RUN      ]";
    set_color(7, 0);
    std::cout << name_of_test << std::endl;

    bool status = test();

    if (status == true) {
        set_color(2, 0);
        std::cout << "[       OK ]" << "\n------------\n\n";
        count_success++;
    }
    else {
        set_color(4, 0);
        std::cout << "[  FAILED  ]" << "\n------------\n\n";
        count_failed++;
    }
    set_color(7, 0);
}

template <class T>
bool check(const T& expected, const T& actual) {
    if (expected == actual) {
        return true;
    }
    else {
        std::cerr << "Expected result is " << expected
        << ", but actual is " << actual << "." << std::endl;
        return false;
    }
}

void print_init_info() {
    set_color(2, 0);
    std::cout << "[==========] " << std::endl;
    set_color(7, 0);
}

void print_final_info() {
    set_color(2, 0);
    std::cout << "[==========] ";
    set_color(7, 0);
    std::cout << count_success + count_failed << " test"
    << (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
    set_color(2, 0);
    std::cout << "[  PASSED  ] ";
    set_color(7, 0);
    std::cout << count_success << " test"
    << (count_success > 1 ? "s" : "") << std::endl;
    if (count_failed > 0) {
        set_color(4, 0);
        std::cout << "[  FAILED  ] ";
        set_color(7, 0);
        std::cout << count_failed << " test"
        <<(count_failed > 1 ? "s." : ".") << std::endl;
    }
}
}; // namespace TestSystem

// Constructors
bool test_1_empty_constructor() {
    TVector<int> empty1, fake_empty(0);
    bool expected_result = true;
    bool actual_result = (empty1 == fake_empty);
    info(empty1);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_empty_constructor_params() {
    TVector<int> empty1;
    bool expected_result = true;
    bool actual_result;
    if (empty1.size() == 0 && empty1.capacity() == CAPACITY
    && empty1.deleted_count() == 0 &&
        empty1.data() != nullptr) {
        actual_result = true;
    } else {
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_size_constructor() {
    TVector<int> vec1(10), vec2(10);
    bool expected_result = true;
    bool actual_result = (vec1 == vec2);
    info(vec1);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_size_constructor_params() {
    TVector<int> vec1(10);
    vec1.push_front(11);
    bool expected_result = true;
    bool actual_result;
    if (vec1.size() == 11 && vec1.capacity() == 25
    && vec1.deleted_count() == 0 &&
        vec1.data() != nullptr && vec1[0] == 11) {
        actual_result = true;
    } else {
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_sizedata_constructor() {
    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TVector<int> vec1(10, arr), vec2(10, arr);
    bool expected_result = true;
    bool actual_result = (vec1 == vec2);
    info(vec1);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_sizedata_constructor_params() {
    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TVector<int> vec1(10, arr);
    bool expected_result = true;
    bool actual_result;
    if (vec1.size() == 10 && vec1.capacity() == 25
    && vec1.deleted_count() == 0) {
        actual_result = true;
        for (int i = 0; i < vec1.size(); i++) {
            if (vec1[i] != i + 1) actual_result = false;
        }
    } else {
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_init_constructor() {
    int arr[5] = { 1, 2, 3, 4, 5 };
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), vec2(5, arr);
    info(vec1);
    bool expected_result = true;
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_initsize_constructor_params() {
    int arr[2] = { 1, 2 };
    TVector<int> vec1(2, { 1, 2, 3, 4, 5 }), vec2(2, arr);
    info(vec1);
    bool expected_result = true;
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_copy_constructor() {
    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TVector<int> vec1(10, arr);
    vec1.pop_back();
    vec1.pop_front();
    TVector<int> vec2(vec1);
    bool expected_result = true;
    bool actual_result = (vec1 == vec2);
    info(vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_copy_constructor_params() {
    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TVector<int> vec1(10, arr);
    vec1.pop_back();
    vec1.pop_front();
    TVector<int> vec2(vec1);
    bool expected_result = true;
    bool actual_result;
    if (vec2.size() == 8 && vec2.capacity() == 25 &&
    vec2[1] == 3) actual_result = true;
    else actual_result = false;
    return TestSystem::check(expected_result, actual_result);
}

// Functions
bool test_1_is_empty() {
    TVector<int> empty1, fake_empty(2);
    fake_empty.pop_front();
    fake_empty.pop_front();
    bool expected_result = true;
    bool actual_result;
    if (empty1.is_empty() && fake_empty.is_empty()) actual_result = true;
    else actual_result = false;
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_at() {
    TVector<int> vec1({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    TVector<int> vec2({ 3, 4, 6, 7, 8 });
    bool expected_result = true;
    vec1.pop_front();
    vec1.pop_front();
    vec1.pop_back();
    vec1.pop_back();
    vec1.erase(2);
    info(vec1);
    for (int i = 0; i < vec1.size(); i++) {
        std::cout << "at(" << i << ") = " << vec1.at(i) << "; ";
    }
    std::cout << std::endl;
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_at_empty() {
    bool expected_result = false;
    bool actual_result;
    TVector<double> empty1;
    try {
        std::cout << empty1.at(0) << std::endl;
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_3_at_assign() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3 });
    std::cout << "Original vec1[1] = " << vec1[1] << std::endl;
    vec1[1] = -4;
    std::cout << "After assignment -4. vec1[1] = " << vec1[1] << std::endl;
    if (vec1[1] == -4) return true;
    else return false;
}

bool test_1_push_front() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 });
    TVector<int> vec2({ -5, -4, -3, -2, -1, 1, 2, 3, 4, 5 });
    info(vec1);
    vec1.push_front(-1);
    vec1.push_front(-2);
    vec1.push_front(-3);
    vec1.push_front(-4);
    vec1.push_front(-5);
    std::cout << "\n* After doing push_front() 5 times:\n";
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_push_front_empty() {
    bool expected_result = true;
    TVector<int> empty1;
    empty1.push_front(5);
    empty1.push_front(5);
    std::cout << "* After doing push_front(5) 2 times to empty:\n";
    info(empty1);
    bool actual_result = (empty1[0] == 5 && empty1[1] == 5);
    return TestSystem::check(expected_result, actual_result);
}

bool test_3_push_front_capacity_overflow() {
    bool expected_result = true;
    TVector<int> empty1;
    std::cout << "* Before doing capacity overflow:\n";
    info(empty1);
    for (int i = 0; i < 15; i++) {
        empty1.push_front(5);
    }
    std::cout << "\n* After doing capacity overflow:\n";
    info(empty1);
    bool actual_result = (empty1.capacity() == 30);
    return TestSystem::check(expected_result, actual_result);
}

bool test_4_push_front_to_deleted() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4 }), vec2({ 5, 5, 3, 4 });
    vec1.pop_front();
    vec1.pop_front();
    vec1.push_front(5);
    vec1.push_front(5);
    info(vec1);
    bool actual_result = ((vec1 == vec2));
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_push_back() {
    bool expected_result = true;
    TVector<int> vec1({ -5, -4, -3, -2, -1 });
    TVector<int> vec2({ -5, -4, -3, -2, -1, 1, 2, 3, 4, 5 });
    info(vec1);
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);
    vec1.push_back(4);
    vec1.push_back(5);
    std::cout << "\n* After doing push_back() 5 times:\n";
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_push_back_empty() {
    bool expected_result = true;
    TVector<int> empty1;
    empty1.push_back(5);
    empty1.push_back(5);
    std::cout << "* After doing push_back(5) 2 times to empty:\n";
    info(empty1);
    bool actual_result = (empty1[0] == 5 && empty1[1] == 5);
    return TestSystem::check(expected_result, actual_result);
}

bool test_3_push_back_capacity_overflow() {
    bool expected_result = true;
    TVector<int> empty1;
    std::cout << "* Before doing capacity overflow:\n";
    info(empty1);
    for (int i = 0; i < 15; i++) {
        empty1.push_back(5);
    }
    std::cout << "\n* After doing capacity overflow:\n";
    info(empty1);
    bool actual_result = (empty1.capacity() == 30);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_insert_to_invalid_index() {
    bool expected_result = false;
    bool actual_result;
    TVector<double> empty1;
    try {
        empty1.insert(2, 3);
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_insert() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4 }), vec2({ 1, 5, 2, 3, 5, 4 });
    vec1.insert(1, 5);
    vec1.insert(4, 5);
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_3_insert_to_empty() {
    bool expected_result = true;
    TVector<int> empty1;
    empty1.insert(0, 5);
    empty1.insert(0, -1);
    info(empty1);
    bool actual_result = (empty1[0] == -1 && empty1[1] == 5);
    return TestSystem::check(expected_result, actual_result);
}

bool test_4_insert_capacity_overflow() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 0 });
    std::cout << "* Before doing capacity overflow:\n";
    info(vec1);
    for (int i = 0; i < 15; i++) {
        vec1.insert(1, 5);
    }
    std::cout << "\n* After doing capacity overflow:\n";
    info(vec1);
    bool actual_result = (vec1.capacity() == 32);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_pop_front() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), vec2({ 3, 4, 5 });
    info(vec1);
    vec1.pop_front();
    vec1.pop_front();
    std::cout << "\n* After doing pop_front() 2 times:\n";
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_pop_front_to_empty() {
    bool expected_result = false;
    bool actual_result;
    TVector<int> empty1;
    try {
        empty1.pop_front();
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_pop_back() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), vec2({ 1, 2, 3 });
    info(vec1);
    vec1.pop_back();
    vec1.pop_back();
    std::cout << "\n* After doing pop_back() 2 times:\n";
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_pop_back_to_empty() {
    bool expected_result = false;
    bool actual_result;
    TVector<int> empty1;
    try {
        empty1.pop_back();
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_erase_to_invalid_index() {
    bool expected_result = false;
    bool actual_result;
    TVector<double> vec1({ 1, 2 });
    try {
        vec1.erase(3);
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_erase() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), vec2({ 1, 4, 5 });
    vec1.erase(1);
    vec1.erase(1);
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_3_erase_to_empty() {
    bool expected_result = false;
    bool actual_result;
    TVector<int> empty1;
    try {
        empty1.erase(3);
        actual_result = true;
    }
    catch (const std::exception & e) {
        std::cerr << "* An exception has been triggered! "
        << e.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_clear() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), empty1;
    std::cout << "* Before clear():\n";
    info(vec1);
    vec1.clear();
    std::cout << "\n* After clear():\n";
    info(vec1);
    bool actual_result = (vec1 == empty1);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_shrink_to_fit_ln() {
    bool expected_result = true;
    TVector<int> vec1(34);
    for (int i = 0; i < 3; i++) {
        vec1.erase(1);
    }
    vec1.erase(vec1.size() - 2);
    std::cout << "* Before shrink_to_fit():\n";
    info(vec1);
    vec1.shrink_to_fit();
    std::cout << "\n* After shrink_to_fit():\n";
    info(vec1);
    bool actual_result = (vec1.size() == vec1.capacity());
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_reserve() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 });
    vec1.reserve(30);
    info(vec1);
    bool actual_result = (vec1.size() == 5 && vec1.capacity() == 30);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_reserve_lts() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 });
    vec1.reserve(4);
    info(vec1);
    bool actual_result = (vec1.size() == 5 && vec1.capacity() == 20);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_resize_shrink() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 });
    vec1.resize(2, 0);
    info(vec1);
    bool actual_result = (vec1.size() == 2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_resize_enlarge_no_filler() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 });
    vec1.resize(8, 0);
    info(vec1);
    bool actual_result = (vec1.size() == 8);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_resize_enlarge_with_filler() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, -11 });
    vec1.resize(8, 1);
    info(vec1);
    bool actual_result = (vec1.size() == 8 && vec1[6] == -11);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_emplace() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5 }), vec2({ 2, 2, 2, 2, 2 });
    std::cout << "* Before emplace() all to 2:\n";
    info(vec1);
    for (int i = 0; i < vec1.size(); i++) vec1.emplace(i, 2);
    std::cout << "\n* After emplace() all to 2:\n";
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

bool test_1_assign_empty() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4 }), empty1;
    vec1 = empty1;
    info(vec1);
    bool actual_result = (vec1 == empty1);
    return TestSystem::check(expected_result, actual_result);
}

bool test_2_assign() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4 }), vec2({ 37, 55, -19, 0, 0, 12 });
    vec1 = vec2;
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}


// Sorting and Shuffle
bool test_1_shuffle_and_sorting() {
    bool expected_result = true;
    TVector<int> vec1({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    TVector<int> vec2({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    fisherYatesShuffle(vec1);
    info(vec1);
    hoara_sort(vec1, 0, vec1.size() - 1);
    info(vec1);
    bool actual_result = (vec1 == vec2);
    return TestSystem::check(expected_result, actual_result);
}

// Searches
bool test_1_searches() {
    bool expected_result = true;
    TVector<int> vec1({ -11, 2, 7, 2, 5, 2, 7, 2, 5, 110 });
    info(vec1);
    std::cout << "First 5 index elem = " << find_first(vec1, 5) << std::endl;
    std::cout << "Last 7 index elem = " << find_last(vec1, 7) << std::endl;
    // std::cout << "Find 2 indeces elems = " << find_all(vec1, 2) << std::endl;
    std::cout << "Failed search = " << find_first(vec1, -1) << std::endl;

    return true;
}

int main() {
    // Constructors
    //    Empty
    TestSystem::start_test(test_1_empty_constructor, "Empty_constructor.test1");
    TestSystem::start_test(test_2_empty_constructor_params,
    "Empty_constructor.test2_params");

    //    Size
    TestSystem::start_test(test_1_size_constructor, "Size_constructor.test1");
    TestSystem::start_test(test_2_size_constructor_params,
    "Size_constructor.test2_params");

    //    Size and Data
    TestSystem::start_test(test_1_sizedata_constructor,
    "SizeData_constructor.test1");
    TestSystem::start_test(test_2_sizedata_constructor_params,
    "SizeData_constructor.test2_params");

    //    Init_list
    TestSystem::start_test(test_1_init_constructor,
    "Init_constructor.test1");

    //    Size and Init_list
    TestSystem::start_test(test_1_initsize_constructor_params,
    "SizeInit_constructor.test1");

    //    Copy
    TestSystem::start_test(test_1_copy_constructor, "Copy_constructor.test1");
    TestSystem::start_test(test_2_copy_constructor_params,
    "Copy_constructor.test2_params");


    // Functions
    //    is_empty()
    TestSystem::start_test(test_1_is_empty, "is_empty().test1");

    //    at()
    TestSystem::start_test(test_1_at, "at().test1");
    TestSystem::start_test(test_2_at_empty, "at(0).test2_empty");
    TestSystem::start_test(test_3_at_assign, "at().test3_assign");

    //    push_front()
    TestSystem::start_test(test_1_push_front, "push_front().test1");
    TestSystem::start_test(test_2_push_front_empty, "push_front().test2_empty");
    TestSystem::start_test(test_3_push_front_capacity_overflow,
    "push_front().test3_overflow");
    TestSystem::start_test(test_4_push_front_to_deleted,
    "push_front().test4_deleted");

    //    push_back()
    TestSystem::start_test(test_1_push_back, "push_back().test1");
    TestSystem::start_test(test_2_push_back_empty, "push_back().test2_empty");
    TestSystem::start_test(test_3_push_front_capacity_overflow,
    "push_back().test3_overflow");

    //    insert()
    TestSystem::start_test(test_1_insert_to_invalid_index,
    "insert().test1_invalid_index");
    TestSystem::start_test(test_2_insert, "insert().test2");
    TestSystem::start_test(test_3_insert_to_empty, "insert().test3_empty");
    TestSystem::start_test(test_4_insert_capacity_overflow,
    "insert().test4_overflow");

    //    pop_front()
    TestSystem::start_test(test_1_pop_front, "pop_front().test1");
    TestSystem::start_test(test_2_pop_front_to_empty,
    "pop_front().test2_empty");

    //    pop_back()
    TestSystem::start_test(test_1_pop_back, "pop_back().test1");
    TestSystem::start_test(test_2_pop_back_to_empty, "pop_back().test2_empty");

    //    erase()
    TestSystem::start_test(test_1_erase_to_invalid_index,
    "erase().test1_invalid_index");
    TestSystem::start_test(test_2_erase, "erase().test2");
    TestSystem::start_test(test_3_erase_to_empty, "erase().test3_empty");

    //    clear()
    TestSystem::start_test(test_1_clear, "clear().test1");

    //    shrink_to_fit()
    TestSystem::start_test(test_1_shrink_to_fit_ln, "shrink_to_fit().test1");

    //    reserve()
    TestSystem::start_test(test_1_reserve, "reserve().test1");
    TestSystem::start_test(test_2_reserve_lts, "reserve().test2_lts");

    //    resize()
    TestSystem::start_test(test_1_resize_shrink, "resize().test1_shrink");
    TestSystem::start_test(test_1_resize_enlarge_no_filler,
    "resize().test1_enlarge_noFiller");
    TestSystem::start_test(test_1_resize_enlarge_with_filler,
    "resize().test1_enlarge_withFiller");

    //    emplace()
    TestSystem::start_test(test_1_emplace, "emplace().test1");

    //    assign()
    TestSystem::start_test(test_1_assign_empty, "assign().test1_empty");
    TestSystem::start_test(test_2_assign, "assign().test2");


    // Sorting and shuffle
    TestSystem::start_test(test_1_shuffle_and_sorting,
    "shuffel and sorting.test1");

    // Searches
    TestSystem::start_test(test_1_searches, "Searches.test1");

    TestSystem::print_final_info();
    system("pause");
    return 0;
}
