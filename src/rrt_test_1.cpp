#include<iostream>
#include<stdio.h>
#include<list>
#include<time.h>
#define MAX_STEP_LENGTH 10

using namespace std;

//when the closest node to the sampled point is determined,
//it is made the parent node to the point
//and the sampled node is added to the parent node's list of child nodes

//Node represents every node in the rrt tree
class node
{
  //col  and row number
  int col;
  int row;
  //pointer list of all child nodes
  list<node> child_nodes;
  //pointer to parent node
  node* parent;

  //public contructor
public:
  node(int col, int row)
  {
    this->col=col;
    this->row=row;
  }

  //just to verify that the node info is correct
  void display()
  {
    cout<<"Column number: "<<col<<endl;
    cout<<"Row number: "<<row<<endl;
  }
};

//class rrt_tree contains a list of all the nodes, and contains all the
//operations pertaining to adding nodes/sampling points etc
class rrt_tree
{
  list<node> all_nodes;

public:
  //this function adds a new node to the list of all nodes
  void addNode(node new_node)
  {
    all_nodes.push_back(new_node);
  }

  //to display the info of all the nodes part of the rrt_tree
  void showAllNodes()
  {
    //declare iterator over the node list that contains all nodes
    list<node>::iterator i;
    for(i=all_nodes.begin(); i!=all_nodes.end(); ++i)
      i->display();
  }

  //this function generates a random configuration
  //that is returned by the function
  node generateRandomConfig()
  {
    srand(time(0));
    node q_rand(rand()%100, rand()%100);
    return q_rand;
  }

  //this function picks the vertex on the tree that is
  //closest to the random node generated q_rand
  node nearestVertex(node q_rand)
  {

  }
};

int main()
{
  cout<<"Hello world!\n";

  rrt_tree main_tree;

  node q_root(49, 49);
  q_root.display();
  main_tree.showAllNodes();
  main_tree.addNode(q_root);

  node node_1(35, 36);
  node node_2(3, 6);
  node node_3(5, 26);

  main_tree.addNode(node_1);
  main_tree.addNode(node_2);
  main_tree.addNode(node_3);

  node q_rand=main_tree.generateRandomConfig();

  main_tree.addNode(q_rand);

  main_tree.showAllNodes();

  //node q_near=main_tree.nearestVertex(q_rand);
  return(0);
}
