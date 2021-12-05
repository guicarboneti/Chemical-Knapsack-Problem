#include <bits/stdc++.h>
#include <chrono>

#define EPS 1E-6
#define MOD 1000000007
#define inf 0x3f3f3f3f
#define llinf 0x3f3f3f3f3f3f3f3f

#define fi first
#define se second
#define pb push_back
#define ende '\n'
#define esp ' '

#define all(x) (x).begin(), (x).end()
#define mset(x, y) memset(&x, (y), sizeof(x))

using namespace std;
using ll = long long;
using ii = pair<int, int>;

typedef struct Item {
    int peso;
    double valor;
    double custo_beneficio;  // valor por item
    double bound;  // limite superior do melhor lucro possível na subárvore do nó
    int id;
    bool operator > (const Item& str) const {
        return (custo_beneficio > str.custo_beneficio);
    }
} Item;

int n, m, c;

vector<int> pesos;
vector<int> valores;
vector<vector<int>> restricoes;

vector<Item> itens;

int nodes;

int melhor_valor;
vector<int> melhor_mochila;

void cout_mochila(vector<int> &mochila) {
    cout << "mochila: ";
    for (int i : mochila) {
        cout << i << esp;
    }
    cout << endl;
}

// Computa se é proibido um item estar junto de outro na mochila
bool proibido (int a, vector<int> &mochila) {
    for (int frasco : restricoes[a]) {
        for (int item : mochila) {
            if (frasco == item) {    // Se um frasco das restrições é um dos itens da mochila
                return true;
            }
        }
    }
    return false;
}

double bound (int pos, int espaco) {
    // cout << "Compute bound" << endl;

    double soma;
    while ((espaco - itens[pos].peso) > 0 && pos < n) {
        espaco -= itens[pos].peso;
        soma += itens[pos].valor;
        pos++;
    }

    if (pos < n) {
        soma += ((double) espaco) / itens[pos].peso * itens[pos].valor;
    }

    return soma;
}

void brach_and_bound (int pos, int peso, int valor, vector<int> &mochila) {
    if (pos >= n) {
        return;
    }

    // cout_mochila(mochila);

    pos++;
    Item item = itens[pos];

    nodes++;

    if (valor > melhor_valor) {
        // cout << "best: " << valor << endl;
        // cout << "Trocou mochila" << endl;
        melhor_valor = valor;
        melhor_mochila = mochila;
        // cout_mochila(mochila);
    }

    if (peso + item.peso <= c && !proibido(item.id, mochila)) {
        mochila.push_back(item.id);
        brach_and_bound (pos, peso + item.peso, valor + item.valor, mochila);
        mochila.pop_back();
    }

    // cout << "Voltou " << pos << endl;

    if ((bound(pos+1, c - peso) + valor) >= melhor_valor) {
        brach_and_bound (pos, peso, valor, mochila);
    }
}

int main() {

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    
    cin >> n >> m >> c;

    // Itens
    melhor_valor = 0;
    nodes = 0;
    itens.resize(n);

    // Pesos
    for (int i = 0; i < n; i++) {
        int aux; cin >> aux;
        itens[i].peso = aux;
    }
    
    // Valores
    for (int i = 0; i < n; i++) {
        int aux; cin >> aux;
        itens[i].valor = aux;
    }

    for (int i = 0; i < n; i++) {
        itens[i].custo_beneficio = itens[i].valor / itens[i].peso;
        itens[i].id = i+1;
    }

    // Restrições

    restricoes.resize(n+1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        
        restricoes[a].push_back(b);
        restricoes[b].push_back(a);
    }

    // for (int i = 0; i < itens.size(); i++) {
    //     cout << i << ": " << itens[i].valor << esp << itens[i].id << endl;
    // }

    sort(itens.begin(), itens.end(), greater<Item>());

    vector<int> mochila;
    brach_and_bound(-1, 0, 0, mochila);
    
    cout << melhor_valor << endl;

    sort(melhor_mochila.begin(), melhor_mochila.end());
    for (int i : melhor_mochila)
        cout << i << esp;
    cout << endl;

    cerr << "Numero de nós: " << nodes << endl; 
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cerr << "Tempo: " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000.0)  << " milisegundos" << endl;

    return 0;
}
