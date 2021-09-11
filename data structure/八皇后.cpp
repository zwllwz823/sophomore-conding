#include <cstdio>
#include <iostream>
#include <memory.h>
using namespace std;
const int MAXN=10;
int n,ans,a[MAXN],b[MAXN],c[MAXN],d[MAXN],result[MAXN][MAXN]; 
void print(){
	ans++; 
	for (int i=1;i<=8;i++) result[i][a[i]]=1;
    for (int i=1;i<=8;i++){
    	for (int j=1;j<=8;j++) printf("%d ",result[i][j]);
    	printf("\n");
	}
	memset(result,0,sizeof(result));
	printf("\n");
}
int dfs(int n1){//n1为行数 
	for (int j=1;j<=8;j++){
		if (b[j]==0 && c[n1+j]==0 && d[n1-j+n]==0){
			a[n1]=j;b[j]=1;c[n1+j]=1;d[n1-j+n]=1;
			if (n1==8) print();
			else dfs(n1+1);
			a[n1]=0;b[j]=0;c[n1+j]=0;d[n1-j+n]=0;
		}
	}
	return 0; 
}
int main(){
	dfs(1);
	cout<<"共有"<<ans<<"种方案"; 
	return 0;
}

