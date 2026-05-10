#include <iostream>
#include <thread>
#include <chrono>

class Singleton {
public:
    static Singleton& instance() {
        static Singleton instance; // thread-safe initialization (C++11)
        return instance;
    }

    void setValue(const std::string& v) {
        value = v;
    }

    std::string getValue() const {
        return value;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
    std::string value = "default";
};

void ThreadFoo() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Singleton& s = Singleton::instance();
    s.setValue("FOO");
    std::cout << "ThreadFoo: " << s.getValue() << std::endl;
}

void ThreadBar() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Singleton& s = Singleton::instance();
    s.setValue("BAR");
    std::cout << "ThreadBar: " << s.getValue() << std::endl;
}

int main() {
    std::cout << "Singleton thread-safe example\n\n";

    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);

    t1.join();
    t2.join();

    std::cout << "\nFinal value: "
              << Singleton::instance().getValue()
              << std::endl;

    return 0;
}