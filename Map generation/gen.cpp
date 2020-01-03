#include <bits/stdc++.h>
using namespace std;

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define fi first
#define se second

typedef long long ll;
typedef vector<int> vi;
typedef vector<ll>  vl;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<pii> vii;
typedef vector<pll> vll;

int main(int argc, char* argv[]){
    ios::sync_with_stdio(false);
    int x,y;
    int h,w;
    int n;
    srand(time(NULL));
    x=stoi(argv[2]);
    y=stoi(argv[3]);
    h=stoi(argv[4]);
    w=stoi(argv[5]);
    n=stoi(argv[6]);
    vector<pii> vec;
    for(int i=0;i<n;++i){
        vec.pb({x+rand()%h,y+rand()%w});
    }
    sort(all(vec),[](pii a,pii b){
        if(a.fi!=b.fi)return a.fi<b.fi;
        return a.se<b.se;
    });
    for(auto a:vec){
        cout<<argv[1]<<" "<<a.fi<<" "<<a.se<<"\n";
    }

}