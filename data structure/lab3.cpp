#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
const int MAXN=108;
struct node{
	node *l,*r;
	char data;
};
node *create(char *pre,char *mid,int len){
	if (len<=0) return NULL;
	node *root;
	root = new node;
	root->data=pre[0];
	int rootindex;
	for (rootindex=0;rootindex<len;rootindex++){
		if (mid[rootindex]==pre[0]) break;
	}
	root->l=create(pre+1,mid,rootindex);
	root->r=create(pre+rootindex+1,mid+rootindex+1,len-rootindex-1);
	printf("%c",root->data);
	return root;
}
void layer(node *root){
	queue<node*> dot;
	if (root){
		dot.push(root);
		cout<<dot.front()->data;
	} 
	while (!dot.empty()){
		node *root=dot.front();
		dot.pop();
		if (root->l){
			cout<<root->l->data;
			dot.push(root->l);
		}
		//cout<<root->r->data;
		if (root->r){
			cout<<root->r->data;
			dot.push(root->r);
		}
	}
}
void Path(node *cur,char *path,char tar,int cnt){
	if (!cur) return;
	path[cnt]=cur->data;
	//cout<<cur->data<<endl;
	if (cur->data==tar) {
		int i;
		for (i=0;i<cnt;i++) printf("%c->",path[i]);
		cout<<path[cnt];
		return;
	}
	Path(cur->l,path,tar,cnt+1);
	Path(cur->r,path,tar,cnt+1);
}
int main(){
	char pre[MAXN],mid[MAXN];char path[MAXN]={'\0'};int i;
	cin>>pre;
	cin>>mid;
	int len=strlen(pre);
	node *root=new node;
	root=create(pre,mid,len);
	printf("\n");
	layer(root);printf("\n");
	char point;cin>>point;bool flag=0;
	//cout<<root->data<<endl;
	for (i=0;i<len;i++){
		if (pre[i]==point){
			flag=1;
			break;
		}
	}
	if (!flag) printf("不存在该节点");
	else Path(root,path,point,0);
	return 0;
}

