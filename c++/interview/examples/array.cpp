#include <iostream>

#define MAX_X 6
#define MAX_Y 6
#define MAX_DEEP 4

using namespace std;

typedef char bmp[MAX_X][MAX_Y][MAX_DEEP];

void build(bmp a) {
  char value = '1';
  for(int i=0; i<MAX_X; i++) {
    for(int j=0; j<MAX_Y; j++) {
      a[i][j][0]=value;
      a[i][j][1]=value;
      a[i][j][2]=value;
      a[i][j][3]=value;
      value++;
    }
  }
}

void rotate(bmp x, bmp y) {
  for(int i=0; i<MAX_X; i++) {
    for(int j=0; j<MAX_Y; j++) {
      int ni = j;
      int nj = MAX_X-i-1;
      y[i][j][0] = x[ni][nj][0];
      y[i][j][1] = x[ni][nj][1];
      y[i][j][2] = x[ni][nj][2];
      y[i][j][3] = x[ni][nj][3];

    }
  }
}

void clean_row(int k, int x[MAX_X][MAX_Y]) {
  for( int j=0; j<MAX_X; j++ ){
    x[k][j]=0;
  }
}

void clean_column(int k, int x[MAX_X][MAX_Y]) {
  for( int i=0; i<MAX_X; i++ ){
    x[i][k]=0;
  }
}


void rec_scan(int i, int j, int x[MAX_X][MAX_Y]) {
  int tmp = x[i][j];
  if( i < MAX_X-1 ) {
    rec_scan(i+1, j, x);
  } else {
    if ( j < MAX_Y-1 ) {
      rec_scan(0, j+1, x);
    }
  }
  if (tmp==0) {  // deferred execution just after the matrix scan
    clean_row(i,x);
    clean_column(j,x);
  }
}

void zeroing(int x[MAX_X][MAX_Y]) {
  rec_scan(0,0,x);
}

void print(bmp x) {
  for(int i=0; i<MAX_X; i++) {
    for(int j=0; j<MAX_Y; j++) {
      cout << x[i][j][0] << "." << x[i][j][1] << ".";
      cout << x[i][j][2] << "." << x[i][j][3] << "  ";
    }
    cout << endl << "\n" << endl;
  }
}

void print(int x[MAX_X][MAX_Y]) {
  for(int i=0; i<MAX_X; i++) {
    for(int j=0; j<MAX_Y; j++) {
      cout << x[i][j] << "  ";
    }
    cout << endl << "\n" << endl;
  }
}

int main(int argn, char **argv) {
  bmp x,y;
  build(x);
  print(x);
  cout << "=====================================" << endl;
  rotate(x,y);
  print(y);
  int i[MAX_X][MAX_Y] = {{1,2,3,4,5,6},
			 {7,7,7,7,7,7},
			 {1,2,3,4,5,6},
			 {8,8,8,0,8,8},
			 {1,2,3,4,5,6},
			 {9,9,9,9,9,9}};
  zeroing(i);
  print(i);

}
