/*
 * Name: Benjamin Brown
 * Date Submitted: 11/29/21
 * Lab Section: 001
 * Assignment Name: Lab 10: water jugs
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int,int> state;

// Each string in edge_label must be one of the following:
const string actions[] = {"Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A"};

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
void search(state source_node)
{
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;
  
  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
      if (!visited[n]) {
	pred[n] = curnode;
	dist[n] = 1 + dist[curnode];
	visited[n] = true;
	to_visit.push(n);
      }
  }
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
  } else {
    cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
  }
}

//helper function to be called every time
//a pair needs to be added to edge_label or nbrs
void add_to_map(state current_state, state next, int action) {

  pair<state,state> edge = make_pair(current_state,next);
  nbrs[current_state].push_back(next);
  edge_label[edge] = actions[action];

}

void build_graph(void)
{
  //Implement this function
  state current_state = make_pair(0,0);
   int A = 3, B = 4;
   state end = make_pair(A,B);
  
   while(current_state!=end)   {
       state next;

        //fill A
        next = make_pair(A,current_state.second);

        /*

        pair<state,state> edge = make_pair(current_state,next);
        nbrs[current_state].push_back(next);
        edge_label[edge] = actions[0];

        */
       add_to_map(current_state,next,0);

        //Fill B
        next = make_pair(current_state.first,B);

        /*

        pair<state,state> edge = make_pair(current_state,next);
        nbrs[current_state].push_back(next);
        edge_label[edge] = actions[0];

        */
       add_to_map(current_state,next,1);

       //Empty A
        next = make_pair(0,current_state.second);

        add_to_map(current_state,next,2);

        //empty B
        next = make_pair(current_state.first,0);
        add_to_map(current_state,next,3);

      //Pour A to B
      int newA = current_state.first;
      int newB = current_state.second;
      if(current_state.second < B)   {
          while(newA > 0 && newB < B)   {
              newB++;
              newA--;
           }
            next = make_pair(newA,newB);

            add_to_map(current_state,next,4);
       }
      
       // Action 6: Pour B into A
       newA = current_state.first;
       newB = current_state.second;
       if(current_state.first < A)   {
           while(newA < A && newB > 0)   {
               newA++;
               newB--;
           }
            next = make_pair(newA,newB);

            add_to_map(current_state,next,5);
       }

       if (current_state.second == B) 
           current_state = make_pair(current_state.first + 1,0);
       
       else 
           current_state = make_pair(current_state.first, current_state.second + 1);

  }
}

int main(void)
{
  build_graph();

  state start = make_pair(0,0);
  
  for (int i=0; i<5; i++)
    nbrs[make_pair(i,5-i)].push_back(make_pair(-1,-1));
  search (start);
  if (visited[make_pair(-1,-1)]) 
    print_path (start, pred[make_pair(-1,-1)]);
  else
    cout << "No path!\n";
  
  return 0;
}
