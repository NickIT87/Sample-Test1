#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <string>
#include <cstdint>

// Свойства вершины
struct VertexData {
    std::uint64_t id;
    std::string label;
};

// Неориентированный граф
using Graph = boost::adjacency_list<
    boost::vecS,            // Хранение рёбер
    boost::vecS,            // Хранение вершин
    boost::undirectedS,     // Неориентированный граф
    VertexData              // Свойства вершины
>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

int main() {
    Graph g;

    // Добавляем вершины.
    // Метки могут повторяться, ID уникальны.
    Vertex v1 = boost::add_vertex({1001, "A"}, g);
    Vertex v2 = boost::add_vertex({1002, "B"}, g);
    Vertex v3 = boost::add_vertex({1003, "A"}, g);
    Vertex v4 = boost::add_vertex({1004, "C"}, g);

    // Добавляем рёбра.
    boost::add_edge(v1, v2, g);
    boost::add_edge(v2, v3, g);
    boost::add_edge(v1, v3, g);
    boost::add_edge(v3, v4, g);

    // Вывод вершин.
    auto [vi, vi_end] = boost::vertices(g);

    for (; vi != vi_end; ++vi) {
        const auto& data = g[*vi];

        std::cout
            << "ID=" << data.id
            << ", label=" << data.label
            << '\n';
    }

    // Вывод рёбер по уникальным ID.
    auto [ei, ei_end] = boost::edges(g);

    for (; ei != ei_end; ++ei) {
        Vertex u = boost::source(*ei, g);
        Vertex v = boost::target(*ei, g);

        std::cout
            << g[u].id << " -- " << g[v].id
            << " (" << g[u].label
            << " -- " << g[v].label << ")\n";
    }

    return 0;
}

/*
ID=1001, label=A
ID=1002, label=B
ID=1003, label=A
ID=1004, label=C
1001 -- 1002 (A -- B)
1002 -- 1003 (B -- A)
1001 -- 1003 (A -- A)
1003 -- 1004 (A -- C)
*/