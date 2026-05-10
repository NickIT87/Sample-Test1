#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include <string>

// ==========================
// Observer interface
// ==========================
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::string& message) = 0;
};

// ==========================
// Subject (thread-safe)
// ==========================
class Subject {
public:
    using ObserverPtr = std::weak_ptr<IObserver>;

    // Подписка
    void subscribe(const std::shared_ptr<IObserver>& observer) {
        std::lock_guard lock(mutex_);
        observers_.push_back(observer);
    }

    // Отписка (по shared_ptr)
    void unsubscribe(const std::shared_ptr<IObserver>& observer) {
        std::lock_guard lock(mutex_);
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [&](const ObserverPtr& wptr) {
                    return wptr.lock() == observer;
                }),
            observers_.end()
        );
    }

    // Уведомление (thread-safe + safe iteration)
    void notify(const std::string& message) {
        std::vector<std::shared_ptr<IObserver>> alive;

        {
            std::lock_guard lock(mutex_);

            // Собираем живых наблюдателей
            for (auto it = observers_.begin(); it != observers_.end();) {
                if (auto obs = it->lock()) {
                    alive.push_back(obs);
                    ++it;
                } else {
                    // удаляем "мертвые"
                    it = observers_.erase(it);
                }
            }
        }

        // ВАЖНО: уведомляем вне mutex (избегаем deadlock)
        for (auto& obs : alive) {
            obs->update(message);
        }
    }

private:
    std::vector<ObserverPtr> observers_;
    std::mutex mutex_;
};

// ==========================
// Concrete Observer
// ==========================
class Observer : public IObserver,
                 public std::enable_shared_from_this<Observer> {
public:
    explicit Observer(std::string name, Subject& subject)
        : name_(std::move(name)), subject_(subject) {}

    // RAII подписка
    void subscribe() {
        subject_.subscribe(shared_from_this());
    }

    // RAII отписка
    void unsubscribe() {
        subject_.unsubscribe(shared_from_this());
    }

    ~Observer() {
        try {
            unsubscribe(); // безопасно
        } catch (...) {
            // никогда не бросаем из деструктора
        }
        std::cout << name_ << " destroyed\n";
    }

    void update(const std::string& message) override {
        std::lock_guard lock(io_mutex_);
        std::cout << "[" << name_ << "] received: " << message << "\n";
    }

private:
    std::string name_;
    Subject& subject_;

    // для потокобезопасного вывода
    static std::mutex io_mutex_;
};

std::mutex Observer::io_mutex_;

// ==========================
// Demo (multi-thread safe)
// ==========================
#include <thread>
#include <chrono>

int main() {
    Subject subject;

    auto obs1 = std::make_shared<Observer>("Observer1", subject);
    auto obs2 = std::make_shared<Observer>("Observer2", subject);
    auto obs3 = std::make_shared<Observer>("Observer3", subject);

    obs1->subscribe();
    obs2->subscribe();
    obs3->subscribe();

    // Поток 1: отправляет события
    std::thread publisher([&]() {
        for (int i = 0; i < 5; ++i) {
            subject.notify("Event #" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Поток 2: отписывает одного наблюдателя
    std::thread modifier([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        obs2->unsubscribe();
        std::cout << "Observer2 manually unsubscribed\n";
    });

    publisher.join();
    modifier.join();

    // obs1, obs2, obs3 уничтожатся автоматически (RAII)
}

// clang++ -std=c++20 observer_modern.cc -o observer_modern
/*
// [Observer1] received: Event #0
// [Observer2] received: Event #0
// [Observer3] received: Event #0
// [Observer1] received: Event #1
// [Observer2] received: Event #1
// [Observer3] received: Event #1
// [Observer1] received: Event #2
// [Observer2] received: Event #2
// [Observer3] received: Event #2
// Observer2 manually unsubscribed
// [Observer1] received: Event #3
// [Observer3] received: Event #3
// [Observer1] received: Event #4
// [Observer3] received: Event #4
// Observer3 destroyed
// Observer2 destroyed
// Observer1 destroyed
*/