#include<bits/stdc++.h>
using namespace std;
int n;
struct my_notes{
    int appear_time,track,sky;
}notes[100005];
int main(){
	freopen("2.aff","r",stdin);
	scanf("%d",&n);
	for(int i = 1;i<=n;++i) scanf("%d%d%d",&notes[i].appear_time,&notes[i].track,&notes[i].sky);
	bool flg = true;
	int wh = 0;
	for(int i = 2;i<=n;++i){
		if(i&1){
			if(notes[i].track > notes[i-1].track) flg = false;
		}else{
			if(notes[i].track < notes[i-1].track) flg = false;
		}
		if(!flg){
			wh = i;
			break;
		}
	}
	printf("地面键是否合法：");
	if(flg) puts("YES");
	else printf("NO:%d\n",wh);

	flg = false;
	for(int i = 1;i+4<=n;++i){
		if(notes[i].track == notes[i+1].track && notes[i].track == notes[i+2].track && notes[i].track == notes[i+3].track && notes[i].track == notes[i+4].track) flg = true;
	}
	printf("是否有五纵：");
	if(flg) puts("YES");
	else puts("NO");
	flg = false;
	wh = 0;
	for(int i = 2;i<=n;++i){
		if(i&1){
			if(notes[i].sky == 3 && notes[i-1].sky == 0 && notes[i-1].track == 1) flg = true;
			if(notes[i].sky && notes[i-1].sky && notes[i].sky > notes[i-1].sky) flg = true;
		}else{
			if(notes[i].sky == 1 && notes[i-1].sky == 0 && notes[i-1].track == 4) flg = true;
			if(notes[i].sky && notes[i-1].sky && notes[i].sky < notes[i-1].sky) flg = true;
		}
	}
	printf("是否天键反手：");
	if(flg) puts("YES");
	else puts("NO");

	int sum = 0;
	for(int i = 1;i+1<=n;++i){
		if(notes[i].track == notes[i+1].track) sum++;
	}
	printf("sum of notes:%d\nsum of chai:%d\n",n,sum);
	int score = 0;
	for(int i = 3;i<=n;++i){
		int frt,nw;
		if(notes[i].sky == 0){
			nw = notes[i].track;
		}else{
			nw = notes[i].sky+2;
		}
		if(notes[i-2].sky == 0){
			frt = notes[i-2].track;
		}else{
			frt = notes[i-2].sky+2;
		}
		score += abs(frt-nw);
	}
	printf("score:%d\n",score);
	return 0;
}