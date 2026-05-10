#include <iostream>
#include <memory>
#include <string>

class Context;

// =====================
// Base State
// =====================
class State {
protected:
    Context* context_ = nullptr;

public:
    virtual ~State() = default;

    void set_context(Context* ctx) noexcept {
        context_ = ctx;
    }

    virtual std::string name() const = 0;

    virtual void handle1() = 0;
    virtual void handle2() = 0;
};

// =====================
// Context
// =====================
class Context {
private:
    std::unique_ptr<State> state_;

public:
    explicit Context(std::unique_ptr<State> state) {
        transition_to(std::move(state));
    }

    // Запрещаем копирование (важно!)
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    // Разрешаем перемещение
    Context(Context&&) = default;
    Context& operator=(Context&&) = default;

    void transition_to(std::unique_ptr<State> new_state) {
        std::cout << "Context: Transition to " << new_state->name() << "\n";

        state_ = std::move(new_state);
        state_->set_context(this);
    }

    void request1() {
        if (state_) state_->handle1();
    }

    void request2() {
        if (state_) state_->handle2();
    }
};

// =====================
// Concrete States
// =====================
class ConcreteStateB; // forward declaration

class ConcreteStateA : public State {
public:
    std::string name() const override {
        return "ConcreteStateA";
    }

    void handle1() override;
    void handle2() override {
        std::cout << "ConcreteStateA handles request2\n";
    }
};

class ConcreteStateB : public State {
public:
    std::string name() const override {
        return "ConcreteStateB";
    }

    void handle1() override {
        std::cout << "ConcreteStateB handles request1\n";
    }

    void handle2() override {
        std::cout << "ConcreteStateB handles request2\n";
        std::cout << "ConcreteStateB -> switching to A\n";

        context_->transition_to(std::make_unique<ConcreteStateA>());
    }
};

// Реализация после объявления B
void ConcreteStateA::handle1() {
    std::cout << "ConcreteStateA handles request1\n";
    std::cout << "ConcreteStateA -> switching to B\n";

    context_->transition_to(std::make_unique<ConcreteStateB>());
}

// =====================
// Client
// =====================
int main() {
    Context ctx(std::make_unique<ConcreteStateA>());

    ctx.request1();
    ctx.request2();

    return 0;
}

/*
// Context: Transition to ConcreteStateA
// ConcreteStateA handles request1
// ConcreteStateA -> switching to B
// Context: Transition to ConcreteStateB
// ConcreteStateB handles request2
// ConcreteStateB -> switching to A
// Context: Transition to ConcreteStateA
*/