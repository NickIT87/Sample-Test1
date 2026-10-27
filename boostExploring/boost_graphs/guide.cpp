// guide.cpp
// Практическое руководство по Boost.Graph.
//
// Boost.Graph — библиотека C++ для представления графов и работы с ними.
// Она предоставляет контейнеры графов, итераторы, свойства вершин и рёбер,
// а также реализации классических алгоритмов теории графов.
//
// Компиляция:
//   g++ -std=c++17 -O2 -Wall -Wextra guide.cpp -o guide
//
// Запуск:
//   ./guide
// 
// ## Что изучать дальше
// - `adjacency_list` — выбор структуры хранения графа.
// - `property_map` — работа с индексами и свойствами вершин и рёбер.
// - `breadth_first_search` и `depth_first_search` — пользовательские visitor-объекты.
// - `dijkstra_shortest_paths` — кратчайшие пути.
// - `topological_sort` — анализ зависимостей.
// - `connected_components` — компоненты связности.
// - `kruskal_minimum_spanning_tree` — минимальное остовное дерево.

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/property_map/property_map.hpp>

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// ============================================================
// 1. Типы графов
// ============================================================

// adjacency_list — один из основных контейнеров Boost.Graph.
//
// Параметры шаблона:
//   OutEdgeListS — контейнер исходящих рёбер;
//   VertexListS  — контейнер вершин;
//   DirectedS    — направленный или ненаправленный граф;
//   VertexProperties — свойства вершин;
//   EdgeProperties   — свойства рёбер.
//
// vecS позволяет обращаться к вершинам по индексам 0, 1, 2...
// listS подходит для других сценариев, но не предоставляет такой же
// встроенный индексный доступ.
//
// Пример ниже: ненаправленный взвешенный граф.

using UndirectedGraph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_weight_t, int>
>;

// Направленный граф без дополнительных свойств рёбер.
using DirectedGraph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS
>;

// ============================================================
// 2. Посетители BFS и DFS
// ============================================================

// В Boost.Graph алгоритмы обхода используют visitor — объект,
// методы которого вызываются при посещении вершин или рёбер.
//
// Наследование от базового visitor позволяет подключить собственное
// поведение, не изменяя реализацию алгоритма.

struct PrintBFSVisitor : boost::default_bfs_visitor {
    template <typename Vertex, typename Graph>
    void discover_vertex(Vertex v, const Graph&) const {
        std::cout << v << ' ';
    }
};

struct PrintDFSVisitor : boost::default_dfs_visitor {
    template <typename Vertex, typename Graph>
    void discover_vertex(Vertex v, const Graph&) const {
        std::cout << v << ' ';
    }
};

// ============================================================
// 3. Создание графа и базовые операции
// ============================================================

UndirectedGraph make_weighted_graph() {
    // Конструктор с числом вершин создаёт вершины с индексами 0..5.
    UndirectedGraph g(6);

    // add_edge(u, v, weight, graph) добавляет ребро с весом.
    // В ненаправленном графе ребро можно проходить в обоих направлениях.
    boost::add_edge(0, 1, 4, g);
    boost::add_edge(0, 2, 2, g);
    boost::add_edge(1, 2, 1, g);
    boost::add_edge(1, 3, 5, g);
    boost::add_edge(2, 3, 8, g);
    boost::add_edge(2, 4, 10, g);
    boost::add_edge(3, 4, 2, g);
    boost::add_edge(3, 5, 6, g);
    boost::add_edge(4, 5, 3, g);

    return g;
}

void example_graph_basics() {
    std::cout << "\n=== 1. Создание графа ===\n";

    auto g = make_weighted_graph();

    std::cout << "Количество вершин: "
              << boost::num_vertices(g) << '\n';

    std::cout << "Количество рёбер: "
              << boost::num_edges(g) << '\n';

    // Итерация по всем вершинам.
    auto [vi, vi_end] = boost::vertices(g);

    std::cout << "Вершины: ";
    for (; vi != vi_end; ++vi) {
        std::cout << *vi << ' ';
    }
    std::cout << '\n';

    // Итерация по всем рёбрам.
    auto [ei, ei_end] = boost::edges(g);

    std::cout << "Рёбра (u -- v, вес):\n";
    for (; ei != ei_end; ++ei) {
        const auto u = boost::source(*ei, g);
        const auto v = boost::target(*ei, g);
        const auto w = boost::get(boost::edge_weight, g, *ei);

        std::cout << "  " << u << " -- " << v
                  << ", вес = " << w << '\n';
    }
}

// ============================================================
// 4. Обход графа в ширину — BFS
// ============================================================

// BFS сначала посещает соседей стартовой вершины, затем соседей
// следующего уровня и так далее.
//
// Типичные применения:
//   - поиск кратчайшего пути в невзвешенном графе;
//   - определение расстояний по числу рёбер;
//   - поиск вершин в пределах заданного числа переходов.

void example_bfs() {
    std::cout << "\n=== 2. Обход в ширину (BFS) ===\n";

    auto g = make_weighted_graph();

    // boost::visitor(...) передаёт пользовательский visitor алгоритму.
    boost::breadth_first_search(
        g,
        boost::vertex(0, g),
        boost::visitor(PrintBFSVisitor{})
    );

    std::cout << "\n";
}

// ============================================================
// 5. Обход графа в глубину — DFS
// ============================================================

// DFS углубляется по одному из путей настолько далеко, насколько
// возможно, после чего возвращается к предыдущим вершинам.
//
// Типичные применения:
//   - поиск циклов;
//   - анализ зависимостей;
//   - топологическая сортировка;
//   - выделение компонент и исследование структуры графа.

void example_dfs() {
    std::cout << "\n=== 3. Обход в глубину (DFS) ===\n";

    auto g = make_weighted_graph();

    // depth_first_search обходит все компоненты графа.
    boost::depth_first_search(
        g,
        boost::visitor(PrintDFSVisitor{})
    );

    std::cout << '\n';
}

// ============================================================
// 6. Кратчайшие пути — алгоритм Дейкстры
// ============================================================

// Алгоритм Дейкстры находит кратчайшие расстояния от одной стартовой
// вершины до всех достижимых вершин графа с НЕОТРИЦАТЕЛЬНЫМИ весами.
//
// Важно: для отрицательных весов Дейкстру использовать нельзя.
// В таком случае следует рассмотреть Bellman–Ford.
//
// predecessor[v] хранит предшественника вершины v в кратчайшем пути.
// distance[v] хранит длину найденного кратчайшего пути.

void example_dijkstra() {
    std::cout << "\n=== 4. Алгоритм Дейкстры ===\n";

    auto g = make_weighted_graph();
    const auto n = boost::num_vertices(g);
    const auto source = boost::vertex(0, g);

    using Vertex = boost::graph_traits<UndirectedGraph>::vertex_descriptor;

    std::vector<int> distance(n);
    std::vector<Vertex> predecessor(n);

    // Инициализация массива предшественников.
    auto [vi, vi_end] = boost::vertices(g);
    for (; vi != vi_end; ++vi) {
        predecessor[*vi] = *vi;
    }

    boost::dijkstra_shortest_paths(
        g,
        source,
        boost::distance_map(
            boost::make_iterator_property_map(
                distance.begin(),
                boost::get(boost::vertex_index, g)
            )
        ).predecessor_map(
            boost::make_iterator_property_map(
                predecessor.begin(),
                boost::get(boost::vertex_index, g)
            )
        )
    );

    std::cout << "Кратчайшие расстояния от вершины 0:\n";

    for (std::size_t v = 0; v < n; ++v) {
        std::cout << "  До " << v << ": ";

        if (distance[v] == (std::numeric_limits<int>::max)()) {
            std::cout << "недостижима\n";
        } else {
            std::cout << distance[v] << '\n';
        }
    }

    // Восстановление пути от 0 до target.
    const std::size_t target = 5;

    std::vector<Vertex> path;
    Vertex current = static_cast<Vertex>(target);

    // Двигаемся от цели к источнику по массиву предшественников.
    // Если вершина является собственным предшественником, значит,
    // мы дошли до источника либо до недостижимой вершины.
    for (std::size_t steps = 0; steps < n; ++steps) {
        path.push_back(current);

        if (current == source) {
            break;
        }

        const Vertex previous = predecessor[current];

        if (previous == current) {
            break;
        }

        current = previous;
    }

    if (path.empty() || path.back() != source) {
        std::cout << "Путь до вершины " << target
                  << " не найден.\n";
        return;
    }

    std::reverse(path.begin(), path.end());

    std::cout << "Путь от 0 до " << target << ": ";
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i != 0) {
            std::cout << " -> ";
        }
        std::cout << path[i];
    }

    std::cout << "\nДлина пути: " << distance[target] << '\n';
}

// ============================================================
// 7. Топологическая сортировка
// ============================================================

// Топологическая сортировка определена для ориентированного
// ациклического графа (DAG).
//
// Если граф содержит ориентированный цикл, топологическая сортировка
// невозможна, и Boost.Graph выбросит исключение not_a_dag.
//
// Пример: порядок выполнения задач с зависимостями.

void example_topological_sort() {
    std::cout << "\n=== 5. Топологическая сортировка ===\n";

    DirectedGraph g(6);

    // Ребро u -> v означает, что u должно предшествовать v.
    boost::add_edge(0, 2, g);
    boost::add_edge(1, 2, g);
    boost::add_edge(1, 3, g);
    boost::add_edge(2, 4, g);
    boost::add_edge(3, 4, g);
    boost::add_edge(4, 5, g);

    std::vector<
        boost::graph_traits<DirectedGraph>::vertex_descriptor
    > order;

    try {
        // topological_sort записывает вершины в обратном
        // топологическом порядке, поэтому после вызова разворачиваем
        // результат с помощью reverse().
        boost::topological_sort(
            g,
            std::back_inserter(order)
        );

        std::reverse(order.begin(), order.end());

        std::cout << "Допустимый порядок задач: ";
        for (const auto v : order) {
            std::cout << v << ' ';
        }
        std::cout << '\n';
    } catch (const boost::not_a_dag&) {
        std::cout << "Граф содержит цикл: сортировка невозможна.\n";
    }
}

// ============================================================
// 8. Компоненты связности
// ============================================================

// connected_components работает с ненаправленными графами.
//
// Каждой вершине алгоритм присваивает номер компоненты.
// Вершины одной компоненты связаны друг с другом путями.
//
// Для направленного графа понятие компонент требует отдельного
// уточнения: например, можно искать сильно связные компоненты.

void example_connected_components() {
    std::cout << "\n=== 6. Компоненты связности ===\n";

    UndirectedGraph g(7);

    // Первая компонента: 0, 1, 2.
    boost::add_edge(0, 1, 1, g);
    boost::add_edge(1, 2, 1, g);
    boost::add_edge(0, 2, 1, g);

    // Вторая компонента: 3, 4.
    boost::add_edge(3, 4, 1, g);

    // Третья компонента: 5, 6.
    boost::add_edge(5, 6, 1, g);

    std::vector<int> component(boost::num_vertices(g));

    const int count = boost::connected_components(
        g,
        boost::make_iterator_property_map(
            component.begin(),
            boost::get(boost::vertex_index, g)
        )
    );

    std::cout << "Количество компонент: " << count << '\n';

    for (std::size_t v = 0; v < component.size(); ++v) {
        std::cout << "Вершина " << v
                  << " принадлежит компоненте "
                  << component[v] << '\n';
    }
}

// ============================================================
// 9. Минимальное остовное дерево — алгоритм Краскала
// ============================================================

// Минимальное остовное дерево (MST) соединяет все вершины связного
// ненаправленного взвешенного графа с минимальной суммой весов.
//
// Краскал рассматривает рёбра в порядке возрастания веса и добавляет
// ребро, если оно не образует цикл с уже выбранными рёбрами.
//
// Если граф несвязный, алгоритм возвращает минимальный остовный лес.

void example_kruskal() {
    std::cout << "\n=== 7. Минимальное остовное дерево (Краскал) ===\n";

    auto g = make_weighted_graph();

    using Edge = boost::graph_traits<UndirectedGraph>::edge_descriptor;

    std::vector<Edge> mst;

    boost::kruskal_minimum_spanning_tree(
        g,
        std::back_inserter(mst),
        boost::weight_map(boost::get(boost::edge_weight, g))
    );

    int total_weight = 0;

    std::cout << "Рёбра минимального остовного дерева:\n";

    for (const auto e : mst) {
        const auto u = boost::source(e, g);
        const auto v = boost::target(e, g);
        const int w = boost::get(boost::edge_weight, g, e);

        std::cout << "  " << u << " -- " << v
                  << ", вес = " << w << '\n';

        total_weight += w;
    }

    std::cout << "Суммарный вес: " << total_weight << '\n';

    // Для связного графа с V вершинами остовное дерево содержит V - 1
    // рёбер. Если рёбер меньше, граф может быть несвязным.
    const auto expected_edges = boost::num_vertices(g) - 1;

    if (mst.size() == expected_edges) {
        std::cout << "Получено остовное дерево.\n";
    } else {
        std::cout << "Получен остовный лес: исходный граф несвязный.\n";
    }
}

// ============================================================
// 10. Точка входа
// ============================================================

int main() {
    std::cout << "====================================\n";
    std::cout << "     Практика Boost.Graph\n";
    std::cout << "====================================\n";

    example_graph_basics();
    example_bfs();
    example_dfs();
    example_dijkstra();
    example_topological_sort();
    example_connected_components();
    example_kruskal();

    std::cout << "\nВсе примеры выполнены.\n";
    return 0;
}

/*
====================================
     Практика Boost.Graph
====================================

=== 1. Создание графа ===
Количество вершин: 6
Количество рёбер: 9
Вершины: 0 1 2 3 4 5 
Рёбра (u -- v, вес):
  0 -- 1, вес = 4
  0 -- 2, вес = 2
  1 -- 2, вес = 1
  1 -- 3, вес = 5
  2 -- 3, вес = 8
  2 -- 4, вес = 10
  3 -- 4, вес = 2
  3 -- 5, вес = 6
  4 -- 5, вес = 3

=== 2. Обход в ширину (BFS) ===
0 1 2 3 4 5 

=== 3. Обход в глубину (DFS) ===
0 1 2 3 4 5 

=== 4. Алгоритм Дейкстры ===
Кратчайшие расстояния от вершины 0:
  До 0: 0
  До 1: 3
  До 2: 2
  До 3: 8
  До 4: 10
  До 5: 13
Путь от 0 до 5: 0 -> 2 -> 1 -> 3 -> 4 -> 5
Длина пути: 13

=== 5. Топологическая сортировка ===
Допустимый порядок задач: 1 3 0 2 4 5 

=== 6. Компоненты связности ===
Количество компонент: 3
Вершина 0 принадлежит компоненте 0
Вершина 1 принадлежит компоненте 0
Вершина 2 принадлежит компоненте 0
Вершина 3 принадлежит компоненте 1
Вершина 4 принадлежит компоненте 1
Вершина 5 принадлежит компоненте 2
Вершина 6 принадлежит компоненте 2

=== 7. Минимальное остовное дерево (Краскал) ===
Рёбра минимального остовного дерева:
  1 -- 2, вес = 1
  0 -- 2, вес = 2
  3 -- 4, вес = 2
  4 -- 5, вес = 3
  1 -- 3, вес = 5
Суммарный вес: 13
Получено остовное дерево.

Все примеры выполнены.
*/