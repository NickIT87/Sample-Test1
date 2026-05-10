#include <iostream>
#include <memory>
#include <vector>

// ================= Iterator =================
template <typename T, typename U>
class Iterator {
public:
    using iter_type = typename std::vector<T>::iterator;

    Iterator(U* data) : m_p_data_(data) {
        First();
    }

    void First() {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void Next() {
        ++m_it_;
    }

    bool IsDone() const {
        return m_it_ == m_p_data_->m_data_.end();
    }

    T& Current() {
        return *m_it_;
    }

private:
    U* m_p_data_;
    iter_type m_it_;
};

// ================= Container =================
template <typename T>
class Container {
    friend class Iterator<T, Container<T>>;

public:
    void Add(const T& value) {
        m_data_.push_back(value);
    }

    std::unique_ptr<Iterator<T, Container<T>>> CreateIterator() {
        return std::make_unique<Iterator<T, Container<T>>>(this);
    }

private:
    std::vector<T> m_data_;
};

// ================= Custom Data =================
class Data {
public:
    Data(int value = 0) : m_data_(value) {}

    int data() const {
        return m_data_;
    }

private:
    int m_data_;
};

// ================= Client =================
void ClientCode() {
    std::cout << "=== Iterator with int ===\n";

    Container<int> cont;
    for (int i = 0; i < 10; ++i) {
        cont.Add(i);
    }

    auto it = cont.CreateIterator();

    for (it->First(); !it->IsDone(); it->Next()) {
        std::cout << it->Current() << "\n";
    }

    std::cout << "\n=== Iterator with custom class ===\n";

    Container<Data> cont2;
    cont2.Add(Data(100));
    cont2.Add(Data(1000));
    cont2.Add(Data(10000));

    auto it2 = cont2.CreateIterator();

    for (it2->First(); !it2->IsDone(); it2->Next()) {
        std::cout << it2->Current().data() << "\n";
    }
}

// ================= main =================
int main() {
    ClientCode();
    return 0;
}