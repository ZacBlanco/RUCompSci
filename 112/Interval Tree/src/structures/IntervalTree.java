package structures;

import java.util.*;

/**
 * Encapsulates an interval tree.
 * 
 * @author runb-cs112
 */
public class IntervalTree {

	/**
	 * The root of the interval tree
	 */
	IntervalTreeNode root;

	/**
	 * Constructs entire interval tree from set of input intervals. Constructing
	 * the tree means building the interval tree structure and mapping the
	 * intervals to the nodes.
	 * 
	 * @param intervals
	 *            Array list of intervals for which the tree is constructed
	 */
	public IntervalTree(ArrayList<Interval> intervals) {

		// make a copy of intervals to use for right sorting
		ArrayList<Interval> intervalsRight = new ArrayList<Interval>(
				intervals.size());
		for (Interval iv : intervals) {
			intervalsRight.add(iv);
		}

		// rename input intervals for left sorting
		ArrayList<Interval> intervalsLeft = intervals;

		// sort intervals on left and right end points
		Sorter.sortIntervals(intervalsLeft, 'l');
		Sorter.sortIntervals(intervalsRight, 'r');

		// get sorted list of end points without duplicates
		ArrayList<Integer> sortedEndPoints = Sorter.getSortedEndPoints(
				intervalsLeft, intervalsRight);

		// build the tree nodes
		root = buildTreeNodes(sortedEndPoints);

		// map intervals to the tree nodes
		mapIntervalsToTree(intervalsLeft, intervalsRight);
	}

	/**
	 * Builds the interval tree structure given a sorted array list of end
	 * points.
	 * 
	 * @param endPoints
	 *            Sorted array list of end points
	 * @return Root of the tree structure
	 */
	public static IntervalTreeNode buildTreeNodes(ArrayList<Integer> endPoints) {
		Queue<IntervalTreeNode> nodes = new Queue<IntervalTreeNode>();

		for (int i = 0; i < endPoints.size(); i++) {
			int val = endPoints.get(i);
			nodes.enqueue(new IntervalTreeNode(val, val, val));
		}

		int temps = nodes.size();
		while (temps > 1) {
			if (nodes.size() == 1) {
				break;
			} else {
				temps = nodes.size();
				while (temps > 1) {
					IntervalTreeNode t1 = nodes.dequeue();
					IntervalTreeNode t2 = nodes.dequeue();
					float largestT1 = t1.maxSplitValue;
					float smallestT2 = t2.minSplitValue;
					float split = (largestT1 + smallestT2) / 2;
					float min = t1.minSplitValue;
					float max = t2.maxSplitValue;
					
					
					IntervalTreeNode n = new IntervalTreeNode(split, min, max);
					n.leftIntervals = new ArrayList<Interval>();
					n.rightIntervals = new ArrayList<Interval>();
					n.leftChild = t1;
					n.rightChild = t2;
					nodes.enqueue(n);
					temps -= 2;
				}
				if (temps == 1) {
					nodes.enqueue(nodes.dequeue());
				}
			}
			temps = nodes.size();
		}
		return nodes.dequeue();

	}

	/**
	 * Maps a set of intervals to the nodes of this interval tree.
	 * 
	 * @param leftSortedIntervals
	 *            Array list of intervals sorted according to left endpoints
	 * @param rightSortedIntervals
	 *            Array list of intervals sorted according to right endpoints
	 */
	public void mapIntervalsToTree(ArrayList<Interval> leftSortedIntervals,
			ArrayList<Interval> rightSortedIntervals) {
		IntervalTreeNode r = getRoot();
		if (r == null) { return; }

		for(Interval n : leftSortedIntervals) {
			IntervalTreeNode h = getHighestSplit(n, root);
			h.leftIntervals.add(n);
		}
		
		for(Interval n : rightSortedIntervals) {
			IntervalTreeNode h = getHighestSplit(n, root);
			h.rightIntervals.add(n);
		}
	}
	
	private boolean intervalContains(Interval n, IntervalTreeNode t) {
		return (t.splitValue >= n.leftEndPoint && t.splitValue <= n.rightEndPoint);
	}
	private IntervalTreeNode getHighestSplit(Interval n, IntervalTreeNode node) {
		IntervalTreeNode first = node;
		
		while( first.leftChild != null || first.rightChild != null ) {
			if( intervalContains(n, first) ) {
				break;
			} else {
				if(first.leftChild.minSplitValue <= n.leftEndPoint && 
					first.leftChild.maxSplitValue >= n.rightEndPoint) {
					first = first.leftChild;
				} else if (first.rightChild.minSplitValue <= n.leftEndPoint && 
							first.rightChild.maxSplitValue >= n.rightEndPoint) {
					first = first.rightChild;
				} else {
					break;
				}
			}
		}
		return first;
	}
	
	public boolean isLeaf(IntervalTreeNode n) {
		return (n.leftChild == null && n.rightChild == null);
	}
	

	/**
	 * Gets all intervals in this interval tree that intersect with a given
	 * interval.
	 * 
	 * @param q
	 *            The query interval for which intersections are to be found
	 * @return Array list of all intersecting intervals; size is 0 if there are
	 *         no intersections
	 */
	public ArrayList<Interval> findIntersectingIntervals(Interval q) {
		return findIntersectingIntervals(getRoot(), q);		
	}
	
	private ArrayList<Interval> findIntersectingIntervals(IntervalTreeNode node, Interval q) {
		ArrayList<Interval> results = new ArrayList<Interval>();
		if (node == null) { return results; }
		if (node.leftChild == null && node.rightChild == null) { return results; }
		if(intervalContains(q, node)) {
			for(Interval n : node.leftIntervals) {
				results.add(n);
			}
			results.addAll(findIntersectingIntervals(node.leftChild, q));
			results.addAll(findIntersectingIntervals(node.rightChild, q));
		} else if (node.splitValue <= q.leftEndPoint) {
			int i = node.rightIntervals.size() - 1;
			while (i >= 0) {
				Interval t = node.rightIntervals.get(i);
				if(t.rightEndPoint >= q.leftEndPoint) {
					results.add(t);
				} else {
					break;
				}
				i--;
			}
			results.addAll(findIntersectingIntervals(node.rightChild, q));
		} else if (node.splitValue >= q.rightEndPoint) {
			int i = node.leftIntervals.size() - 1;
			while (i >= 0) {
				Interval t = node.leftIntervals.get(i);
				if(q.rightEndPoint >= t.leftEndPoint) {
					results.add(t);
				} else {
					break;
				}
				i--;
			}
			results.addAll(findIntersectingIntervals(node.leftChild, q));
		}
		
		return results;	
	}

	/**
	 * Returns the root of this interval tree.
	 * 
	 * @return Root of interval tree.
	 */
	public IntervalTreeNode getRoot() {
		return root;
	}
}
