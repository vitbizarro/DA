//Declaração de estrutura para as carrinhas
struct carrinha{
    int volMax;
    int pesoMax;
    int custo;
    int tamanhoMax;
};

//Declaração de estrutura para as encomendas
struct encomenda{
    int volume;
    int peso;
    int recompensa;
    int duracao;
    int tamanho;
};

//Declaração da função de comparação para ordenação das carrinhas
bool compareCarrinhas(carrinha c1, carrinha c2);

//Declaração da função de comparação para ordenação das encomendas
bool compareEncomendas(encomenda e1, encomenda e2);