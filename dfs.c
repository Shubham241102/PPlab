#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#define MAX_VERTICES 100

int numVertices;
bool adjMatrix[MAX_VERTICES][MAX_VERTICES];
bool visited[MAX_VERTICES];

void initializeGraph() {
    numVertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            adjMatrix[i][j] = false;
        }
        visited[i] = false;
    }
}

void addEdge(int u, int v) {
    adjMatrix[u][v] = true;
    adjMatrix[v][u] = true;
}

void dfsSerial(int startVertex) {
    visited[startVertex] = true;
    printf("Visited %d\n", startVertex);

    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[startVertex][i] && !visited[i]) {
            dfsSerial(i);
        }
    }
}

void dfsParallel(int startVertex) {
    visited[startVertex] = true;
    printf("Visited %d\n", startVertex);

    #pragma omp parallel for
    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[startVertex][i] && !visited[i]) {
            dfsParallel(i);
        }
    }
}

int main() {
    initializeGraph();

    // Add edges to the graph (example)
    addEdge(0, 1);
    addEdge(0, 4);
    addEdge(1, 2);
    addEdge(1, 3);
    addEdge(1, 4);
    addEdge(2, 3);
    addEdge(3, 4);
    numVertices = 5;

    double start_time, end_time;

    // Serial DFS
    start_time = omp_get_wtime();
    dfsSerial(0);
    end_time = omp_get_wtime();
    printf("Serial DFS execution time: %f seconds\n", end_time - start_time);

    // Reset visited array
    for (int i = 0; i < MAX_VERTICES; i++) {
        visited[i] = false;
    }

    // Parallel DFS
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        dfsParallel(0);
    }
    end_time = omp_get_wtime();
    printf("Parallel DFS execution time: %f seconds\n", end_time - start_time);

    return 0;
}
