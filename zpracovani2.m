%% Dulezite procedury
clc; clear; close all;
%% nacitani vysledku
data=textread('vysledek.txt');
sizes=data(1,1:2);
mWidth=sizes(1);
mHeight=sizes(2);
data=data(2,1:mWidth*mHeight);
%% konstanty
x_exit=randi([1 mWidth],1,1);
y_exit=randi([1 mHeight],1,1);
%% zpracovani vysledku
data2=zeros(mHeight,mWidth);
for (i=1:mHeight)
    for (j=1:mWidth)
        data2(i,j)=data(j+(i-1)*mWidth);
    end
end

%% alalyza nejkratsi cesty od konce k zacatku
steps=[x_exit,y_exit];
x_closer=x_exit;
y_closer=y_exit;
while (y_closer~=1)||(x_closer~=1)
    
    right=9999;
    left=9999;
    top=9999;
    bottom=9999;
    
    if (x_closer<mWidth)
        right=data2(y_closer,x_closer+1);
    end
    if (x_closer>1)
        left=data2(y_closer,x_closer-1);
    end
    if (y_closer<mHeight)
        top=data2(y_closer+1,x_closer);
    end
    if (y_closer>1)
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

%% vykresloavni vysledku
I = imread('img.jpg');

[A,B,C]=size(I);
rectHeight=A/mHeight;
rectWidth=B/mWidth;

figure(1)
for (i=1:mHeight)
    for (j=1:mWidth)
          if j==x_exit && i==y_exit
              I =insertShape(I,'FilledRectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","red");
          elseif i==1 && j==1
              I =insertShape(I,'FilledRectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","green");
          elseif data2(i,j)>=500
              I =insertShape(I,'FilledRectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","black");
          else
              I =insertShape(I,'Rectangle',[(j-1)*rectWidth (i-1)*rectHeight rectWidth rectHeight],'LineWidth',5,"Color","black");
          end
     end
end

 [D,E]=size(steps);
 for (i=1:(D-1))
     I =insertShape(I,'Line',[steps(i,1)*rectWidth-rectWidth/2 steps(i,2)*rectHeight-rectHeight/2 steps(i+1,1)*rectWidth-rectWidth/2 steps(i+1,2)*rectHeight-rectHeight/2 ],'LineWidth',5,"Color","blue");
 end

imshow(I);