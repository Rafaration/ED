#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

enum Cor {BRANCO, CINZA, PRETO};    

class Grafo 
{
    private:
        int numVertices;
        int numArestas;
        
        vector<vector<int>> ADJ;

        bool direcionado;

        bool conexo;

        bool ciclico;

    public:

        // Construtor do grafo
        // Recebe o Num de vértices
        Grafo(int vertices = 0, bool dir = false, bool con = true, bool cic = false) 
        {
            // Define o número de vértices
            numVertices = vertices;
            
            // Define o número de arestas
            numArestas = 0;
            
            // Redimenciona a lista de adjacencia para alojar os vértices
            ADJ.resize(vertices);

            // Define o grafo base como não direcionado por padrão
            direcionado = dir;
            
            // Como grafico vazio é conexo e acíclico...
            // Define o grafico base como conexo por padrão
            conexo = con;

            // Define o frafo base como acíclico por padrão
            ciclico = cic;
        }

        // Função para ler grafo de arquivo
        // Recebe o nome do arquivo de entrada
        bool lerDoArquivo(const string& nomeArquivo)
        {
            // Abre o arquivo para leitura
            ifstream arquivo(nomeArquivo);
            
            // Verifica se o arquivo foi aberto com sucesso
            if (!arquivo.is_open())
            {
                cout << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
                return false;
            }

            // Declara variáveis para armazenar os dados do arquivo
            int vertices, arestas;
            char tipo;
            
            // Lê número de vértices, arestas e tipo (D=direcionado, N=não direcionado)
            arquivo >> vertices >> arestas >> tipo;
            
            // Define o número de vértices do grafo
            numVertices = vertices;
            
            // Define se o grafo é direcionado ou não baseado no tipo lido
            direcionado = (tipo == 'D' || tipo == 'd');
            
            // Limpa a lista de adjacência anterior
            ADJ.clear();
            
            // Redimensiona a lista de adjacência para o novo número de vértices
            ADJ.resize(vertices);
            
            // Reinicia o contador de arestas
            numArestas = 0;

            // Lê todas as arestas do arquivo
            for (int i = 0; i < arestas; i++)
            {
                // Declara variáveis para armazenar os vértices da aresta
                int u, v;
                
                // Lê o vértice fonte (u) e o vértice destino (v)
                arquivo >> u >> v;
                
                // Adiciona a aresta ao grafo
                addVertice(u, v);
            }

            // Fecha o arquivo
            arquivo.close();
            
            // Retorna verdadeiro indicando sucesso na leitura
            return true;
        }

        // Função para adicionar um novo vértice
        // Recebe o vértice fonte e o vértice destino
        void addVertice(int fonte, int destino)
        {
            // Adiciona o vert destino a lista de ADJ do vert fonte
            ADJ[fonte].push_back(destino);
            
            // Incrementa contador de arestas
            numArestas++;

            // Caso o grafo for não direcionado 
            // Adiciona o vert fonte a lista de ADJ do vert destino
            if (!direcionado)
            {
                ADJ[destino].push_back(fonte);
            }
        }

        // Função para imprimir o grafo
        void printGrafo ()
        {
            // Imprime o cabeçalho da lista de adjacência
            cout << "Lista de Adjacência: " << endl; 

            // Para cada vértice i do grafo
            for (int i = 0; i < numVertices; i++)
            {
                // Imprime o índice do vértice
                cout << "\t[" << i << "] ="; 
                
                // Se a lista de adjacência do vértice i estiver vazia
                if (ADJ[i].empty())  
                {
                    cout << " ∅" << endl;  
                    continue;              
                }

                // Percorre os elementos da lista de adjacência do vértice i
                for (int j : ADJ[i])
                {
                    // Se for o último elemento da lista, imprime e quebra a linha
                    if (j == ADJ[i].back())
                        cout << " " << j << endl;
                    // Caso contrário, apenas imprime o elemento seguido de espaço
                    else
                        cout << " " << j;
                } 
            }
        }

        // Implementação do algoritmo BFS (Busca em largura) apenas para grafos conexos (simples) 
        // Recebe o vértice inicial/fonte S
        void BFS_simples(int S)
        {
            // Cria um vetor todo branco para armazenar as cores das vertices
            vector<Cor> cor(numVertices, BRANCO);

            queue<int> Q;      // Cria a fila inicial

            cor[S] = CINZA;    // Pinta a fonte de cinza
            Q.push(S);         // Enfileira a fonte

            while (!Q.empty())
            {
                int u = Q.front();  // u recebe o primeiro valor da fila
                Q.pop();            // Desenfileira o primeiro elemento que está armazenado em u

                // Passa pelos vertices adjasentes
                for (int v : ADJ[u])
                {
                    if (cor[v] == BRANCO)   // Ao visitar o vértice v, se ele for branco 
                    {
                        cor[v] = CINZA;    // Pinta ele de cinza
                        Q.push(v);          // Enfileira o vértice v
                    }
                }

                cor[u] = PRETO;  // Pinta o vértice u de preto
            }
        }

        // Implementação do algoritmo BFS (Busca em largura) para todo grafo
        void BFS() 
        {
            // Cria um vetor todo branco para armazenar as cores das vertices
            vector<Cor> cor(numVertices, BRANCO); 

            // Para cada vértice do grafo
            for (int i = 0; i < numVertices; i++)
            {
                if (cor[i] == BRANCO)   // Verifica se o vértice i está branco e...
                {
                    BFS_simples(i);     // Chama a BFS simples para cada componente conexa
                }
            }
        }

        // Função que verifica se o grafo é ciclico ou aciclico utilizando a lógica da BFS
        void testaCiclicoBFS()
        {
            // Marco todos os vértices como brancos (não visitados)
            vector<Cor> cor(numVertices, BRANCO);
        
            // Armazena os pais de cada vértice
            vector<int> pai(numVertices, -1);

            // Para cada vértice não descoberto do grafo
            for (int i = 0; i < numVertices; i++)
            {

                if (cor[i] == BRANCO)   // Se o vértice for branco (não descoberto)
                {
                    queue<int> Q;     // Cria a fila Q
                    Q.push(i);        // Enfileira o elemento i
                    cor[i] = CINZA;  // Pinta i como CINZA (elemento visitado mas não completo)

                    while (!Q.empty())   // Enquanto existem elementos na fila...
                    {
                        int u = Q.front();  // u recebe o primeiro valor da fila
                        Q.pop();            // Elimina o primeiro valor da fila, que agora está armezanado em u

                        // Percorre todos os vértices adj de u
                        for (int v : ADJ[u])
                        {
                            if (cor[v] == BRANCO)  // Se o vert for BRANCO (não descoberto) ...
                            {
                                cor[v] = CINZA;   // Pinta de Cinza (ja descoberto)
                                Q.push(v);         // Enfileiramos o vizinho v
                                pai[v] = u;        // Definimos u como pai de v
                            }

                            // Se o vert v já foi visitado (!BRANCO) &&&&& o pai de v não é u 
                            else if (v != pai[u]) 
                            {
                                ciclico =true; // Encontramos um cíclo!!!
                            }
                        }

                        // cor [u] = PRETO;  // Pinta u como PRETO (completo)
                    }
                }
            }

            // Não encontou nenhum ciclo e ainda assim visitou todos os vértices
            ciclico = false;
        }

        // Função auxiliar que imprime se o grafo é cíclico ou acíclico
        void ehCiclico()
        {
            // Chama a função que testa se o grafo é cíclico
            testaCiclicoBFS();

            // Se o grafo for cíclico
            if (ciclico)
                cout << "O grafo é ciclico!" << endl;
            // Caso contrário
            else
                cout << "O grafo é aciclico!" << endl;
        }

        // Função que verifica se o grafo é conexo utilizando a lógica da BFS
        // Recebe o vértice inicial S
        void testaConexoBFS(int S)
        {
            // Cria um vetor todo branco para armazenar as cores das vertices
            vector<Cor> cor(numVertices, BRANCO);

            queue<int> Q;      // Cria a fila inicial

            cor[S] = CINZA;    // Pinta a fonte de cinza
            Q.push(S);         // Enfileira a fonte

            while (!Q.empty())
            {
                int u = Q.front();  // u recebe o primeiro valor da fila
                Q.pop();            // Desenfileira o primeiro elemento que está armazenado em u

                // Passa pelos vertices adjasentes
                for (int v : ADJ[u])
                {
                    if (cor[v] == BRANCO)   // Ao visitar o vértice v, se ele for branco 
                    {
                        cor[v] = CINZA;    // Pinta ele de cinza
                        Q.push(v);          // Enfileira o vértice v
                    }
                }

                // cor[u] = PRETO;  // Pinta o vértice u de preto (completo)
            }

            // Para cada vértice do grafo
            for (int i = 0; i < numVertices; i++)
            {
                if (cor[i] == BRANCO)   // Verifica se existe algum vértice branco
                {
                    conexo = false;     // Se existir, o grafo é desconexo
                    return;
                }
            }
        }

        // Função auxiliar que imprime se o grafo é conexo ou desconexo
        void ehConexo()
        {
            // Chama a função que testa se o grafo é conexo a partir do vértice 0
            testaConexoBFS(0);
            
            // Se o grafo for conexo
            if (conexo)
                cout << "O grafo é conexo!" << endl;
            // Caso contrário
            else
                cout << "O grafo não é conexo" << endl;
        }

        // Função que busca uma componente conexa a partir de um vértice S
        // Recebe o vértice inicial S, o vetor de componentes e o vetor de cores
        void buscarComponentes(int S, vector<vector<int>> &componentes, vector<Cor> &cor)
        {
            queue<int> Q;  // Cria a fila Q

            cor[S] = CINZA;  // Pinta o vértice S de cinza
            Q.push(S);       // Enfileira o vértice S

            vector<int> componenteAtual;  // Cria um vetor para armazenar a componente atual

            // Enquanto a fila não estiver vazia
            while (!Q.empty())
            {
                int u = Q.front();  // u recebe o primeiro valor da fila
                Q.pop();            // Desenfileira o primeiro elemento

                componenteAtual.push_back(u);  // Adiciona u à componente atual

                // Percorre todos os vértices adjacentes de u
                for (int v : ADJ[u])
                {
                    if (cor[v] == BRANCO)  // Se o vértice v for branco
                    {
                        cor[v] = CINZA;  // Pinta v de cinza
                        Q.push(v);       // Enfileira v
                    }
                }

                // cor[u] = PRETO;  // Pinta u de preto (completo)
            }

            // Adiciona a componente atual ao vetor de componentes
            componentes.push_back(componenteAtual);
        }

        // Função que lista todas as componentes conexas do grafo
        void listarComponentesConexos()
        {
            // Cria um vetor para armazenar todas as componentes conexas
            vector<vector<int>> componentes;
            
            // Cria um vetor todo branco para armazenar as cores dos vértices
            vector<Cor> cor(numVertices, BRANCO);

            // Para cada vértice do grafo
            for (int i = 0; i < numVertices; i++)
            {
                if (cor[i] == BRANCO)  // Se o vértice i for branco
                {
                    // Busca a componente conexa a partir do vértice i
                    buscarComponentes(i, componentes, cor);
                }
            }

            // Imprime o número de componentes conexas
            cout << "Número de componentes conexas: " << componentes.size() << endl;
            
            // Para cada componente conexa
            for (size_t i = 0; i < componentes.size(); i++)
            {
                // Imprime o número da componente
                cout << "Componente " << i + 1 << ":";
                
                // Para cada vértice da componente
                for (int v : componentes[i])
                {
                    // Imprime o vértice
                    cout << " " << v;
                }
                cout << endl;
            }
        }

        // Função para calcular o grau de um vértice
        // Recebe o índice do vértice
        int calcularGrau(int vertice)
        {
            // Se o grafo for direcionado
            if (direcionado)
            {
                // Para grafos direcionados, soma grau de entrada + saída
                int grauSaida = ADJ[vertice].size();  // Grau de saída é o tamanho da lista de adjacência
                int grauEntrada = 0;  // Inicializa o grau de entrada
                
                // Percorre todos os vértices do grafo
                for (int i = 0; i < numVertices; i++)
                {
                    // Percorre a lista de adjacência do vértice i
                    for (int v : ADJ[i])
                    {
                        // Se o vértice v é igual ao vértice que estamos calculando
                        if (v == vertice)
                            grauEntrada++;  // Incrementa o grau de entrada
                    }
                }
                
                // Retorna a soma do grau de entrada e saída
                return grauSaida + grauEntrada;
            }
            else
            {
                // Para grafos não direcionados, o grau é o tamanho da lista de adjacência
                return ADJ[vertice].size();
            }
        }

        // Função para identificar vértices com alto grau (possíveis elos)
        void identificarElos()
        {
            // Se não há vértices no grafo
            if (numVertices == 0)
            {
                cout << "Não há vértices no grafo." << endl;
                return;
            }

            // Calcula o grau médio
            double grauTotal = 0;  // Inicializa a soma dos graus
            vector<pair<int, int>> graus; // Vetor de pares (vértice, grau)

            // Para cada vértice do grafo
            for (int i = 0; i < numVertices; i++)
            {
                // Calcula o grau do vértice i
                int grau = calcularGrau(i);
                
                // Adiciona o par (vértice, grau) ao vetor
                graus.push_back({i, grau});
                
                // Acumula o grau total
                grauTotal += grau;
            }

            // Calcula o grau médio dividindo o total pelo número de vértices
            double grauMedio = grauTotal / numVertices;

            // Ordena o vetor de graus em ordem decrescente
            sort(graus.begin(), graus.end(), 
                 [](const pair<int, int>& a, const pair<int, int>& b) {
                     return a.second > b.second;  // Ordena pelo grau (segundo elemento)
                 });

            // Identifica vértices com grau significativamente acima da média
            cout << "\nVértices com alto grau (possíveis elos entre grupos):" << endl;
            cout << "Grau médio: " << grauMedio << endl;

            // Flag para verificar se encontrou algum elo
            bool encontrouElos = false;
            
            // Para cada vértice no vetor ordenado
            for (auto& p : graus)
            {
                // Considera elo se:
                // O grau do vértice for 30% maior que o grau médio  
                if (p.second > grauMedio * 1.3)
                {
                    // Imprime o vértice e seu grau
                    cout << "  Vértice " << p.first << " com grau " << p.second << endl;
                    encontrouElos = true;  // Marca que encontrou pelo menos um elo
                }
            }

            // Se não encontrou nenhum vértice com alto grau
            if (!encontrouElos)
            {
                cout << "  Não foram identificados vértices com alto grau relativo." << endl;
            }
        }

        // Função para exibir saída completa conforme requisitos do trabalho
        void exibirAnaliseCompleta()
        {
            // Imprime o cabeçalho da análise
            cout << "\n=== ANÁLISE DO GRAFO SOCIAL ===" << endl;
            
            // Imprime o número de vértices
            cout << "\nGrafo com " << numVertices << " vértices" << endl;
            
            // Imprime o número de arestas (divide por 2 se não for direcionado)
            cout << "Número de arestas: " << (direcionado ? numArestas : numArestas/2) << endl;
            
            // Testa se o grafo é conexo a partir do vértice 0
            testaConexoBFS(0);
            
            // Imprime se é conexo ou não
            if (conexo)
                cout << "O grafo é conexo" << endl;
            else
                cout << "O grafo não é conexo" << endl;

            // Cria um vetor para armazenar todas as componentes
            vector<vector<int>> componentes;
            
            // Cria um vetor de cores para marcar vértices visitados
            vector<Cor> cor(numVertices, BRANCO);

            // Para cada vértice do grafo
            for (int i = 0; i < numVertices; i++)
            {
                if (cor[i] == BRANCO)  // Se o vértice não foi visitado
                {
                    // Busca a componente conexa a partir desse vértice
                    buscarComponentes(i, componentes, cor);
                }
            }

            // Imprime o número de componentes conexas
            cout << "Número de componentes conexas: " << componentes.size() << endl;
            
            // Para cada componente conexa
            for (size_t i = 0; i < componentes.size(); i++)
            {
                // Imprime o número da componente
                cout << "Componente " << i + 1 << ":";
                
                // Para cada vértice da componente
                for (int v : componentes[i])
                {
                    // Imprime o vértice
                    cout << " " << v;
                }
                cout << endl;
            }

            // Identifica e imprime os possíveis elos entre grupos
            identificarElos();
        }

        // Getter para número de arestas
        int getNumArestas()
        {
            // Retorna o número de arestas (divide por 2 se não for direcionado)
            return direcionado ? numArestas : numArestas / 2;
        }
};

int main ()
{
    // Cria um objeto grafo vazio
    Grafo g;

    // Lê o grafo do arquivo de entrada
    if (!g.lerDoArquivo("grafo_3_componentes.txt"))
    {
        cout << "Não foi possível ler o arquivo de entrada." << endl;
        return 1;  // Retorna erro
    }

    // Exibe a análise completa do grafo
    g.exibirAnaliseCompleta();

    return 0;  
}