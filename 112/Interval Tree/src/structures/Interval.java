package structures;

/**
 * Encapsulates an interval whose endpoints are integers. Each interval can contain an additional
 * string that provides information about that interval that may be useful for an application.
 * 
 * @author runb-cs112
 */
public class Interval {
	
	/**
	 * The left endpoint
	 */
	public int leftEndPoint;
	
	/**
	 * The right endpoint
	 */
	public int rightEndPoint;
	
	/**
	 * Additional information
	 */
	public String info;
	
	/**
	 * Initializes a new Interval object with end points and application-specific information.
	 * The left end point must be less than the right end point.
	 * 
	 * @param leftEndPoint Left end point of the interval
	 * @param rightEndPoint Left end point of the interval
	 * @param info Application-specific information that is tied to this interval.
	 */
	public Interval(int leftEndPoint, int rightEndPoint, String info) {
		this.leftEndPoint = leftEndPoint;
		this.rightEndPoint = rightEndPoint;
		this.info = info;
	}
	
	/**
	 * Returns a string representation of this interval.
	 * 
	 * @return String that represents this interval.
	 */
	public String toString() {
		return "[" + leftEndPoint + "," + rightEndPoint + "]: " + info.toString();
	}
	
	/**
	 * Determines if this interval contains the given point.
	 * 
	 * @param point Point that is queried for containment in interval
	 * @return True if the point lies in this interval, false otherwise
	 */
	public boolean contains(float point) {
		return point >= leftEndPoint && point <= rightEndPoint;
	}
	
	/**
	 * Determines if this interval intersects another interval.
	 * 
	 * @param other The other interval with which intersection is examined
	 * @return True if this interval overlaps with the other, false is there is no overlap
	 */
	public boolean intersects(Interval other) {
		return leftEndPoint <= other.rightEndPoint && rightEndPoint >= other.leftEndPoint;
	}
}
