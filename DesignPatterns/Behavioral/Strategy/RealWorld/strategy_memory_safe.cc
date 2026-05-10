#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

// ======================= Strategy =======================

class Strategy {
public:
    virtual ~Strategy() = default;

    // noexcept + [[nodiscard]] — важные production-практики
    [[nodiscard]]
    virtual std::string do_algorithm(std::string_view data) const = 0;
};

// ======================= Concrete Strategies =======================

class SortAscending final : public Strategy {
public:
    [[nodiscard]]
    std::string do_algorithm(std::string_view data) const override {
        std::string result{data};
        std::sort(result.begin(), result.end());
        return result;
    }
};

class SortDescending final : public Strategy {
public:
    [[nodiscard]]
    std::string do_algorithm(std::string_view data) const override {
        std::string result{data};
        std::sort(result.begin(), result.end(), std::greater<>{});
        return result;
    }
};

// ======================= Context =======================

class Context {
public:
    Context() = default;

    explicit Context(std::unique_ptr<Strategy> strategy) noexcept
        : strategy_(std::move(strategy)) {}

    // запрещаем копирование (важно для unique_ptr)
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    // разрешаем перемещение
    Context(Context&&) noexcept = default;
    Context& operator=(Context&&) noexcept = default;

    void set_strategy(std::unique_ptr<Strategy> strategy) noexcept {
        strategy_ = std::move(strategy);
    }

    [[nodiscard]]
    std::optional<std::string> execute(std::string_view data) const {
        if (!strategy_) {
            return std::nullopt;
        }
        return strategy_->do_algorithm(data);
    }

private:
    std::unique_ptr<Strategy> strategy_;
};

// ======================= Client =======================

int main() {
    Context ctx;

    ctx.set_strategy(std::make_unique<SortAscending>());
    if (auto result = ctx.execute("aecbd")) {
        std::cout << "Ascending: " << *result << '\n';
    }

    ctx.set_strategy(std::make_unique<SortDescending>());
    if (auto result = ctx.execute("aecbd")) {
        std::cout << "Descending: " << *result << '\n';
    }
}

// clang++ -std=c++20 strategy_memory_safe.cc
/*
// Ascending: abcde
// Descending: edcba
*/