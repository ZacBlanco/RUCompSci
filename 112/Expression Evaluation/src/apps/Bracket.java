package apps;

/**
 * This class enacapsulates a parenthesis ('(' or ')') or square bracket ('[' or ']'), and
 * its position in the expression.
 * 
 * @author run-nb-cs112
 *
 */
public class Bracket {
	
	/**
	 * Paren or square bracket
	 */
	public char ch;
	
	/**
	 * Position at which bracket occurs in expression
	 */
	public int pos;
	
	/**
	 * Initializes this bracket to given char and position
	 * 
	 * @param ch Bracket character
	 * @param pos Position in expression
	 */
	public Bracket(char ch, int pos) {
		this.ch = ch;
		this.pos = pos;
	}
}
