#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

const int INF = numeric_limits<int>::max();

class Graf {
private:
    int n;
    vector<vector<int>> matriceAdiacenta;

public:
    Graf(int noduri) : n(noduri) {
        matriceAdiacenta.assign(noduri, vector<int>(noduri, INF));
        for (int i = 0; i < noduri; i++) {
            matriceAdiacenta[i][i] = 0;
        }
    }

    void adaugaMuchie(int u, int v, int cost) {
        matriceAdiacenta[u][v] = cost;
        matriceAdiacenta[v][u] = cost;  // graf neorientat
    }

    pair<vector<int>, long long> dijkstra(int sursa) {
        long long operatii = 0;
        vector<int> dist(n, INF);
        vector<bool> vizitat(n, false);

        dist[sursa] = 0;
        operatii += 3;  // initializari

        for (int i = 0; i < n; i++) {
            operatii += 2;  // incrementare i si comparare

            // Găsim nodul cu distanța minimă nevizitat
            int u = -1;
            for (int j = 0; j < n; j++) {
                operatii += 2;  // incrementare j si comparare
                if (!vizitat[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                    operatii += 2;  // comparare si atribuire
                }
            }
            operatii += 1;  // comparare u == -1

            if (u == -1) {
                break;
            }

            vizitat[u] = true;
            operatii += 1;  // atribuire

            // Actualizăm distanțele pentru vecinii lui u
            for (int v = 0; v < n; v++) {
                operatii += 2;  // incrementare v si comparare
                if (matriceAdiacenta[u][v] != INF && !vizitat[v]) {
                    operatii += 2;  // verificari
                    int nouaDistanta = dist[u] + matriceAdiacenta[u][v];
                    operatii += 2;  // adunare si atribuire
                    if (nouaDistanta < dist[v]) {
                        dist[v] = nouaDistanta;
                        operatii += 1;  // atribuire
                    }
                    operatii += 1;  // comparare
                }
            }
        }

        return {dist, operatii};
    }

    pair<vector<vector<int>>, long long> dijkstraToateNodurile() {
        long long totalOperatii = 0;
        vector<vector<int>> toateDistantele(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++) {
            auto [dist, op] = dijkstra(i);
            toateDistantele[i] = dist;
            totalOperatii += op;
        }

        return {toateDistantele, totalOperatii};
    }

    pair<vector<vector<int>>, long long> floydWarshall() {
        long long operatii = 0;
        vector<vector<int>> dist = matriceAdiacenta;

        operatii += n * n;  // initializare matrice

        for (int k = 0; k < n; k++) {
            operatii += 2;  // incrementare k si comparare

            for (int i = 0; i < n; i++) {
                operatii += 2;  // incrementare i si comparare

                for (int j = 0; j < n; j++) {
                    operatii += 2;  // incrementare j si comparare

                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        operatii += 2;  // verificari
                        int nouaDistanta = dist[i][k] + dist[k][j];
                        operatii += 2;  // adunare si atribuire

                        if (dist[i][j] > nouaDistanta) {
                            dist[i][j] = nouaDistanta;
                            operatii += 1;  // atribuire
                        }
                        operatii += 1;  // comparare
                    }
                }
            }
        }

        return {dist, operatii};
    }

    double getDensitate() {
        int muchiiExistente = 0;
        int maxMuchii = n * (n - 1) / 2;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (matriceAdiacenta[i][j] != INF) {
                    muchiiExistente++;
                }
            }
        }

        return maxMuchii > 0 ? (double)muchiiExistente / maxMuchii : 0.0;
    }
};

class GeneratorGraf {
private:
    mt19937 gen;

public:
    GeneratorGraf() : gen(random_device{}()) {}

    Graf genereazaGrafRar(int n) {
        Graf g(n);
        uniform_int_distribution<> distCost(1, 100);

        // Creăm un arbore (n-1 muchii) - densitate minimă
        for (int i = 1; i < n; i++) {
            uniform_int_distribution<> distParinte(0, i - 1);
            int parinte = distParinte(gen);
            int cost = distCost(gen);
            g.adaugaMuchie(parinte, i, cost);
        }

        return g;
    }

    Graf genereazaGrafDens(int n) {
        Graf g(n);
        uniform_int_distribution<> distCost(1, 100);

        // Graf complet - densitate maximă
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int cost = distCost(gen);
                g.adaugaMuchie(i, j, cost);
            }
        }

        return g;
    }

    Graf genereazaGrafMediu(int n, double densitate) {
        Graf g(n);
        uniform_int_distribution<> distNod(0, n - 1);
        uniform_int_distribution<> distCost(1, 100);

        // Mai întâi creăm un arbore pentru conectivitate
        for (int i = 1; i < n; i++) {
            uniform_int_distribution<> distParinte(0, i - 1);
            int parinte = distParinte(gen);
            int cost = distCost(gen);
            g.adaugaMuchie(parinte, i, cost);
        }

        // Adăugăm muchii suplimentare conform densității
        int maxMuchiiAditionale = (n * (n - 1) / 2) - (n - 1);
        int targetMuchiiAditionale = (int)(densitate * maxMuchiiAditionale);

        for (int i = 0; i < targetMuchiiAditionale; i++) {
            int u = distNod(gen);
            int v = distNod(gen);
            if (u != v) {
                int cost = distCost(gen);
                g.adaugaMuchie(u, v, cost);
            }
        }

        return g;
    }
};

void afiseazaTabelHeader() {
    cout << "\n" << string(90, '=') << "\n";
    cout << "| " << left << setw(15) << "Tip Graf"
         << " | " << setw(8) << "Noduri"
         << " | " << setw(12) << "Densitate"
         << " | " << setw(15) << "Operatii Dijkstra"
         << " | " << setw(15) << "Operatii Floyd"
         << " | " << setw(10) << "Raport" << " |\n";
    cout << string(90, '=') << "\n";
}

void afiseazaRandTabel(const string& tip, int noduri, double densitate,
                       long long opDijkstra, long long opFloyd) {
    double raport = opFloyd > 0 ? (double)opDijkstra / opFloyd : 0.0;

    cout << "| " << left << setw(15) << tip
         << " | " << setw(8) << noduri
         << " | " << setw(12) << fixed << setprecision(3) << densitate
         << " | " << setw(15) << opDijkstra
         << " | " << setw(15) << opFloyd
         << " | " << setw(10) << fixed << setprecision(2) << raport << " |\n";
}

int main() {
    int n;
    cout << "Introduceți numărul de noduri: ";

    if (!(cin >> n)) {
        cout << "Numărul de noduri trebuie să fie un întreg!\n";
        return 1;
    }

    if (n < 2) {
        cout << "Numărul de noduri trebuie să fie cel puțin 2!\n";
        return 1;
    }

    GeneratorGraf generator;

    afiseazaTabelHeader();

    // Test 1: Graf rar (densitate minimă)
    cout << "Generare graf rar...\n";
    Graf grafRar = generator.genereazaGrafRar(n);
    auto [distDijkstraRar, opDijkstraRar] = grafRar.dijkstraToateNodurile();
    auto [distFloydRar, opFloydRar] = grafRar.floydWarshall();
    afiseazaRandTabel("Rar", n, grafRar.getDensitate(), opDijkstraRar, opFloydRar);

    // Test 2: Grafuri medii
    vector<double> densitati = {0.25, 0.5, 0.75};
    for (int i = 0; i < densitati.size(); i++) {
        cout << "Generare graf mediu " << (i + 1) << "...\n";
        Graf grafMediu = generator.genereazaGrafMediu(n, densitati[i]);
        auto [distDijkstraMediu, opDijkstraMediu] = grafMediu.dijkstraToateNodurile();
        auto [distFloydMediu, opFloydMediu] = grafMediu.floydWarshall();
        afiseazaRandTabel("Mediu " + to_string(i + 1), n, grafMediu.getDensitate(),
                         opDijkstraMediu, opFloydMediu);
    }

    // Test 3: Graf dens (densitate maximă)
    cout << "Generare graf dens...\n";
    Graf grafDens = generator.genereazaGrafDens(n);
    auto [distDijkstraDens, opDijkstraDens] = grafDens.dijkstraToateNodurile();
    auto [distFloydDens, opFloydDens] = grafDens.floydWarshall();
    afiseazaRandTabel("Dens", n, grafDens.getDensitate(), opDijkstraDens, opFloydDens);

    cout << string(90, '=') << "\n";

    // Analiza complexității
    cout << "\nANALIZA COMPLEXITĂȚII:\n";
    cout << "Dijkstra pentru toate nodurile: O(n × n²) = O(n³)\n";
    cout << "Floyd-Warshall: O(n³)\n";
    cout << "În practică, Floyd-Warshall are constanta multiplicativă mai mică.\n";

    return 0;
}