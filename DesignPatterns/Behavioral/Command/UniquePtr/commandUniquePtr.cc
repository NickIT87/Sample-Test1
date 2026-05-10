#include <iostream>
#include <memory>
#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() const = 0;
};

class SimpleCommand : public Command {
private:
    std::string payload_;
public:
    explicit SimpleCommand(std::string payload) : payload_(std::move(payload)) {}
    void Execute() const override {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" 
                  << payload_ << ")\n";
    }
};

class Receiver {
public:
    void DoSomething(const std::string &a) {
        std::cout << "Receiver: Working on (" << a << ".)\n";
    }
    void DoSomethingElse(const std::string &b) {
        std::cout << "Receiver: Also working on (" << b << ".)\n";
    }
};

class ComplexCommand : public Command {
private:
    Receiver* receiver_;
    std::string a_;
    std::string b_;
public:
    ComplexCommand(Receiver* receiver, std::string a, std::string b)
        : receiver_(receiver), a_(std::move(a)), b_(std::move(b)) {}

    void Execute() const override {
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        receiver_->DoSomething(a_);
        receiver_->DoSomethingElse(b_);
    }
};

class Invoker {
private:
    std::unique_ptr<Command> on_start_;
    std::unique_ptr<Command> on_finish_;
public:
    void SetOnStart(std::unique_ptr<Command> command) {
        on_start_ = std::move(command);
    }
    void SetOnFinish(std::unique_ptr<Command> command) {
        on_finish_ = std::move(command);
    }

    void DoSomethingImportant() {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (on_start_) on_start_->Execute();

        std::cout << "Invoker: ...doing something really important...\n";

        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (on_finish_) on_finish_->Execute();
    }
};

int main() {
    Invoker invoker;

    invoker.SetOnStart(std::make_unique<SimpleCommand>("Say Hi!"));
    Receiver receiver;
    invoker.SetOnFinish(std::make_unique<ComplexCommand>(&receiver, "Send email", "Save report"));

    invoker.DoSomethingImportant();

    return 0;
}

/*
// Invoker: Does anybody want something done before I begin?
// SimpleCommand: See, I can do simple things like printing (Say Hi!)
// Invoker: ...doing something really important...
// Invoker: Does anybody want something done after I finish?
// ComplexCommand: Complex stuff should be done by a receiver object.
// Receiver: Working on (Send email.)
// Receiver: Also working on (Save report.)
*/