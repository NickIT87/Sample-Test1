
#include <boost/graph/adjacency_list.hpp>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

// Данные вершины
struct VertexData {
    std::uint64_t id;
    std::string label;
};

// Неориентированный граф
using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    VertexData
>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

class GraphBuilder {
public:
    Graph g;

private:
    // Уникальный ID -> дескриптор вершины
    std::unordered_map<std::uint64_t, Vertex> vertices;

    Vertex get_or_create(
        std::uint64_t id,
        const std::string& label
    ) {
        auto it = vertices.find(id);

        if (it != vertices.end()) {
            // ID уже существует.
            // Проверяем согласованность метки.
            if (g[it->second].label != label) {
                throw std::runtime_error(
                    "Conflicting labels for vertex ID " +
                    std::to_string(id)
                );
            }

            return it->second;
        }

        // Создаём вершину автоматически.
        Vertex v = boost::add_vertex(g);
        g[v] = VertexData{id, label};

        vertices.emplace(id, v);

        return v;
    }

public:
    // Добавление ребра автоматически создаёт
    // обе вершины, если они ещё не существуют.
    void add_edge(
        std::uint64_t id1,
        const std::string& label1,
        std::uint64_t id2,
        const std::string& label2
    ) {
        Vertex u = get_or_create(id1, label1);
        Vertex v = get_or_create(id2, label2);

        boost::add_edge(u, v, g);
    }

    const Graph& graph() const {
        return g;
    }

    void print() const {
        std::cout << "Vertices:\n";

        auto [vi, vi_end] = boost::vertices(g);

        for (; vi != vi_end; ++vi) {
            const auto& data = g[*vi];

            std::cout
                << "  ID=" << data.id
                << ", label=" << data.label
                << '\n';
        }

        std::cout << "\nEdges:\n";

        auto [ei, ei_end] = boost::edges(g);

        for (; ei != ei_end; ++ei) {
            Vertex u = boost::source(*ei, g);
            Vertex v = boost::target(*ei, g);

            std::cout
                << "  "
                << g[u].id << " (" << g[u].label << ")"
                << " -- "
                << g[v].id << " (" << g[v].label << ")"
                << '\n';
        }
    }
};

int main() {
    GraphBuilder builder;

    // Вершины создаются автоматически при добавлении рёбер.
    builder.add_edge(1001, "A", 1002, "B");
    builder.add_edge(1002, "B", 1003, "A");
    builder.add_edge(1003, "A", 1004, "C");
    builder.add_edge(1001, "A", 1003, "A");

    builder.print();

    return 0;
}

/*
Vertices:
  ID=1001, label=A
  ID=1002, label=B
  ID=1003, label=A
  ID=1004, label=C

Edges:
  1001 (A) -- 1002 (B)
  1002 (B) -- 1003 (A)
  1003 (A) -- 1004 (C)
  1001 (A) -- 1003 (A)
*/
