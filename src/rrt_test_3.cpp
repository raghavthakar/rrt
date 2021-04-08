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
    // cout<<"Parent Column number: "<<parent->getCol()<<endl;
    // cout<<"Parent Row number: "<<parent->getRow()<<endl;
  }
};

//class rrt_tree contains a list of all the nodes, and contains all the
//operations pertaining to adding nodes/sampling points etc
class rrt_tree
{
  list<node> all_nodes;

public:
  //this function adds a new node to the list of all nodes
  //overloaded to accept or not accept parent node
  void addNode(node q_new)
  {
    all_nodes.push_back(q_new);
  }

  //to display the info of all the nodes part of the rrt_tree
  void showAllNodes()
  {
    //declare iterator over the node list that contains all nodes
    list<node>::iterator i;
    for(i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      i->display();
      cout<<endl;
    }
  }

  //this function generates a random configuration
  //that is returned by the function
  node generateRandomConfig()
  {
    srand(time(0));
    node q_rand(rand()%100, rand()%100);//NOTE: must be %100
    cout<<"\n- Random is: ";
    q_rand.display();
    return q_rand;
  }

  //returns the absolute squre of distance between two nodes
  int distanceBetween(node node1, node node2)
  {
    return sqrt(pow((node1.getCol()-node2.getCol()), 2) + pow((node1.getRow()-node2.getRow()), 2));
  }

  //this function picks the vertex on the tree that is
  //closest to the random node generated q_rand
  node nearestVertex(node q_rand)
  {
    list<node>::iterator i;
    node q_near=*all_nodes.begin();//set q_near to root node. using
    //*all_nodes.begin() because all_nodes.begin() returns pointer

    //if the iterator node is closer to q_rand than q_near
    //then set q_near to the iterator node
    for(i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      q_near=(distanceBetween(q_rand, *i) < distanceBetween(q_rand, q_near))?*i:q_near;
    }
    cout<<"- Nearest to random is: \n";
    q_near.display();
    return q_near;
  }

  //tries to make a new node that is step distance away from
  //nearest vertex i nthe direction of the random vertex
  node newConfiguration(node q_near, node q_rand)
  {
    //if the q_rand is further than MAX_STEP_LENGTH then
    //we need to make q_new at a point in direction of q_rand from q_near
    //but only MAX_STEP_LENGTH units away
    if(distanceBetween(q_rand, q_near)>MAX_STEP_LENGTH)
    {
      //scale factor is the fraction by which the distance vector
      //between q_near and q_rand must be multiplied with to get a vector
      //that is step length distance away. Add this to the q_near to get the
      //exact point q_new
      float scale_factor=(float)MAX_STEP_LENGTH/distanceBetween(q_rand, q_near);

      int col_diff=q_rand.getCol()-q_near.getCol();
      int row_diff=q_rand.getRow()-q_near.getRow();

      //multiplying the difference in columns with scale factor and adding to
      //column of q_near gives the column of q_new
      int new_col=q_near.getCol()+scale_factor*col_diff;
      int new_row=q_near.getRow()+scale_factor*row_diff;

      node q_new(new_col, new_row);
      cout<<"- New node made at: \n";
      q_new.display();
      return q_new;
    }
    //if the sampled point is less than step_length units away then set
    //q_new as the sampled q_rand itself
    else
    {
      node q_new=q_rand;
      cout<<"- New node made at: \n";
      q_new.display();
      return q_new;
    }
  }

  void addEdge(node q_near)
  {
    // node* q_new=all_nodes.end();
    for(list<node>::iterator i=all_nodes.begin(); i!=all_nodes.end(); i++)
    {
      if(i->getCol()==q_near.getCol()&&i->getRow()==q_near.getRow())
      {
        cout<<"In the addEdge fn: \n";
        i->display();
      }
    }
  }
};

int main()
{
  cout<<"Hello world!\n";

  rrt_tree main_tree;

  node null_node(0, 0);

  node q_root(49, 49);
  main_tree.addNode(q_root);

  node q_rand(0, 0), q_near(0, 0), q_new(0, 0);

  for(int i=0; i<25; i++)
  {
    sleep(1);
    q_rand=main_tree.generateRandomConfig();
    q_near=main_tree.nearestVertex(q_rand);
    q_new=main_tree.newConfiguration(q_near, q_rand);
    main_tree.addNode(q_new);
    main_tree.addEdge(q_near);
  }

  main_tree.showAllNodes();

  return(0);
}
