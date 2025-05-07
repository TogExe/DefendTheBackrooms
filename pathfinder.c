#include "generator.h"
#include "pathfinder.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    int x, y;
} Node;

static int heuristic(Node a, Node b, int side) {
    int goal_dist = abs(a.x - b.x) + abs(a.y - b.y);
    int center_x = side / 2;
    int center_y = side / 2;
    int center_dist = abs(a.x - center_x) + abs(a.y - center_y);
    // Encourage paths closer to center: smaller center_dist = better
    return goal_dist + center_dist / 2;
}

static int contains(char *list, int count, char value) {
    for (int i = 0; i < count; i++)
        if (list[i] == value)
            return 1;
    return 0;
}

static int cost(char tile, char *avoid, int avoid_count, char *prefer, int prefer_count, int diag) {
    if (contains(avoid, avoid_count, tile)) return 1000000;
    if (contains(prefer, prefer_count, tile)) return diag ? 5 : 1;
    return diag ? 14 : 10;
}

int generate_organic_path(char grid[MAX_SIDE][MAX_SIDE], int side) {
    Node start = {0, 0}, end = {side-1,side-1};
    char avoid[5] = {'w','x','e','K','J'};
    char prefer[7] = {'a','b','c','d','e','f','g'};
    int avoid_count=5; 
    int prefer_count=7;
    // Dynamically allocate memory for came_from, g_score, and open
    int (*came_from)[side][2] = malloc(sizeof(int) * side * side * 2);
    int (*g_score)[side] = malloc(sizeof(int) * side * side);
    int (*open)[2] = malloc(sizeof(int) * side * side * 2);

    // Check if allocation failed
    if (!came_from || !g_score || !open) {
        printf("Memory allocation failed.\n");
        free(came_from);
        free(g_score);
        free(open);
        return-1;
    }

    // Initialize arrays
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            g_score[i][j] = 1000000;
            came_from[i][j][0] = -1;
            came_from[i][j][1] = -1;
        }
    }

    g_score[start.x][start.y] = 0;
    open[0][0] = start.x;
    open[0][1] = start.y;
    int open_count = 1;

    while (open_count > 0) {
        // Find the best node in open list
        int best = 0;
        for (int i = 1; i < open_count; i++) {
            Node a = {open[i][0], open[i][1]};
            Node b = {open[best][0], open[best][1]};
            int f_a = g_score[a.x][a.y] + heuristic(a, end, side);
            int f_b = g_score[b.x][b.y] + heuristic(b, end, side);
            if (f_a < f_b) best = i;
        }

        Node current = {open[best][0], open[best][1]};
        // Remove current from open list
        open[best][0] = open[--open_count][0];
        open[best][1] = open[open_count][1];

        if (current.x == end.x && current.y == end.y)
            break;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (nx < 0 || ny < 0 || nx >= side || ny >= side) continue;

                int diag = abs(dx) + abs(dy) > 1;
                int new_cost = g_score[current.x][current.y] + cost(grid[nx][ny], avoid, avoid_count, prefer, prefer_count, diag);
                if (new_cost < g_score[nx][ny]) {
                    g_score[nx][ny] = new_cost;
                    came_from[nx][ny][0] = current.x;
                    came_from[nx][ny][1] = current.y;
                    if (open_count < side * side) { // Avoid overflow
                        open[open_count][0] = nx;
                        open[open_count++][1] = ny;
                    }
                }
            }
        }
    }

    // If no path was found, return early
    if (came_from[end.x][end.y][0] == -1) {

        free(came_from);
        free(g_score);
        free(open);
        return -1;
    }

    // Reconstruct the path
    int x = end.x, y = end.y;
    while (!(x == start.x && y == start.y)) {
        grid[x][y] = ' ';
        int nx = came_from[x][y][0];
        int ny = came_from[x][y][1];
        x = nx; y = ny;
    }
    grid[start.x][start.y] = 'P';

    // Free dynamically allocated memory
    free(came_from);
    free(g_score);
    free(open);
    return 0;
}
