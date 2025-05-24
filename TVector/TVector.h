// "Copyright 2025 Artem Denisov 33824B1PR2"
#include <iostream>
#include <ctime>
#include <initializer_list>
#pragma once
#define CAPACITY 15
#define DELETED_LIMIT 0.15

enum State { empty, busy, deleted };

template<class T> class TVector {
    T* _data = nullptr;
    int _size = 0;
    int _capacity = CAPACITY;

public:
    // Constructors
    TVector();
    TVector(int);
    TVector(int, const T*);
    TVector(std::initializer_list<T>);
    TVector(int, std::initializer_list<T>);
    TVector(const TVector<T>&);

    // Destructor
    ~TVector();

    // Getters
    T* data() const noexcept;
    int size() const noexcept;
    int capacity() const noexcept;
    size_t deleted_count() const noexcept;
    T& front() const;
    T& back() const;
    T* begin() const;
    T* end() const;

    // Functions
    bool is_empty() const noexcept;
    bool is_full() const noexcept;
    T& at(int) const;
    void print() const noexcept;
    void service_print() const;
    void emplace(int, const T&);
    void assign(const TVector<T>&);

    // Insertion functions
    void push_front(const T&);
    void push_back(const T&);
    void insert(int, const T&);

    // Deletion functions
    void pop_front();
    void pop_back();
    void erase(int);

    // Memory management functions
    void clear() noexcept;
    void shrink_to_fit();
    void reserve(int);
    void resize(int, bool);

    // Operators overload
    void operator=(const TVector<T>&);
    bool operator==(const TVector<T>&) const;
    bool operator!=(const TVector<T>&) const;
    T& operator[](int) const;

private:
    size_t _deleted = 0;
    State* _states = nullptr;
    void effective_deletion();
    T& reverse_at(int) const;
};

// Realization

// Constructors
template<class T> TVector<T>::TVector() {
    _data = new T[_capacity];
    _states = new State[_capacity];
    for (int i = 0; i < _capacity; i++) _states[i] = empty;
}

template<class T> TVector<T>::TVector(int size) {
    if (_size < 0) throw std::logic_error("Size can't be negative!");
    _size = size;
    if (_size == 0) {
        _capacity = 0;
        _data = nullptr;
        _states = nullptr;
    }
    else {
        _capacity = size + CAPACITY;
        _data = new T[_capacity];
        _states = new State[_capacity];
        for (int i = 0; i < _size; i++) _states[i] = busy;
        for (int i = _size; i < _capacity; i++) _states[i] = empty;
    }
}

template<class T> TVector<T>::TVector(int size, const T* data) {
    if (_size < 0) throw std::invalid_argument("Size can't be negative!");
    if (data == nullptr && size > 0) throw std::invalid_argument("Data is nullptr!");
    _size = size;
    _capacity = _size + CAPACITY;
    _data = new T[_capacity];
    _states = new State[_capacity];
    for (int i = 0; i < _size; i++) {
        _data[i] = data[i];
        _states[i] = busy;
    }
    for (int i = _size; i < _capacity; i++) {
        _states[i] = empty;
    }
}

template<class T> TVector<T>::TVector(std::initializer_list<T> init) {
    if (init.size() > 0) {
        _size = init.size();
        _capacity = _size + CAPACITY;
        _data = new T[_capacity];
        _states = new State[_capacity];
        const T* src = init.begin();
        for (int i = 0; i < _size; i++) {
            _data[i] = src[i];
            _states[i] = busy;
        }
        for (int i = _size; i < _capacity; i++) {
            _states[i] = empty;
        }
    }
}

template<class T> TVector<T>::TVector(int size, std::initializer_list<T> init) {
    if (size < 0) throw std::invalid_argument("Size can't be negative!");
    if (size > 0) {
        _size = size;
        _capacity = _size + CAPACITY;
        _data = new T[_capacity];
        _states = new State[_capacity];
        const T* src = init.begin();
        for (int i = 0; i < _size; i++) {
            _data[i] = src[i];
            _states[i] = busy;
        }
        for (int i = _size; i < _capacity; i++) {
            _states[i] = empty;
        }
    }
}

template<class T> TVector<T>::TVector(const TVector<T>& other) {
    _size = other._size;
    _capacity = other._capacity;
    _data = new T[_capacity];
    _deleted = other._deleted;
    _states = new State[_capacity];
    for (int i = 0; i < _capacity; i++) {
        _data[i] = other._data[i];
        _states[i] = other._states[i];
    }
}


// Destructor
template<class T> TVector<T>::~TVector() {
    delete[] _data;
    delete[] _states;
}

// Getters
template<class T> T* TVector<T>::data() const noexcept {
    return _data;
}

template<class T> int TVector<T>::size() const noexcept {
    return _size - _deleted;
}

template<class T> int TVector<T>::capacity() const noexcept {
    return _capacity;
}

template<class T> size_t TVector<T>::deleted_count() const noexcept {
    return _deleted;
}

template<class T> T& TVector<T>::front() const {
    return at(0);
}

template<class T> T& TVector<T>::back() const {
    return reverse_at(size() - 1);
}

template<class T> T* TVector<T>::begin() const {
    if (_data == nullptr) throw std::logic_error("Data does not exist!");
    return _data;
}

template<class T> T* TVector<T>::end() const {
    if (_data == nullptr) throw std::logic_error("Data does not exist!");
    return _data + _size;
}

// Functions
template<class T> bool TVector<T>::is_empty() const noexcept {
    if (_size <= 0 || size() <= 0) return true;
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy || _states[i] == deleted) return false;
    }
    return true;
}

template<class T> bool TVector<T>::is_full() const noexcept {
    return _size >= _capacity;
}

template<class T> T& TVector<T>::at(int index) const {
    if (index > size() || index < 0) throw std::out_of_range("Index out of range");
    if (size() <= 0) throw std::logic_error("The size is 0!");
    int real_index = -1, cnt;
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy) real_index++;
        if (real_index == index) {
            cnt = i;
            break;
        }
    }
    return _data[cnt];
}

template<class T> void TVector<T>::print() const noexcept {
    std::cout << "{ ";
    for (int i = 0; i < _size - 1; i++) {
        if (_states[i] == busy) std::cout << _data[i] << ", ";
    }
    std::cout << _data[_size - 1] << "}" << std::endl;
}

template<class T> void TVector<T>::service_print() const {
    std::cout << "{ ";
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy) std::cout << "B_" << _data[i] << " ";
        if (_states[i] == deleted) std::cout << "D_" << _data[i] << " ";
        if (_states[i] == empty) std::cout << "E_" << _data[i] << " ";
    }
    std::cout << "}" << std::endl;
}

template<class T> void TVector<T>::emplace(int index, const T& value) {
    if (index >= size() || index < 0) throw std::out_of_range("Index out of range");
    if (size() == 0) throw std::logic_error("Placement of the value is impossible! The size is 0!");
    at(index) = value;
}

template<class T> void TVector<T>::assign(const TVector<T>& other) {
    _size = other._size;
    _capacity = other._capacity;
    _data = new T[_capacity];
    _deleted = other._deleted;
    _states = new State[_capacity];
    for (int i = 0; i < _capacity; i++) {
        _data[i] = other._data[i];
        _states[i] = other._states[i];
    }
}

// Insertion functions
template<class T> void TVector<T>::push_front(const T& value) {
    if (_size == 0) _size++;
    if (_states[0] == busy) {
        _size++;
        if (is_full()) reserve(_size + CAPACITY);
        for (int i = _size - 1; i > 0; i--) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }
    }
    if (_states[0] == deleted) _deleted--;
    _data[0] = value;
    _states[0] = busy;
}

template<class T> void TVector<T>::push_back(const T& value) {
    if (_size == 0) _size++;
    if (_states[_size - 1] == busy) {
        _size++;
        if (is_full()) reserve(_size + CAPACITY);
    }
    if (_states[_size - 1] == deleted) _deleted--;
    _data[_size - 1] = value;
    _states[_size - 1] = busy;
}

template<class T> void TVector<T>::insert(int index, const T& value) {
    if (_size == 0) _size++;
    if (index >= size() || index < 0) throw std::out_of_range("Index out of range");
    if (_states[index] == busy) {
        _size++;
        if (is_full()) reserve(_size + CAPACITY);
        for (int i = _size; i > index; i--) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }
    }
    _data[index] = value;
    _states[index] = busy;
}

// Deletion functions
template<class T> void TVector<T>::pop_front() {
    if (size() == 0) throw std::logic_error("Deletion is not possible! The size is 0!");

    int index = 0;
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy) {
            index = i;
            break;
        }
    }
    if (index == _size - 1) pop_back();
    else {
        _states[index] = deleted;
        _deleted++;
    }
    if (_deleted >= (int)(_size * DELETED_LIMIT)) effective_deletion();
}

template<class T> void TVector<T>::pop_back() {
    if (size() == 0) throw std::logic_error("Deletion is not possible! The size is 0!");

    int index = 0;
    for (int i = _size - 1; i >= 0; i--) {
        if (_states[i] == busy) {
            index = i;
            break;
        }
    }
    _states[index] = empty;
    _size--;
}

template<class T> void TVector<T>::erase(int index) {
    if (index >= size() || index < 0) throw std::out_of_range("Index out of range");
    if (size() == 0) throw std::logic_error("Deletion is not possible! The size is 0!");

    if (index == _size - 1) {
        pop_back();
        return;
    }

    int real_index = 0, cnt = -1;
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy) {
            cnt++;
            if (cnt == index) {
                real_index = i;
                break;
            }
        }
    }
    _states[real_index] = deleted;
    _deleted++;
    if (_deleted >= (int)(_size * DELETED_LIMIT)) effective_deletion();
}

// Memory management functions
template<class T> void TVector<T>::clear() noexcept {
    delete[] _data;
    delete[] _states;
    _size = 0;
    _capacity = CAPACITY;
    _deleted = 0;
    _data = new T[_capacity];
    _states = new State[_capacity];
    for (int i = 0; i < _capacity; i++) _states[i] = empty;
}

template<class T> void TVector<T>::shrink_to_fit() {
    effective_deletion();
    if (_size < _capacity) {
        T* new_data = new T[_size];
        State* new_states = new State[_size];
        for (int i = 0; i < _size; i++) {
            new_data[i] = _data[i];
            new_states[i] = _states[i];
        }
        delete[] _data;
        delete[] _states;
        _data = new_data;
        _states = new_states;
        _capacity = _size;
    }
}

template<class T> void TVector<T>::reserve(int new_capacity) {
    effective_deletion();
    if (new_capacity > _capacity) {
        T* new_data = new T[new_capacity];
        State* new_states = new State[new_capacity];
        for (int i = 0; i < _capacity; i++) {
            new_data[i] = _data[i];
            new_states[i] = _states[i];
        }
        for (int i = _capacity; i < new_capacity; i++) new_states[i] = empty;
        delete[] _data;
        delete[] _states;
        _data = new_data;
        _states = new_states;
        _capacity = new_capacity;
    }
}

template<class T> void TVector<T>::resize(int new_size, bool toFill) {
    if (new_size < 0) throw std::invalid_argument("The size cannot be negative!");

    effective_deletion();
    if (new_size < _size) {
        int cnt = 0;
        for (int i = new_size; i < _size; i++) _states[i] = empty;
        for (int i = 0; i < new_size; i++) {
            if (_states[i] == busy) cnt++;
        }
        _size = cnt;
    }
    else {
        if (new_size >= _capacity) reserve(new_size + CAPACITY);
        if (toFill == true) {
            for (int i = _size; i < new_size; i++) {
                _states[i] = busy;
                _data[i] = _data[_size - 1];
            }
        }
        else {
            for (int i = _size; i < new_size; i++) _states[i] = busy;
        }
        _size = new_size;
    }
}


// Operators overload
template <class T> void TVector<T>::operator=(const TVector<T>& other) {
    assign(other);
}

template <class T> bool TVector<T>::operator==(const TVector<T>& other) const {
    if (size() != other.size()) return false;
    if (size() == 0) return true;
    if (is_empty() && other.is_empty()) return true;
    for (int i = 0; i < size(); i++) {
        if (at(i) != other.at(i)) return false;
    }
    return true;
}

template <class T> bool TVector<T>::operator!=(const TVector<T>& other) const {
    return !(*this == other);
}

template <class T> T& TVector<T>::operator[](int index) const {
    return at(index);
}

// Private functions
template<class T> void TVector<T>::effective_deletion() {
    int new_size = size();
    int index = 0;
    T* new_data = new T[_capacity];
    State* new_states = new State[_capacity];
    for (int i = 0; i < _size; i++) {
        if (_states[i] == busy) {
            new_data[index] = _data[i];
            new_states[index] = busy;
            index++;
        }
    }
    for (int i = new_size; i < _capacity; i++) new_states[i] = empty;
    delete[] _data;
    delete[] _states;
    _data = new_data;
    _states = new_states;
    _size = new_size;
    _deleted = 0;
}

template<class T> T& TVector<T>::reverse_at(int index) const {
    if (index > size() || index < 0) throw std::out_of_range("Index out of range");
    if (size() <= 0) throw std::logic_error("The size is 0!");
    int real_index = -1, cnt;
    for (int i = _size - 1; i >= 0; i--) {
        if (_states[i] == busy) real_index++;
        if (real_index == index) {
            cnt = i;
            break;
        }
    }
    return _data[cnt];
}

// Friend functions

// Search functions
template <class T> int find_first(const TVector<T>& vector, const T& value) {
    if (vector.size() == 0) throw std::logic_error("The size is 0!");
    for (int i = 0; i < vector.size(); i++) {
        if (vector.at(i) == value) return i;
    }
    return INT_MIN;
}

template <class T> int find_last(const TVector<T>& vector, const T& value) {
    if (vector.size() == 0) throw std::logic_error("The size is 0!");
    for (int i = vector.size() - 1; i >= 0; i--) {
        if (vector.at(i) == value) return i;
    }
    return INT_MAX;
}

template <class T> int find_all(const TVector<T>& vector, const T& value) {
    if (vector.size() == 0) throw std::logic_error("The size is 0!");
    int* result = nullptr, size_res = 0;
    for (int i = 0; i < vector.size(); i++) {
        if (vector.at(i) == value) size_res++;
    }

    if (size_res == 0) {
        result = new int[1];
        result[0] = INT_MIN;
    }

    else {
        result = new int[size_res];
        for (int i = 0, j = 0; j < size_res; i++) {
            if (vector.at(i) == value) {
                result[j] = i;
                j++;
            }
        }
    }
    return result;
}

// Sorting and shuffling
template <class T> void hoara_sort(TVector<T>& mass, int start, int end) {
    if (start >= end) return;
    int l = start, r = end;
    T base_elem = mass[(l + r) / 2];
    while (l <= r) {
        while (mass[l] < base_elem) l++;
        while (mass[r] > base_elem) r--;
        if (l <= r) {
            T temp = mass[l];
            mass[l] = mass[r];
            mass[r] = temp;
            l++;
            r--;
        }
    }
    hoara_sort(mass, start, r);
    hoara_sort(mass, l, end);
}

template <class T> void fisherYatesShuffle(TVector<T>& mass) {
    srand(time(nullptr));
    for (int i = mass.size() - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        T temp = mass[i];
        mass[i] = mass[j];
        mass[j] = temp;
    }
}
