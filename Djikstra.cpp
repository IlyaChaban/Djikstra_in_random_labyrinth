#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
using std::ofstream;

int minimumDist(int dist[], int visit[], int size) {
	int min=9999,index;

	for(int i=0; i<size; i++) {
		if(visit[i]==0 && dist[i]<=min) {
			min=dist[i];
			index=i;
		}
	}
	return index;
}


int main() {

// Constants
	int mHeight=15;
	int mWidth=25;
	int x_start_point=2;
	int y_start_point=2;
	int wall=500;
	int connection=1;
// checking if everything is fine
	if (x_start_point>mHeight-1 or y_start_point>mWidth-1) {
		return 1;
	}
	x_start_point=x_start_point-1;
	y_start_point=y_start_point-1;
// Generating map of sizes  mHeight x mWidth
	int  map[mWidth*mHeight][mWidth*mHeight];//creating matrix of zeroes
	for (int y=0; y<mWidth*mHeight; y++) {
		for (int x=0; x<mWidth*mHeight; x++) {
			map[x][y]=0;
		}
	}



	for (int y=0; y<mHeight; y++) {
		for (int x=0; x<mWidth; x++) {

			if (x<mWidth-1) {
				map[(x) + (y)*mWidth][((x)+1) + (y)*mWidth  ]=wall; // creating right connection
			}
			if (x>0) {
				map[(x) + (y)*mWidth][((x)-1) + (y)*mWidth  ]=wall; // creating left connection
			}
			if (y<mHeight-1) {
				map[(x) + (y)*mWidth][(x)     + (y+1)*mWidth]=wall; // creating upper connection
			}
			if (y>0) {
				map[(x) + (y)*mWidth][(x)     + (y-1)*mWidth]=wall; // creating bottom connection
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//generation maze
	srand (time(0));

	int visit[mWidth][mHeight];//nothing is visited
	for (int y=0; y<mHeight; y++) {
		for (int x=0; x<mWidth; x++)
			visit[x][y]=0;
	}

	int start_point	=(x_start_point) + (y_start_point)*mWidth;
	int now_point	=start_point;
	int x_now_point	=x_start_point;
	int x_try		=x_start_point;
	int y_now_point	=y_start_point;
	int y_try		=y_start_point;

	int stack[mWidth*mHeight][2];
	int random_number;
	int counter=0;

	int now_try;
	bool flag=false;

	stack[counter][0]=x_start_point;
	stack[counter][1]=y_start_point;
	visit[y_start_point][x_start_point]=1;

	while (flag!=true) {

		if(y_now_point+1<=mHeight-1 && visit[y_now_point+1][x_now_point]==0 or y_now_point-1>=0 && visit[y_now_point-1][x_now_point]==0 or x_now_point-1>=0 && visit[y_now_point][x_now_point-1]==0 or x_now_point+1<=mWidth-1 && visit[y_now_point][x_now_point+1]==0) {

			random_number= rand() %4;
			x_try=x_now_point;
			y_try=y_now_point;
			switch (random_number) {

				case 0://check right
					x_try=x_now_point+1;
					break;

				case 1://check left
					x_try=x_now_point-1;
					break;

				case 2://check bottom
					y_try=y_now_point+1;
					break;

				case 3://check top
					y_try=y_now_point-1;
					break;
			}

			now_try=(x_try) + (y_try)*mWidth;

			if (map[now_point][now_try]!=0 && visit[y_try][x_try]==0) {

				counter++;
				int remeberx=x_now_point;
				int remebery=y_now_point;
				int rememberpos=(remeberx) + (remebery)*mWidth;
				
				switch (random_number) {
					case 0://go right
						x_now_point=x_now_point+1;
						break;

					case 1://go left
						x_now_point=x_now_point-1;
						break;

					case 2://go bottom
						y_now_point=y_now_point+1;
						break;

					case 3://go top
						y_now_point=y_now_point-1;
						break;
				}

				
				now_point=((x_now_point)) + (y_now_point)*mWidth;
				map[rememberpos ][now_point ]=connection;
				map[now_point ][rememberpos ]=connection;
				visit[y_now_point][x_now_point]=1;

				stack[counter][0]=x_now_point;
				stack[counter][1]=y_now_point;				
			}
		}

		else {

			counter--;
			x_now_point=stack[counter][0];
			y_now_point=stack[counter][1];
			now_point=((x_now_point)) + (y_now_point)*mWidth;

			for (int y=0; y<mHeight; y++) {
				for (int x=0; x<mWidth; x++) {
					if(visit[y][x]==0) {
						flag=false;
						goto OUT;
					} else {
						flag=true;
					}
				}
			}
			OUT:;
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//make matrix symmetric
	for (int y=0; y<mHeight*mWidth; y++) {
		for (int x=0; x<mHeight*mWidth; x++) {
			if ( map[x][y] ==500 ) {
				map[y][x]=500;
				map[x][y]=500;
			}
		}
	}

	// print map
	ofstream myfile2;
	myfile2.open ("map.txt");
	for (int i=0; i<mWidth*mHeight; i++) {
		for (int j=0; j<mWidth*mHeight; j++) {
			myfile2 << map[i][j] <<"\t";
		}
		myfile2 << std::endl;
	}
	myfile2.close();

//	Start Dijkstra
	int visited[mWidth*mHeight];
	int dist[mWidth*mHeight]; //distance to everywhere is infinite
	for (int y=0; y<mWidth*mHeight; y++) {
		dist[y]=9999;
		visited[y]=0;
	}

	dist[start_point]=0;                            //start point is 0m away from start point exactly

//examining nodes from first to last
	for (int i=start_point; i<mHeight*mWidth; i++) {
		int m=minimumDist(dist,visited,mHeight*mWidth);   // min distance search
		visited[m]=1;                                 // visit point which is the closest

		for (int x=0; x<mHeight*mWidth; x++) {

			if(visited[x] == 0 && map[m][x]!=0 && dist[m]!=9999 && dist[m]+map[m][x]<dist[x] ) { // if point is not visited && if distance ot it not infinite && it is shorter than now && it is avilable to go
				dist[x]=dist[m]+map[m][x];
			}

		}

	}
	int backward_constant=start_point;

	if (start_point>0) {

		while (backward_constant>-1) {
			int m=minimumDist(dist,visited,mHeight*mWidth);   // min distance search
			visited[m]=1;                                 // visit point which is the closest

			for (int x=0; x<mHeight*mWidth; x++) {

				if(visited[x] == 0 && map[m][x]!=0 && dist[m]!=9999 && dist[m]+map[m][x]<dist[x] ) { // if point is not visited && if distance ot it not infinite && it is shorter than now && it is avilable to go
					dist[x]=dist[m]+map[m][x];
				}
				backward_constant--;
			}

		}
	}

	ofstream myfile;
	myfile.open ("result.txt");
	myfile << mWidth <<" " << mHeight<< " " << x_start_point<<" " << y_start_point<<std::endl;
	for (int i=0; i<mHeight*mWidth; i++) {
		myfile << dist[i] <<" ";
	}
	myfile.close();



	return 0;
}
