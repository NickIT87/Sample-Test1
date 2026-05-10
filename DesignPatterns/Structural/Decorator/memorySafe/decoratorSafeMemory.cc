#include <iostream>
#include <memory>
#include <string>

/**
 * Base Component
 */
class Component {
public:
    virtual ~Component() = default;
    virtual std::string Operation() const = 0;
};

/**
 * Concrete Component
 */
class ConcreteComponent : public Component {
public:
    std::string Operation() const override {
        return "ConcreteComponent";
    }
};

/**
 * Base Decorator
 */
class Decorator : public Component {
protected:
    std::unique_ptr<Component> component_;

public:
    explicit Decorator(std::unique_ptr<Component> component)
        : component_(std::move(component)) {}

    std::string Operation() const override {
        return component_->Operation();
    }
};

/**
 * Concrete Decorator A
 */
class ConcreteDecoratorA : public Decorator {
public:
    explicit ConcreteDecoratorA(std::unique_ptr<Component> component)
        : Decorator(std::move(component)) {}

    std::string Operation() const override {
        return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
    }
};

/**
 * Concrete Decorator B
 */
class ConcreteDecoratorB : public Decorator {
public:
    explicit ConcreteDecoratorB(std::unique_ptr<Component> component)
        : Decorator(std::move(component)) {}

    std::string Operation() const override {
        return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
    }
};

/**
 * Client code
 */
void ClientCode(const Component& component) {
    std::cout << "RESULT: " << component.Operation() << '\n';
}

int main() {
    std::cout << "Client: I've got a simple component:\n";

    auto simple = std::make_unique<ConcreteComponent>();
    ClientCode(*simple);

    std::cout << "\nClient: Now I've got a decorated component:\n";

    auto decorated =
        std::make_unique<ConcreteDecoratorB>(
            std::make_unique<ConcreteDecoratorA>(
                std::make_unique<ConcreteComponent>()
            )
        );

    ClientCode(*decorated);

    return 0;
}