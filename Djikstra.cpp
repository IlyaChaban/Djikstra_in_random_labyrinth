#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include<ctime>
using std::ofstream;

int minimumDist(int dist[], int visit[], int size) 
{
	int min=9999,index;
              
	for(int i=0;i<size;i++) 
	{
		if(visit[i]==0 && dist[i]<=min)      
		{
			min=dist[i];
			index=i;
		}
	}
	return index;
}


int main()
{
    
// Constants
int mHeight=10;
int mWidth=10;
int chanse=8;
int start_point=0;

// Generating map of sizes  mHeight x mWidth
int  map[mWidth*mHeight][mWidth*mHeight];//creating matrix of zeroes
for (int y=0; y<mWidth*mHeight; y++){
	for (int x=0; x<mWidth*mHeight; x++){
	map[x][y]=0;
	}		
}


srand(time(0));
for (int y=0; y<mHeight; y++){
	for (int x=0; x<mWidth; x++){
		
		int random_number = (rand() %11);
				
		if(random_number<chanse) {
			if (x<mWidth-1){
           		map[(x) + (y)*mWidth][((x)+1) + (y)*mWidth  ]=1; // creating right connection
			}
       		if (x>0){
       	    	map[(x) + (y)*mWidth][((x)-1) + (y)*mWidth  ]=1; // creating left connection
       	 	}
			if (y<mHeight-1){
        	    map[(x) + (y)*mWidth][(x)     + (y+1)*mWidth]=1; // creating upper connection
       	 	}
			if (y>0){
        	    map[(x) + (y)*mWidth][(x)     + (y-1)*mWidth]=1; // creating bottom connection
       		}
		}
		
		if(random_number>=chanse) {
			if (x<mWidth-1){
           		map[(x) + (y)*mWidth][((x)+1) + (y)*mWidth  ]=500; // creating right connection for obstacle
			}
       		if (x>0){
       	    	map[(x) + (y)*mWidth][((x)-1) + (y)*mWidth  ]=500; // creating left connection for obstacle
       	 	}
			if (y<mHeight-1){
        	    map[(x) + (y)*mWidth][(x)     + (y+1)*mWidth]=500; // creating upper connection for obstacle
       	 	}
			if (y>0){
        	    map[(x) + (y)*mWidth][(x)     + (y-1)*mWidth]=500; // creating bottom connection for obstacle
       		}
		}
	}   
}

for (int y=0; y<mHeight*mWidth; y++){
	for (int x=0; x<mHeight*mWidth; x++){
		if ( map[x][y] ==500 ){
			map[y][x]=500;
			map[x][y]=500;
		}	
	}
}

	
// vypis mapu
ofstream myfile2;
myfile2.open ("mapa.txt");
for (int i=0; i<mWidth*mHeight; i++){
	for (int j=0; j<mWidth*mHeight; j++){
		myfile2 << map[i][j] <<"\t";
	}
	myfile2 << std::endl;
}
myfile2.close();

//	Start Dijkstra
int dist[mWidth*mHeight]; //distance to everywhere is infinite
for (int y=0; y<mWidth*mHeight; y++){
	dist[y]=9999;
}

dist[start_point]=0;                            //start point is 0m away from start point exactly

int visit[mWidth*mHeight];//nothing is visited

for (int y=0; y<mWidth*mHeight; y++){
	visit[y]=0;
}
//examining nodes from first to last
	for (int i=0; i<mHeight*mWidth; i++){
	    int m=minimumDist(dist,visit,mHeight*mWidth);   // min distance search
	    visit[m]=1;                                 // visit point which is the closest
	    
	    for (int x=0; x<mHeight*mWidth; x++){
	        
	        if(visit[x] == 0 && map[m][x]!=0 && dist[m]!=9999 && dist[m]+map[m][x]<dist[x] ){ // if point is not visited && if distance ot it not infinite && it is shorter than now && it is avilable to go
	            dist[x]=dist[m]+map[m][x];
	        }
			
	    }
	        
	}

ofstream myfile;
myfile.open ("vysledek.txt");
myfile << mWidth <<" " << mHeight<< std::endl;
for (int i=0; i<mHeight*mWidth; i++){
	myfile << dist[i] <<" ";
}
myfile.close();



return 0;
}
