#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

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
    int cod;
};

//Declaração da função de comparação para ordenação das carrinhas
bool compareCarrinhas(carrinha c1, carrinha c2);

//Declaração da função de comparação para ordenação das encomendas consoante a prioridade
bool compareEncomendasVolume(encomenda e1, encomenda e2);
bool compareEncomendasPeso(encomenda e1, encomenda e2);
bool compareEncomendasRentabilidade(encomenda e1, encomenda e2);

//Consoante a prioridade de peso/volume, avaliar se a encomenda pode ser recolhida e atualizar os valores das carrinhas e dos pedidos
bool aceitaEncomendas(vector<carrinha>::iterator &itrcarrinhas, vector<encomenda> &prioridades, vector<encomenda>::iterator &itrprioridades, vector<int> &estado, int &pedidos);
bool aceitaEncomendas2(vector<carrinha>::iterator &itrcarrinhas, vector<encomenda> &prioridades, vector<encomenda>::iterator &itrprioridades, vector<int> &estado, int &pedidos, int &lucro);

//Declaração da função de comparação para ordenação das carrinhas para o cenário 2
bool compareCarrinhas2(carrinha c1, carrinha c2);

bool compareEncomendasDuracao(encomenda e1, encomenda e2);

//Declaração da função pra avaliar a rentabilidade de cada carrinha
bool avaliarRentabilidade(vector<carrinha>::iterator &itrcar, int &receitadiaria, int &receita, int &custo);