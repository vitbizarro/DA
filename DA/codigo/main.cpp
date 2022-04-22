#include "header.h" //Header com a definição da estruturas CARRINHA e ENCOMENDA, bem como das funções secundárias

/// <summary>
/// Função principal do programa, recebe 3 argumentos: cenário, carrinhas e encomenda.
/// </summary>
int main(int argc, char *argv[]) {

    //Restringir o mau uso
    /// <param name="argc">Contagem de argumentos</param>
    if (argc != 4){
        cout << "ERROR! USAGE: *cenario*(1,2,3) *carrinhas.txt* *encomendas.txt*" << endl;
        return 1;
    }

    //Leitura dos respectivos cenários
    int cenario = stoi(argv[1]);

    //Declarar variáveis para leitura das carrinhas
    std::vector<carrinha> carrinhas;
    int volMax;
    int pesoMax;
    int custo;

    //Abertura para leitura de file das carrinhas
    fstream my_carrinhas;
    /// <param name="argv">Terceiro argumento (carrinhas)</param>
    my_carrinhas.open(argv[2], ios::in);

    //Testar se abriu corretamente
    if (!my_carrinhas) {
        cout << "File error!" << endl;
        return 1;
    }

    //Leitura das carrinhas a serem inseridas no vetor. Para a inserção, uma lista poderia sem mais eficiente. Entretanto, uma vez que esta estrutura será usada constantemente para índices específicos, optou-se por um vetor.
    /// <summary>
    /// Leitura do valor de cada coluna do ficheiro das carrinhas
    /// </summary>
    while(!my_carrinhas.eof()){
        my_carrinhas >> volMax;
        my_carrinhas >> pesoMax;
        my_carrinhas >> custo;
        //Um pushback é mais eficiente do que um simples insert pois não tem de "puxar" todos os outros já alocados e a ordem é indiferente pois voltarão a ser reorddenados.
        carrinhas.push_back({volMax, pesoMax, custo, volMax+pesoMax});
    }

    //Declarar variáveis para leitura das encomendas
    std::vector<encomenda> volumes; //Vetor de todas as encomendas a serem ordenadas pelo volume
    std::vector<encomenda> pesos; //Vetor de todas as encomendas a serem ordenadas pelo peso
    std::vector<encomenda> rentaveis; //Vetor de todas as encomendas a serem ordenadas pela rentabilidade (tamanho/recompensa)
    std::vector<int> estado; //Vetor para registar todas as encomendas já registadas
    int volume;
    int peso;
    int recompensa;
    int duracao;

    //Abertura para leitura de file das encomendas
    fstream my_encomendas;
    my_encomendas.open(argv[3], ios::in);

    //Testar se abriu corretamente
    if (!my_encomendas) {
        cout << "File error!" << endl;
        return 1;
    }

    //Leitura das carrinhas a serem inseridas no vetor. Para a inserção, uma lista poderia sem mais eficiente. Entretanto, uma vez que esta estrutura será usada constantemente para índices específicos, optou-se por um vetor.
    /// <summary>
    /// Leitura do valor de cada coluna do ficheiro das encomendas
    /// </summary>
    int i = 0;
    while(!my_encomendas.eof()){
        my_encomendas >> volume;
        my_encomendas >> peso;
        my_encomendas >> recompensa;
        my_encomendas >> duracao;
        //Adicionar as encomendas com os respetivos valores e com o i como código
        volumes.push_back({volume, peso, recompensa, duracao, i});
        pesos.push_back({volume, peso, recompensa, duracao, i});
        rentaveis.push_back({volume, peso, recompensa, duracao, i});
        //Iniciar os valores de estados das encomendas a 0
        estado.push_back(0);
        i++;
    }

    //Procedimentos para o CENÁRIO 1
    /// <summary>
    /// Execução do cenário 1 para a minimização do número de carrinhas para o máximo de encomendas
    /// </summary>
    if(cenario == 1){

        //Ordenar as carrinhas por ordem DECRESCENTE de tamanho
        sort(carrinhas.begin(), carrinhas.end(), compareCarrinhas);
        //Ordenar as encomendas por ordem CRESCENTE de volume
        sort(volumes.begin(), volumes.end(), compareEncomendasVolume);
        //Ordenar as encomendas por ordem CRESCENTE de peso
        sort(pesos.begin(), pesos.end(), compareEncomendasPeso);

        //Declaração de variáveis para acompanhar o número de pedidos e de estafetas, bem como o andamento das carrinhas
        int estafetas = 1; //O número de estafetas começa a 1, mas caso não haja pedidos ele é zerado ao fim
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
                /// <param name="itrcar">Iterador das carrinhas</param>
                /// <param name="pesos">Vector ordenado pelo peso</param>
                /// <param name="itrpes">Vector ordenado pelo peso</param>
                /// <param name="estado">Vector das encomendas já aceites</param>
                /// <param name="pedidos">Inteiro de pedidos aceites</param>
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
                /// <param name="itrcar">Iterador das carrinhas</param>
                /// <param name="volumes">Vector ordenado pelo volume</param>
                /// <param name="itrvol">Iterador dos volumes</param>
                /// <param name="estado">Vector das encomendas já aceites (0 NÃO VISITADO, 1 VISITADO)</param>
                /// <param name="pedidos">Inteiro de pedidos aceites</param>
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
            //Atualizar o iterador do vetor-prioridade dos volumes até que apareça uma encomenda que não foi aceite
            while (itrvol != volumes.end() && estado[(*itrvol).cod] == 1){
                itrvol++;
            }
        }
        //Ajustar o caso-base, uma vez que o algoritmo não funciona para ele
        if (pedidos == 0) estafetas = 0;
        //Gerar os resultados
        cout << "Etafetas: " << estafetas << endl;
        cout << "Pedidos: " << pedidos << endl;
    }
    //Procedimentos para o CENÁRIO 2
    /// <summary>
    /// Execução do cenário 2 para a maximização do lucro, independente do número de pedidos aceites ou de carrinhas usados
    /// </summary>
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
        int receita = 0; //Total de receita
        int receitadiaria = 0; //Receita do dia em questão
        int custo = (*carrinhas.begin()).custo;
        int pedidos = 0;
        auto itrvol = volumes.begin();
        auto itrpes = pesos.begin();
        auto itrren = rentaveis.begin();

        //Iterar todas as carrinhas
        for(auto itrcar = carrinhas.begin(); itrcar != carrinhas.end(); itrcar++){

            //Caso as encomendas já tenham chegado ao fim, acabar o ciclo
            if(itrpes == pesos.end() && itrvol == volumes.end() && itrren == rentaveis.end()){
                //Avaliar antes de encerrar se a carrinha em questão faz valer a pena ser adicionada ou não
                /// <param name="itrcar">Iterador das carrinhas</param>
                /// <param name="receitadiaria">Referễncia de inteiro para o valor diário de receite (é sempre dado reset a 0)</param>
                /// <param name="receita">Soma das receitas (no final de cada interação com a receitadiaria)/param>
                /// <param name="custo">Referência de inteiro para o custo da carrinha/param>
                if (avaliarRentabilidade(itrcar, receitadiaria, receita, custo)) {}
                break;
            }

            if (aceitaEncomendas2(itrcar, rentaveis, itrren, estado, pedidos, receitadiaria)) {}

            //Avaliar se a carrinha tem mais volume ou peso
            else if ((*itrcar).volMax >= (*itrcar).pesoMax) {
                //Avaliar se a carrinha consegue AINDA aceitar a encomendar de menor peso
                /// <param name="itrcar">Iterador das carrinhas</param>
                /// <param name="pesos">Vector ordenado pelo peso</param>
                /// <param name="itrpes">Iterador dos pesos</param>
                /// <param name="estado">Vector das encomendas já aceites (0 NÃO VISITADO, 1 VISITADO)</param>
                /// <param name="pedidos">Inteiro de pedidos aceites</param>
                /// <param name="receitadiaria">Referễncia de inteiro para o valor diário de receite (é sempre dado reset a 0)</param>
                if (!aceitaEncomendas2(itrcar, pesos, itrpes, estado, pedidos, receitadiaria)) {
                    //Avaliar se a carrinhas consegue AINDA aceitar a encomenda de menor volume caso, não consiga a de menor peso
                    if (!aceitaEncomendas2(itrcar, volumes, itrvol, estado, pedidos, receitadiaria)) {
                        //Avaliar a necessidade de encerrar se a carrinha em questão faz valer a pena ser adicionada ou não (subentende-se que se esta não valer a pena, mais nenhuma vai)
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
                /// <param name="itrcar">Iterador das carrinhas</param>
                /// <param name="volumes">Vector ordenado pelo volume</param>
                /// <param name="itrvol">Iterador dos volumes</param>
                /// <param name="estado">Vector das encomendas já aceites (0 NÃO VISITADO, 1 VISITADO)</param>
                /// <param name="pedidos">Inteiro de pedidos aceites</param>
                /// <param name="receitadiaria">Referễncia de inteiro para o valor diário de receite (é sempre dado reset a 0)</param>
                if (!aceitaEncomendas2(itrcar, volumes, itrvol, estado, pedidos, receitadiaria)) {
                    //Avaliar se a carrinhas consegue AINDA aceitar a encomenda de menor volume caso, não consiga a de menor peso
                    if (!aceitaEncomendas2(itrcar, pesos, itrpes, estado, pedidos, receitadiaria)) {
                        //Avaliar a necessidade de encerrar se a carrinha em questão faz valer a pena ser adicionada ou não (subentende-se que se esta não valer a pena, mais nenhuma vai)
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
            //Atualizar o iterador do vetor-prioridade dos volumes até que apareça uma encomenda que não foi aceite
            while (itrvol != volumes.end() && estado[(*itrvol).cod] == 1){
                itrvol++;
            }
            //Atualizar o iterador do vetor-prioridade dos rentáveis até que apareça uma encomenda que não foi aceite
            while (itrren != rentaveis.end() && estado[(*itrren).cod] == 1){
                itrren++;
            }
        }
        //Gerar os resultados
        cout << "Lucro: " << receita-custo << endl;
    }

    //Procedimentos para o CENÁRIO 3
    /// <summary>
    /// Execução do cenário 3 para a otimização do tempo médio de entregas-expresso, independente do peso e do volume
    /// </summary>
    else {
        //Ordenar consoante as encomendas de menor duração
        sort(volumes.begin(), volumes.end(), compareEncomendasDuracao);

        int timeleft = 28800; //Tempo hábil total em segundos (das 09 às 17)
        int counter = 0; //Contagem de pedidos

        //Iterar todas as encomendas
        for(int i = 0; i < volumes.size(); i++) {
            timeleft -= volumes.at(i).duracao; //Subtrair o tempo da próxima encomenda do tempo total disponível
            counter += 1; //Aumentar o número de pedidos total

            if (timeleft <= 0) {
                //Caso a encomenda em questão não puder mais ser levada, reverter a ação e encerrar o ciclo
                timeleft += volumes.at(i).duracao;
                counter -= 1;
                break;
            }
        }

        //Gerar a média do tempo total gasto por encomenda
        cout << "Tempo médio: " << (float)(28800-timeleft)/counter << endl;
        cout << "Contagem: " << counter << endl;

    }

    return 0;
}

/// <summary>
/// Função para ordenar as carrinhas em consonância com o tamanho máximo maior
/// </summary>
bool compareCarrinhas(carrinha c1, carrinha c2){
    return c1.tamanhoMax > c2.tamanhoMax;
}

/// <summary>
/// Função para ordenar as carrinhas do CENÁRIO 2, em consonância com o maior tamanho máximo para um menor custo
/// </summary>
bool compareCarrinhas2(carrinha c1, carrinha c2){
    return (c1.tamanhoMax/c1.custo) > (c2.tamanhoMax/c2.custo);
}

/// <summary>
/// Função para ordenar as encomendas em consonância com o menor volume
/// </summary>
bool compareEncomendasVolume(encomenda e1, encomenda e2){
    return e1.volume < e2.volume;
}

/// <summary>
/// Função para ordenar as encomendas em consonância com o menor peso
/// </summary>
bool compareEncomendasPeso(encomenda e1, encomenda e2){
    return e1.peso < e2.peso;
}

/// <summary>
/// Função para ordenar as encomendas em consonância com a rentabilidade, ou seja a maior recompensa para um menor tamanho
/// </summary>
bool compareEncomendasRentabilidade(encomenda e1, encomenda e2){
    return (e1.recompensa/e1.peso+e1.volume) > (e2.recompensa/e2.peso+e2.volume);
}

/// <summary>
/// Função para ordenar as encomendas em consonância com a menor duração
/// </summary>
bool compareEncomendasDuracao(encomenda e1, encomenda e2) {
    return e1.duracao< e2.duracao;
}

//Consoante a prioridade de peso/volume, avaliar se a encomenda pode ser recolhida e atualizar os valores das carrinhas e dos pedidos
/// <summary>
/// Função para retornar TRUE caso a encomenda for recolhida e os valores atualizados e FALSE para o contracaso
/// </summary>
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
/// <summary>
/// Extensão da função anterior que, para o caso TRUE, em que a encomenda é recolhida, leva-se em consideração o lucro
/// </summary>
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
/// <summary>
/// Função para processar a soma da receita diária com a receita total para o caso em que a receitaé rentável
/// </summary>
bool avaliarRentabilidade(vector<carrinha>::iterator &itrcar, int &receitadiaria, int &receita, int &custo){
    if(receitadiaria <= (*itrcar).custo){
        custo -= (*itrcar).custo;
        return true;
    }
    receita += receitadiaria;
    receitadiaria = 0;
    return false;
}