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
		// COMPLETE THIS METHOD
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
		// COMPLETE THIS METHOD
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
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE PROGRAM COMPILE
		return null;
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
