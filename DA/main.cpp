#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "header.h" //Header com a definição da estruturas CARRINHA e ENCOMENDA, bem como das funções secundárias

using namespace std;

int main(int argc, char *argv[]) {

    //Leitura do cenário
    int cenario = stoi(argv[1]);

    //Declarar variáveis para leitura das carrinhas
    std::vector<carrinha> carrinhas;
    int volMax;
    int pesoMax;
    int custo;

    //Abertura para leitura de file das carrinhas
    fstream my_carrinhas;
    my_carrinhas.open(argv[2], ios::in);

    //Testar se abriu corretamente
    if (!my_carrinhas) {
        cout << "File error!";
        return 1;
    }

    //Leitura das carrinhas PARA INSERIR NO VECTOR, UM PUSH_BACK É MAIS EFICIENTE QUE UM INSERT POIS NÃO PRECISA MOVER TODOS OS OBJETOS JÁ ALOCADOS
    while(!my_carrinhas.eof()){
        my_carrinhas >> volMax;
        my_carrinhas >> pesoMax;
        my_carrinhas >> custo;
        carrinhas.push_back({volMax, pesoMax, custo, volMax+pesoMax});
    }

    /*
    //Teste leitura carrinhas
    for (auto itr = carrinhas.begin(); itr != carrinhas.end(); itr++) {
        cout << (*itr).volMax << " " << (*itr).pesoMax << " " << (*itr).custo << " " << (*itr).tamanhoMax << endl;
    }
     */

    //Declarar variáveis para leitura das encomendas
    std::vector<encomenda> encomendas;
    int volume;
    int peso;
    int recompensa;
    int duracao;

    //Abertura para leitura de file das encomendas
    fstream my_encomendas;
    my_encomendas.open(argv[3], ios::in);

    //Testar se abriu corretamente
    if (!my_encomendas) {
        cout << "File error!";
        return 1;
    }

    //Leitura das encomendas PARA INSERIR NO VECTOR, UM PUSH_BACK É MAIS EFICIENTE QUE UM INSERT POIS NÃO PRECISA MOVER TODOS OS OBJETOS JÁ ALOCADOS
    while(!my_encomendas.eof()){
        my_encomendas >> volume;
        my_encomendas >> peso;
        my_encomendas >> recompensa;
        my_encomendas >> duracao;
        encomendas.push_back({volume, peso, recompensa, duracao, volume+peso});
    }

    /*
    //Teste leitura encomendas
    for (auto itr = encomendas.begin(); itr != encomendas.end(); itr++) {
        cout << (*itr).volume << " " << (*itr).peso << " " << (*itr).recompensa << " " << (*itr).duracao << " " << (*itr).tamanho << endl;
    }
     */

    //Procedimentos para o CENÁRIO 1
    if(cenario == 1){
        //Declaração de variáveis para acompanhar o número de pedidos e de estafetas, bem como o andamento das carrinhas
        int estafetas = 1;
        int pedidos = 0;
        auto itrcar = carrinhas.begin();

        //Ordenar as carrinhas por ordem CRESCENTE de tamanho
        sort(carrinhas.begin(), carrinhas.end(), compareCarrinhas);
        //Ordenar as encomendas por ordem CRESCENTE de tamanho
        sort(encomendas.begin(), encomendas.end(), compareEncomendas);

        //Iterar todas as encomendas
        for(auto itrenc = encomendas.begin(); itrenc != encomendas.end(); itrenc++){

            //Avaliar se a carrinha consegue levar a encomenda em questão
            if ((*itrcar).volMax >= (*itrenc).volume && (*itrcar).pesoMax >= (*itrenc).peso){
                (*itrcar).volMax -= (*itrenc).volume;
                (*itrcar).pesoMax -= (*itrenc).peso;
                pedidos++;
            }
            //Caso não consiga, passá-la para a próxima carrinha
            else{
                estafetas++;
                itrcar++;
                //Caso já não haja carrinhas disponíveis, terminar a operação
                if (itrcar == carrinhas.end()){
                    break;
                }
                itrenc--;
            }
        }
        //Gerar os resultados
        cout << "Etafetas: " << estafetas << endl;
        cout << "Pedidos: " << pedidos << endl;
    }

    return 0;
}

bool compareCarrinhas(carrinha c1, carrinha c2){
    return c1.tamanhoMax > c2.tamanhoMax;
}

bool compareEncomendas(encomenda e1, encomenda e2){
    return e1.tamanho < e2.tamanho;
}
