# Caminho-Minimo-com-Restricao-de-Tempo-usando-Programacao-Dinamica (Bellman-Ford Variant)
Este projeto implementa uma solução para um problema de otimização em grafos direcionados, no qual o objetivo é encontrar o caminho de menor distância entre duas cidades respeitando uma restrição de tempo máximo.

Diferente do problema clássico de caminho mínimo, esta abordagem considera duas métricas simultaneamente:

- Minimização da distância total percorrida
- Restrição no tempo total de trajeto

Para resolver esse desafio, foi utilizada uma adaptação do algoritmo de Bellman-Ford combinada com Programação Dinâmica, permitindo explorar o espaço de soluções considerando diferentes limites de tempo.

## Como funciona

O problema é modelado como um grafo direcionado, onde cada aresta possui:

- Distância (custo)
- Tempo de deslocamento

A solução utiliza uma abordagem baseada em estados, onde cada posição representa o menor custo possível para alcançar um vértice dentro de um determinado tempo.

O algoritmo:

- Inicializa os estados com valores infinitos
- Atualiza iterativamente as soluções com base nas arestas do grafo
- Avalia todas as possibilidades dentro do limite de tempo definido
- Seleciona a melhor solução válida

Caso não exista caminho que satisfaça a restrição, o algoritmo identifica corretamente a impossibilidade da entrega.

## Saída do algoritmo

Para cada consulta, o programa retorna:

- Possível: quando existe um caminho válido
- Distância mínima encontrada
- Tempo correspondente
- Impossível: quando não há solução dentro do limite de tempo

Em casos de empate na distância, é escolhido o caminho com menor tempo.

## Tecnologias utilizadas
- C
- Programação Dinâmica
- Algoritmos em grafos
- Variação do Bellman-Ford
