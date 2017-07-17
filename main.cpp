#include <bits/stdc++.h>

using namespace std;
const int MAX = 27;
const int SAMPLE_SIZE = 100;
const int PARENTS_SIZE = 50;
const int ITERATIONS = 1000;
int graph[MAX][MAX];
int n;
vector<string> population;
vector<string> parents;
vector<string> children;
vector<int> pop_fitness;

string encoding(vector<int> seq)
{
    string ans = "";
    for(int i=0;i<n;i++){
        ans += ('a' + seq[i] - 1);
    }
    return ans;
}
bool Find(vector<int> arr, int x)
{
    for(int i=0;i<(int) arr.size();i++){
        if(arr[i] == x) return true;
    }
    return false;
}
vector<string> gen_pop()
{
    vector<string> ans;
    while((int) ans.size() < SAMPLE_SIZE){
        vector<int> sample;
        while((int)sample.size()<n){
            int x = 1 + rand()%n;
            while(Find(sample,x)) x = 1 + rand()%n;
            sample.push_back(x);
        }
        ans.push_back(encoding(sample));
    }
    return ans;
}
vector<string> select_parents(int pourcentage)
{
//    bool visited[SAMPLE_SIZE];
//    memset(visited,0,sizeof visited);
    int par_size = SAMPLE_SIZE*pourcentage/100;
    vector<string> res;
    while((int)res.size()<par_size){
        int idx = rand()%SAMPLE_SIZE;
        res.push_back(population[idx]);
//        cout << res[res.size()-1] << "\n";
    }
    return res;
}
int fitness(string seq)
{
    int res = 0, cur, prev;
    for(int i=1;i<n;i++){
        cur = seq[i] - 'a' + 1;
        prev = seq[i-1] - 'a' + 1;
        res += graph[prev][cur];
    }
    res += graph[cur][seq[0]-'a'+1];
    return res;
}
bool cmp(string a, string b)
{
    return fitness(a) < fitness(b);
}
string crossover()
{
//    string par1, par2;
    int idx1 = rand()%parents.size();
    int idx2 = rand()%parents.size();
    string par1 = parents[idx1];
    string par2 = parents[idx2];
    string child = par2;
//    cout << child << " " << par1 << " " << par2 << endl;
    int idx = rand()%n;
    char cur = par2[idx];
    child[idx] = par1[idx];

    while(cur != par1[idx]){
        int i;
        for(i=0;i<n;i++){
            if(par1[i] == cur){
                cur = par2[i];
                break;
            }
        }
        child[i] = par1[i];
    }
//    cout << child << endl;
    return child;
}
void mutation(int pourcentage)
{
    int cnt = children.size()*pourcentage/100;
    for(int i=0;i<cnt;i++){
        int idx = rand()%((int)children.size());
        int subtour[2];
        subtour[0] = rand()%n;
        subtour[1] = rand()%n;
        sort(subtour,subtour+2);
        reverse(children[idx].begin()+subtour[0], children[idx].begin()+subtour[1]);
    }
}
vector<int> decoding()
{
    string best = population[0];
    vector<int> ham_seq;
    for(int i=0;i<n;i++){
        ham_seq.push_back((int)(best[i]-'a'+1));
    }
    return ham_seq;
}
//void test_encoding()
//{
//    /// Encoding testing
//    int a[3] = {4,2,3};
//    vector<int> tmp(a,3);
//    cout << encoding(tmp);
//}
//void test_gen_pop()
//{
//    /// Gen_pop testing
//    set<int> mySet;
//    vector<int> pop = gen_pop();
//    for(int i=0;i<pop.size();i++){
//        mySet.insert(pop[i]);
//    }
//    mySet.size() == n ? puts("valid") : puts("invalid");
//    for(int i=0;i<pop.size();i++){
//        cout << pop[i] << " ";
//    }
//}
void inputEdges()
{
    cin >> n;
    assert(n <= 26);

    for(int i=0;i<(n*(n-1))>>1;i++){
        int a,b,c;
        cin >> a >> b >> c;
        graph[a][b] = c;
        graph[b][a] = c;
    }
}
void inputMatrix(int _n)
{
    n = _n;
    int tmp;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            scanf("%d",&tmp),graph[i][j] = tmp;
}
void showGraph()
{
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",graph[i][j]);
        puts("");
    }
}
int fitness2(string seq)
{
    int res = 0, cur, prev;
    for(int i=1;i<n;i++){
        cur = seq[i] - 'a' + 1;
        prev = seq[i-1] - 'a' + 1;
        res += graph[prev][cur];
        cout << prev << " - " << cur << " : " << graph[prev][cur] << " res = " << res << endl;
    }
    res += graph[cur][seq[0]-'a'+1];
    return res;
}
int main()
{
    freopen("test1.in","r",stdin);

    inputMatrix(26);
//    showGraph();
    population = gen_pop();

    for(int i=0;i<ITERATIONS;i++)
    {
        sort(population.begin(), population.end(), cmp);
        parents.clear();
        parents.insert(parents.end(),population.begin(),population.begin()+PARENTS_SIZE);
        children.clear();
        for(int i=0;i<SAMPLE_SIZE-PARENTS_SIZE;i++){
            string child =  crossover();
            children.push_back(child);
        }
        mutation(5);    /// mutate 5% of population
        population.clear();
        population.insert(population.end(),parents.begin(),parents.end());
        population.insert(population.end(),children.begin(),children.end());
    }
    sort(population.begin(), population.end(), cmp);
    vector<int> output = decoding();
    for(int i=0;i<n;i++) cout << output[i] << " ";
    cout << endl;
    int res = fitness(population[0]);
    cout << '\n' << res << '\n';
    return 0;
}
