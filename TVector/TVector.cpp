#define CAPACITY 15
enum State { empty, busy, deleted };

template<class T>
class TVector {
    T* _data;
    int _size;
    int _capacity;
public:
    //������������
    TVector();
    TVector(int);
    TVector(int, const T*);
    TVector(const TVector&);

    //����������
    ~TVector();

    //������� � �������
    T* data();
    int size();
    int capacity();
    T& front();
    T& back();
    T* begin();
    T* end();

    //�������
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

    //���������� ����������
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
/* ��� ��������� ������� ������, ���� �������� ��������������� ����������� �������, �� �� ������ ���� ��� ���������� �� ���������! */