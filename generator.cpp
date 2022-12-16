#include<bits/stdc++.h>
#define ll long long
using namespace std;


const double bpm = 150.00;//曲目BPM
const double beat_per_second = 60.00/bpm;
const double START_TIME = 4*beat_per_second;//第一个键按下时间
const double END_TIME = 384*beat_per_second;//最后一个键按下时间的后一个十六分音符的时间


mt19937 rnd(time(0));
int num_of_pattern;
bool is_mirror;//为false则现在是左手起，为true则现在右手起
struct node{
    int len,tracks;
}patterns[105];
int change_time_int(double x){
    return (int)(1000.0*x+0.5);
}
int mirror_mode(int num,int tracks){
    int track[10];
    for(int i = 0;i<num;++i){
        track[num-1-i] = 5-(tracks%10);
        tracks /= 10;
    }
    int ret = 0;
    for(int i = 0;i<num;++i){
        ret = ret*10+track[i];
    }
    return ret;
}
struct what_I_get{
    double end_time;
    int end_track;
};
struct my_notes{
    int appear_time,track,sky;
    void print(){
        printf("%d %d %d\n",appear_time,track,sky);
    }
}notes[100005];
int num_of_notes;
//生成num(num <= 9)个一组，固定轨道的交互
what_I_get gen_jh(double start_time,int len,int which_div_note,int num,int tracks){
    int track[10];
    for(int i = 0;i<num;++i){
        track[num-1-i] = tracks%10;
        tracks /= 10;
    }
    double custom_time = start_time;
    double interval_between_note = beat_per_second/(0.25*which_div_note);
    for(int i = 0;i<len;++i){
        notes[++num_of_notes] = {change_time_int(custom_time),track[i%num],0};
        //notes[num_of_notes].print();
        custom_time += interval_between_note;
    }
    if(len&1){
        if(is_mirror) is_mirror = false;
        else is_mirror = true;
    }
    return (what_I_get){custom_time,track[(len-1)%num]};
}
//生成随机无反手有/无拆叠键交互
what_I_get random_jh(double start_time,int len,int which_div_note,int last_track,bool is_chai){
    int nowl = rnd()%last_track+1;
    int nowr = rnd()%(5-last_track)+last_track;
    double custom_time = start_time;
    double interval_between_note = beat_per_second/(0.25*which_div_note);
    int which_hand;
    if(is_mirror){
        notes[++num_of_notes] = {change_time_int(custom_time),nowr,0};
        last_track = nowr;
        custom_time += interval_between_note;
        which_hand = 1;
    }else{
        notes[++num_of_notes] = {change_time_int(custom_time),nowl,0};
        last_track = nowl;
        custom_time += interval_between_note;
        which_hand = 0;
    }
    for(int i = 2;i<=len;++i){
        if((i&1) == which_hand){
            if(is_chai){
                if(nowl == nowr){
                    if(nowl != 4) nowr = rnd()%(4-nowl)+nowl+1;
                }else{
                    nowr = rnd()%(5-nowl)+nowl;
                }
            }else{
                nowr = rnd()%(4-nowl)+nowl+1;
            }
            notes[++num_of_notes] = {change_time_int(custom_time),nowr,0};
            last_track = nowr;
        }else{
            if(is_chai){
                if(nowl == nowr){
                    if(nowr != 1) nowl = rnd()%(nowr-1)+1;
                }else{
                    nowl = rnd()%nowr+1;
                }
            }else{
                nowl = rnd()%(nowr-1)+1;
            }
            notes[++num_of_notes] = {change_time_int(custom_time),nowl,0};
            last_track = nowl;
        }
        custom_time += interval_between_note;
    }
    return (what_I_get){custom_time,last_track};
}
//随机出一个pattern
pair<int,int> get_pattern(int last_track){
    if(is_mirror) last_track = 5-last_track;
    int roll_num,tmp_pattern,rborder,track[10];
    while(true){
        memset(track,0,sizeof(track));
        roll_num = rnd()%num_of_pattern+1;
        rborder = 1;
        tmp_pattern = patterns[roll_num].tracks;
        for(int i = 1;i<=patterns[roll_num].len;++i){
            track[patterns[roll_num].len-i+1] = tmp_pattern%10;
            rborder = max(rborder,tmp_pattern%10);
            tmp_pattern /= 10;
        }
        if(last_track < track[1]) continue;
        if(rborder == 4 && last_track == track[1]) continue;
        if(last_track == track[1] || rborder == 4){
            if(!is_mirror) return make_pair(patterns[roll_num].len,patterns[roll_num].tracks);
            else{
                tmp_pattern = 0;
                for(int i = 1;i<=patterns[roll_num].len;++i){
                    tmp_pattern = tmp_pattern*10+(5-track[i]);
                }
                return make_pair(patterns[roll_num].len,tmp_pattern);
            }
        }
        tmp_pattern = patterns[roll_num].tracks;
        break;
    }
    int delx[4],my_tmp = 0;
    for(int i = 0;i<=3;++i){
        if(track[1]+i <= last_track && rborder+i <= 4) delx[++my_tmp] = i;
    }
    int delta_x = delx[rnd()%my_tmp+1];
    for(int i = 1;i<=patterns[roll_num].len;++i){
        track[i] += delta_x;
    }
    tmp_pattern = 0;
    if(!is_mirror){
        for(int i = 1;i<=patterns[roll_num].len;++i){
            tmp_pattern = tmp_pattern*10+track[i];
        }
    }else{
        for(int i = 1;i<=patterns[roll_num].len;++i){
            tmp_pattern = tmp_pattern*10+(5-track[i]);
        }
    }
    return make_pair(patterns[roll_num].len,tmp_pattern);
}
//去除四纵、五纵
void go_delta_ground(){
    for(int i = 1;i+3<=num_of_notes;++i){
        if(i&1){
            if(notes[i].track != 4 && notes[i].track == notes[i+1].track && notes[i].track == notes[i+2].track && notes[i].track == notes[i+3].track){
                if(rnd()%2 == 1 && notes[i].track != 1){
                    notes[i+2].track = rnd()%(notes[i].track-1)+1;
                }else{
                    notes[i+1].track = rnd()%(4-notes[i].track)+notes[i].track+1;
                }
            }
        }else{
            if(notes[i].track != 1 && notes[i].track == notes[i+1].track && notes[i].track == notes[i+2].track && notes[i].track == notes[i+3].track){
                if(rnd()%2 == 1 && notes[i].track != 4){
                    notes[i+2].track = rnd()%(4-notes[i].track)+notes[i].track+1;
                }else{
                    notes[i+1].track = rnd()%(notes[i].track-1)+1;
                }
            }
        }
    }
    for(int i = 1;i+2 <= num_of_notes;++i){
        if(i&1){
            if(notes[i].track == notes[i+1].track && notes[i].track == notes[i+2].track){
                if(notes[i+2].track != 1) notes[i+2].track = notes[i].track-1;
                else notes[i+1].track = notes[i].track+1;
            }
        }else{
            if(notes[i].track == notes[i+1].track && notes[i].track == notes[i+2].track){
                if(notes[i+2].track != 4) notes[i+2].track = notes[i].track+1;
                else notes[i+1].track = notes[i].track-1;
            }
        }
    }
}
//地键上天
void go_sky(){
    for(int i = 3;i<=num_of_notes;++i){
        if(notes[i-1].sky != 0 && notes[i-2].sky != 0) continue;
        if(i&1){
            if(rnd()%10 <= 2){//sky
                if(notes[i-1].sky == 0){
                    int rborder = 3;
                    if(notes[i-1].track == 1) rborder = 2;
                    notes[i].sky = rnd()%rborder+1;
                }else{
                    int rborder = notes[i-1].sky;
                    notes[i].sky = rnd()%rborder+1;
                }
            }
        }else{
            if(rnd()%10 <= 2){
                if(notes[i-1].sky == 0){
                    int lborder = 1;
                    if(notes[i-1].track == 4) lborder = 2;
                    notes[i].sky = rnd()%(4-lborder)+lborder;
                }else{
                    int lborder = notes[i-1].sky;
                    notes[i].sky = rnd()%(4-lborder)+lborder;
                }
            }
        }
    }
}
//arc(14666,16000,1.00,0.00,b,1.00,1.00,1,none,true)[arctap(15000),arctap(15666)];
vector<int> arcs[4];

void AddNotesToSky(){
    for(int i = 1;i<=num_of_notes;++i){
        arcs[notes[i].sky].push_back(notes[i].appear_time);
    }
}
//输出
void PrintNotes(int opt){
    if(opt == 0){
        printf("%d\n",num_of_notes);
        for(int i = 1;i<=num_of_notes;++i) notes[i].print();
    }else{
        printf("AudioOffset:0\n-\ntiming(0,%.2lf,4.00);\n",bpm);
        for(int i = 1;i<=3;++i){
            printf("arc(%d,%d,%.2lf,%.2lf,s,1.00,1.00,0,none,true)[",change_time_int(START_TIME),change_time_int(END_TIME),0.5*i-0.5,0.5*i-0.5);
            int si = arcs[i].size();
            for(int j = 0;j < si-1;++j) printf("arctap(%d),",arcs[i][j]);
            if(si != 0) printf("arctap(%d)",arcs[i][si-1]);
            printf("];\n");
        }
        for(int i = 1;i<=num_of_notes;++i){
            if(notes[i].sky == 0){
                printf("(%d,%d);\n",notes[i].appear_time,notes[i].track);
            }
        }
    }
}
void RestAWhile(){

}
int main(){
    freopen("pattern1.txt","r",stdin);
    freopen("2.aff","w",stdout);
    scanf("%d",&num_of_pattern);
    for(int i = 1;i<=num_of_pattern;++i) scanf("%d%d",&patterns[i].len,&patterns[i].tracks);
    
    double local_time = START_TIME;
    int lst_track = 4;
    is_mirror = false;
    while(local_time <= END_TIME-5*beat_per_second){
        int rndseed = rnd()%3;
        //1/3概率使用纯随机，2/3概率从pattern库挑一个
        if(rndseed == 0){
            what_I_get current_time = random_jh(local_time,16,16,lst_track,true);
            local_time = current_time.end_time;
            lst_track = current_time.end_track;
        }else{
            pair<int,int> pattern_name = get_pattern(lst_track);
            int patlen = pattern_name.first,pattrack = pattern_name.second;
            what_I_get current_time = gen_jh(local_time,patlen,16,patlen,pattrack);
            local_time = current_time.end_time;
            lst_track = current_time.end_track;
        }
        int p_of_mirror = rnd()%2;
        //1/2概率换手
        if(p_of_mirror){
            if(!is_mirror){
                lst_track = rnd()%lst_track+1; 
                is_mirror = true;
            }else{
                lst_track = rnd()%(5-lst_track)+lst_track;
                is_mirror = false;
            }
            notes[++num_of_notes] = {change_time_int(local_time),lst_track,0};
            local_time += beat_per_second/(0.25*16);
        }
    }
    double TMP = local_time;
    int COUNTTIMES = 0;
    while(TMP < END_TIME) TMP += beat_per_second/(0.25*16),COUNTTIMES++;
    random_jh(local_time,COUNTTIMES,16,lst_track,true);
    go_delta_ground();
    go_sky();
    AddNotesToSky();
    PrintNotes(1);
    return 0;
}
