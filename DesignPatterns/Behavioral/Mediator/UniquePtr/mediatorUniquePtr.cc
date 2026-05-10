#include <iostream>
#include <memory>
#include <string>

class BaseComponent;

// Mediator interface
class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void Notify(BaseComponent* sender, const std::string& event) const = 0;
};

// Base component
class BaseComponent {
protected:
    Mediator* mediator_ = nullptr; // non-owning

public:
    explicit BaseComponent(Mediator* mediator = nullptr)
        : mediator_(mediator) {}

    void set_mediator(Mediator* mediator) {
        mediator_ = mediator;
    }
};

// Concrete Component1
class Component1 : public BaseComponent {
public:
    using BaseComponent::BaseComponent;

    void DoA() {
        std::cout << "Component 1 does A.\n";
        mediator_->Notify(this, "A");
    }

    void DoB() {
        std::cout << "Component 1 does B.\n";
        mediator_->Notify(this, "B");
    }
};

// Concrete Component2
class Component2 : public BaseComponent {
public:
    using BaseComponent::BaseComponent;

    void DoC() {
        std::cout << "Component 2 does C.\n";
        mediator_->Notify(this, "C");
    }

    void DoD() {
        std::cout << "Component 2 does D.\n";
        mediator_->Notify(this, "D");
    }
};

// Concrete Mediator
class ConcreteMediator : public Mediator {
private:
    Component1* component1_; // non-owning
    Component2* component2_; // non-owning

public:
    ConcreteMediator(Component1* c1, Component2* c2)
        : component1_(c1), component2_(c2) {
        component1_->set_mediator(this);
        component2_->set_mediator(this);
    }

    void Notify(BaseComponent* sender, const std::string& event) const override {
        if (event == "A") {
            std::cout << "Mediator reacts on A:\n";
            component2_->DoC();
        }

        if (event == "D") {
            std::cout << "Mediator reacts on D:\n";
            component1_->DoB();
            component2_->DoC();
        }
    }
};

// Client code
void ClientCode() {
    auto c1 = std::make_unique<Component1>();
    auto c2 = std::make_unique<Component2>();

    ConcreteMediator mediator(c1.get(), c2.get());

    std::cout << "Client triggers operation A.\n";
    c1->DoA();

    std::cout << "\nClient triggers operation D.\n";
    c2->DoD();
}

int main() {
    ClientCode();
    return 0;
}

/*
// Client triggers operation A.
// Component 1 does A.
// Mediator reacts on A:
// Component 2 does C.

// Client triggers operation D.
// Component 2 does D.
// Mediator reacts on D:
// Component 1 does B.
// Component 2 does C.
*/