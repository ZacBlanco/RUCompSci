package apps;

import structures.*;
import java.util.ArrayList;

public class MST {

	/**
	 * Initializes the algorithm by building single-vertex partial trees
	 * 
	 * @param graph
	 *            Graph for which the MST is to be found
	 * @return The initial partial tree list
	 */
	public static PartialTreeList initialize(Graph graph) {

		if(graph == null) {
			return null;
		}
		//Steps 1+2 of the Algorithm
		
		//Create the PTL
		PartialTreeList L = new PartialTreeList();
		
		//Initialize Arcs + PT for each Vertex in the graph.
		for (Vertex v : graph.vertices) {
			PartialTree t = new PartialTree(v);
			MinHeap<PartialTree.Arc> arcs = t.getArcs();
			Vertex.Neighbor n = v.neighbors;
			while (n != null) {
				arcs.insert(new PartialTree.Arc(v, n.vertex, n.weight));
				n = n.next;
			}
			
			//Make sure we add the Partial Tree to the list.
			L.append(t);
		}

		return L;
	}

	/**
	 * Executes the algorithm on a graph, starting with the initial partial tree
	 * list
	 * 
	 * @param graph
	 *            Graph for which MST is to be found
	 * @param ptlistist
	 *            Initial partial tree list
	 * @return Array list of all arcs that are in the MST - sequence of arcs is
	 *         irrelevant
	 */
	public static ArrayList<PartialTree.Arc> execute(Graph graph, PartialTreeList ptlist) {

		//if either are null we want to simply return an empty list. 
		if (graph == null || ptlist == null) {
			return new ArrayList<PartialTree.Arc>();
		}
		
		//Result that we end up returning.
		ArrayList<PartialTree.Arc> result = new ArrayList<>();
		
		//Make sure that we only loop while we have > 1 item left
		// (We usually end up removing 2)
		while (ptlist.size() > 1) {
			PartialTree ptx = ptlist.remove();
			
			//Retrieve the arcs of the PTX, initialize our alpha variable.
			MinHeap<PartialTree.Arc> arcs = ptx.getArcs();
			PartialTree.Arc alpha = null;
			
			boolean broke = false; // This variable tells us whether we ran out of items when deleting from our
			// heap. If we broke out of the loop we should make sure to remove the tree that contains the v2 in
			// alpha
			
			while (!arcs.isEmpty()) {
				alpha = arcs.deleteMin();
				if (!PTContains(ptx, alpha.v2)) {
					broke = true;
					break;
				}
			}

			if (broke) {
				
				//We know alpha is a component of the MST
				result.add(alpha);

				//Algorithm states we should remove the PT which contains the vertex V2
				PartialTree pty = ptlist.removeTreeContaining(alpha.v2);
				
				//The first thing we want to do is make sure that we properly merge
				// the two priority queues of ptx+pty.
				pty.getArcs().merge(ptx.getArcs());
				
				
				//Next, because PTY's priority queue is now merged with PTX
				// The only way to refence the vertices in PTX is if we set PTY's 
				//maximum parent (endmost) as having a parent of PTX's root 
				
				
				//Here we iterate to the top of the tree of pty (maximum parent)
				// then we set this vertex's parent as the root of PTX
				// Essentially this is a linked list of vertexes with a priority queue of arcs at the base.
				Vertex ptr = pty.getRoot();
				Vertex trail = null;
				while (ptr != trail) { //the tree "ends" when the vertex references itself as the parent.
					trail = ptr;
					ptr = ptr.parent;
				}
				
				//finally setting PTY's max parent as PTX's root.
				ptr.parent = ptx.getRoot();

				
				//lastly adding our newly modified pty back to the queue
				ptlist.append(pty);
			}
		}
		return result;
	}

	
	/***
	 * 
	 * @param pt The partial tree for which we conduct our search on
	 * @param v the vertex that you are searching for in the tree.
	 * @return true or false depending on whether the tree contains the vertex, V
	 */
	private static boolean PTContains(PartialTree pt, Vertex v) {
		
		//poor input case - partial tree is null - automatically false.
		//poor input case - vertex is null - cannot conduct search, return false.
		if (pt == null) {
			return false;
		} else if (v == null) {
			return false;
		}

	
		//start with a pointer at the root of the tree.
		//traverse up the tree until we find the vertex 
		//or until we hit the "maximum" parent
		Vertex ptr = pt.getRoot();
		Vertex trail = null;
		
		if (ptr == null) {
			return false;
		} else if (ptr.name.equals(v.name)) {
			return true;
		} else {
			trail = ptr;
			ptr = ptr.parent;
		}

		while (ptr != trail) {
			if (ptr.name.equals(v.name)) {
				return true;
			}
			trail = ptr;
			ptr = ptr.parent;
		}
		return false;
	}

}
