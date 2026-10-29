import matplotlib.pyplot as plt
import networkx as nx

from graph_module import GraphBuilder


def main():
    # Создаём C++-объект.
    graph = GraphBuilder()

    # Добавляем рёбра.
    # Вершины создаются автоматически.
    graph.add_edge(100, "Alice", 200, "Bob")
    graph.add_edge(200, "Bob", 300, "Charlie")
    graph.add_edge(100, "Alice", 300, "Charlie")

    # Получаем результат из C++.
    nodes = graph.get_nodes()
    edges = graph.get_edges()

    print("Вершины:")
    for vertex_id, label in nodes:
        print(f"  ID={vertex_id}, label={label}")

    print("\nРёбра:")
    for id1, id2 in edges:
        print(f"  {id1} -- {id2}")

    print("\nКоличество вершин:", graph.vertex_count())
    print("Количество рёбер:", graph.edge_count())


def show():
    # Строим граф на стороне C++.
    builder = GraphBuilder()

    builder.add_edge(100, "Alice", 200, "Bob")
    builder.add_edge(200, "Bob", 300, "Charlie")
    builder.add_edge(100, "Alice", 300, "Charlie")

    # Забираем данные из C++.
    nodes = builder.get_nodes()
    edges = builder.get_edges()

    # Создаём Python-граф только для визуализации.
    G = nx.Graph()

    # ID используется как идентификатор узла.
    # Метка хранится отдельно и может повторяться.
    labels = {}

    for vertex_id, label in nodes:
        G.add_node(vertex_id)
        labels[vertex_id] = label

    for id1, id2 in edges:
        G.add_edge(id1, id2)

    # Располагаем узлы на плоскости.
    pos = nx.spring_layout(G, seed=42)

    nx.draw_networkx(
        G,
        pos=pos,
        labels=labels,
        with_labels=True,
        node_size=1800,
        font_size=10,
    )

    plt.title("Graph generated in C++")
    plt.axis("off")
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
    show()


# PYTHONPATH=build python main.py
# 
# Вершины:
#   ID=100, label=Alice
#   ID=200, label=Bob
#   ID=300, label=Charlie

# Рёбра:
#   100 -- 200
#   200 -- 300
#   100 -- 300

# Количество вершин: 3
# Количество рёбер: 3
