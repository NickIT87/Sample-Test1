#include <boost/graph/adjacency_list.hpp>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Данные вершины.
struct VertexData {
    std::uint64_t id;
    std::string label;
};

// Неориентированный граф.
// VertexData хранится в каждой вершине.
using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    VertexData
>;

// Тип дескриптора вершины.
using Vertex =
    boost::graph_traits<Graph>::vertex_descriptor;

class GraphBuilder {
private:
    Graph graph;

    // Уникальный ID -> дескриптор вершины.
    std::unordered_map<std::uint64_t, Vertex> vertices;

    Vertex get_or_create(
        std::uint64_t id,
        const std::string& label
    ) {
        // 1. Ищем вершину по ID.
        auto it = vertices.find(id);

        // 2. Если нашли, используем существующую.
        if (it != vertices.end()) {
            Vertex v = it->second;

            // Не допускаем конфликтующих меток.
            if (graph[v].label != label) {
                throw std::runtime_error(
                    "Same vertex ID has different labels"
                );
            }

            return v;
        }

        // 3. Если вершины нет, создаём её.
        Vertex v = boost::add_vertex(graph);

        // 4. Сохраняем ID и метку в данных вершины.
        graph[v] = VertexData{id, label};

        // 5. Запоминаем соответствие ID и дескриптора.
        vertices[id] = v;

        // 6. Возвращаем дескриптор.
        return v;
    }

public:
    void add_edge(
        std::uint64_t id1,
        const std::string& label1,
        std::uint64_t id2,
        const std::string& label2
    ) {
        // Создаём вершины, только если их ещё нет.
        Vertex u = get_or_create(id1, label1);
        Vertex v = get_or_create(id2, label2);

        // Добавляем неориентированное ребро.
        boost::add_edge(u, v, graph);
    }

    // Возвращает список (ID, метка).
    std::vector<std::tuple<std::uint64_t, std::string>>
    get_nodes() const {
        std::vector<
            std::tuple<std::uint64_t, std::string>
        > result;

        auto range = boost::vertices(graph);

        for (auto it = range.first;
             it != range.second;
             ++it) {
            const Vertex v = *it;

            result.emplace_back(
                graph[v].id,
                graph[v].label
            );
        }

        return result;
    }

    // Возвращает список (ID первой вершины, ID второй).
    std::vector<std::tuple<std::uint64_t, std::uint64_t>>
    get_edges() const {
        std::vector<
            std::tuple<std::uint64_t, std::uint64_t>
        > result;

        auto range = boost::edges(graph);

        for (auto it = range.first;
             it != range.second;
             ++it) {
            const auto e = *it;

            const Vertex u = boost::source(e, graph);
            const Vertex v = boost::target(e, graph);

            result.emplace_back(
                graph[u].id,
                graph[v].id
            );
        }

        return result;
    }

    std::size_t vertex_count() const {
        return boost::num_vertices(graph);
    }

    std::size_t edge_count() const {
        return boost::num_edges(graph);
    }
};

// Название модуля должно совпадать с именем в CMake.
PYBIND11_MODULE(graph_module, m) {
    m.doc() = "C++ graph builder based on Boost.Graph";

    py::class_<GraphBuilder>(m, "GraphBuilder")
        .def(py::init<>())

        .def(
            "add_edge",
            &GraphBuilder::add_edge,
            py::arg("id1"),
            py::arg("label1"),
            py::arg("id2"),
            py::arg("label2"),
            "Create missing vertices and add an edge"
        )

        .def(
            "get_nodes",
            &GraphBuilder::get_nodes
        )

        .def(
            "get_edges",
            &GraphBuilder::get_edges
        )

        .def(
            "vertex_count",
            &GraphBuilder::vertex_count
        )

        .def(
            "edge_count",
            &GraphBuilder::edge_count
        );
}

/*
cmake -S . -B build \
  -DPython3_EXECUTABLE="$(python -c 'import sys; print(sys.executable)')" \
  -Dpybind11_DIR="$(python -m pybind11 --cmakedir)"

cmake --build build --config Release -j 4
*/