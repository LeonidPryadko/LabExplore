#ifndef LABEXPLR_H
#define LABEXPLR_H
/**
 * @file labexplr.h 
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

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include <stdlib.h>
#include <stdio.h>

#define min(a,b) (a<b?a:b)

#include <time.h>

#define SMAX 10             /* max size */
#define PMAX 10             /* max number of players */
#define debug 0
#define STATS
  typedef int field[SMAX][SMAX];
  typedef int players[PMAX+1];
  enum fields { out, plain, forest, arsenal, eexit, delta, river, hole };
  enum directions {up=8,down=2,right=6,left=4,shoot=5,build=1,destroy=0,quit=9};

  void getxy(int *x,int *y);
  void getxyany(int *x,int *y);
  void putklad (void);
  int readgoo(char ch);
  void trymove(int dir);
  //void beep(int i);
  void bds(int dir);      /* 
			   *  build,destroy,shoot 
			   */
  int movexy(int *x1,int *y1,int x,int y,int dir); /* 
						    *  dir 8 up,etc=num.keypad
						    */
  int goodup(int x,int y);   /* 
			      * check closure up from the point 
			      */
  int goodri(int x,int y);   /* 
			      * check closure right from the point 
			      */
  int wall(int x, int y,int dir); /* 
				   * check presence of a wall in given
				   * direction 
				   */

  static int rand_int(int max){
    return rand() % max;
  }
  
  int flow(int x, int y, int dir);

#define ERROR(fmt,...)							\
  do{									\
    fprintf (stderr, "%s:%d: *** ERROR in function '%s()' ***\n", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr,"[31;1m " fmt " [0m \n",##__VA_ARGS__);		\
    /*    printf(" " fmt " \n",##__VA_ARGS__); fflush(stdout); */	\
    exit(-1);								\
  }									\
  while(0)
  
#ifdef __cplusplus
}
#endif

#endif /* LABEXPLR_H */
