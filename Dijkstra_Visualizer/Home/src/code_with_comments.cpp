#include<graphics.h>          //old Borland Graphics Interface, BGI
#include<time.h>
#include<bits/stdc++.h>
#include<string>

using namespace std;

// array of vectors representing adjacency list, having 38*38=1444 vectors representing each node
vector<int> adja[38 * 38];
                   
//The startx, starty, endx, and endy variables are used to store the pixel coordinates where the user clicks to set the start and end nodes. These coordinates are then converted to node indices (start_node_index and end_node_index) using the grid configuration.
int startx = 0, starty = 0, endx = 0 , endy = 0 , dis = 0;
int start_node_index = 0;
int end_node_index = 0;
//start_of_maxtrix_x and start_of_maxtrix_y define where the grid begins on the graphical window in terms of pixels. The gap_lines variable defines the spacing between the lines in the grid, allowing the code to draw the grid correctly and map the pixel coordinates to logical nodes.
int start_of_maxtrix_x = 10;
int start_of_maxtrix_y = 10;
int gap_lines = 20;
int num_columns = 38;
//record_obstacles stores the indices of nodes that are obstacles. The Dijkstra algorithm will skip these nodes while searching for the shortest path.
set<int>record_obstacles;

//this function converts the graph node to graphical position to highlight
// the node number are derived as same as given 3*3 matrix and nodes are:           0 1 2
//                                                                                  3 4 5
//                                                                                  6 7 8
void nodes_to_map_points(int node,int color,int del)
{
	
	int y = node/38;                //y represents row
	int x = node%38;                // x represents column
	y = (y * 20) + 10;              // finding pixel position, +10 for adjustment due to grid starting pos and *20 as each cell is of 20 pixels
	x = (x * 20) + 10;              // finding pixel position
	setfillstyle(1,color);          //used to change the color and type of style used
	delay(del);                     // add delay for smoother transition 
	bar(x+5, y+5 , x + 15 , y + 15); // make rectangle filled block at position x1,y1,x2,y2
	setfillstyle(1,5);               //set the color to default,5=Magenta
	bar(endx+5, endy+5 , endx + 15 , endy + 15);       // Highlight the end node with the default color (5)Magenta
	
}


//this functions converts the x,y cursor positions to the appropriate node
// This function is typically used to map the position of a cursor (e.g., when a user clicks on a grid) to the corresponding node in the graph. This allows the program to identify which node the user has interacted with based on the cursor's position.
int map_points_to_nodes(int x, int y , int gap_lines , int num_columns)
{
   x-=10;
   y-=10;
   
   x = x / gap_lines;
   y = y / gap_lines;
   
  // cout<<"Gap of Lines passed is "<<gap_lines<<" dabba is: "<<((y * num_columns) + x)<<endl;
   return ((y * num_columns) + x);     // calculate the node number with the help of row number(y) and col number(x) in grid
   
}



void add_obstacles( int start, int end) // passing the staring and end nodes, to not consider then as obstacles nodes 
{
	
	//font,direction,character size
	settextstyle(1, HORIZ_DIR,2);
	//x coordinate, y coordinate , test To Display
	outtextxy(800, 20 + 20, "SELECT THE ");
	outtextxy(800, 20 + 20 + 20 + 10, "OBSTACLES");
	outtextxy(800, 20 + 20 + 20 + 20 + 20, "NODES    ");
	outtextxy(800, 20 + 20 + 20 + 20 + 60, "Right Click");
	outtextxy(800, 20 + 20 + 20 + 20 + 80, "To Finish");
	outtextxy(800, 20 + 20 + 20 + 20 + 100, "Obs. Selection");
	
	
	POINT cursorPosition;  //POINT is a structure which represnets a x and y pixel coordinate of a point on screen
	int mx = 0,my = 0;
	int count = 0;
	//map<int,int> mymap;
	while(1)
	 {
	 		 
	 GetCursorPos(&cursorPosition);      //this function gets the cursor position on graphics screen and store it into POINT

	 	// The subtraction of 10 and 30 is necessary to account for specific offsets between the screen coordinates and the internal drawing coordinates of your application. These offsets ensure that user interactions with the cursor are correctly aligned with the graphical elements on the screen.
	 	mx = cursorPosition.x - 10; 
	 	my = cursorPosition.y - 30;  
	 	
	 //	cout<<mx<<" "<<my<<endl;
	 	
	 	if(GetAsyncKeyState(VK_LBUTTON))         //Handeling when left mouse button is clicked
		 {

		 	int tempx = mx - start_of_maxtrix_x ; //geting the cursor position with respect to grid
		 	int tempy = my - start_of_maxtrix_y ;
			if(tempx >= gap_lines)
			{
			  tempx = tempx / gap_lines;           //geting the cursor position at node
			  tempx = (tempx * gap_lines) + start_of_maxtrix_x;    // Accurately tranforming node position to graphic pixel position to change the colour
		    }
            //If tempx or tempy is less than gap_lines, the cursor is near the top or left edge of the grid. In this case, it defaults to 10, ensuring it snaps to the first grid cell.
		    else
		    {
		    	tempx = 10;
			}
			  
			if(tempy >= gap_lines)
			 {
			   tempy = tempy / gap_lines;
			   tempy = (tempy* gap_lines) + start_of_maxtrix_y;
		     }
		     else
		     {
		     	tempy = 10;
			 }	
		
			if(tempx < 770 && tempy < 770)            // cheching that the cursor is within the boundries if the grid
			   {  
			      setfillstyle(1,15); //used to change the color and type of style used, 15 for white,1 for solid fill style
			      
			      int node_index = map_points_to_nodes(tempx, tempy , gap_lines, num_columns);
			      if(node_index != start_node_index && node_index!= end_node_index)
				   {
				      bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
			          record_obstacles.insert(node_index);
			      }
			     //cout<<"Obstacle size is :"<<record_obstacles.size()<<endl;
			
			}
		 } 
		 else if(GetAsyncKeyState(VK_RBUTTON))  //handeling right click
				{
						settextstyle(1, HORIZ_DIR,2);
	                    //x coordinate, y coordinate , test To Display
                        //These lines clear the previous text displayed by overwriting it with blank spaces.
                    	outtextxy(800, 20 + 20, "                                       ");
						outtextxy(800, 20 + 20 + 20 + 10, "                            ");
						outtextxy(800, 20 + 20 + 20 + 20 + 20, "                        ");
						outtextxy(800, 20 + 20 + 20 + 20 + 60, "                         ");
						outtextxy(800, 20 + 20 + 20 + 20 + 80, "                          ");
						outtextxy(800, 20 + 20 + 20 + 20 + 100, "                          ");
                        //These lines display new text at specified positions
						outtextxy(800, 20 + 20, "Visualizing");
	 					outtextxy(800, 20 + 20 + 20 + 10, "Dijkstra");
	 					outtextxy(800, 20 + 20 + 20 + 20 + 20, "Algorithm");
	 					outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "Select");
				    return;}
	 	delay(2);
	 }
	
}




//to highlight the path in the red boxes
void path_color(vector<int> path)
{
	for(int i = 0 ;i < path.size() ;i++)
	{
		nodes_to_map_points(path[i],4,10);    //path[i] is node, 4 is colour that is RED, and 10 is delay in ms
	}
}

//heart of the project //Dijkstra Algorithm-------------------------------------------------------------------------------------------
bool dijkstra(int start, int ends, int num_of_nodes)
{
	//< weight, Node > Pair 
   	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> minheap;    
   	vector<int> parent_track(num_of_nodes, -1);            //path_arrary, which will help us to find path
   	vector<int> S(38*38,INT_MAX);                          //distance_Array, which will tell us the shortest distance

    minheap.push({0,start});

	while(!minheap.empty())
	{ 
	   setfillstyle(1,3);
	   bar(startx+5, starty+5 , startx + 15 , starty + 15);     //highlighting start node
	   
	   setfillstyle(1,5);
	   bar(endx+5, endy+5 , endx + 15 , endy + 15);             //highlighting end node

	   pair<int,int> current=minheap.top();
       minheap.pop();
	   vector<int> adjacent_vertices = adja[current.second];

	   int parent_weight = current.first;
	   int parent = current.second;
	   S[parent]=parent_weight;

	   for(int i = 0 ; i < adjacent_vertices.size(); i++)
	   {
            if((1+parent_weight)<S[adjacent_vertices[i]])               // "1 is the cost of travelling to adjacent node"
            {
                S[adjacent_vertices[i]]=1+parent_weight;
                minheap.push({1+parent_weight,adjacent_vertices[i]});
                parent_track[adjacent_vertices[i]]=parent;
            }
       }
       nodes_to_map_points(parent,14,0);
      
	  if(parent == ends)
	    break;
	
    }
    if(S[ends]!=INT_MAX)
     { 
        int dist =S[ends];
       dis = dist;

	   int temp = parent_track[ends];
	   vector<int> path;
	   while( temp!= -1)
	 {  
	    path.push_back(temp);
	    temp = parent_track[temp];	
	 }
	  path.erase(path.end()-1);
	  std::reverse(path.begin(),path.end());
	
	/*for(int i = 0 ; i < path.size() ; i++)
	{
		cout<<path[i]<<"->";
	}
	cout<<endl;*/	
	  path_color(path);
	  return true;
  }
  else
  {
  	  return false;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
//this function creates the matrix on the screen witha a fixed gap passed.
void make_graph(int screen_width, int screen_height ,int gap_lines)
{
    for(int i = 10 ; i <=screen_width ; i+=gap_lines)
	 {
		line(i,10,i,screen_height);	                                //Draw vertical lines from (i,10) to (i,screen_height)
	 }
	 
	 for(int i = 10 ; i <=screen_height ; i+=gap_lines)
	 {
		line(10,i,screen_width,i);	                                //Draw horizontal lines from (10,i) to (screen_width,i)
	 }	
}
 
//logic to create the adjancecy list
//atmost 8 nodes are adjacent to a particular node
void make_adjacency_list(int num_of_rows,int num_of_columns)
{
	//vector<int> adja[num_of_columns * num_of_rows];, initialized at th top
	for(int i = 0 ; i < num_of_rows; i++)
	{
		for(int j = 0 ; j < num_of_columns ; j++)
		{	
		
			int temp = 1;
			temp = (num_of_columns * i) + j; // temp is used to calculate the index of the current node using the formula temp = (num_of_columns * i) + j.
			if((i-1) >= 0)
	 	   {
			  if((j-1) >= 0)                //Top-left diagonal neighbor: (i-1, j-1)
			    {
			    	int temp2 = 1;          
			    	temp2 = (num_of_columns * (i-1)) + (j - 1);                //Node index of top-left diagonal neighbor
			    	if(record_obstacles.find(temp2) == record_obstacles.end()) //checking if the node is not a obstacle
			    	    adja[temp].push_back(temp2);                           // adding into adjayency list
				} 
			  if(j >= 0)                  // Top neighbor(i-1, j)
			  {
			   	    int temp2 = 1;
			    	temp2 = (num_of_columns * (i-1)) + (j);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  }
			  
			  if((j + 1) < num_of_columns)       //Top-right diagonal neighbor: (i-1, j+1)
			  {
			     	int temp2 = 1;
			    	temp2 = (num_of_columns * (i-1)) + (j + 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  }
			    
	       }
	       if(i >= 0)
	       {
	    		if((j-1) >= 0)              //Left neighbor: (i, j-1)
			   {
			    	int temp2 = 1;
			    	temp2 = (num_of_columns * (i)) + (j - 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
				}
		   
		  		 if((j + 1) < num_of_columns)                //Right neighbor: (i, j+1)
			  {
			     	int temp2 = 1;
			    	temp2 = (num_of_columns * (i)) + (j + 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  }
	    	}
	      if((i+1) < num_of_rows)
	      {
	      	if((j-1) >= 0)                 //Bottom-left diagonal neighbor: (i+1, j-1)
			    {
			    	int temp2 = 1;
			    	temp2 = (num_of_columns * (i+1)) + (j - 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
				}
			  if(j >= 0)                   //Bottom neighbor: (i+1, j)
			  {
			   	    int temp2 = 1;
			    	temp2 = (num_of_columns * (i+1)) + (j);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  }
			  
			  if((j + 1) < num_of_columns)                //Bottom-right diagonal neighbor: (i+1, j+1)
			  {
			     	int temp2 = 1;
			    	temp2 = (num_of_columns * (i+1)) + (j + 1);
			    	if(record_obstacles.find(temp2) == record_obstacles.end())
			    	adja[temp].push_back(temp2);
			  }
	      	
		  }
		}
	}
	}
	
/*	for(int i = 0 ; i < 1444 ; i++)
	{
	  /*	for(int j = 0; j < adja[i].size() ;j++)
		{
			cout<<adja[i][j]<<" ";
			//cout<<adja[i].size();
		}
		cout << "\n Adjacency list of vertex "
             << i << "\n head "; 
        for (vector<int>::iterator itr = adja[i].begin(); itr < adja[i].end() ; itr++) 
           cout << "-> " << (*itr); 
		cout<<endl;
	}
	
	
}
}*/


//it converts the calculated distance to string such that it can be displayed onto the screen
string distance_to_string(int distance)
{
    if (distance == 0)
    {
        return "0";
    }
    string temp2 = "";
    while (distance != 0)
    {
        int temp = distance % 10;
        temp += '0';  // Convert integer to character
        temp2 += temp;
        distance /= 10;
    }
    reverse(temp2.begin(), temp2.end());  // Reverse the string
    return temp2;
}

int main()
{
     DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);     //GetSystemMetrics(SM_CXSCREEN) and GetSystemMetrics(SM_CYSCREEN) retrieve the width and height of the system screen in pixels.
	 DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
	 int start_of_maxtrix_x = 10;  //start_of_maxtrix_x and start_of_maxtrix_y are set to 10. These represent the starting coordinates of the matrix on the screen
	 int start_of_maxtrix_y = 10;
     initwindow(1080,850,"test");	 //initwindow(1080, 850, "test") initializes a graphics window with a width of 1080 pixels, a height of 850 pixels, and the title "test".
	 int num_lines_vertical; 
	 int num_lines_horizontal; 
	 int gap_lines = 20;
	 int screen_width = 770;   //screen_width and screen_height are both set to 770. These values define the area within the window where the grid will be drawn
	 int screen_height = 770;
	 int num_rows = 38;
	 int num_columns = 38;
	 int start_node_index = 0;
	 int end_node_index = 0;
	 int num_of_nodes = 38*38;

  make_graph(screen_width,screen_height,gap_lines); 

	 while(1)	  
{
	
	  make_graph(screen_width,screen_height,gap_lines); 
     

	  POINT cursorPosition;
	  int mx = 0,my = 0;
	  int count = 0;
	  map<int,int> mymap;
	  
	  
	  
	  settextstyle(1, HORIZ_DIR,2);    //font,direction,size
	  
	 //used to beautify the main screen
	  // for starting node
	  setfillstyle(1,3); //used to change the color and type of style used 
	  bar(780+5, 450+5 , 780 + 15 , 450 + 15);
	  outtextxy(800, 450, "Starting Node");
	  
	  setfillstyle(1,5); //for ending node
	  bar(780+5, 490+5 , 780 + 15 , 490 + 15);
	  outtextxy(800, 490, "Ending Node");
	  
	  setfillstyle(1,14); //for relaxed nodes
	  bar(780+5, 530+5 , 780 + 15 , 530 + 15);
	  outtextxy(800, 530, "Relaxed Node");
	  
	  setfillstyle(1,4); //for path Nodes
	  bar(780+5, 570+5 , 780 + 15 , 570 + 15);
	  outtextxy(800, 570, "Path Node");
	   
	  setfillstyle(1,15); //for obstacle Nodes
	  bar(780+5, 610+5 , 780 + 15 , 610 + 15);
	  outtextxy(800, 610, "Obstacle Node");  
	 while(1)
	 {
	 	//font,direction,character size
	 settextstyle(1, HORIZ_DIR,2);
	 //x coordinate, y coordinate , test To Display
	 outtextxy(800, 20 + 20, "Visualizing");
	 outtextxy(800, 20 + 20 + 20 + 10, "Dijkstra");
	 outtextxy(800, 20 + 20 + 20 + 20 + 20, "Algorithm");
	 outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "Select");
	 
	 if(mymap.size() == 0)
	   outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "The Starting Node");
	   
	 GetCursorPos(&cursorPosition);
	 	
	 	mx = cursorPosition.x - 10; //to overcome the difference
	 	my = cursorPosition.y - 30;  //to overcome the difference
	 	
	 //	cout<<mx<<" "<<my<<endl;
	 	
	 	if(GetAsyncKeyState(VK_LBUTTON))
		 {
		 	int tempx = mx - start_of_maxtrix_x ;
		 	int tempy = my - start_of_maxtrix_y ;
			
			if(tempx >= gap_lines)
			{
			  tempx = tempx / gap_lines;
			  tempx = (tempx * gap_lines) + start_of_maxtrix_x;
		    }
		    else
		    {
		    	tempx = 10;
			}
			  
			if(tempy >= gap_lines)
			 {
			   tempy = tempy / gap_lines;
			   tempy = (tempy* gap_lines) + start_of_maxtrix_y;
		     }
		     else
		     {
		     	tempy = 10;
			 }	
			
			mymap.insert({mx,my});
			if(mymap.size() == 1)
			   {  setfillstyle(1,3); //used to change the color and type of style used
			      bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
			      startx = tempx;
			      starty = tempy;
			     // cout<<tempx<<" "<<tempy<<endl;
			     start_node_index = map_points_to_nodes(tempx, tempy , gap_lines, num_columns);
			     outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "The Ending  Node");        //changing "The Ending Node" from "The Starting Node"
			      
			}
			if(mymap.size() == 2)
			   {  setfillstyle(1,5);
			      bar(tempx+5, tempy+5 , tempx + 15 , tempy + 15);
			      outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "                ");                 //removing "Select"
			      outtextxy(800, 20 + 20 + 20 + 20 + 20 + 120, "                            ");    //removing "The Ending Node"
			      end_node_index = map_points_to_nodes(tempx, tempy, gap_lines, num_columns);
			      //cout<<tempx<<" "<<tempy;
			      endx = tempx;
			      endy = tempy;
			}
			
			if(mymap.size() == 2)
			  break;
		 } 
	 	delay(10);
	 }

     if(start_node_index != end_node_index)
{
     outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "Obstacles? (Y/N)");   
     outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "Input Compulsary");
     
      while(1)
	 {
	 	char c;
	 	c  = (char) getch();
	 	
		 if( c == 121 || c == 89) //ascii value of 'y' of 'Y'
		{
			add_obstacles(start_node_index,end_node_index);       //calling function to draw obstacles on grid
			outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "                                   ");      //removing "Obstacles....."
            outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "                                   ");
			break;
		}
		 if( c == 110 || c == 78) //ascii value of 'n' of 'N'
		 {
		 	outtextxy(800, 20 + 20 + 20 + 20 + 20 + 140, "                                   ");
            outtextxy(800, 20 + 20 + 20 + 20 + 20 + 160, "                                   ");
		    break;	
		 }
		 else
		{
			//wrong key pressed Try Again                     
			outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Wrong Key Pressed   ");
	        outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press Y to try again");
	        outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 120, "Press N to exit");
	                                                          
		}
	 }
	 
	 
	 
	 //making the new adjacency_List after every obstacles recorded
	  make_adjacency_list(38,38);	 
 

	 
	 cout<<"Start Node Index " << start_node_index <<" End Node Index "<<end_node_index;
	 
	 //dijkstra code starts here
	 
	  if(dijkstra(start_node_index, end_node_index,num_of_nodes))
	 {
	  cout<<"\n Hello World";
	  outtextxy(800,20 + 20 + 20 + 20 + 20 + 100, "          ");
	  string s = "The Distance is: ";
	  
      // cout<<s.length()<<endl;
	  s+=distance_to_string(dis);
      s+= "  ";
  
     int n = s.length(); 
     //cout<<n<<endl;
     // declaring character array 
     char char_array[n]; 
  
     // copying the contents of the 
     // string to char array 
     strcpy(char_array, s.c_str()); 
     outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120, char_array);
    }
    else
    {
    	outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120, "No Path Found Retry");
	}
     
}

else
{
	     outtextxy(780, 20 + 20 + 20 + 20 + 20 + 140, "Start and End Same");
         outtextxy(780, 20 + 20 + 20 + 20 + 20 + 160, "Distance is : 0");
}

	 outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Press R to try again");
	 outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press Esc to exit");
	 
	 while(1)
	 {
	 	char c;
	 	c  = (char) getch();
	 	if(c == 27) //27 is the ascii for "esc" 
	 	{
	 		exit(1);
		 }
		 if( c == 114 || c == 82) //ascii value of 'r' of 'R'
		   break;
	
		 else
		{
			//wrong key pressed Try Again                     
			outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 60, "Wrong Key Pressed   ");
	        outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 90, "Press R to try again");
	        outtextxy(780, 20 + 20 + 20 + 20 + 20 + 120 + 120, "Press Esc to exit");
	                                                          
		}
	 }
	 
	 //clearing the adjacency list also the recorded obstacles for the next iteration is requested
	 record_obstacles.clear();
	 for(int i = 0 ; i < num_of_nodes ; i++)
	    adja[i].clear();
	 cleardevice();    //The cleardevice() function in the graphics library (such as graphics.h) is used to clear the graphics screen, effectively removing all drawings and resetting the window to its initial state.
}
	 
	 getch();   //getch(): This function waits for a key press. It is typically used to pause the program execution so that the user can see the output on the graphics screen before the program closes
	 closegraph();    //closegraph(): This function closes the graphics mode and returns the system to text mode. It effectively shuts down the graphics window

	return 0;
}















//https://chatgpt.com/share/5fcafda2-c98f-4086-898d-3d8ce03fa2d3