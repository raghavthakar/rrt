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
#define SLEEP_TIME 100000
#define MAP_LENGTH 1000

using namespace std;

//when the closest node to the sampled point is determined,
//it is made the parent node to the point
//and the sampled node is added to the parent node's list of child nodes

//Node represents every node in the rrt tree
class node
{
public:
  //col  and row number
  int col;
  int row;
  //node ID
  int id;
  //pointer list of all child nodes
  list<node> child_nodes;
  //pointer to parent node
  node* parent;

  //public contructor
// public:
  node(int col, int row)
  {
    this->col=col;
    this->row=row;
  }

  node(node* n)
  {
    this->col=n->getCol();
    this->row=n->getRow();
  }

  void setID(int id)
  {
    this->id=id;
  }

  //return the sum of square of the coordinates of the node
  //useful when comparinfg distance to another node
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

  //just to verify that the node info is correct
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
    for(list<node>::iterator i=child_nodes.begin(); i!=child_nodes.end(); i++)
    {
      i->display();
    }
  }
};

//class rrt_tree contains a list of all the nodes, and contains all the
//operations pertaining to adding nodes/sampling points etc
class rrt_tree
{
  list<node> all_nodes;

public:
  //add a node to the list of all nodes
  void addNode(node* new_node)
  {
    all_nodes.push_back(*new_node);
  }

  //returns the absolute squre of distance between two nodes
  int distanceBetween(node node1, node node2)
  {
    return sqrt(pow((node1.getCol()-node2.getCol()), 2) + pow((node1.getRow()-node2.getRow()), 2));
  }

  //tells whether a noed at given coordinates is legal or not
  bool isLegal(int new_col, int new_row)
  {
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      if(i->getCol()==new_col&&i->getRow()==new_row)
        return false;
    }
    return true;
  }

  node* generateRandomConfig()
  {
    mt19937 mt(time(nullptr));
    node* q_rand=new node(mt()%MAP_LENGTH, mt()%MAP_LENGTH);//NOTE: must be %100
    return q_rand;
  }

  node* nearestVertex(node* q_rand)
  {
    node nearest_node=*all_nodes.begin();
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      nearest_node=(distanceBetween(*q_rand, *i) < distanceBetween(*q_rand, nearest_node))?*i:nearest_node;
    }

    node* q_near=new node(nearest_node);
    return q_near;
  }

  node* newConfiguration(node* q_rand, node* q_near)
  {
    node* q_new;
    bool repeat_coordinates=false;
    if(distanceBetween(*q_rand, *q_near)>MAX_STEP_LENGTH)
    {
      float scale_factor=(float)MAX_STEP_LENGTH/distanceBetween(*q_rand, *q_near);

      int col_diff=q_rand->getCol()-q_near->getCol();
      int row_diff=q_rand->getRow()-q_near->getRow();

      int new_col=q_near->getCol()+scale_factor*col_diff;
      int new_row=q_near->getRow()+scale_factor*row_diff;

      //RESTARTS THE NODE MAKING PrOCESS IF NEW NODE IS ILLEGAL
      if(!isLegal(new_col, new_row))
      {
        cout<<"Illegal node. Should try again. \n";
        q_new=new node(ILLEGAL_NODE, ILLEGAL_NODE);
      }
      else
        q_new=new node(new_col, new_row);

      return q_new;
    }
    //if the sampled point is less than step_length units away then set
    //q_new as the sampled q_rand itself
    else
    {
      //RESTARTS THE NODE MAKING PrOCESS IF NEW NODE IS ILLEGAL
      if(!isLegal(q_rand->getCol(), q_rand->getRow()))
      {
        cout<<"Illegal node. Should try again. \n\n";
        q_new=new node(ILLEGAL_NODE, ILLEGAL_NODE);
      }
      else
        q_new=new node(q_rand);

      return q_new;
    }
  }

  //Sets nearest node as parent of new node
  //Sets new node as child of parent node by pushing to list
  void addEdge(node* q_near)
  {
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
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
    //declare iterator over the node list that contains all odes
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
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

  //Writes all the node to a csv file
  //Format: col_num, row_num, parent_col_num, parent_row_num
  void writeNodeToCSV()
  {
    ofstream csv_file;
    csv_file.open("tree.csv", ios::app);
    csv_file<<all_nodes.back().getCol()<<","<<all_nodes.back().getRow()<<","<<all_nodes.back().parent->getCol()<<","<<all_nodes.back().parent->getRow()<<endl;
    csv_file.close();
  }
};

int main()
{
  cout<<"Hello world!\n";
  rrt_tree main_tree;

  node* q_null=new node(ROOT_COORDINATE, ROOT_COORDINATE);

  node* q_rand;
  node* q_near;
  node* q_new;
  node* q_root=new node(ROOT_COORDINATE, ROOT_COORDINATE);
  q_root->parent=q_null;

  main_tree.addNode(q_root);

  main_tree.clearCSV();
  main_tree.writeNodeToCSV();

  for(int i=0; i<ITERATIONS; i++)
  {
    usleep(SLEEP_TIME);

    q_rand=main_tree.generateRandomConfig();
    cout<<"Randomly sampled point is: \n";
    q_rand->display();

    q_near=main_tree.nearestVertex(q_rand);
    cout<<"Nearest node is: \n";
    q_near->display();

    q_new=main_tree.newConfiguration(q_rand, q_near);

    //If the newConfiguration is illegal, repeat iteration of the loop
    //to try again
    if(q_new->getCol()==ILLEGAL_NODE||q_new->getRow()==ILLEGAL_NODE)
    {
      i--;
      continue;
    }

    else
    {
      cout<<"New node made at: \n";
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
