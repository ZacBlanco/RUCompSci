package structures;

import java.util.ArrayList;

/**
 * This class is a repository of sorting methods used by the interval tree.
 * It's a utility class - all methods are static, and the class cannot be instantiated
 * i.e. no objects can be created for this class.
 * 
 * @author runb-cs112
 */
public class Sorter {

	private Sorter() { }
	
	/**
	 * Sorts a set of intervals in place, according to left or right endpoints.  
	 * At the end of the method, the parameter array list is a sorted list. 
	 * 
	 * @param intervals Array list of intervals to be sorted.
	 * @param lr If 'l', then sort is on left endpoints; if 'r', sort is on right endpoints
	 */
	public static void sortIntervals(ArrayList<Interval> intervals, char lr) {
		// COMPLETE THIS METHOD
		
		//Sorting according to left or right
		int mark = 0;
		if (intervals.size() <= 0) { return; }
		while ( mark < intervals.size() ) {
			Interval intMark = intervals.get(mark);
			
			for(int i = 0; i < mark; i++) {
				Interval intI = intervals.get(i);
				if(lr == 'l') {
					if( intI.leftEndPoint > intMark.leftEndPoint ) {
						intervals.add(i, intMark);
						intervals.remove(mark + 1);
						break;
					}
				} else if (lr == 'r') {
					if( intI.rightEndPoint  > intMark.rightEndPoint ) {
						intervals.add(i, intMark);
						intervals.remove(mark + 1);
						break;
					}
				} else {
					throw new Error("Do not know how to sort according to char " + lr);
				}
			}
			mark++;
		}
		return;
		
	}
	
	
	/**
	 * Given a set of intervals (left sorted and right sorted), extracts the left and right end points,
	 * and returns a sorted list of the combined end points without duplicates.
	 * 
	 * @param leftSortedIntervals Array list of intervals sorted according to left endpoints
	 * @param rightSortedIntervals Array list of intervals sorted according to right endpoints
	 * @return Sorted array list of all endpoints without duplicates
	 */
	public static ArrayList<Integer> getSortedEndPoints(ArrayList<Interval> leftSortedIntervals, ArrayList<Interval> rightSortedIntervals) {
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE PROGRAM COMPILE
		return null;
	}
}
