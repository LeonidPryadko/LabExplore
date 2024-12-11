/**
 * @file labexplr.c 
 *
 * @brief LabExplore - a text mode labyrinth game 
 *
 * @author Leonid P. Pryadko (University of California, Riverside)
 *
 * Copyright (C) 2024 Leonid Pryadko
 * University of California, Riverside
 * All rights reserved.
 *
 */

#include "labexplr.h"
#include <termios.h>

/* get a single char from stdin    */
int getch(void)
{
   struct termios oldattr, newattr;
   int ch;
   tcgetattr(0, &oldattr);
   newattr=oldattr;
   newattr.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( 0, TCSANOW, &newattr);
   ch=getchar();
   tcsetattr(0, TCSANOW, &oldattr);
   return(ch);
}

char player[PMAX][30]={"A","B","C","D","E"}, name[30];
char DIR[PMAX][9]={
  "destroy", "build", "down", "  ", "left", "shoot", "right", "  ", "up"
};

players got,px,py,kla;
int size,nplay,kladov,num,mov,get,xp,yp;
char tpoint[8][20]= {"out", "plain", "forest", "arsenal", "exit",
  "delta","river", "hole"  };
field labx ,laby ,upw,rigw,place,klad ;    /* An array  */

#ifdef DOS
void beep(int i){sound(i);delay(100);nosound();}
#else
void beep(int i){fprintf(stderr,"\007");} 
#endif

void operate(void){ 
  switch (place[xp][yp]){
  case forest:
    if (kla[num]){
      printf("%9s",tpoint[place[xp][yp]]);kla[num]=0;putklad();
    }
    else{ 
      printf("%9s",tpoint[1]);
    }
    break;
  case arsenal: 
    printf("%9s",tpoint[place[xp][yp]]);
    get=7;
    break;
  case eexit:   
    printf("%9s",tpoint[place[xp][yp]]);
    if (kla[num]) {
      if (kladov==1){
        printf(" \n\nCongratulations! Treasure is true!\n"); 
        printf(" %s won!\n",player[num]); 
        exit(0);
      }
      else{
        printf(" Treasure is false!"); 
        kla[num]=0;
        kladov--;
      }}
    break;
  default:      
    printf("%9s",tpoint[place[xp][yp]]);
  }
  if (klad[xp][yp]){
    printf(" Treasure! ");
    if (kla[num]==0){
      klad[xp][yp]=0; kla[num]=1;
    }}
}

int readgoo(char ch){
  int dir;
  //  if(ch ==3)    ERROR("character number %d, terminating",ch);
#ifdef chardebug
  printf("entered: %i\n",ch);
#endif
  dir=ch-48;
  if ((dir==up)+(dir==down)+(dir==right)+(dir==left)){
    printf("%10s",DIR[dir]);
    trymove(dir);
    operate(); 
    return(1);
  }
  if((get>1)&& ((dir==build)||(dir==destroy)||(dir==shoot))){
    printf("%s",DIR[dir]);
    bds(dir);
    return(1);
  }
  beep(20);    
  //printf("can't go %i",dir); 
  return(0);
}

void trymove(int dir){
  int x,y;
  if (wall(xp,yp,dir)) {
    beep(5000);
    printf("  wall   ");
    return;
  }
  switch (dir){
  case up:    x=xp-1;y=yp;break;
  case down:  x=xp+1;y=yp;break;
  case left:  x=xp;y=yp-1;break;
  case right: x=xp;y=yp+1; break;
  default:
    ERROR("invalid direction %d",dir);
  }
  if ((place[x][y]==hole)+(place[x][y]==river)*
      ((xp!=labx[x][y])+(yp!=laby[x][y]))){
    xp=labx[x][y];
    yp=laby[x][y];
    return;
  }
  xp=x;yp=y;
}

void bds(int dir){ 
  int x,y,x1,y1,i,dir1,kill; int *poi;
  char ch;
  printf("\n      Direction  :> ");
  do {
    ch = getch();
    //    if(ch==3)      ERROR("ch=%d, terminating",ch);
    dir1=ch-48;
  } while
      (!((dir1==up)+(dir1==down)+(dir1==left)+(dir1==right)));
  printf("%10s",DIR[dir1]);
  get--;
  if ((dir==build)+(dir==destroy))    
    {
      switch (dir1)
        {
        case up:    poi=&upw[xp][yp];break;
        case down:  poi=&upw[xp+1][yp];break;
        case left:  poi=&rigw[xp][yp-1];break;
        case right: poi=&rigw[xp][yp]; break;
	default:
	  ERROR("invalid direction %d",dir1);
        }
      if (dir==destroy){
        if (*poi==1){printf("  Ok");*poi=0;}
        if (*poi==2){printf("  Outer");}
        if (*poi==0){printf("  Was no wall");}
      }
      if (dir==build){
        if (*poi)
          printf("  Already present");
        else{
          if (!flow(xp,yp,dir1)) {
            printf("    Ok");*poi=1;}
          if (flow(xp,yp,dir1))
            printf("  River flow");
        }
      }
      return;       
    }
  x=xp;y=yp;kill=0;
  while(!(wall(x,y,dir1)+kill)){movexy(&x1,&y1,x,y,dir1);x=x1;y=y1;
    for(i=0;i<nplay; i++){
      if ((i!=num)&&(px[i]==x)&&(py[i]==y))
	{
	  got[i]=-3;
	  px[i]=0;
	  py[i]=0;
	  if (kla[i]) {
	    kla[i]=0;
	    putklad();
	  }
	  printf("\n%s killed!",player[i]);kill=1;}}}
}

void putklad (void){
  int x,y;
  do getxyany(&x,&y); 
  while(klad[x][y]!=0);
  klad[x][y]=1;
}

void getxyany(int *x,int *y){
  do  {
    *x=1+rand_int(size);
    *y=1+rand_int(size);
  }
  while(place[*x][*y]==eexit);
}

void getxy(int *x,int *y){
  *x=1+rand_int(size);
  *y=1+rand_int(size);
  while(place[*x][*y]!=1)
    {
      *x=*x+1;
      if (*x>size+1){
	*x=1;
	*y=*y+1;
	if (*y>size+1)
	  *y=1;
      }
    }
}

int movexy(int *x1,int *y1,int x,int y,int dir) { /* dir 8 up,etc=num.keypad*/
  switch (dir){
  case up:    *x1=x-1;*y1=y;break;
  case down:  *x1=x+1;*y1=y;break;
  case left:  *x1=x;*y1=y-1;break;
  case right: *x1=x;*y1=y+1;
  }
  return (place[*x1][*y1]==1);
}

int flow(int x, int y, int dir){
  int ax=0,ay=0;
  if ((place[x][y]!=river)&&(place[x][y]!=delta))
    return(0);
  movexy(&ax,&ay,x,y,dir);
  if ((labx[x][y]==ax)&&(laby[x][y]==ay))
    return 1;
  if ((labx[ax][ay]==x)&&(laby[ax][ay]==y))
    return 1;
  return(0);
}

int goodri(int x,int y){   /*check closure ************************* par */
  return(!(wall(x,y,down) && wall(x,y,up) && wall(x,y,left)) &&
         (y!=size) && (!flow(x,y,right)));}/* ********************reka!!!!!!*/

int goodup(int x,int y){   /*check closure ************************* par */
  return(!(wall(x,y,down)&&wall(x,y,left) &&  wall(x,y,right)) &&
         (x!=1) && (!flow(x,y,up)));  /* *********************reka!!!!!!*/
}

int wall(int x, int y,int dir){
  int r=0;                     /*check presence of wall in given direction*/
  switch (dir){
  case up:
    r=upw[x][y];
    break;
  case down:
    r=upw[x+1][y];
    break;
  case left:
    r=rigw[x][y-1];
    break;
  case right:
    r=rigw[x][y];
    break;
  default:
    ERROR("invalid direction %d",dir);
  }
  return (r);
}



int main(void){
  int i,j,k,r,s;
  int  x, y,x1,y1;
  
  srand(time(NULL));  /* Initialize random number generator */  
  //  printf("\x1B[2J");
  printf("\nWelcome to Labyrinth!\n");
  //  beep(5000);
#if debug
  size=4;
  printf("start of the program.\n");
#else
  printf(" Size  : ");
  if (1 != scanf("%d", &size))
    return -1;
#endif
#ifdef STATS
  printf(" Initializing....");
#endif
  
  for (i=0;i<SMAX;i++)
    for (j=0;j<SMAX;j++){
      place[i][j] = 0;
      labx[i][j]=i;
      laby[i][j]=j ;
      upw[i][j]=0;  
      rigw[i][j]=0;
    };
  for (i=0;i<SMAX;i++){
    upw[1][i]=2;
    upw[size+1][i]=2;
    rigw[i][0]=2;
    rigw[i][size]=2;
  }
  for (i=1;i<=size;i++)
    for (j=1;j<=size;j++) 
      place[i][j] = 1;
  for (i=forest;i<=eexit;i++){
    getxy(&x,&y);place[x][y]=i;
  }
  r=rand_int(size )+size/2;      /*********************************** par*/
  // was: rand_int(size/2)+size/2
#ifdef STATS
  printf(" up to %i rivers...", r);
#endif  
  for (i=1,k=rand_int(size)+size/2;(k>0) && (i<=r);i++){             /* rivers */
    getxy(&x,&y),
      place[x][y]=delta;
    if (k<2) s=k; else s=rand_int(k)+1;
    k-=s;
    while(s>0){
      s--;
      if(movexy (&x1,&y1,x,y,rand_int(4)*2+2)){
        labx[x1][y1]=x;
        laby[x1][y1]=y;
        place[x1][y1]=river;
        x=x1;y=y1;
      }}}
  for (i=1,k=0;i<=size;i++)   /* k= number free sites*/
    for (j=1;j<=size;j++) 
      k+=(place[i][j]==1);
#if debug
  printf(" so far %i sites\n",k);
#endif

  k-= rand_int(min(k,rand_int(size )+size/2+1))+2;
  /* k= number holes*/    /*********************************** par*/  
  // was: rand_int(min(k,rand_int(size/2 )+size/2+1))+1;
#ifdef STATS
  printf(" %i holes...\n",k);
#endif
  while(k>0){
    if (k<=2) s=k; else s=rand_int(k-1)+2; /* s= cycle length*/
    k-=s;
    getxy(&x,&y);
    s--;
    xp=x;
    yp=y;
    place[x][y]=hole;
    while(s>0){
      s--; 
      getxy(&x1,&y1);
      labx[x1][y1]=x;
      laby[x1][y1]=y;
      place[x1][y1]=hole;
      x=x1;
      y=y1;
    };
    x1=xp;
    y1=yp;
    labx[x1][y1]=x;
    laby[x1][y1]=y;
  };
  for (i=size;i>0;i--)
    for (j=1;j<=size;j++){
      if(goodri(i,j)) 
        rigw[i][j]=(rand_int(99)<45);
      if (goodup(i,j)) 
        upw[i][j] = (rand_int(99)<45);
    }
  if (rigw[size-1][1] && upw[size][size-1])
    rigw[size-1][1]=0; 

#if debug
  nplay=1;
#else
  printf("  Number of players : ");
  if (1 != scanf("%d", &nplay))
    return -1;
  for (i=0;i<nplay;i++)
    {
      printf(" Player %d :",i+1);
#ifdef DOSOS
      scanf("%s",&player[i]);
#else
      if (1 != scanf("%s",player[i]))
	return -1;
#endif
    }
#endif

#if debug
  printf("setting up the treasures, "); 
#endif
  kladov=rand_int(size)+1+nplay;
#if debug
  printf("total of %i ",kladov);
#endif
  for(k=kladov;k>0;k--) 
    putklad();
#if debug
  printf("  ... done.\n");
  printf("beginning the game.\n");
#endif

  while(kladov)
    for(num=0;num<nplay;num++)
      if(got[num]<0){
        got[num]++;
      } 
      else{
        if (got[num]==0){
          getxyany(&xp,&yp);
          px[num]=labx[xp][yp];
          py[num]=laby[xp][yp];
          got[num]=1;
        }
        get=got[num];
        xp=px[num];
        yp=py[num];
        printf("\n\n %10s has %d units",player[num],get-1);
        if (kla[num]) 
          printf(" and a Treasure");
        for (mov=1;mov<=3;mov++){
          if ((place[xp][yp]!=forest)+(kla[num]==1))
            printf("\nMove %d : %10s in %10s ",
                   mov,player[num],tpoint[place[xp][yp]]);
          else 
            printf("\nMove %d : %10s in %10s ",
                   mov,player[num],tpoint[plain]);
#if debug 
          printf("at %i %i ",yp,xp);
#endif
          printf(":> ");
	  char ch;	  
          do{ 
	    ch = getch();
	  }
          while(!readgoo(ch));
        }
        got[num]=get;
        px[num]=xp;
        py[num]=yp;
      }
}
