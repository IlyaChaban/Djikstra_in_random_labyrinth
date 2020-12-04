#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
using std::ofstream;

// Defining finction wich will find the sortest path thru the cloest and available points
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
	int mHeight=25;
	int mWidth=25;
	int x_start_point=2;
	int y_start_point=2;
	int wall=500;
	int connection=1;
// Checking that uset inputed coordinates wich is inside the borders
	if (x_start_point>mHeight-1 or y_start_point>mWidth-1) {
		return 1;
	}
// Conversion between mechanical and C++ coordinates
	x_start_point=x_start_point-1;
	y_start_point=y_start_point-1;

// Generating empty map of sizes  mHeight x mWidth
	int  map[mWidth*mHeight][mWidth*mHeight];//creating matrix of zeroes
	for (int y=0; y<mWidth*mHeight; y++) {
		for (int x=0; x<mWidth*mHeight; x++) {
			map[x][y]=0;
		}
	}

// Creating connection between elements
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

// Generation maze
	srand (time(0));
// Creating matrix of unvisited points
	int visit[mWidth][mHeight];// Nothing is visited
	for (int y=0; y<mHeight; y++) {
		for (int x=0; x<mWidth; x++)
			visit[x][y]=0;
	}
// Defining constants that will be used after
	int start_point	=(x_start_point) + (y_start_point)*mWidth;// Conversion from 2D to 1D for start point
	int now_point	=start_point;// Defininng that now we are at the start point
	int x_now_point	=x_start_point;
	int x_try		=x_start_point;// Just to make sure that it will be something not random
	int y_now_point	=y_start_point;
	int y_try		=y_start_point;// Just to make sure that it will be something not random

	int stack[mWidth*mHeight][2];// Defining stack to follow the flow of movement
	int random_number;
	int counter=0;

	int now_try;// 1D coordinates of now point
	bool flag=false;// flag for following situation about visited points

// First point is insterted as first in stack and alsa marked as visited
	stack[counter][0]=x_start_point;
	stack[counter][1]=y_start_point;
	visit[y_start_point][x_start_point]=1;

// Starting to generate a maze
	while (flag!=true) {

// If coordinates are in border and it has at least one neighbour
		if(y_now_point+1<=mHeight-1 && visit[y_now_point+1][x_now_point]==0 or y_now_point-1>=0 && visit[y_now_point-1][x_now_point]==0 or x_now_point-1>=0 && visit[y_now_point][x_now_point-1]==0 or x_now_point+1<=mWidth-1 && visit[y_now_point][x_now_point+1]==0) {

// Choose random direction and try to go there
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

// If point is not visited and it is connected with the preview
			if (map[now_point][now_try]!=0 && visit[y_try][x_try]==0) {

// Recors the previous point 1D coordinates
				counter++;
				int remeberx=x_now_point;
				int remebery=y_now_point;
				int rememberpos=(remeberx) + (remebery)*mWidth;

// Going to the direction that is already checked
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

// The point that we stepped into become now point
				now_point=((x_now_point)) + (y_now_point)*mWidth;

// Destroying the wall between the points
				map[rememberpos][now_point]=connection;
				map[now_point][rememberpos ]=connection;

// Mark point as visited
				visit[y_now_point][x_now_point]=1;

// Print matrix of visited points to follow situation
				ofstream visitf;
				visitf.open ("visit_matrix.txt");
				for (int i=0; i<mHeight; i++) {
					for (int j=0; j<mWidth; j++) {
						visitf << visit[i][j] <<"\t";
					}
					visitf << std::endl;
				}
				visitf.close();

// Write information to the stack to follow the steps
				stack[counter][0]=x_now_point;
				stack[counter][1]=y_now_point;
			}
		}

		else {

// In situation when we don't have not visited points around we need to go back step by step while not visited point appear
			counter--;
			x_now_point=stack[counter][0];
			y_now_point=stack[counter][1];

// Previous point from the stack become realtime position
			now_point=((x_now_point)) + (y_now_point)*mWidth;

// Checking that we have all existing points has already been visited
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
OUT:
			;
		}
	}

// Just to make sure that the matrix is symmetric symmetric (that means that the way 1 -> 2 and 2 -> 1 is the same)
	for (int y=0; y<mHeight*mWidth; y++) {
		for (int x=0; x<mHeight*mWidth; x++) {
			if ( map[x][y] ==500 ) {
				map[y][x]=500;
				map[x][y]=500;
			}
		}
	}

// Printing map for visualization
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
	int dist[mWidth*mHeight];
	for (int y=0; y<mWidth*mHeight; y++) {
		dist[y]=9999;// To make sure that we will find the shortest path it need to define that the distance from the start is "infinite"
		visited[y]=0;// Marking everything as unvisited
	}

	dist[start_point]=0;// Start point is 0m away from start point...  exactly

// Examining nodes from first to last
	for (int i=start_point; i<mHeight*mWidth; i++) {

		int m=minimumDist(dist,visited,mHeight*mWidth);   // Min distance search
		visited[m]=1;                                 // Visit point which is the closest

		for (int x=0; x<mHeight*mWidth; x++) {

			if(visited[x] == 0 && map[m][x]!=0 && dist[m]!=9999 && dist[m]+map[m][x]<dist[x] ) { // if point is not visited && if distance ot it not infinite && it is shorter than now && it is avilable to go
				dist[x]=dist[m]+map[m][x];
			}
		}
	}

// If we begin algorithm not from point 0 or in 2D (0,0) we will need to examen all the noded that has already been before it
	int backward_constant=start_point;

	if (start_point>0) {

		while (backward_constant>-1) {
			int m=minimumDist(dist,visited,mHeight*mWidth);   // Min distance search
			visited[m]=1;                                 // Visit point which is the closest

			for (int x=0; x<mHeight*mWidth; x++) {

				if(visited[x] == 0 && map[m][x]!=0 && dist[m]!=9999 && dist[m]+map[m][x]<dist[x] ) { // if point is not visited && if distance ot it not infinite && it is shorter than now && it is avilable to go
					dist[x]=dist[m]+map[m][x];
				}
				backward_constant--;
			}

		}
	}

// Printing results to the file
	ofstream myfile;
	myfile.open ("result.txt");
	myfile << mWidth <<" " << mHeight<< " " << x_start_point<<" " << y_start_point<<std::endl;
	for (int i=0; i<mHeight*mWidth; i++) {
		myfile << dist[i] <<" ";
	}
	myfile.close();

	return 0;
}
