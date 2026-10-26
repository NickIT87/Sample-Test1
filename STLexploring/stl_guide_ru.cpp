/*
===============================================================================
STL В C++17 — ПОДРОБНЫЙ УЧЕБНИК В ОДНОМ ФАЙЛЕ
===============================================================================
Компиляция:
  g++ -std=c++17 -Wall -Wextra -pedantic stl_guide_ru.cpp -o stl_guide
  clang++ -std=c++17 -Wall -Wextra -pedantic stl_guide_ru.cpp -o stl_guide
  ./stl_guide

Этот файл одновременно является справочником и исполняемой программой.
Запустите его, изучите вывод, измените данные и повторите запуск.

Что обычно называют STL:
  * контейнеры — структуры для хранения данных;
  * итераторы — средства обхода диапазонов;
  * алгоритмы — сортировка, поиск, преобразование и т. п.;
  * функциональные объекты и лямбда-функции — условия и операции для алгоритмов.

Важно: стандартная библиотека C++ шире, чем STL. Например, string и потоки
ввода-вывода тоже входят в стандартную библиотеку.

Ожидаемый вывод напечатан демонстрациями. Порядок элементов unordered_map и
unordered_set не гарантирован стандартом и может отличаться.

===============================================================================
*/

#include <algorithm>
#include <array>
#include <cctype>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout;
using std::string;
using std::vector;

template <typename Range>
void print_range(const Range& range, const string& separator = " ") {
    bool first = true;
    for (const auto& value : range) {
        if (!first) cout << separator;
        cout << value;
        first = false;
    }
    cout << '\n';
}

void title(const string& text) {
    cout << "\n============================================================\n"
         << text
         << "\n============================================================\n";
}

/*
1. vector — динамический массив
-------------------------------
Универсальный контейнер, часто лучший выбор по умолчанию.
Индексирование — O(1), push_back — амортизированно O(1), вставка в середину
обычно O(n). Данные лежат непрерывно в памяти.

reserve(n) резервирует память, но не добавляет элементы.
resize(n) меняет число элементов. push_back может перевыделить память,
из-за чего указатели, ссылки и итераторы на элементы могут стать недействительными.
*/
void demo_vector() {
    title("1. vector — динамический массив");
    vector<int> v{10, 20, 30};
    v.push_back(40);
    v.emplace_back(50);

    cout << "Исходные данные с добавленными элементами: ";
    print_range(v);
    cout << "v[1] = " << v[1] << ", size = " << v.size() << '\n';

    v.insert(v.begin() + 1, 15);
    cout << "После insert: ";
    print_range(v);

    v.erase(v.begin() + 2);
    cout << "После erase: ";
    print_range(v);

    v.reserve(100);
    cout << "После reserve(100): size = " << v.size()
         << ", capacity >= 100: " << std::boolalpha
         << (v.capacity() >= 100) << '\n';
}

/*
2. array — массив фиксированного размера
----------------------------------------
std::array<T, N> удобен, когда количество элементов известно при компиляции.
Он поддерживает size(), итераторы, at() и стандартные алгоритмы.
*/
void demo_array() {
    title("2. array — фиксированный размер");
    std::array<int, 5> a{7, 2, 9, 1, 5};
    std::sort(a.begin(), a.end());
    cout << "После sort: ";
    print_range(a);
    cout << "a.at(2) = " << a.at(2) << '\n';
}

/*
3. deque — двусторонняя очередь
-------------------------------
Эффективно добавляет и удаляет элементы с начала и конца; также поддерживает
индексирование. В отличие от vector, данные не обязаны лежать одним блоком.
*/
void demo_deque() {
    title("3. deque — работа с обоими концами");
    std::deque<int> d{2, 3};
    d.push_front(1);
    d.push_back(4);
    cout << "После добавления: ";
    print_range(d);
    d.pop_front();
    d.pop_back();
    cout << "После удаления с концов: ";
    print_range(d);
}

/*
4. list — двусвязный список
---------------------------
Вставка/удаление по уже известному итератору эффективны, но поиск позиции —
O(n), и быстрого доступа по индексу нет. Для большинства задач vector проще
и быстрее; выбирайте list только при понятной причине.
*/
void demo_list() {
    title("4. list — двусвязный список");
    std::list<string> names{"Анна", "Борис", "Вера"};
    auto it = std::next(names.begin());
    names.insert(it, "Алексей");
    cout << "После вставки: ";
    print_range(names, ", ");
    names.remove("Вера");
    names.sort();
    cout << "После удаления Веры и сортировки: ";
    print_range(names, ", ");
}

/*
5. map и unordered_map — словари ключ -> значение
-------------------------------------------------
map хранит ключи в отсортированном порядке; поиск обычно O(log n).
unordered_map — хеш-таблица, поиск в среднем O(1), но порядок не гарантирован.
Оператор [] создаёт новый элемент, если ключ отсутствует. Для чтения
существующего ключа используйте at() или find().
*/
void demo_maps() {
    title("5. map и unordered_map");
    std::map<string, int> ages{{"Вера", 24}, {"Анна", 31}, {"Борис", 28}};
    ages["Глеб"] = 20;

    cout << "map по отсортированным ключам:\n";
    for (const auto& [name, age] : ages) {
        cout << "  " << name << ": " << age << '\n';
    }
    cout << "Возраст Анны: " << ages.at("Анна") << '\n';

    std::unordered_map<string, int> freq;
    for (const string& word : vector<string>{"кот", "пёс", "кот", "кот", "пёс"}) {
        ++freq[word];
    }
    cout << "Частоты слов (порядок не гарантирован):\n";
    for (const auto& [word, count] : freq) {
        cout << "  " << word << ": " << count << '\n';
    }
}

/*
6. set и unordered_set — множества уникальных значений
-----------------------------------------------------
set сортирует значения и обеспечивает операции примерно за O(log n).
unordered_set использует хеширование, операции в среднем O(1).
Повторяющиеся элементы в обоих контейнерах не дублируются.
*/
void demo_sets() {
    title("6. set и unordered_set");
    std::set<int> s{5, 1, 3, 1, 5, 2};
    cout << "set удаляет дубликаты и сортирует: ";
    print_range(s);
    s.insert(4);
    s.erase(2);
    cout << "После insert(4), erase(2): ";
    print_range(s);

    std::unordered_set<string> tags{"cpp", "stl", "cpp", "code"};
    cout << "Размер unordered_set = " << tags.size() << '\n';
    cout << "Содержит stl? " << std::boolalpha
         << (tags.find("stl") != tags.end()) << '\n';
}

/*
7. stack, queue и priority_queue
--------------------------------
stack — LIFO: последний добавлен, первый извлечён.
queue — FIFO: первый добавлен, первый извлечён.
priority_queue по умолчанию извлекает наибольший элемент первым.
С минимальной кучей можно использовать std::greater<T>.
*/
void demo_adapters() {
    title("7. stack, queue, priority_queue");
    std::stack<int> st;
    st.push(10); st.push(20); st.push(30);
    cout << "stack top = " << st.top() << '\n';
    st.pop();
    cout << "После pop top = " << st.top() << '\n';

    std::queue<string> q;
    q.push("первый"); q.push("второй"); q.push("третий");
    cout << "queue front = " << q.front() << '\n';
    q.pop();
    cout << "После pop front = " << q.front() << '\n';

    std::priority_queue<int> max_heap;
    for (int x : {4, 10, 1, 7}) max_heap.push(x);
    cout << "priority_queue (по убыванию): ";
    while (!max_heap.empty()) {
        cout << max_heap.top() << ' ';
        max_heap.pop();
    }
    cout << '\n';

    std::priority_queue<int, vector<int>, std::greater<int>> min_heap;
    for (int x : {4, 10, 1, 7}) min_heap.push(x);
    cout << "Минимальная куча: ";
    while (!min_heap.empty()) {
        cout << min_heap.top() << ' ';
        min_heap.pop();
    }
    cout << '\n';
}

/*
8. Итераторы
------------
begin() — первый элемент, end() — позиция ПОСЛЕ последнего.
Диапазон [begin, end) включает begin, но не включает end.
Не разыменовывайте end(). Итераторы позволяют использовать один алгоритм
с разными контейнерами.
*/
void demo_iterators() {
    title("8. Итераторы");
    vector<int> v{11, 22, 33, 44};
    cout << "Range-based for: ";
    for (int x : v) cout << x << ' ';
    cout << "\nЧерез итераторы: ";
    for (auto it = v.begin(); it != v.end(); ++it) cout << *it << ' ';
    cout << '\n';

    auto it = std::find(v.begin(), v.end(), 33);
    if (it != v.end()) {
        cout << "33 найдено, индекс = " << std::distance(v.begin(), it) << '\n';
    }
}

/*
9. Основные алгоритмы
---------------------
Алгоритмы из <algorithm> работают с диапазонами итераторов.
sort сортирует, find ищет, count считает совпадения, reverse разворачивает.
remove сам по себе не сокращает размер vector: используйте erase-remove.
*/
void demo_algorithms() {
    title("9. sort, find, count, reverse, min/max_element, remove");
    vector<int> v{8, 3, 8, 1, 5, 8, 2};
    std::sort(v.begin(), v.end());
    cout << "sort: "; print_range(v);

    auto it = std::find(v.begin(), v.end(), 5);
    cout << "find(5): " << (it != v.end() ? "найдено" : "не найдено") << '\n';
    cout << "count(8) = " << std::count(v.begin(), v.end(), 8) << '\n';

    std::reverse(v.begin(), v.end());
    cout << "reverse: "; print_range(v);
    cout << "min = " << *std::min_element(v.begin(), v.end())
         << ", max = " << *std::max_element(v.begin(), v.end()) << '\n';

    v.erase(std::remove(v.begin(), v.end(), 8), v.end());
    cout << "После удаления всех 8: "; print_range(v);
}

/*
10. accumulate, transform и предикаты
-------------------------------------
accumulate находится в <numeric>. transform преобразует каждый элемент.
count_if считает элементы по условию; any_of/all_of проверяют предикат.
Лямбда [захват](параметры) { тело } — компактная функция.
*/
void demo_numeric() {
    title("10. accumulate, transform, count_if, any_of, all_of");
    vector<int> v{1, 2, 3, 4, 5};
    cout << "Сумма = " << std::accumulate(v.begin(), v.end(), 0) << '\n';

    vector<int> squares(v.size());
    std::transform(v.begin(), v.end(), squares.begin(),
                   [](int x) { return x * x; });
    cout << "Квадраты: "; print_range(squares);

    cout << "Чётных чисел = "
         << std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; })
         << '\n';
    cout << "Есть число > 4? " << std::boolalpha
         << std::any_of(v.begin(), v.end(), [](int x) { return x > 4; }) << '\n';
    cout << "Все положительные? "
         << std::all_of(v.begin(), v.end(), [](int x) { return x > 0; }) << '\n';

    vector<int> mixed{1, 6, 3, 8, 2, 9, 4};
    auto boundary = std::partition(mixed.begin(), mixed.end(),
                                   [](int x) { return x % 2 == 0; });
    cout << "После partition чётные в начале: "; print_range(mixed);
    cout << "Количество чётных = " << std::distance(mixed.begin(), boundary)
         << " (порядок групп не гарантирован)\n";
}

/*
11. Пользовательский компаратор и сортировка строк
-------------------------------------------------
Компаратор sort должен задавать строгий слабый порядок. Не используйте <=
или >= в качестве компаратора. Сначала можно сравнить один критерий, затем
второй для разрешения равенства.
*/
void demo_custom_sort() {
    title("11. Сортировка по пользовательскому правилу");
    vector<int> v{5, 1, 9, 3, 7};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    cout << "Числа по убыванию: "; print_range(v);

    vector<string> words{"pear", "fig", "watermelon", "plum", "kiwi"};
    std::sort(words.begin(), words.end(), [](const string& a, const string& b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    });
    cout << "Слова по длине, затем по алфавиту: ";
    print_range(words, ", ");
}

/*
12. pair, struct и structured bindings
--------------------------------------
pair хранит два значения. Для предметных сущностей чаще понятнее struct.
Structured bindings (C++17) позволяют распаковать пару/структуру.
*/
struct Student {
    string name;
    int grade;
};

std::ostream& operator<<(std::ostream& os, const Student& s) {
    return os << s.name << '(' << s.grade << ')';
}

void demo_custom_types() {
    title("12. pair, struct и сортировка объектов");
    std::pair<string, int> product{"карандаш", 12};
    auto [name, amount] = product;
    cout << "Товар: " << name << ", количество = " << amount << '\n';

    vector<Student> students{{"Ирина", 91}, {"Максим", 76},
                             {"Олег", 91}, {"Анна", 84}};
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  if (a.grade != b.grade) return a.grade > b.grade;
                  return a.name < b.name;
              });
    cout << "Студенты по оценке (убывание), затем по имени:\n";
    for (const auto& student : students) cout << "  " << student << '\n';
}

/*
13. string и алгоритмы
----------------------
string поддерживает итераторы и стандартные алгоритмы.
std::tolower из <cctype> не является полноценным преобразованием Unicode;
для русских букв и других Unicode-текстов нужна специализированная библиотека.
*/
void demo_strings() {
    title("13. string и алгоритмы");
    string text = "Hello STL";
    cout << "Строка = " << text << ", длина = " << text.size() << '\n';
    std::reverse(text.begin(), text.end());
    cout << "После reverse = " << text << '\n';

    string word = "programming";
    auto pos = word.find("gram");
    if (pos != string::npos) cout << "\"gram\" найдено с позиции " << pos << '\n';

    string ascii = "Cpp StL";
    std::transform(ascii.begin(), ascii.end(), ascii.begin(),
                   [](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   });
    cout << "ASCII в нижнем регистре: " << ascii << '\n';
}

/*
14. Практический пример: частоты чисел
--------------------------------------
Подсчитать количество вхождений каждого числа, затем отсортировать пары
(число, частота) по убыванию частоты и по возрастанию числа.
*/
void demo_frequency() {
    title("14. Практический пример: частоты чисел");
    vector<int> data{4, 2, 4, 7, 2, 4, 7, 1, 2};
    std::unordered_map<int, int> frequency;
    for (int x : data) ++frequency[x];

    vector<std::pair<int, int>> result(frequency.begin(), frequency.end());
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    cout << "Число -> частота:\n";
    for (const auto& [number, count] : result) {
        cout << "  " << number << " -> " << count << '\n';
    }
}

/*
15. Практические задания с решениями
------------------------------------
A. Сумма только чётных элементов.
B. Удалить отрицательные числа, сохранив порядок.
C. Сортировать слова по длине, затем по алфавиту.
D. Найти максимальную оценку и число оценок >= 90.
E. Посчитать частоты слов в строке.

Сначала попробуйте решить задачи сами, а потом посмотрите код ниже.
*/
void demo_exercises() {
    title("15. Практические задания — решения");

    // A. Сумма чётных элементов.
    vector<int> a{1, 2, 3, 4, 5, 6};
    int even_sum = std::accumulate(a.begin(), a.end(), 0,
        [](int sum, int x) { return sum + (x % 2 == 0 ? x : 0); });
    cout << "A) Сумма чётных = " << even_sum << '\n';

    // B. Erase-remove удаляет все отрицательные числа, сохраняя порядок остальных.
    vector<int> b{-3, 4, -1, 0, 7, -9, 2};
    b.erase(std::remove_if(b.begin(), b.end(), [](int x) { return x < 0; }), b.end());
    cout << "B) Без отрицательных: "; print_range(b);

    // C. Сортировка строк по длине и алфавиту.
    vector<string> c{"pear", "fig", "plum", "apple", "kiwi"};
    std::sort(c.begin(), c.end(), [](const string& x, const string& y) {
        if (x.size() != y.size()) return x.size() < y.size();
        return x < y;
    });
    cout << "C) Слова: "; print_range(c, ", ");

    // D. Максимальная оценка и количество оценок не ниже 90.
    vector<int> grades{78, 95, 88, 100, 91, 67};
    cout << "D) Максимум = " << *std::max_element(grades.begin(), grades.end())
         << ", оценок >= 90 = "
         << std::count_if(grades.begin(), grades.end(), [](int x) { return x >= 90; })
         << '\n';

    // E. Простой разбор слов, разделённых пробелами.
    string line = "red blue red green blue red";
    std::unordered_map<string, int> word_count;
    std::size_t pos = 0;
    while (pos < line.size()) {
        while (pos < line.size() && line[pos] == ' ') ++pos;
        if (pos == line.size()) break;
        std::size_t end = line.find(' ', pos);
        if (end == string::npos) end = line.size();
        ++word_count[line.substr(pos, end - pos)];
        pos = end;
    }
    std::map<string, int> ordered(word_count.begin(), word_count.end());
    cout << "E) Частоты слов:\n";
    for (const auto& [word, count] : ordered) {
        cout << "   " << word << ": " << count << '\n';
    }
}

/*
16. Типичные ошибки и шпаргалка
-------------------------------
* Не разыменовывайте end(); сначала проверяйте it != end().
* Не меняйте размер vector во время обычного range-based for.
* push_back у vector может инвалидировать итераторы, указатели и ссылки.
* map[key] вставляет ключ, если его не было; используйте find()/at() для чтения.
* Не полагайтесь на порядок unordered_map и unordered_set.
* std::sort требует итераторов произвольного доступа; для list есть list.sort().
* remove/remove_if сами не сокращают размер vector: завершайте erase-remove.
* Для большинства последовательностей начните с vector.
* Не используйте <= или >= в компараторе sort.
* Диапазон алгоритма обычно [begin, end): конечная позиция не включается.

Контейнер             Назначение
vector                Динамический массив, универсальный выбор
array                 Размер известен заранее
deque                 Добавление/удаление с обоих концов
list                  Двусвязный список
map                   Отсортированный словарь
unordered_map         Хеш-словарь
set                   Отсортированное множество
unordered_set         Хеш-множество
stack                 LIFO
queue                 FIFO
priority_queue        Приоритетная очередь

Алгоритм              Назначение
sort                  Сортировка
find                  Поиск значения
count / count_if      Подсчёт совпадений / элементов по условию
reverse               Обратный порядок
min_element/max_element Минимум / максимум
accumulate            Свёртка или сумма, <numeric>
transform             Преобразование элементов
any_of/all_of         Проверка условия
partition             Разделение по предикату
remove/remove_if      Подготовка диапазона к удалению через erase

Совет для дальнейшего обучения: уверенно освойте vector, string, map, set,
sort, find, count_if, accumulate и лямбда-функции — они встречаются постоянно.
*/

int main() {
    cout << "STL в C++17 — учебные примеры\n";
    demo_vector();
    demo_array();
    demo_deque();
    demo_list();
    demo_maps();
    demo_sets();
    demo_adapters();
    demo_iterators();
    demo_algorithms();
    demo_numeric();
    demo_custom_sort();
    demo_custom_types();
    demo_strings();
    demo_frequency();
    demo_exercises();
    title("Конец учебника");
    cout << "Изменяйте входные данные и экспериментируйте с примерами.\n";
    return 0;
}

/*
STL в C++17 — учебные примеры

============================================================
1. vector — динамический массив
============================================================
Исходные данные с добавленными элементами: 10 20 30 40 50
v[1] = 20, size = 5
После insert: 10 15 20 30 40 50
После erase: 10 15 30 40 50
После reserve(100): size = 5, capacity >= 100: true

============================================================
2. array — фиксированный размер
============================================================
После sort: 1 2 5 7 9
a.at(2) = 5

============================================================
3. deque — работа с обоими концами
============================================================
После добавления: 1 2 3 4
После удаления с концов: 2 3

============================================================
4. list — двусвязный список
============================================================
После вставки: Анна, Алексей, Борис, Вера
После удаления Веры и сортировки: Алексей, Анна, Борис

============================================================
5. map и unordered_map
============================================================
map по отсортированным ключам:
  Анна: 31
  Борис: 28
  Вера: 24
  Глеб: 20
Возраст Анны: 31
Частоты слов (порядок не гарантирован):
  пёс: 2
  кот: 3

============================================================
6. set и unordered_set
============================================================
set удаляет дубликаты и сортирует: 1 2 3 5
После insert(4), erase(2): 1 3 4 5
Размер unordered_set = 3
Содержит stl? true

============================================================
7. stack, queue, priority_queue
============================================================
stack top = 30
После pop top = 20
queue front = первый
После pop front = второй
priority_queue (по убыванию): 10 7 4 1 
Минимальная куча: 1 4 7 10 

============================================================
8. Итераторы
============================================================
Range-based for: 11 22 33 44 
Через итераторы: 11 22 33 44 
33 найдено, индекс = 2

============================================================
9. sort, find, count, reverse, min/max_element, remove
============================================================
sort: 1 2 3 5 8 8 8
find(5): найдено
count(8) = 3
reverse: 8 8 8 5 3 2 1
min = 1, max = 8
После удаления всех 8: 5 3 2 1

============================================================
10. accumulate, transform, count_if, any_of, all_of
============================================================
Сумма = 15
Квадраты: 1 4 9 16 25
Чётных чисел = 2
Есть число > 4? true
Все положительные? true
После partition чётные в начале: 4 6 2 8 3 9 1
Количество чётных = 4 (порядок групп не гарантирован)

============================================================
11. Сортировка по пользовательскому правилу
============================================================
Числа по убыванию: 9 7 5 3 1
Слова по длине, затем по алфавиту: fig, kiwi, pear, plum, watermelon

============================================================
12. pair, struct и сортировка объектов
============================================================
Товар: карандаш, количество = 12
Студенты по оценке (убывание), затем по имени:
  Ирина(91)
  Олег(91)
  Анна(84)
  Максим(76)

============================================================
13. string и алгоритмы
============================================================
Строка = Hello STL, длина = 9
После reverse = LTS olleH
"gram" найдено с позиции 3
ASCII в нижнем регистре: cpp stl

============================================================
14. Практический пример: частоты чисел
============================================================
Число -> частота:
  2 -> 3
  4 -> 3
  7 -> 2
  1 -> 1

============================================================
15. Практические задания — решения
============================================================
A) Сумма чётных = 12
B) Без отрицательных: 4 0 7 2
C) Слова: fig, kiwi, pear, plum, apple
D) Максимум = 100, оценок >= 90 = 3
E) Частоты слов:
   blue: 2
   green: 1
   red: 3

============================================================
Конец учебника
============================================================
Изменяйте входные данные и экспериментируйте с примерами.
*/