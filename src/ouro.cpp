#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using boost::multiprecision::cpp_int;

vector<int> small_primes;
void init_primes(){
    const int LIM=10000;
    vector<bool> is(LIM+1,true);
    is[0]=is[1]=false;
    for(int i=2;i*i<=LIM;i++) if(is[i]) for(int j=i*i;j<=LIM;j+=i) is[j]=false;
    for(int i=2;i<=LIM;i++) if(is[i]) small_primes.push_back(i);
}

cpp_int modpow_fw(cpp_int a, cpp_int e, cpp_int mod){
    if(e==0) return 1;
    a %= mod;
    cpp_int table[8];
    table[0]=1;
    table[1]=a;
    cpp_int a2 = (a*a)%mod;
    table[2]=a2;
    table[3]=(a2*a)%mod;
    table[4]=(a2*a2)%mod;
    table[5]=(table[3]*a2)%mod;
    table[6]=(table[3]*table[3])%mod;
    table[7]=(table[3]*a2*a2)%mod;
    
    int bits = msb(e);
    cpp_int r = 1;
    int i = bits;
    while(i>=0){
        if(i>=2){
            r = (r*r)%mod;
            r = (r*r)%mod;
            r = (r*r)%mod;
            int w = 0;
            if( (e>>i)&1 ) w |= 4;
            if( (e>>(i-1))&1 ) w |= 2;
            if( (e>>(i-2))&1 ) w |= 1;
            if(w) r = (r*table[w])%mod;
            i-=3;
        } else {
            r = (r*r)%mod;
            if( (e>>i)&1 ) r = (r*table[1])%mod;
            i--;
        }
    }
    return r;
}

bool isPrime(const cpp_int& n){
    for(int p: small_primes){
        if(n==p) return true;
        if(n%p==0) return false;
        if(p>1000 && n%p!=0 && small_primes.size()>300) {} // keep fast
    }
    cpp_int d=n-1;
    int s=0;
    while((d&1)==0){ d>>=1; ++s; }
    for(int a: {2,3,5,7,11}){
        if(cpp_int(a)>=n) continue;
        cpp_int x=modpow_fw(a,d,n);
        if(x==1 || x==n-1) continue;
        bool comp=true;
        for(int r=1;r<s;r++){
            x=(x*x)%n;
            if(x==n-1){ comp=false; break; }
        }
        if(comp) return false;
    }
    return true;
}

void gen_race(cpp_int &out, atomic<bool> &done){
    thread_local mt19937_64 rng(random_device{}() ^ (uint64_t)hash<thread::id>{}(this_thread::get_id()) ^ (uint64_t)chrono::steady_clock::now().time_since_epoch().count());
    const int wheel[8]={1,7,11,13,17,19,23,29};
    int wi=rng()%8;
    while(!done.load()){
        cpp_int c=0;
        for(int i=0;i<32;i++){ c<<=64; c+=rng(); }
        c |= cpp_int(1)<<2047;
        c |= cpp_int(1)<<2046;
        c &= ~cpp_int(31);
        c += wheel[wi];
        wi=(wi+1)&7;
        if(isPrime(c)){
            bool exp=false;
            if(done.compare_exchange_strong(exp,true)) out=c;
            break;
        }
    }
}

int main(){
    init_primes();
    auto t0=chrono::high_resolution_clock::now();
    cpp_int p,q;
    atomic<bool> dp(false), dq(false);
    vector<thread> th;
    for(int i=0;i<6;i++) th.emplace_back([&]{ gen_race(p,dp); });
    for(int i=0;i<6;i++) th.emplace_back([&]{ gen_race(q,dq); });
    for(auto &t:th) t.join();
    cpp_int n=p*q;
    double tot=chrono::duration<double>(chrono::high_resolution_clock::now()-t0).count();
    ofstream outf("ouro.txt");
    outf<<"p="<<p<<"\nq="<<q<<"\nn="<<n<<"\ntime="<<tot<<"s\n";
    cout<<"TOTAL "<<tot<<"s N="<<msb(n)+1<<" bits -> ouro.txt\n";
}