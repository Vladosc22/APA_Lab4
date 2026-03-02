# Dijkstra (All-Pairs via Repeated Runs) vs Floyd–Warshall — Operations & Density Study

This C++ project compares two approaches for computing **all-pairs shortest paths** on a **weighted, undirected graph** represented by an **adjacency matrix**:

- **Dijkstra repeated for every source node** (all-pairs by running Dijkstra `n` times)
- **Floyd–Warshall** (classic dynamic programming solution)

The program generates graphs with different **densities** (sparse / medium / dense), runs both approaches, and prints a table comparing the **number of counted operations**.

---

## Key Ideas

### What is being compared?
- **Repeated Dijkstra**: runs `dijkstra(source)` for every node `source ∈ [0..n-1]`
- **Floyd–Warshall**: computes all-pairs shortest paths in one algorithmic pass

### Graph representation
- Uses an **adjacency matrix** `n × n`
- Missing edges are stored as `INF`
- Diagonal is `0`

---

## Features

✅ Generate graphs with different densities:
- **Sparse graph (Rar)**: builds a connected tree (`n - 1` edges)
- **Medium graphs (Mediu 1/2/3)**: densities `{0.25, 0.50, 0.75}`
- **Dense graph (Dens)**: complete graph (maximum number of edges)

✅ Run and compare:
- Total **operations** for all-pairs via repeated Dijkstra
- Total **operations** for Floyd–Warshall
- Prints a ratio:  
  `ratio = operations(Dijkstra_all) / operations(Floyd)`

✅ Console table output (formatted)

---

## Example Output (Table)

The program prints a table like:

| Graph Type | Nodes | Density | Dijkstra Ops | Floyd Ops | Ratio |
|----------:|------:|--------:|-------------:|----------:|------:|
| Sparse    | 100   | 0.010   | ...          | ...       | ...   |
| Medium 1  | 100   | 0.250   | ...          | ...       | ...   |
| Medium 2  | 100   | 0.500   | ...          | ...       | ...   |
| Medium 3  | 100   | 0.750   | ...          | ...       | ...   |
| Dense     | 100   | 1.000   | ...          | ...       | ...   |

---

## Complexity (Theoretical)

Both methods end up in **O(n³)** in this project’s implementation because:

- **Dijkstra here is implemented with adjacency matrix + O(n²) selection**
  - One Dijkstra run: `O(n²)`
  - Repeated for all sources: `O(n × n²) = O(n³)`

- **Floyd–Warshall**
  - `O(n³)`

> Note: In this implementation, Floyd–Warshall may appear faster due to lower constant factors.

---

## How to Compile

Using **g++ (C++17)**:

```bash
g++ -std=c++17 -O2 -o shortest_paths main.cpp
How to Run
./shortest_paths

You will be asked to input the number of nodes n (must be n ≥ 2).

Notes & Limitations

The graph is undirected and edge weights are randomly generated in range [1..100].

The "operation count" is an approximate manual counter, not CPU-instructions or time profiling.

INF is numeric_limits<int>::max(), and the code avoids adding INF + something via checks.

Educational Purpose

This project is useful for:

Understanding shortest path algorithms

Observing how graph density impacts algorithm behavior

Comparing practical differences even when asymptotic complexity is the same

[Laborator 4 APA.docx](https://github.com/user-attachments/files/25694657/Laborator.4.APA.docx)

