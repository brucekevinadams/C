/*
 * Author: Bruce Adams
 * email: ezaroth@gmail.com
 * website: austingamestudios.com
 *
 * C program from a Hackerrank problem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _tree{
  int *y;
  long long *a;
  int size;
  int N;
  int *left_idx;
  int *right_idx;
} tree;

int diff(int x,int y);
long long max(long long x,long long y);
void init(int n,int *N);
long long range_sum( int i, int j);
void updatea(int i);
void build(int v);
void merge(tree *t,tree *x,tree *y);
int get_i(int*a,int num,int size);
int med(int*a,int size);
long long query(int v);
void update(int v,int idx);
int x1,x2,y1,y2,N,tl[800000],tr[800000];
long long val,*tt;
int lat[200000]={0},lon[200000]={0},poi[200000],tla[200000]={0};
tree t[800000];

int main(){
  int n,x,y,c,i,j,max_idx=-1,a,b,C,d;
  long long max=0,dp;
  scanf("%d%d%d",&n,&x,&y);
  for(i=c=0;i<n;i++){
    scanf("%d%d%d%d",&a,&b,&C,&d);
    if(d>0)
      c++;
    lat[C-1]=a;
    lon[C-1]=b;
    poi[C-1]=d;
    tla[a-1]=b;
  }
  if(!c){
    printf("0");
    return 0;
  }
  tl[1]=0;
  tr[1]=199999;
  build(1);
  for(i=199999;i>=0;i--)
    if(lat[i]){
      if(max_idx!=-1 && diff(lat[max_idx],lat[i])<=x && diff(lon[max_idx],lon[i])<=y)
        dp=max;
      else{
        x1=lat[i]-x-1;
        x2=lat[i]+x-1;
        y1=lon[i]-y;
        y2=lon[i]+y;
        dp=query(1);
      }
      if(dp>0)
        dp+=poi[i];
      else
        dp=poi[i];
      if(dp>max){
        max=dp;
        max_idx=i;
      }
      if(dp>0){
        x1=lat[i]-1;
        y1=lon[i];
        val=dp;
        update(1,-1);
      }
    }
  printf("%lld",max);
  return 0;
}
int diff(int x,int y){
  return (x<y)?(y-x):(x-y);
}
long long max(long long x,long long y){
  return (x>y)?x:y;
}
void init(int n,int *N){
  (*N) = 1;
  while( (*N) < n ) (*N) *= 2;
}
long long range_sum( int i, int j){
  long long ans = 0;
  for( i += N, j += N; i <= j; i = ( i + 1 ) / 2, j = ( j - 1 ) / 2 )
  {
    if( i % 2 == 1 ) ans = max(ans,tt[i]);
    if( j % 2 == 0 ) ans = max(ans,tt[j]);
  }
  return ans;
}
void updatea(int i){
  int j;
  for( j = i + N; j; j /= 2 ) tt[j] = max(tt[j],val);
}
void build(int v){
  if(tl[v]==tr[v]){
    t[v].size=1;
    t[v].y=(int*)malloc(t[v].size*sizeof(int));
    t[v].a=(long long*)malloc(4*t[v].size*sizeof(long long));
    memset(t[v].a,0,4*t[v].size*sizeof(long long));
    t[v].y[0]=tla[tl[v]];
    init(t[v].size,&t[v].N);
  }
  else{
    int tm=(tl[v]+tr[v])/2;
    tl[2*v]=tl[v];
    tl[2*v+1]=tm+1;
    tr[2*v]=tm;
    tr[2*v+1]=tr[v];
    build(v*2);
    build(v*2+1);
    merge(&t[v],&t[v*2],&t[v*2+1]);
  }
  return;
}
void merge(tree *t,tree *x,tree *y){
  int i=0,j=0;
  t->size=x->size+y->size;
  t->y=(int*)malloc(t->size*sizeof(int));
  t->left_idx=(int*)malloc(t->size*sizeof(int));
  t->right_idx=(int*)malloc(t->size*sizeof(int));
  t->a=(long long*)malloc(t->size*sizeof(long long)*4);
  memset(t->a,0,t->size*sizeof(long long)*4);
  init(t->size,&t->N);
  while(i<x->size || j<y->size){
    if(i==x->size){
      t->y[i+j]=y->y[j];
      t->left_idx[i+j]=i-1;
      t->right_idx[i+j]=j;
      j++;
    } 
    else if(j==y->size){
      t->y[i+j]=x->y[i];
      t->left_idx[i+j]=i;
      t->right_idx[i+j]=j-1;
      i++;
    }
    else if(x->y[i]<=y->y[j]){
      t->y[i+j]=x->y[i];
      t->left_idx[i+j]=i;
      t->right_idx[i+j]=j-1;
      i++;
    } 
    else{
      t->y[i+j]=y->y[j];
      t->left_idx[i+j]=i-1;
      t->right_idx[i+j]=j;
      j++;
    }
  }
  return;
}
int get_i(int*a,int num,int size){
  if(size==0)
    return 0;
  if(num>med(a,size))
    return get_i(&a[(size+1)>>1],num,size>>1)+((size+1)>>1);
  else
    return get_i(a,num,(size-1)>>1);
}
int med(int*a,int size){
  return a[(size-1)>>1];
}
long long query(int v){
  int i,j;
  if(tl[v]>x2 || tr[v]<x1 || !t[v].a[1])
    return 0;
  if(tl[v]>=x1 && tr[v]<=x2){
    i=get_i(t[v].y,y1,t[v].size);
    j=get_i(t[v].y,y2+1,t[v].size)-1;
    if(j<i)
      return 0;
    N=t[v].N;
    tt=t[v].a;
    return range_sum(i,j);
  }
  else if(tl[v]!=tr[v])
    return max(query(2*v),query(2*v+1));
  return 0;
}
void update(int v,int idx){
  if(tl[v]<=x1 && tr[v]>=x1){
    if(idx==-1)
      idx=get_i(t[v].y,y1,t[v].size);
    N=t[v].N;
    tt=t[v].a;
    updatea(idx);
    if(tl[v]!=tr[v]){
      update(v*2,t[v].left_idx[idx]);
      update(v*2+1,t[v].right_idx[idx]);
    }
  }
  return;
}
