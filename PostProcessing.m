%% Important stuff
clc; clear; close all;
%% Reading results
data=textread('result.txt');
%some information about the map and start point
sizes=data(1,1:4);
mWidth=sizes(1);
mHeight=sizes(2);
x_start=sizes(3)+1;
y_start=sizes(4)+1;
data=data(2,1:mWidth*mHeight);
%picking up information about map
map=textread('map.txt');
map=map(1:mHeight*mWidth,1:mHeight*mWidth);
counter=1;
walls=[0,0];
% pulling out information about the walls
for(i=1:mHeight*mWidth)
    for(j=1:mHeight*mWidth)
        if (map(i,j)==500)
            walls(counter,:)=[i,j];
            counter=counter+1;
        end
    end
end

%% Constants wich define exit from the maze
x_exit=randi([1 mWidth],1,1);
y_exit=randi([1 mHeight],1,1);
%% Formatting data to be comfortable to read
data2=zeros(mHeight,mWidth);
for (i=1:mHeight)
    for (j=1:mWidth)
        data2(i,j)=data(j+(i-1)*mWidth);
    end
end
%% Analyze the shortest way out
steps=[x_exit,y_exit];
x_closer=x_exit;
y_closer=y_exit;

while (x_closer~=x_start)||(y_closer~=y_start)
    
    right=9999;
    left=9999;
    top=9999;
    bottom=9999;
    
    if (x_closer<mWidth  && map((x_closer)+(y_closer-1)*mWidth,x_closer+1+(y_closer-1)*mWidth)~=500)
        right=data2(y_closer,x_closer+1);
    end
    if (x_closer>1       && map(x_closer+(y_closer-1)*mWidth,x_closer-1+(y_closer-1)*mWidth)~=500)
        left=data2(y_closer,x_closer-1);
    end
    if (y_closer<mHeight && map(x_closer+(y_closer-1)*mWidth,x_closer+((y_closer-1)+1)*mWidth)~=500)
        top=data2(y_closer+1,x_closer);
    end
    if (y_closer>1       && map(x_closer+(y_closer-1)*mWidth,x_closer+(y_closer-1-1)*mWidth)~=500)
        bottom=data2(y_closer-1,x_closer);
    end
    
    [step,index]=min([right,left,top,bottom]);
    
    if index == 1
        x_closer=x_closer+1;
    end
    if index == 2
        x_closer=x_closer-1;
    end
    if index == 3
        y_closer=y_closer+1;
    end
    if index == 4
        y_closer=y_closer-1;
    end
    
    if x_closer<1 || x_closer>mWidth || y_closer<1 || y_closer>mHeight
        fprintf('nelze z tohoto bodu projit bludiste robot narazil na hranici mapy')
        break;
    end        
    
    steps=[steps; x_closer,y_closer];        
end

%% Drawing result 
I = imread('img.jpg');

[A,B,C]=size(I);
rectHeight=A/mHeight;
rectWidth=B/mWidth;
%drawing field
figure(1)
for (i=1:mHeight)
    for (j=1:mWidth)
          if j==x_exit && i==y_exit
              I =insertShape(I,'FilledRectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","red");
          elseif i==y_start && j==x_start
              I =insertShape(I,'FilledRectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","green");
          end
     end
end
%drawing walls
[walls_size,nothing]=size(walls);

for (i=1:walls_size)
    wall1=coordinate_transformation(walls(i,1),mWidth);
    wall1x=wall1(1);
    wall1y=wall1(2);
    wall2=coordinate_transformation(walls(i,2),mWidth);
    wall2x=wall2(1);
    wall2y=wall2(2);
    middle_wall_x= (((wall1x+wall2x))/2-1)*rectWidth;
    middle_wall_y= (((wall1y+wall2y))/2-1)*rectHeight;
    
    if(abs(walls(i,1)-walls(i,2))>1)
        %draw horizontal wall       
        I =insertShape(I,'Line',[middle_wall_x ...
                                 middle_wall_y+0.5*rectHeight ...
                                 middle_wall_x+rectWidth ...
                                 middle_wall_y+0.5*rectHeight ...
                                 ],'LineWidth',5,"Color","black");
        
    elseif(abs(walls(i,1)-walls(i,2))==1)
        %draw vertical wall
        I =insertShape(I,'Line',[middle_wall_x+0.5*rectWidth ...
                                 middle_wall_y ...
                                 middle_wall_x+0.5*rectWidth ...
                                 middle_wall_y+rectHeight ...
                                 ],'LineWidth',5,"Color","black");
        
    end
end
%drawing the shortest path
[D,E]=size(steps);
 for (i=1:(D-1))
     I =insertShape(I,'Line',[steps(i,1)*rectWidth-rectWidth/2 steps(i,2)*rectHeight-rectHeight/2 steps(i+1,1)*rectWidth-rectWidth/2 steps(i+1,2)*rectHeight-rectHeight/2 ],'LineWidth',5,"Color","blue");
     imshow(I);
     %creating animation gif
     filename='vysledek.gif';
     frame = getframe(1); 
      im = frame2im(frame); 
      [imind,cm] = rgb2ind(im,256); 
      % Write to the GIF File 
      if i == 1 
          imwrite(imind,cm,filename,'gif', 'Loopcount',inf); 
      else 
          imwrite(imind,cm,filename,'gif','WriteMode','append'); 
      end 
 end
