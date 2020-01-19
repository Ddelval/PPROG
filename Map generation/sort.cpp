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
int main(){
    map<int,int> hei;
    hei.insert({23,82});
    hei.insert({22,82});
    hei.insert({6001,82});
    hei.insert({7001,82});
    hei.insert({21,82});
    hei.insert({20,82});
    hei.insert({6,57});
    hei.insert({7,46});
    hei.insert({8,45});
    hei.insert({8,69});
    vector<pair<int,pii> > ve;
    int a,b,c;
    while(cin>>a>>b>>c){
        ve.pb({a,{b,c}});
    }
    sort(all(ve),[hei](auto a,auto b){
        if(hei.at(a.fi)+a.se.fi!=hei.at(b.fi)+b.se.fi)return hei.at(a.fi)+a.se.fi<hei.at(b.fi)+b.se.fi;
        else return a.se.se<b.se.se;
    });
    for(auto a:ve)cout<<a.fi<<" "<<a.se.fi<<" "<<a.se.se<<"\n";
}