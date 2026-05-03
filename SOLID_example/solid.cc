#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <string_view>

// =====================================================
// 🔹 SOLID: I (Interface Segregation) + D (Dependency Inversion)
// =====================================================
// - Интерфейс минимальный → только то, что реально нужно (I)
// - Высокоуровневый код будет зависеть от ЭТОЙ абстракции, а не от реализаций (D)
class INotificationSender {
public:
    virtual ~INotificationSender() = default;

    // Минимальный контракт → отправка сообщения
    virtual void send(std::string_view recipient,
                      std::string_view message) const = 0;
};

// =====================================================
// 🔹 SOLID: S (Single Responsibility)
// =====================================================
// Каждый класс отвечает ТОЛЬКО за один способ отправки

class EmailSender : public INotificationSender {
public:
    void send(std::string_view recipient,
              std::string_view message) const override {
        std::cout << "[Email] to: " << recipient
                  << " | " << message << "\n";
    }
};

class SmsSender : public INotificationSender {
public:
    void send(std::string_view recipient,
              std::string_view message) const override {
        std::cout << "[SMS] to: " << recipient
                  << " | " << message << "\n";
    }
};

// =====================================================
// 🔹 SOLID: O (Open/Closed)
// =====================================================
// Мы ДОБАВЛЯЕМ новую функциональность (PushSender),
// НЕ изменяя существующий код → система расширяется, но не ломается

class PushSender : public INotificationSender {
public:
    void send(std::string_view recipient,
              std::string_view message) const override {
        std::cout << "[Push] to: " << recipient
                  << " | " << message << "\n";
    }
};

// =====================================================
// 🔹 SOLID: D (Dependency Inversion)
// =====================================================
// Высокоуровневый модуль НЕ зависит от EmailSender/SmsSender напрямую.
// Он работает через абстракцию INotificationSender.

class NotificationService {
public:
    // Внедрение зависимости (Dependency Injection)
    // Мы ПЕРЕДАЁМ реализацию извне → слабая связность
    explicit NotificationService(std::unique_ptr<INotificationSender> sender)
        : sender_(std::move(sender)) {}

    void notify(std::string_view user,
                std::string_view msg) const {
        sender_->send(user, msg);
    }

private:
    std::unique_ptr<INotificationSender> sender_;
};

// =====================================================
// 🔹 SOLID: L (Liskov Substitution Principle)
// =====================================================
// Любой класс-наследник должен корректно заменять базовый.
// Здесь это соблюдается, потому что:
// - поведение не ломает ожидания
// - сигнатура одинаковая
// - нет скрытых ограничений
//
// (Прямого кода тут нет — это гарантия дизайна)


// =====================================================
// 🔹 Modern C++: constexpr + string_view
// =====================================================
constexpr bool is_valid_user(std::string_view user) {
    return !user.empty() && user.size() < 32;
}

// =====================================================
// 🔹 Композиция приложения
// =====================================================
int main() {
    // Modern C++: RAII + отсутствие raw new/delete
    std::vector<std::unique_ptr<NotificationService>> services;

    // =================================================
    // Dependency Injection (D)
    // =================================================
    // Мы сами решаем, какую реализацию передать

    services.push_back(std::make_unique<NotificationService>(
        std::make_unique<EmailSender>()
    ));

    services.push_back(std::make_unique<NotificationService>(
        std::make_unique<SmsSender>()
    ));

    services.push_back(std::make_unique<NotificationService>(
        std::make_unique<PushSender>()
    ));

    std::string user = "nick";
    std::string message = "Hello from modern C++ 🚀";

    // Compile-time friendly логика
    if (!is_valid_user(user)) {
        std::cerr << "Invalid user\n";
        return 1;
    }

    // =================================================
    // Полиморфизм → LSP в действии
    // =================================================
    // Нам всё равно, какой именно sender внутри
    for (const auto& service : services) {
        service->notify(user, message);
    }

    return 0;
}