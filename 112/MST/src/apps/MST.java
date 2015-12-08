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

		PartialTreeList L = new PartialTreeList();
		for (Vertex v : graph.vertices) {
			PartialTree t = new PartialTree(v);
			MinHeap<PartialTree.Arc> arcs = t.getArcs();
			Vertex.Neighbor n = v.neighbors;
			while (n != null) {
				arcs.insert(new PartialTree.Arc(v, n.vertex, n.weight));
				n = n.next;
			}
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

		ArrayList<PartialTree.Arc> result = new ArrayList<>();

		while (ptlist.size() > 1) {

			PartialTree ptx = ptlist.remove();

			MinHeap<PartialTree.Arc> arcs = ptx.getArcs();

			PartialTree.Arc alpha = null;
			boolean broke = false;
			while (!arcs.isEmpty()) {
				alpha = arcs.deleteMin();
				if (!PTContains(ptx, alpha.v2)) {
					broke = true;
					break;
				}
			}

			if (broke) {
				result.add(alpha);
				
				// Testing for case 2
				PartialTree pty = ptlist.removeTreeContaining(alpha.v2);
				pty.getArcs().merge(ptx.getArcs());
				
				Vertex ptr = pty.getRoot();
				Vertex trail = null;
				while (ptr != trail) {
					trail = ptr;
					ptr = ptr.parent;
				}
				ptr.parent = ptx.getRoot();

				ptlist.append(pty);
			}

		}

		return result;
	}

	private static boolean PTContains(PartialTree pt, Vertex v) {
		if (pt == null) {
			return false;
		} else if (v == null) {
			return false;
		}

	
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
