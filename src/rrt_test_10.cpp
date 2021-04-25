#include<iostream>
#include<fstream>
#include<list>
#include<time.h>
#include<cmath>
#include<unistd.h>
#include<random>
#define MAX_STEP_LENGTH 50
#define ILLEGAL_NODE -50
#define ROOT_COORDINATE 49
#define TARGET_COL 99
#define TARGET_ROW 99
#define ITERATIONS 300
#define SLEEP_TIME 500000
#define MAP_LENGTH 1000

using namespace std;

//when the closest Node to the sampled point is determined,
//it is made the parent Node to the point
//and the sampled Node is added to the parent Node's list of child nodes

//Node represents every Node in the rrt tree
class Node
{
public:
  //col  and row number
  int col;
  int row;
  //Node ID
  int id;
  //pointer list of all child nodes
  list<Node> child_nodes;
  //pointer to parent Node
  Node* parent;

  //public contructor
// public:
  Node(int col, int row)
  {
    this->col = col;
    this->row = row;
  }

  Node(Node* n)
  {
    this->col = n->getCol();
    this->row = n->getRow();
  }

  void setID(int id)
  {
    this->id = id;
  }

  //return the sum of square of the coordinates of the Node
  //useful when comparinfg distance to another Node
  int getSquareOfCoordinates()
  {
    return(col*col + row*row);
  }

  //returns the col
  int getCol()
  {
    return col;
  }

  //returns the row
  int getRow()
  {
    return row;
  }

  //returns the ID
  int getID()
  {
    return this->id;
  }

  //just to verify that the Node info is correct
  void display()
  {
    cout<<"Column number: "<<col<<endl;
    cout<<"Row number: "<<row<<endl;
  }

  void displayWithParentInfo()
  {
    cout<<"Column number: "<<col<<endl;
    cout<<"Row number: "<<row<<endl;
    cout<<"Parent column number: "<<parent->getCol()<<endl;
    cout<<"Parent row number: "<<parent->getRow()<<endl;
    cout<<"Child nodes: \n";
    for(list<Node>::iterator i=child_nodes.begin(); i!=child_nodes.end(); i++)
    {
      i->display();
    }
  }
};

//class Map contains information about the Map where RRT is to be implemented.
//Ex: Start, Target, Obstacles etc.
class Map
{
public:
  //This structure specifies the four corners of an Obstacle
  struct Obstacle
  {
    int top_left_column;
    int top_left_row;
    int bottom_right_column;
    int bottom_right_row;

    Obstacle(int top_left_column, int top_left_row,
             int bottom_right_column, int bottom_right_row)
    {
      this->top_left_column=top_left_column;
      this->top_left_row = top_left_row;
      this->bottom_right_column=bottom_right_column;
      this->bottom_right_row = bottom_right_row;
    }
  };

  //A list of 4 element integer arrays that specify corners of each Obstacle.
  list<Obstacle> all_obstacles;

  Map()
  {
    int tl[2], br[2]; //cool
    cout<<"Enter top left column number: ";
    cin>>tl[0];
    cout<<"Enter top left row number: ";
    cin>>tl[1];
    cout<<"Enter bottom right column number: ";
    cin>>br[0];
    cout<<"Enter bottom right row number: ";
    cin>>br[1];
    //push a test Obstacle
    all_obstacles.push_back(*(new Obstacle(tl[0], tl[1], br[0], br[1])));
  }
};

//class rrt_tree contains a list of all the nodes, and contains all the
//operations pertaining to adding nodes/sampling points etc
class rrt_tree
{
  list<Node> all_nodes;

public:
  //add a Node to the list of all nodes
  void addNode(Node* new_node)
  {
    all_nodes.push_back(*new_node);
  }

  //returns the absolute squre of distance between two nodes
  int distanceBetween(Node node1, Node node2)
  {
    //this is literal black magic
    return sqrt(pow((node1.getCol()-node2.getCol()), 2) + pow((node1.getRow()-node2.getRow()), 2));
  }

  //tells whether a noed at given coordinates is legal or not
  bool isLegal(int new_col, int new_row)
  {
    for(list<Node>::iterator i = all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      if(i->getCol()==new_col&&i->getRow()==new_row)
        return false;
    }
    return true;
  }

  Node* generateRandomConfig()
  {
    mt19937 mt(time(nullptr));
    Node* q_rand = new Node(mt()%MAP_LENGTH, mt()%MAP_LENGTH);//NOTE: must be %100
    return q_rand;
  }

  Node* nearestVertex(Node* q_rand)
  {
    Node nearest_node=*all_nodes.begin();
    for(list<Node>::iterator i = all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      nearest_node=(distanceBetween(*q_rand, *i) < distanceBetween(*q_rand, nearest_node))?*i:nearest_node;
    }

    Node* q_near = new Node(nearest_node);
    return q_near;
  }

  Node* newConfiguration(Node* q_rand, Node* q_near)
  {
    Node* q_new;
    bool repeat_coordinates = false;
    if(distanceBetween(*q_rand, *q_near)>MAX_STEP_LENGTH)
    {
      float scale_factor=(float)MAX_STEP_LENGTH/distanceBetween(*q_rand, *q_near);

      int col_diff = q_rand->getCol()-q_near->getCol();
      int row_diff = q_rand->getRow()-q_near->getRow();

      int new_col = q_near->getCol()+scale_factor*col_diff;
      int new_row = q_near->getRow()+scale_factor*row_diff;

      //RESTARTS THE Node MAKING PrOCESS IF NEW Node IS ILLEGAL
      if(!isLegal(new_col, new_row))
      {
        cout<<"Illegal Node. Should try again. \n";
        q_new = new Node(ILLEGAL_NODE, ILLEGAL_NODE);
      }
      else
        q_new = new Node(new_col, new_row);

      return q_new;
    }
    //if the sampled point is less than step_length units away then set
    //q_new as the sampled q_rand itself
    else
    {
      //RESTARTS THE Node MAKING PrOCESS IF NEW Node IS ILLEGAL
      if(!isLegal(q_rand->getCol(), q_rand->getRow()))
      {
        cout<<"Illegal Node. Should try again. \n\n";
        q_new = new Node(ILLEGAL_NODE, ILLEGAL_NODE);
      }
      else
        q_new = new Node(q_rand);

      return q_new;
    }
  }

  //Sets nearest Node as parent of new Node
  //Sets new Node as child of parent Node by pushing to list
  void addEdge(Node* q_near)
  {
    for(list<Node>::iterator i = all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      if(i->getCol()==q_near->getCol()&&i->getRow()==q_near->getRow())
      {
        all_nodes.back().parent=&(*i);
        i->child_nodes.push_back(all_nodes.back());
      }
    }
  }

  //to display the info of all the nodes part of the rrt_tree
  void showAllNodes()
  {
    //declare iterator over the Node list that contains all odes
    for(list<Node>::iterator i = all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      i->displayWithParentInfo();
      cout<<endl;
    }
  }

  //Clears the CSV file
  void clearCSV()
  {
    ofstream csv_file;
    csv_file.open("tree.csv");
    csv_file.close();
  }

  //Writes all the Node to a csv file
  //Format: col_num, row_num, parent_col_num, parent_row_num
  void writeNodeToCSV()
  {
    ofstream csv_file;
    csv_file.open("tree.csv", ios::app);
    csv_file<<all_nodes.back().getCol()<<","<<all_nodes.back().getRow()
      <<","<<all_nodes.back().parent->getCol()<<","
      <<all_nodes.back().parent->getRow()<<endl;
    csv_file.close();
  }
};

int main()
{
  cout<<"Hello world!\n";
  rrt_tree main_tree;

  Node* q_null = new Node(ROOT_COORDINATE, ROOT_COORDINATE);

  Node* q_rand;
  Node* q_near;
  Node* q_new;
  Node* q_root = new Node(ROOT_COORDINATE, ROOT_COORDINATE);
  q_root->parent = q_null;

  main_tree.addNode(q_root);

  main_tree.clearCSV();
  main_tree.writeNodeToCSV();

  //Function to accept corners of rectiangular Obstacle
  Map rrt_map;

  for(int i = 0; i<ITERATIONS; i++)
  {
    usleep(SLEEP_TIME);

    q_rand = main_tree.generateRandomConfig();
    cout<<"Randomly sampled point is: \n";
    q_rand->display();

    q_near = main_tree.nearestVertex(q_rand);
    cout<<"Nearest Node is: \n";
    q_near->display();

    q_new = main_tree.newConfiguration(q_rand, q_near);

    //If the newConfiguration is illegal, repeat iteration of the loop
    //to try again
    if(q_new->getCol()==ILLEGAL_NODE||q_new->getRow()==ILLEGAL_NODE)
    {
      i--;
      continue;
    }

    else
    {
      cout<<"New Node made at: \n";
      q_new->display();

      main_tree.addNode(q_new);

      main_tree.addEdge(q_near);

      main_tree.writeNodeToCSV();

      cout<<endl;

      if(q_new->getCol()==TARGET_COL&&q_new->getRow()==TARGET_ROW)
        break;
    }
  }

  main_tree.showAllNodes();

  return(0);
}
