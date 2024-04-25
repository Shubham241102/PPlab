#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#define MAX_VERTICES 100

int graph[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int level[MAX_VERTICES];
int n; // Number of vertices

void bfs_serial(int start) {
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        level[i] = INT_MAX;
    }

    visited[start] = 1;
    level[start] = 0;
    queue[rear++] = start;

    while (front != rear) {
        int u = queue[front++];

        for (int v = 0; v < n; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = 1;
                level[v] = level[u] + 1;
                queue[rear++] = v;
            }
        }
    }
}

void bfs_parallel(int start) {
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        level[i] = INT_MAX;
    }

    visited[start] = 1;
    level[start] = 0;
    queue[rear++] = start;

    while (front != rear) {
        int u = queue[front++];

        #pragma omp parallel for
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && !visited[v]) {
                #pragma omp critical
                {
                    if (!visited[v]) {
                        visited[v] = 1;
                        level[v] = level[u] + 1;
                        queue[rear++] = v;
                    }
                }
            }
        }
    }
}

int main() {
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    int start_vertex;
    printf("Enter the starting vertex: ");
    scanf("%d", &start_vertex);

    double start_time, end_time;

    // Serial BFS
    start_time = omp_get_wtime();
    bfs_serial(start_vertex);
    end_time = omp_get_wtime();
    printf("Serial BFS execution time: %f seconds\n", end_time - start_time);

    // Parallel BFS
    start_time = omp_get_wtime();
    bfs_parallel(start_vertex);
    end_time = omp_get_wtime();
    printf("Parallel BFS execution time: %f seconds\n", end_time - start_time);

    return 0;
}
