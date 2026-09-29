#include <bits/stdc++.h>
using namespace std;

vector<double> x = {60,23,15,85,71,98,50,20,40,75,82,91,12,37,28,66,55,73,88,42};
vector<double> y = {200,45,150,90,123,45,220,30,180,155,60,120,210,100,77,89,130,140,33,170};

vector<vector<double>> d;
mt19937 gen(42);

vector<int> makeTour(){
    vector<int> a;
    for(int i=1;i<20;i++) a.push_back(i);
    shuffle(a.begin(),a.end(),gen);
    a.insert(a.begin(),0);
    return a;
}

double distTour(const vector<int>& a){
    double s=0;
    for(int i=0;i<20;i++) s += d[a[i]][a[(i+1)%20]];
    return s;
}

vector<int> ox(const vector<int>& a, const vector<int>& b){
    int n=a.size();
    vector<int> c(n,-1);
    uniform_int_distribution<int> pos(1,n-1);
    int l=pos(gen), r=pos(gen);
    if(l>r) swap(l,r);
    for(int i=l;i<=r;i++) c[i]=a[i];
    vector<int> rem;
    for(int i=1;i<n;i++){
        bool used=false;
        for(int j=l;j<=r;j++) if(c[j]==b[i]) used=true;
        if(!used) rem.push_back(b[i]);
    }
    int k=0;
    for(int i=r+1;i<n;i++) if(c[i]==-1) c[i]=rem[k++];
    for(int i=1;i<=r;i++) if(c[i]==-1) c[i]=rem[k++];
    c[0]=0;
    return c;
}

void mutate(vector<int>& a){
    uniform_real_distribution<double> q(0,1);
    if(q(gen)<0.15){
        uniform_int_distribution<int> pos(1,19);
        int i=pos(gen), j=pos(gen);
        swap(a[i],a[j]);
    }
}

int selectParent(const vector<vector<int>>& pop){
    vector<double> f(pop.size());
    double total=0;
    for(int i=0;i<(int)pop.size();i++){
        f[i]=1.0/distTour(pop[i]);
        total+=f[i];
    }
    uniform_real_distribution<double> q(0,total);
    double r=q(gen), s=0;
    for(int i=0;i<(int)pop.size();i++){
        s+=f[i];
        if(r<=s) return i;
    }
    return pop.size()-1;
}

pair<vector<int>,double> runGA(int popSize,int generations){
    vector<vector<int>> pop;
    for(int i=0;i<popSize;i++) pop.push_back(makeTour());
    for(int g=0;g<generations;g++){
        int best=0;
        for(int i=1;i<popSize;i++) if(distTour(pop[i])<distTour(pop[best])) best=i;
        vector<vector<int>> next;
        next.push_back(pop[best]);
        while((int)next.size()<popSize){
            int p1=selectParent(pop), p2=selectParent(pop);
            vector<int> child=ox(pop[p1],pop[p2]);
            mutate(child);
            next.push_back(child);
        }
        pop=next;
    }
    int best=0;
    for(int i=1;i<popSize;i++) if(distTour(pop[i])<distTour(pop[best])) best=i;
    return {pop[best],distTour(pop[best])};
}

int main(){
    d.assign(20,vector<double>(20));
    for(int i=0;i<20;i++) for(int j=0;j<20;j++)
        d[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));

    vector<pair<int,int>> tests={{10,100},{20,100},{30,100},{10,200},{20,200},{30,200}};
    for(auto t:tests){
        auto ans=runGA(t.first,t.second);
        cout << "Population = " << t.first << ", Generations = " << t.second << "\n";
        cout << "Best tour: ";
        for(int v:ans.first) cout << v+1 << " ";
        cout << 1 << "\n";
        cout << fixed << setprecision(2) << "Total distance: " << ans.second << "\n\n";
    }
}
