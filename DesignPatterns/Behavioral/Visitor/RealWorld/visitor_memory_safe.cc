#include <iostream>
#include <array>
#include <memory>
#include <string>

// Forward declarations
class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void VisitConcreteComponentA(const ConcreteComponentA& element) const = 0;
    virtual void VisitConcreteComponentB(const ConcreteComponentB& element) const = 0;
};

class Component {
public:
    virtual ~Component() = default;
    virtual void Accept(Visitor& visitor) const = 0;
};

class ConcreteComponentA : public Component {
public:
    void Accept(Visitor& visitor) const override {
        visitor.VisitConcreteComponentA(*this);
    }

    std::string ExclusiveMethodOfConcreteComponentA() const {
        return "A";
    }
};

class ConcreteComponentB : public Component {
public:
    void Accept(Visitor& visitor) const override {
        visitor.VisitConcreteComponentB(*this);
    }

    std::string SpecialMethodOfConcreteComponentB() const {
        return "B";
    }
};

class ConcreteVisitor1 : public Visitor {
public:
    void VisitConcreteComponentA(const ConcreteComponentA& element) const override {
        std::cout << element.ExclusiveMethodOfConcreteComponentA()
                  << " + ConcreteVisitor1\n";
    }

    void VisitConcreteComponentB(const ConcreteComponentB& element) const override {
        std::cout << element.SpecialMethodOfConcreteComponentB()
                  << " + ConcreteVisitor1\n";
    }
};

class ConcreteVisitor2 : public Visitor {
public:
    void VisitConcreteComponentA(const ConcreteComponentA& element) const override {
        std::cout << element.ExclusiveMethodOfConcreteComponentA()
                  << " + ConcreteVisitor2\n";
    }

    void VisitConcreteComponentB(const ConcreteComponentB& element) const override {
        std::cout << element.SpecialMethodOfConcreteComponentB()
                  << " + ConcreteVisitor2\n";
    }
};

void ClientCode(const std::array<std::unique_ptr<Component>, 2>& components,
                Visitor& visitor) {
    for (const auto& comp : components) {
        comp->Accept(visitor);
    }
}

int main() {
    std::array<std::unique_ptr<Component>, 2> components = {
        std::make_unique<ConcreteComponentA>(),
        std::make_unique<ConcreteComponentB>()
    };

    ConcreteVisitor1 visitor1;
    std::cout << "Visitor1:\n";
    ClientCode(components, visitor1);

    ConcreteVisitor2 visitor2;
    std::cout << "\nVisitor2:\n";
    ClientCode(components, visitor2);

    return 0;
}

/*
// Visitor1:
// A + ConcreteVisitor1
// B + ConcreteVisitor1

// Visitor2:
// A + ConcreteVisitor2
// B + ConcreteVisitor2
*/