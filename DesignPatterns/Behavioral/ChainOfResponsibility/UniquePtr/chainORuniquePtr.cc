#include <iostream>
#include <memory>
#include <vector>

/**
 * The Handler interface declares a method for building the chain of handlers.
 * It also declares a method for executing a request.
 */
class Handler {
public:
    virtual ~Handler() = default;

    virtual Handler* SetNext(std::unique_ptr<Handler> handler) = 0;
    virtual std::string Handle(const std::string& request) = 0;
};

/**
 * The default chaining behavior can be implemented inside a base handler class.
 */
class AbstractHandler : public Handler {
private:
    std::unique_ptr<Handler> next_handler_;

public:
    Handler* SetNext(std::unique_ptr<Handler> handler) override {
        next_handler_ = std::move(handler);
        return next_handler_.get(); // возвращаем сырой указатель для chaining
    }

    std::string Handle(const std::string& request) override {
        if (next_handler_) {
            return next_handler_->Handle(request);
        }
        return {};
    }
};

/**
 * Concrete Handlers
 */
class MonkeyHandler : public AbstractHandler {
public:
    std::string Handle(const std::string& request) override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

class SquirrelHandler : public AbstractHandler {
public:
    std::string Handle(const std::string& request) override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

class DogHandler : public AbstractHandler {
public:
    std::string Handle(const std::string& request) override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

/**
 * Client code
 */
void ClientCode(Handler& handler) {
    std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};

    for (const auto& f : food) {
        std::cout << "Client: Who wants a " << f << "?\n";
        std::string result = handler.Handle(f);

        if (!result.empty()) {
            std::cout << "  " << result;
        } else {
            std::cout << "  " << f << " was left untouched.\n";
        }
    }
}

int main() {
    auto monkey = std::make_unique<MonkeyHandler>();
    auto squirrel = std::make_unique<SquirrelHandler>();
    auto dog = std::make_unique<DogHandler>();

    // строим цепочку
    Handler* chain = monkey->SetNext(std::move(squirrel));
    chain->SetNext(std::move(dog));

    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);

    std::cout << "\nSubchain: Squirrel > Dog\n\n";
    // доступ к подцепочке
    // monkey->next_handler_ недоступен, поэтому используем chain
    ClientCode(*chain);

    return 0;
}