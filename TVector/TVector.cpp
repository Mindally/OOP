#define CAPACITY 15
enum State { empty, busy, deleted };

template<class T>
class TVector {
    T* _data;
    int _size;
    int _capacity;
public:
    //Конструкторы
    TVector();
    TVector(int);
    TVector(int, const T*);
    TVector(const TVector&);

    //Деструктор
    ~TVector();

    //Геттеры и сеттеры
    T* data();
    int size();
    int capacity();
    T& front();
    T& back();
    T* begin();
    T* end();

    //Функции
    bool is_empty();
    void push_front();
    void push_back();
    void insert(int);
    void pop_front();
    void pop_back();
    void erase(int);
    void emplace(int, const T&);
    void assign(int, const T&);
    T& at(int);
    void clear();
    void shrink_to_fit();
    void reserve(int);
    void resize(int);

    //Перегрузка операторов
    TVector& operator=(const TVector&);
    bool operator!=(const TVector&) const;
    bool operator==(const TVector&) const;
    T& operator[](int);
private:
    size_t deleted;
    State* _states;
    bool is_full();
};

//TVector<T>::TVector() { /* ... */ }
// my_vector.cpp
/* для шаблонных классов пустой, либо содержит вспомогательные нешаблонные функции, но он обязан быть или библиотека не соберется! */