#inlcude <iostream>
using namespace std;

vector<vector<int>> adj;
vector<int> dfsn, low, comp; // initialize dfsn and low to -1
vector<bool> instack; // initialize to false
stack<int> st;
vector<vector<int>> comps;
int n, m, idx, cnt;

void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    st.push(u);
    instack[u] = true;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
        }else if(instack[v]){
            low[u] = min(low[u], dfsn[v]);
        }
    }
    if(dfsn[u] == low[u]){
        comps.push_back(vector<int>());
        int v;
        do{
            v = st.top();
            st.pop();
            instack[v] = false;
            comp[v] = cnt;
            comps[cnt].push_back(v);
        }while(u != v);
        cnt++;
    }
}

vector<vector<int>> newadj;
// for directed
void computeCompGraph(){
    //TODO initialize newadj 
    for (int i = 0; i < sz(adj); i++){
        for(int j = 0; j < sz(adj[i]); j++){
            int k = adj[i][j];
            if(comp[k] != comp[i])
                newadj[comp[i]].push_back(comp[k]);
                //! newadj[comp[k]].push_back(comp[i]);  for undirected
        }
    }
}

//* bridges
vector<pair<int, int>> bridges;
void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if(low[v] > dfsn[u]){
                bridges.push_back({u, v});
            }
        }else{
            low[u] = min(low[u], dfsn[v]);
        }
    }
}

//* articulation points
vector<int> artPoints;
void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    int children = 0;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            children++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if(low[v] >= dfsn[u] && p != -1){
                artPoints.push_back(u);
            }
        }else{
            low[u] = min(low[u], dfsn[v]);
        }
    }
    if(p == -1 && children > 1){
        artPoints.push_back(u);
    }
}








int main(){
    return 0;
}