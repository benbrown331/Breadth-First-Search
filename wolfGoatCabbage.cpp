/*
 * Name: Benjamin Brown
 * Date Submitted: 11/28/21
 * Lab Section: 001
 * Assignment Name: Lab 10: wolfGoatCabbage 
*/

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
using namespace std;

// Reflects what each node represents...
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x>>i) & 1; }
const int wolf=0, goat=1, cabbage=2, me=3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
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

string state_string(state s)
{
  string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
  for (int i=0; i<4; i++)
    if (!bit(s, i)) result += items[i] + " ";
  result += " |river| ";
  for (int i=0; i<4; i++)
    if (bit(s, i)) result += items[i] + " ";
  return result;
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
  } else {
    cout << "Initial state: " << state_string(s) << "\n";
  }
}

string neighbor_label(int s, int t)
{
  string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
  if (bit(s,me) == bit(t,me)) return "";  // must cross river
  int cross_with = 0;
  for (int i=0; i<3; i++) {
    if (bit(s,i) != bit(t,i) && bit(s,i)==bit(s,me)) { cross_with++; which_cross = items[i]; }
    if (bit(s,i) != bit(t,i) && bit(s,i)!=bit(s,me)) return "";
  }
  if (cross_with > 1) return "";
  if (cross_with == 0) return "Cross alone";
  else return "Cross with " + which_cross;
}

//check to see if current position breaks any rules
bool validPos(bitset<4> pos) {

   //if the wold and goat are on the same side, me must be attending them
   if (pos[wolf] == pos[goat])   {

       if(pos[me] == pos[wolf]) { return true;}
   }

   //if goat is with cabbage, it must be attended by me
   else if (pos[goat] == pos[cabbage]) {

       if (pos[me] == pos[goat]) {return true;}

   }
   else
       return true;
   return false;
}
void build_graph(void)
{
  //Implement this function
  bitset<4> current_state=0;
  bitset<4> next_state;

  //when current state is 15 (1111), solution is found
  while ((int)current_state.to_ulong()<15) {
    //loop for each position
      for (int i = 0; i < 4; i++) {
         //if
         if(current_state[me] == current_state[i])   {

            //set next state to flip the current posion
            next_state = current_state;
            next_state[me].flip();

            //if i is not the position of me, flip the next state again
             if (i!=me) {
               next_state[i].flip();
             }

             //if next state is valid, update nbrs and edge_label maps
             if(validPos(next_state))   {

                     int curr = (int)current_state.to_ulong();
                     int next = (int)next_state.to_ulong();
                     //for the current state, add the value of the next state to vecor
                     nbrs[curr].push_back(next);
                     //store information of th current state and next action
                     edge_label[make_pair(curr,next)] = neighbor_label(curr,next);
             }
         }
     }
     //incriment int value of current state by 1
     current_state = (int)current_state.to_ulong() + 1;
  }
}

int main(void)
{
  build_graph();

  state start = 0, end = 15;
  
  search(start);
  if (visited[end])
    print_path (start, end);
  else
    cout << "No path!\n";
  
  return 0;
}