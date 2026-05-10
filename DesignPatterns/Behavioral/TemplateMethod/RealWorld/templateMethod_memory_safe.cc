// template_method_concepts_fixed.cpp
#include <iostream>
#include <concepts>

// 🔷 Concept (используем только в функциях, не в CRTP)
template<typename T>
concept TemplateOps = requires(const T& t) {
    { t.RequiredOperation1() } -> std::same_as<void>;
    { t.RequiredOperation2() } -> std::same_as<void>;
    { t.Hook1() } -> std::same_as<void>;
    { t.Hook2() } -> std::same_as<void>;
};

// 🔷 CRTP base БЕЗ concept
template<typename T>
class TemplateMethodBase {
public:
    void run() const {
        BaseOperation1();
        impl().RequiredOperation1();
        BaseOperation2();
        impl().Hook1();
        impl().RequiredOperation2();
        BaseOperation3();
        impl().Hook2();
    }

private:
    const T& impl() const {
        return static_cast<const T&>(*this);
    }

    static void BaseOperation1() {
        std::cout << "Base: doing bulk work\n";
    }

    static void BaseOperation2() {
        std::cout << "Base: letting subclass override\n";
    }

    static void BaseOperation3() {
        std::cout << "Base: finishing work\n";
    }
};

// 🔷 Concrete #1
class Concrete1 : public TemplateMethodBase<Concrete1> {
public:
    void RequiredOperation1() const {
        std::cout << "Concrete1: Op1\n";
    }

    void RequiredOperation2() const {
        std::cout << "Concrete1: Op2\n";
    }

    void Hook1() const {}
    void Hook2() const {}
};

// 🔷 Concrete #2
class Concrete2 : public TemplateMethodBase<Concrete2> {
public:
    void RequiredOperation1() const {
        std::cout << "Concrete2: Op1\n";
    }

    void RequiredOperation2() const {
        std::cout << "Concrete2: Op2\n";
    }

    void Hook1() const {
        std::cout << "Concrete2: Hook1 override\n";
    }

    void Hook2() const {}
};

// 🔷 Functional version (вот тут используем concept)
template<TemplateOps T>
void runTemplate(const T& obj) {
    std::cout << "Func: Base1\n";
    obj.RequiredOperation1();
    std::cout << "Func: Base2\n";
    obj.Hook1();
    obj.RequiredOperation2();
    std::cout << "Func: Base3\n";
    obj.Hook2();
}

int main() {
    std::cout << "=== CRTP version ===\n";
    Concrete1 c1;
    Concrete2 c2;

    c1.run();   // ✅ теперь работает
    std::cout << "----\n";
    c2.run();

    std::cout << "\n=== Functional version ===\n";
    runTemplate(c1);
    std::cout << "----\n";
    runTemplate(c2);

    return 0;
}

//clang++ templateMethod_memory_safe.cc -std=c++20 -Wall -Wextra -O2 -o app

/*
// === CRTP version ===
// Base: doing bulk work
// Concrete1: Op1
// Base: letting subclass override
// Concrete1: Op2
// Base: finishing work
// ----
// Base: doing bulk work
// Concrete2: Op1
// Base: letting subclass override
// Concrete2: Hook1 override
// Concrete2: Op2
// Base: finishing work

// === Functional version ===
// Func: Base1
// Concrete1: Op1
// Func: Base2
// Concrete1: Op2
// Func: Base3
// ----
// Func: Base1
// Concrete2: Op1
// Func: Base2
// Concrete2: Hook1 override
// Concrete2: Op2
// Func: Base3
*/