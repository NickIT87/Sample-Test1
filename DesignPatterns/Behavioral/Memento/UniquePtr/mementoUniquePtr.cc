#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

// --- Memento ---
class Memento {
public:
    virtual ~Memento() = default;
    virtual std::string GetName() const = 0;
    virtual std::string date() const = 0;
    virtual std::string state() const = 0;
};

class ConcreteMemento : public Memento {
private:
    std::string state_;
    std::string date_;

public:
    explicit ConcreteMemento(std::string state)
        : state_(std::move(state)) {
        std::time_t now = std::time(nullptr);
        date_ = std::ctime(&now);
    }

    std::string state() const override {
        return state_;
    }

    std::string GetName() const override {
        return date_ + " / (" + state_.substr(0, 9) + "...)";
    }

    std::string date() const override {
        return date_;
    }
};

// --- Originator ---
class Originator {
private:
    std::string state_;

    std::string GenerateRandomString(int length = 10) {
        const char alphanum[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        std::string result;
        for (int i = 0; i < length; i++) {
            result += alphanum[std::rand() % (sizeof(alphanum) - 1)];
        }
        return result;
    }

public:
    explicit Originator(std::string state)
        : state_(std::move(state)) {
        std::cout << "Initial state: " << state_ << "\n";
    }

    void DoSomething() {
        state_ = GenerateRandomString(30);
        std::cout << "State changed to: " << state_ << "\n";
    }

    std::unique_ptr<Memento> Save() {
        return std::make_unique<ConcreteMemento>(state_);
    }

    void Restore(const Memento& memento) {
        state_ = memento.state();
        std::cout << "Restored to: " << state_ << "\n";
    }
};

// --- Caretaker ---
class Caretaker {
private:
    std::vector<std::unique_ptr<Memento>> mementos_;
    Originator& originator_;

public:
    explicit Caretaker(Originator& originator)
        : originator_(originator) {}

    void Backup() {
        std::cout << "Saving state...\n";
        mementos_.push_back(originator_.Save());
    }

    void Undo() {
        if (mementos_.empty()) return;

        auto memento = std::move(mementos_.back());
        mementos_.pop_back();

        std::cout << "Restoring: " << memento->GetName() << "\n";
        originator_.Restore(*memento);
    }

    void ShowHistory() const {
        for (const auto& m : mementos_) {
            std::cout << m->GetName() << "\n";
        }
    }
};

// --- Client ---
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Originator originator("Initial state");
    Caretaker caretaker(originator);

    caretaker.Backup();
    originator.DoSomething();

    caretaker.Backup();
    originator.DoSomething();

    caretaker.Backup();
    originator.DoSomething();

    std::cout << "\nHistory:\n";
    caretaker.ShowHistory();

    std::cout << "\nUndo:\n";
    caretaker.Undo();

    std::cout << "\nUndo again:\n";
    caretaker.Undo();
}

/*
// Initial state: Initial state
// Saving state...
// State changed to: wmxcRNqyFGFfaXYKMu5k8Jq2gbe0TK
// Saving state...
// State changed to: pcTKRkRwq6ZuxIP426ovch0KhUh6QC
// Saving state...
// State changed to: ReK2CvFFHkUyAfzAyLCbiOviPh9Yww

// History:
// Mon Apr  6 13:59:41 2026
//  / (Initial s...)
// Mon Apr  6 13:59:41 2026
//  / (wmxcRNqyF...)
// Mon Apr  6 13:59:41 2026
//  / (pcTKRkRwq...)

// Undo:
// Restoring: Mon Apr  6 13:59:41 2026
//  / (pcTKRkRwq...)
// Restored to: pcTKRkRwq6ZuxIP426ovch0KhUh6QC

// Undo again:
// Restoring: Mon Apr  6 13:59:41 2026
//  / (wmxcRNqyF...)
// Restored to: wmxcRNqyFGFfaXYKMu5k8Jq2gbe0TK
*/