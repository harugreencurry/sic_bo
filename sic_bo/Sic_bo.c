#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define START_MONEY 1000000
#define NO 0
#define YES 1
#define QUOTA (START_MONEY/100)
#define MAX_ROUND 3
#define MAXDIG 20
#define MIN_DICE 1
#define MAX_DICE 6
#define PECIES_OF_DICE 3
#define MIN_SUM (MIN_DICE * PECIES_OF_DICE)
#define MAX_SUM (MAX_DICE * PECIES_OF_DICE)

struct people{
  int money;
  int bet;
};
struct status{
  int d[PECIES_OF_DICE];
  int sum;
  int flag_repdigit;
};
char str[MAXDIG];

void command(struct people *p,struct status *d);
void roll_dice(struct status *p);
void print_result(struct status *p);
int rate_small_or_big(int c,struct status *d);
int rate_single(int c,struct status *d);
int rate_pair(int c[],struct status *d);
int rate_total(int c,struct status *d);
int rate_double_repdigit(int c,struct status *d);
int rate_any_triple(struct status *d);
int rate_triple(int c,struct status *d);
void how_to_bet(void);
void bet_money(struct people *p);
void print_introduce(void);
void add_comma(int money,char str[MAXDIG]);
void choice_numbers(int choice[],int count,int min,int max);
void display_highscore(int money);
  
int main(void){
  struct people you;
  struct status dice;
  int i,ok=NO;
  
  you.money=START_MONEY;
  print_introduce();
  printf("これからSic-boを始めます.%dRound行います.\n",MAX_ROUND);
  add_comma(you.money,str);
  printf("はじめの所持金は%s円です.所持金を増やせるように頑張ってください!\n",str);
  while(1){
    printf("始めてよければ%dを入力してください: ",YES);scanf("%d",&ok);
    if(ok==YES){
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      break;
    }
  }
  for(i=0;i<MAX_ROUND;i++){
    printf("_______________________________________________________________________________\n");
    if(you.money<=0){
      printf("所持金がなくなりました.\n");
      break;
    }
    printf("Round%d\n",i+1);
    command(&you,&dice);
  }

  printf("_______________________________________________________________________________\n");
  if(i==MAX_ROUND){
    printf("%dRoundすべて終わりました.\n",MAX_ROUND);
  }
  add_comma(you.money,str);
  printf("最終的な所持金は%s円です.\n",str);
  display_highscore(you.money);
  printf("またのお越しをお待ちしております.\n");
  return 0;
}


void command(struct people *p,struct status *d){
  int settlement=p->money,rate=0,choice[2],command=0,count=0,continuation=0;
  roll_dice(d);
  while(1){
    count++;
    how_to_bet();
    printf("どの方法で賭けますか？(番号を入力してください): ");scanf("%d",&command);
    switch(command){
      case 0:
        printf("(0) Small(%d~%d)\n(1) Big(%d~%d)\n",MIN_SUM+1,(MIN_SUM+MAX_SUM)/2,(MIN_SUM+MAX_SUM)/2+1,MAX_SUM-1);
	choice_numbers(choice,1,0,1);
	bet_money(p);	
        rate = rate_small_or_big(choice[0],d);
        break;
      case 1:
	choice_numbers(choice,1,MIN_DICE,MAX_DICE);
	bet_money(p);
        rate = rate_single(choice[0],d);
        break;
      case 2:
	while(1){
          choice_numbers(choice,2,MIN_DICE,MAX_DICE);
	  if(choice[0]!=choice[1])
	    break;
	  printf("異なる数字を入力してください.");
        }
	bet_money(p); 
        rate = rate_pair(choice,d);
        break;
      case 3:
	printf("和が4or17:配当60倍\n　　5or16:配当30倍\n　　6or15:配当17倍\n　　7or14:配当12倍\n　　8or13:配当8倍\n　　9〜12:配当6倍\n");
	choice_numbers(choice,1,MIN_SUM+1,MAX_SUM-1);
	bet_money(p);
        rate = rate_total(choice[0],d);
        break;
      case 4:
	choice_numbers(choice,1,MIN_DICE,MAX_DICE);
	bet_money(p);
        rate = rate_double_repdigit(choice[0],d);
        break;
      case 5:
	bet_money(p);
        rate = rate_any_triple(d);
        break;
      case 6:
	choice_numbers(choice,1,MIN_DICE,MAX_DICE);
	bet_money(p);
        rate = rate_triple(choice[0],d);
        break;
    }
    p->money -= p->bet;
    settlement -= p->bet;
    settlement += p->bet * rate;
    printf("_______________________________________________________\n");
    if(count>10||p->money<QUOTA){
      printf("No more bet!\n");
      printf("_______________________________________________________\n");
      break;
    }
    printf("まだ賭けますか？\n");
    printf("(%d) NO\n(%d) YES\n%dか%dを入力してください.: ",NO,YES,NO,YES);scanf("%d",&continuation);
    printf("_______________________________________________________\n");
    if(continuation==NO)
      break;
    
  }
  print_result(d);
  if(settlement > p->money){
    add_comma(settlement-p->money,str);
    printf("配当金として%s円得ました!\n",str);
  }
  else if(settlement==p->money)
    printf("予想が外れてしまいました.");
  p->money = settlement;
  add_comma(p->money,str);
  printf("現在の所持金は%s円です.\n",str);
}

void roll_dice(struct status *p){
  int i;
  p->sum=0;
  p->flag_repdigit=YES;
  srand(time(NULL));
  for(i=0;i<PECIES_OF_DICE;i++){
    p->d[i]= rand() % MAX_DICE + MIN_DICE;
    p->sum += p->d[i];
    if(p->d[i]!=p->d[0])
      p->flag_repdigit=NO;
  }
}

void print_result(struct status *p){
  int i;
  printf("オープン!\n\n");
  for(i=0;i<PECIES_OF_DICE;i++)
    printf("%dつ目:%d\n",i+1,p->d[i]);
  printf("合計:%d\n",p->sum);
}

int rate_small_or_big(int c,struct status *d){
  if(d->flag_repdigit==YES)
    return 0;
  if((c==0&&d->sum>MIN_SUM&&d->sum<(MIN_SUM+MAX_SUM)/2)+1||(c==1&&d->sum>(MIN_SUM+MAX_SUM)/2&&d->sum<MAX_SUM))
    return 2;
  else
    return 0;
}

int rate_single(int c,struct status *d){
  int count=0,i;
  for(i=0;i<PECIES_OF_DICE;i++){
    if(d->d[i]==c)
      count++;
  }
  if(count!=0)
    count++;
  return count;
}

int rate_pair(int c[],struct status *d){
  int count=0,i,j;
  for(j=0;j<2;j++){
    for(i=0;i<PECIES_OF_DICE;i++){
      if(d->d[i]==c[j]){
        count++;
        break;
      }
    }
  }
  if(count==2)
    return 6;
  return 0;
}

int rate_total(int c,struct status *d){
  if(d->sum!=c)
    return 0;
  switch(d->sum){
    case 4: case 17:
      return 60;
    case 5: case 16:
      return 30;
    case 6: case 15:
      return 17;
    case 7: case 14:
      return 12;
    case 8: case 13:
      return 8;
    default:
      return 6;
  }
}

int rate_double_repdigit(int c,struct status *d){
  int count=0,i;
  for(i=0;i<PECIES_OF_DICE;i++){
    if(d->d[i]==c)
      count++;
  }
  if(count!=0)
    count++;
  return count;
}

int rate_any_triple(struct status *d){
  if(d->flag_repdigit==YES)
    return 30;
  return 0;
}

int rate_triple(int c,struct status *d){
  if(d->flag_repdigit==YES&&c==d->d[0])
    return 180;
  return 0;
}

void how_to_bet(void){
   printf("How to bet\n(0) Small or Big\n   3つのサイコロの和が大か小かを予想する.配当2倍.3つとも同じ目なら負け.\n");
  printf("(1) Single\n   3つのサイコロのうち,任意の1つの出目を予想する.n(>0)個一致すると,配当n+1倍.\n");
  printf("(2) Pair\n   3つのサイコロのうち,任意の2つの出目(ゾロ目以外)の組を予想する.配当6倍.\n");
  printf("(3) Total\n   3つのサイコロの和を予想する.配当は的中した和に応じる.\n");
  printf("(4) Double\n   3つのサイコロのうち,ゾロ目になる数字を予想する.配当11倍.\n");
  printf("(5) Any triple\n   3つのサイコロのうち,1～6のいずれかが3つとも同じ数字になれば勝ち.配当30倍.\n");
  printf("(6) Triple\n   3つのサイコロのうち,3つとも同じ出目になる数字を予想する.配当180倍.\n");
  
  printf("_______________________________________________________\n");
}

void bet_money(struct people *p){
  add_comma(p->money,str);
  printf("現在の所持金は%s円です。\n",str);
  while(1){
    add_comma(QUOTA,str);
    printf("いくら賭けますか？%s円以上の賭け額を入力してください.: ",str);scanf("%d",&p->bet);
    if(p->bet>p->money)
      printf("お金が足りません.\n");
    else if(p->bet<QUOTA){
      add_comma(QUOTA,str);
      printf("%s円以上賭けてください.\n",str);
    }
    else
      break;
  }
}

void print_introduce(void){
  printf("Sic-boの世界にようこそ!\n");
  printf("Sic-boとはサイコロの３つの和や数字,組み合わせを当てるゲームです.\n別名「大小（だいしょう）」や「タイサイ」とも呼ばれます.\nアジアを中心に絶大な人気があり,また世界各地でも広く行われています.\nシンプルなルールですので、カジノ初心者にも楽しみやすいゲームとなっています.\n\n");
  printf("ルールを説明します.\nSic-boのスタートは自分が賭けることからはじまるため,まずは「3つのサイコロは何の出目がでるか？」を予想します.\n予想が完了し,賭けたい数字や組み合わせが決まったら,賭ける方法・金額を決めます.「No more bet（ベットは終了です）」というディーラーの掛け声まで好きなだけ賭けられます.\n");
  printf("最後に,ディーラーが容器内にあるシャッフル済みの３つのサイコロをオープン.その出目とあなたの賭けたパターンを見比べ,勝敗を確定させます.\n\n");
}

void add_comma(int money,char str[MAXDIG]){
  int i=0,j,digit=0,tem;
  if(money==0){
    str[0]='0';
    str[1]=0;
  }
  else{
    while(money!=0&&i<MAXDIG){
      str[i++]=money%10+'0';
      digit++;
      money /= 10;
      if(digit%3==0&&money!=0)
        str[i++] = ',';
    }
    str[i] = 0;
  }  
  j = i-1;
  i = 0;
  while(j>i){
    tem = str[i];
    str[i] = str[j];
    str[j] = tem;
    i++;
    j--;
  }
}

void choice_numbers(int choice[],int count,int min,int max){
  int i;
  printf("%d~%dの中から%dつの数字を入力してください\n",min,max,count);
  for(i=0;i<count;i++){
    while(1){
      printf("%dつ目: ",i+1);scanf("%d",&choice[i]);
      if(choice[i]>=min&&choice[i]<=max)
	break;
      printf("%d~%dの中から選んでください\n",min,max);
    }
  }
}

void display_highscore(int money){
  FILE *f;
  int highscore;
  f=fopen("highscore.txt","r");
  if(f==NULL){
    fprintf(stderr,"highscore.txt cannot be opened\n");
    exit(1);
  }
  fscanf(f,"%d",&highscore);
  fclose(f);
  add_comma(highscore,str);
  printf("これまでのハイスコアは%s円です.\n",str);
  if(money>highscore){
    printf("ハイスコアを更新しました！\n");
    highscore=money;
    f=fopen("highscore.txt","w");
    fprintf(f,"%d",highscore);
    fclose(f);
  }
}
