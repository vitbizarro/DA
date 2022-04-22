#include "header2.h" //Header com a definição da estruturas CARRINHA e ENCOMENDA, bem como das funções secundárias

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
    std::vector<encomenda> volumes;
    std::vector<encomenda> pesos;
    std::vector<encomenda> rentaveis;
    std::vector<int> estado;
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
    int i = 0;
    while(!my_encomendas.eof()){
        my_encomendas >> volume;
        my_encomendas >> peso;
        my_encomendas >> recompensa;
        my_encomendas >> duracao;
        volumes.push_back({volume, peso, recompensa, duracao, i});
        pesos.push_back({volume, peso, recompensa, duracao, i});
        rentaveis.push_back({volume, peso, recompensa, duracao, i});
        estado.push_back(0);
        i++;
    }

    //Procedimentos para o CENÁRIO 1
    if(cenario == 1){

        //Ordenar as carrinhas por ordem DECRESCENTE de tamanho
        sort(carrinhas.begin(), carrinhas.end(), compareCarrinhas);
        //Ordenar as encomendas por ordem CRESCENTE de volume
        sort(volumes.begin(), volumes.end(), compareEncomendasVolume);
        //Ordenar as encomendas por ordem CRESCENTE de peso
        sort(pesos.begin(), pesos.end(), compareEncomendasPeso);

        //Declaração de variáveis para acompanhar o número de pedidos e de estafetas, bem como o andamento das carrinhas
        int estafetas = 1;
        int pedidos = 0;
        auto itrvol = volumes.begin();
        auto itrpes = pesos.begin();

        //Iterar todas as carrinhas
        for(auto itrcar = carrinhas.begin(); itrcar != carrinhas.end(); itrcar++){

            //Caso as encomendas já tenham chegado ao fim, acabar o ciclo
            if(itrpes == pesos.end() || itrvol == volumes.end()){
                break;
            }

                //Avaliar se a carrinha tem mais volume ou peso
                if ((*itrcar).volMax >= (*itrcar).pesoMax) {
                    //Avaliar se a carrinha consegue aceitar a encomendar de menor peso
                    if (!aceitaEncomendas(itrcar, pesos, itrpes, estado, pedidos)) {
                        //Avaliar se a carrinhas consegue aceitar a encomenda de menor volume caso, não consiga a de menor peso
                        if (!aceitaEncomendas(itrcar, volumes, itrvol, estado, pedidos)) {
                            itrcar++;
                            //Caso ainda haja carrinhas, passar o pedido para o próximo
                            if (itrcar != carrinhas.end()) estafetas++;
                            itrcar--;
                        }
                    }
                }
                else {
                    //Avaliar se a carrinha consegue aceitar a encomendar de menor volume
                    if (!aceitaEncomendas(itrcar, volumes, itrvol, estado, pedidos)) {
                        //Avaliar se a carrinhas consegue aceitar a encomenda de menor volume caso, não consiga a de menor peso
                        if (!aceitaEncomendas(itrcar, pesos, itrpes, estado, pedidos)) {
                            itrcar++;
                            //Caso ainda haja carrinhas, passar o pedido para o próximo
                            if (itrcar != carrinhas.end()) estafetas++;
                            itrcar--;
                        }
                    }
                }
                //Atualizar o iterador do vetor-prioridade dos pesos até que apareça uma encomenda que não foi aceite
                while (itrpes != pesos.end() && estado[(*itrpes).cod] == 1){
                    itrpes++;
                }
                //Atualizar o iterador do vetor-prioridade dos pesos até que apareça uma encomenda que não foi aceite
                while (itrvol != volumes.end() && estado[(*itrvol).cod] == 1){
                    itrvol++;
                }
        }
        //Gerar os resultados
        if (pedidos == 0) estafetas = 0;
        cout << "Etafetas: " << estafetas << endl;
        cout << "Pedidos: " << pedidos << endl;
    }
    else if (cenario == 2){

        //Ordenar as carrinhas por ordem DECRESCENTE de tamanho
        sort(carrinhas.begin(), carrinhas.end(), compareCarrinhas2);
        //Ordenar as encomendas por ordem CRESCENTE de volume
        sort(volumes.begin(), volumes.end(), compareEncomendasVolume);
        //Ordenar as encomendas por ordem CRESCENTE de peso
        sort(pesos.begin(), pesos.end(), compareEncomendasPeso);
        //Ordenar as encomendas por ordem DECRESCENTE de rentabilidade
        sort(rentaveis.begin(), rentaveis.end(), compareEncomendasRentabilidade);

        //Declaração de variáveis para acompanhar o lucro e os custos, bem como o andamento das carrinhas
        int receita = 0;
        int receitadiaria = 0;
        int custo = (*carrinhas.begin()).custo;
        int pedidos = 0;
        auto itrvol = volumes.begin();
        auto itrpes = pesos.begin();
        auto itrren = rentaveis.begin();

        //Iterar todas as carrinhas
        for(auto itrcar = carrinhas.begin(); itrcar != carrinhas.end(); itrcar++){

            //Caso as encomendas já tenham chegado ao fim, acabar o ciclo
            if(itrpes == pesos.end() && itrvol == volumes.end() && itrren == rentaveis.end()){
                if (avaliarRentabilidade(itrcar, receitadiaria, receita, custo)) {}
                break;
            }

            if (aceitaEncomendas2(itrcar, rentaveis, itrren, estado, pedidos, receitadiaria)) {}

            //Avaliar se a carrinha tem mais volume ou peso
            else if ((*itrcar).volMax >= (*itrcar).pesoMax) {
                //Avaliar se a carrinha consegue AINDA aceitar a encomendar de menor peso
                if (!aceitaEncomendas2(itrcar, pesos, itrpes, estado, pedidos, receitadiaria)) {
                    //Avaliar se a carrinhas consegue AINDA aceitar a encomenda de menor volume caso, não consiga a de menor peso
                    if (!aceitaEncomendas2(itrcar, volumes, itrvol, estado, pedidos, receitadiaria)) {
                        if (avaliarRentabilidade(itrcar, receitadiaria, receita, custo)) break;
                        itrcar++;
                        //Caso ainda haja carrinhas, passar o pedido para o próximo
                        if (itrcar != carrinhas.end()) custo += (*itrcar).custo;
                        itrcar--;
                    }
                }
            }
            else {
                //Avaliar se a carrinha consegue AINDA aceitar a encomendar de menor volume
                if (!aceitaEncomendas2(itrcar, volumes, itrvol, estado, pedidos, receitadiaria)) {
                    //Avaliar se a carrinhas consegue AINDA aceitar a encomenda de menor volume caso, não consiga a de menor peso
                    if (!aceitaEncomendas2(itrcar, pesos, itrpes, estado, pedidos, receitadiaria)) {
                        if (avaliarRentabilidade(itrcar, receitadiaria, receita, custo)) break;
                        itrcar++;
                        //Caso ainda haja carrinhas, passar o pedido para o próximo
                        if (itrcar != carrinhas.end()) custo += (*itrcar).custo;
                        itrcar--;
                    }
                }
            }
            //Atualizar o iterador do vetor-prioridade dos pesos até que apareça uma encomenda que não foi aceite
            while (itrpes != pesos.end() && estado[(*itrpes).cod] == 1){
                itrpes++;
            }
            //Atualizar o iterador do vetor-prioridade dos pesos até que apareça uma encomenda que não foi aceite
            while (itrvol != volumes.end() && estado[(*itrvol).cod] == 1){
                itrvol++;
            }
            //Atualizar o iterador do vetor-prioridade dos pesos até que apareça uma encomenda que não foi aceite
            while (itrren != rentaveis.end() && estado[(*itrren).cod] == 1){
                itrren++;
            }
        }
        //Gerar os resultados
        cout << "Lucro: " << receita-custo << endl;
    }

    return 0;
}

bool compareCarrinhas(carrinha c1, carrinha c2){
    return c1.tamanhoMax > c2.tamanhoMax;
}

bool compareCarrinhas2(carrinha c1, carrinha c2){
    return (c1.tamanhoMax/c1.custo) > (c2.tamanhoMax/c2.custo);
}

bool compareEncomendasVolume(encomenda e1, encomenda e2){
    return e1.volume < e2.volume;
}

bool compareEncomendasPeso(encomenda e1, encomenda e2){
    return e1.peso < e2.peso;
}

bool compareEncomendasRentabilidade(encomenda e1, encomenda e2){
    return (e1.recompensa/e1.peso+e1.volume) > (e2.recompensa/e2.peso+e2.volume);
}

//Consoante a prioridade de peso/volume, avaliar se a encomenda pode ser recolhida e atualizar os valores das carrinhas e dos pedidos
bool aceitaEncomendas(vector<carrinha>::iterator &itrcarrinhas, vector<encomenda> &prioridades, vector<encomenda>::iterator &itrprioridades, vector<int> &estado, int &pedidos){
    //Generalização da função em decorrência de duas prioridades, peso e volume, que se alteram quando preciso
    if (itrprioridades != prioridades.end() && (*itrcarrinhas).volMax >= (*itrprioridades).volume && (*itrcarrinhas).pesoMax >= (*itrprioridades).peso &&
        estado[(*itrprioridades).cod] == 0) {
        (*itrcarrinhas).volMax -= (*itrprioridades).volume;
        (*itrcarrinhas).pesoMax -= (*itrprioridades).peso;
        estado[(*itrprioridades).cod] = 1;
        pedidos++;
        itrcarrinhas--;
        return true;
    }
    return false;
}

//Consoante a prioridade de peso/volume, avaliar se a encomenda pode ser recolhida e atualizar os valores das carrinhas e dos pedidos
bool aceitaEncomendas2(vector<carrinha>::iterator &itrcarrinhas, vector<encomenda> &prioridades, vector<encomenda>::iterator &itrprioridades, vector<int> &estado, int &pedidos, int &lucro){
    //Generalização da função em decorrência de duas prioridades, peso e volume, que se alteram quando preciso
    if (itrprioridades != prioridades.end() && (*itrcarrinhas).volMax >= (*itrprioridades).volume && (*itrcarrinhas).pesoMax >= (*itrprioridades).peso &&
        estado[(*itrprioridades).cod] == 0) {
        (*itrcarrinhas).volMax -= (*itrprioridades).volume;
        (*itrcarrinhas).pesoMax -= (*itrprioridades).peso;
        estado[(*itrprioridades).cod] = 1;
        pedidos++;
        lucro += (*itrprioridades).recompensa;
        itrcarrinhas--;
        return true;
    }
    return false;
}

//Avaliar se a carrinha atual é rentável ou se não deve ser usada
bool avaliarRentabilidade(vector<carrinha>::iterator &itrcar, int &receitadiaria, int &receita, int &custo){
    if(receitadiaria <= (*itrcar).custo){
        custo -= (*itrcar).custo;
        return true;
    }
    receita += receitadiaria;
    receitadiaria = 0;
    return false;
}