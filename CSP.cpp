#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> domain = {{11,21,31},{21,22,31,42},{12,42}};
vector<int> current(3,-1), used(3,0);
string name[3]={"A","B","C"};

int timeOf(int x){ return x/10; }
int labOf(int x){ return x%10; }

bool valid(int a,int b,int c){
    if(a!=-1 && b!=-1 && timeOf(a)==timeOf(b)) return false;
    if(b!=-1 && c!=-1 && timeOf(b)==timeOf(c)) return false;
    if(a!=-1 && c!=-1){
        if(timeOf(a)==timeOf(c) && labOf(a)==labOf(c)) return false;
        if(abs(timeOf(a)-timeOf(c))==1) return false;
    }
    return true;
}

bool pairValid(int v,int value,int u,int other){
    int t1=timeOf(value), t2=timeOf(other);
    if((v==0 && u==1) || (v==1 && u==0)) return t1!=t2;
    if((v==1 && u==2) || (v==2 && u==1)) return t1!=t2;
    if((v==0 && u==2) || (v==2 && u==0))
        return abs(t1-t2)!=1 && !(t1==t2 && labOf(value)==labOf(other));
    return true;
}

int degree(int v){
    int d=0;
    for(int u=0;u<3;u++) if(u!=v && !used[u]) d++;
    return d;
}

int chooseVariable(){
    int best=-1;
    for(int i=0;i<3;i++){
        if(used[i]) continue;
        if(best==-1 || domain[i].size()<domain[best].size() ||
           (domain[i].size()==domain[best].size() && degree(i)>degree(best))) best=i;
    }
    return best;
}

void printDomain(){
    for(int i=0;i<3;i++){
        if(used[i]) continue;
        cout << name[i] << ": ";
        for(int x:domain[i]) cout << "(" << timeOf(x) << ", L" << labOf(x) << ") ";
        cout << "\n";
    }
}

bool solve(){
    int left=0;
    for(int i=0;i<3;i++) if(!used[i]) left++;
    if(left==0) return true;

    int v=chooseVariable();
    cout << "MRV selects " << name[v] << "\n";

    vector<int> old=domain[v];
    for(int value:old){
        bool good=true;
        for(int u=0;u<3;u++) if(used[u]){
            if(!pairValid(v,value,u,current[u])) good=false;
        }
        if(!good){
            cout << "Try " << name[v] << " = (" << timeOf(value) << ", L" << labOf(value) << ") -> constraint violated\n";
            continue;
        }

        cout << "Try " << name[v] << " = (" << timeOf(value) << ", L" << labOf(value) << ")\n";
        used[v]=1;
        current[v]=value;

        vector<vector<int>> oldDomains=domain;
        bool empty=false;
        for(int u=0;u<3;u++) if(!used[u]){
            vector<int> nd;
            for(int x:domain[u]) if(pairValid(v,value,u,x)) nd.push_back(x);
            domain[u]=nd;
            if(nd.empty()){
                cout << "Forward checking: domain of " << name[u] << " becomes empty\n";
                empty=true;
            }
        }

        if(!empty){
            cout << "Domains after forward checking:\n";
            printDomain();
            if(solve()) return true;
        }

        cout << "Backtracking from " << name[v] << " = (" << timeOf(value) << ", L" << labOf(value) << ")\n";
        domain=oldDomains;
        used[v]=0;
        current[v]=-1;
    }
    return false;
}

int main(){
    cout << "Initial domains:\n";
    printDomain();
    cout << "\n";
    solve();
    cout << "\nFinal schedule\n";
    cout << "Section  Time Slot  Lab\n";
    for(int i=0;i<3;i++) cout << name[i] << "        " << timeOf(current[i]) << "          L" << labOf(current[i]) << "\n";
}
