#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>


/** p=- *************************** Settings **********************************/

#define LENOVO_XP
#define TIME_LIMIT 9.8
#define RDTSC
#define MERSENNE_TWISTER

/** (<) ************************* Timekeeping *********************************/

#ifdef GETTIMEOFDAY
#define TIMER(t) struct timeval t##_i,t##_f
#define TIMER_START(t) gettimeofday(&t##_i,NULL)
#define TIMER_TIME(t) ({gettimeofday(&t##_f,NULL);t##_f.tv_sec-t##_i.tv_sec+   \
	(t##_f.tv_usec-t##_i.tv_usec)*1e-6;})
#elif defined(RDTSC)
#ifdef LENOVO_XP
#define RDTSC_K (1/1.9e9)
#elif defined(IBM_UBUNTU)
#define RDTSC_K (1/1.8e9)
#else
#define RDTSC_K (1/3.6e9)
#endif
#define TIMER(t) double t##_i; unsigned long long t##_t;
#define TIMER_START(t) do{__asm__ volatile("rdtsc":"=A"(t##_t));               \
t##_i=t##_t*RDTSC_K;}while(0)
#define TIMER_TIME(t) ({__asm__ volatile("rdtsc":"=A"(t##_t));                 \
t##_t*RDTSC_K-t##_i;})
#else
#define TIMER(t) double t##_i
#define TIMER_START(t) do{t##_i=(double)clock()/CLOCKS_PER_SEC;}while(0)
#define TIMER_TIME(t) ((double)clock()/CLOCKS_PER_SEC-t##_i)
#endif

/** X ~ ************************** Statistics *********************************/

#ifdef MERSENNE_TWISTER
#define MT_HM 0x80000000u
#define MT_LM 0x7FFFFFFFu
struct MT_{enum{N=624,M=397};unsigned v[N];int n;MT_(int s=1){v[0]=s&0xFFFFFFFFu
;for(n=1;n<N;++n)v[n]=(1812433253u*(v[n-1]^(v[n-1]>>30))+n);}unsigned g(){
unsigned y;static unsigned m[2]={0u,0x9908B0DFu};if(n>=N){int k;for(k=0;k<N-M;
++k){y=(v[k]&MT_HM)|(v[k+1]&MT_LM);v[k]=v[k+M]^(y>>1)^m[y&1u];}for(;k<N-1;++k){
y=(v[k]&MT_HM)|(v[k+1]&MT_LM);v[k]=v[k+M-N]^(y>>1)^m[y&1u];}y=(v[N-1]&MT_HM)|(
v[0]&MT_LM);v[N-1]=v[M-1]^(y>>1)^m[y&1u];n=0;}y=v[n++];y^=(y>>11);y^=(y<<7)&
0x9d2c5680UL;y^=(y<<15)&0xefc60000UL;y^=(y>>18);return y;}};
#define GEN(s) MT_ mt_(s)
#define GEN_N32() mt_.g()
#else
#define GEN(s) srand(s)
#define GEN_N32() rand()
#endif
#define GEN_DU(a,b) ((a)+GEN_N32()%((b)-(a)+1))
#define GEN_DU0(b) (GEN_N32()%(b))

/******************************* End of library *******************************/
