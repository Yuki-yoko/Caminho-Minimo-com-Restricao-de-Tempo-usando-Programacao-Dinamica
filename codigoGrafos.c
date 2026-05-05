// Dado um grafo dirigido (n vértices, m arestas), cada aresta possui:
//   - custo (c) em km
//   - tempo (t) em minutos

// Para cada consulta (s, d, h):
//   encontrar o menor custo para ir de s até d usando até h minutos.
//   dp[t][v] = menor custo para chegar em v usando EXATAMENTE t minutos

// Recorrência:
//   dp[0][s] = 0
//   dp[0][v != s] = INF

//   Para t >= 1:
//       dp[t][v] = min(
//            dp[t-1][v],
//            min_{(u -> v) com tempo tt <= t}( dp[t-tt][u] + custo )
//       )
//
// Resposta da consulta: menor dp[t][dest] para 0 <= t <= h.

// Complexidade por origem:
//    O(m * h_max)
// Memória:
//    O(n * h_max)
//EXPLICAÇÃO
//   A complexidade do algoritmo é O(m · h_max) porque, para cada origem, ele monta uma 
//tabela de programação dinâmica que é preenchida minuto a minuto até o limite h_max. 
//  Em cada minuto, o algoritmo percorre todas as arestas do grafo para relaxá-las, o 
//que custa O(m). Como esse processo é repetido h_max vezes, o custo total por origem 
//é O(m · h_max).

#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000000000000LL  // 10^18

// Estrutura de aresta de entrada (u → v)
typedef struct {
    int u;
    int cost;
    int tim;
} IncomingEdge;

// Estrutura para consulta
typedef struct {
    int s, d, h;
} Query;

// Estrutura para armazenar lista de incoming edges por vértice
typedef struct {
    IncomingEdge *edges;
    int size;
    int capacity;
} EdgeList;

// Função para adicionar aresta à lista
void add_edge(EdgeList *list, int u, int cost, int tim) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->edges = realloc(list->edges, list->capacity * sizeof(IncomingEdge));
    }
    list->edges[list->size].u = u;
    list->edges[list->size].cost = cost;
    list->edges[list->size].tim = tim;
    list->size++;
}


//   SOLVE INSTANCE
// Resolve todas as consultas agrupando por origem 
char **solve_instance(int n, int m, EdgeList incoming[], Query queries[], int k, int inst_num) {

    // Lista de respostas
    char **answers = malloc(k * sizeof(char*));

    // Agrupar consultas por origem
    int *orig_count = calloc(n+1, sizeof(int));
    for (int i = 0; i < k; i++) {
        orig_count[queries[i].s]++;
    }

    // Para cada origem, cria lista de índices de consultas
    int **by_origin = malloc((n+1)*sizeof(int*));
    for (int i = 1; i <= n; i++) {
        if (orig_count[i] > 0)
            by_origin[i] = malloc(orig_count[i] * sizeof(int));
        else
            by_origin[i] = NULL;
    }

    // Preencher listas
    int *pos = calloc(n+1, sizeof(int));
    for (int i = 0; i < k; i++) {
        int s = queries[i].s;
        by_origin[s][pos[s]++] = i;
    }

    // Processar cada origem
    for (int s = 1; s <= n; s++) {
        if (orig_count[s] == 0)
            continue;

        // Descobre maior h dessa origem
        int h_max = 0;
        for (int j = 0; j < orig_count[s]; j++) {
            int qi = by_origin[s][j];
            if (queries[qi].h > h_max)
                h_max = queries[qi].h;
        }

        // Criar DP
        long long **dp = malloc((h_max + 1) * sizeof(long long*));
        for (int t = 0; t <= h_max; t++) {
            dp[t] = malloc((n + 1) * sizeof(long long));
            for (int v = 1; v <= n; v++)
                dp[t][v] = INF;
        }
        dp[0][s] = 0;

        // Preencher DP
        for (int t = 1; t <= h_max; t++) {

            // copiar dp[t-1] → dp[t]
            for (int v = 1; v <= n; v++)
                dp[t][v] = dp[t-1][v];

            // relaxar arestas
            for (int v = 1; v <= n; v++) {
                long long best = dp[t][v];

                for (int e = 0; e < incoming[v].size; e++) {
                    int u = incoming[v].edges[e].u;
                    int cost = incoming[v].edges[e].cost;
                    int tim = incoming[v].edges[e].tim;

                    if (tim <= t) {
                        long long cand = dp[t - tim][u];
                        if (cand != INF && cand + cost < best)
                            best = cand + cost;
                    }
                }
                dp[t][v] = best;
            }
        }

        // Responder consultas dessa origem
        for (int j = 0; j < orig_count[s]; j++) {
            int qi = by_origin[s][j];
            int dest = queries[qi].d;
            int h = queries[qi].h;

            long long best = INF;
            int best_t = -1;

            for (int t = 0; t <= h; t++) {
                if (dp[t][dest] < best) {
                    best = dp[t][dest];
                    best_t = t;
                }
            }

            answers[qi] = malloc(50);

            if (best == INF)
                sprintf(answers[qi], "Impossivel");
            else
                sprintf(answers[qi], "Possivel - %lld km, %d min", best, best_t);
        }

        // Free do DP
        for (int t = 0; t <= h_max; t++)
            free(dp[t]);
        free(dp);
    }

    free(orig_count);
    free(pos);
    free(by_origin);

    return answers;
}

//   MAIN
int main() {
    int n, m;
    int inst_num = 1;
    int first = 1;

    while (1) {
        if (scanf("%d %d", &n, &m) != 2)
            break;
        if (n == 0)
            break;

        // Criar listas de incoming
        EdgeList *incoming = malloc((n+1) * sizeof(EdgeList));
        for (int i = 1; i <= n; i++) {
            incoming[i].size = 0;
            incoming[i].capacity = 4;
            incoming[i].edges = malloc(4 * sizeof(IncomingEdge));
        }

        // Ler arestas
        for (int i = 0; i < m; i++) {
            int x, y, c, t;
            scanf("%d %d %d %d", &x, &y, &c, &t);
            add_edge(&incoming[y], x, c, t);
        }

        // Ler consultas
        int k;
        scanf("%d", &k);
        Query *queries = malloc(k * sizeof(Query));

        for (int i = 0; i < k; i++)
            scanf("%d %d %d", &queries[i].s, &queries[i].d, &queries[i].h);

        // Resolver instância
        char **answers = solve_instance(n, m, incoming, queries, k, inst_num);

        // Imprimir com linha em branco entre instâncias
        if (!first)
            printf("\n");
        first = 0;

        printf("Instancia %d\n", inst_num++);

        for (int i = 0; i < k; i++)
            printf("%s\n", answers[i]);

        // Liberar memória
        for (int i = 1; i <= n; i++)
            free(incoming[i].edges);
        free(incoming);
        for (int i = 0; i < k; i++)
            free(answers[i]);
        free(answers);
        free(queries);
    }

    return 0;
}
