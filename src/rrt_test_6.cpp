#include<iostream>
#include<stdio.h>
#include<list>
#include<time.h>
#include<cmath>
#include<unistd.h>
#define MAX_STEP_LENGTH 10

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

  node* generateRandomConfig()
  {
    srand(time(0));
    node* q_rand=new node(rand()%100, rand()%100);//NOTE: must be %100
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

      q_new=new node(new_col, new_row);
      return q_new;
    }
    //if the sampled point is less than step_length units away then set
    //q_new as the sampled q_rand itself
    else
    {
      q_new=new node(q_rand);
      return q_new;
    }
  }

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
    //declare iterator over the node list that contains all nodes
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      i->displayWithParentInfo();
      cout<<endl;
    }
  }
};

int main()
{
  cout<<"Hello world!\n";
  rrt_tree main_tree;

  node* q_null=new node(69, 69);

  node* q_rand;
  node* q_near;
  node* q_new;
  node* q_root=new node(49, 49);
  q_root->parent=q_null;

  main_tree.addNode(q_root);

  for(int i=0; i<100; i++)
  {
    q_rand=main_tree.generateRandomConfig();
    cout<<"Randomly sampled point is: \n";
    q_rand->display();

    q_near=main_tree.nearestVertex(q_rand);
    cout<<"Nearest node is: \n";
    q_near->display();

    q_new=main_tree.newConfiguration(q_rand, q_near);
    cout<<"New node made at: \n";
    q_new->display();

    main_tree.addNode(q_new);

    main_tree.addEdge(q_near);
    cout<<endl;
    usleep(400000);
  }

  main_tree.showAllNodes();

  return(0);
}
