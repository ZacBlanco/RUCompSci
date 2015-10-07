package poly;

import java.io.*;
import java.util.StringTokenizer;

/**
 * This class implements a term of a polynomial.
 * 
 * @author runb-cs112
 *
 */
class Term {
	/**
	 * Coefficient of term.
	 */
	public float coeff;

	/**
	 * Degree of term.
	 */
	public int degree;

	/**
	 * Initializes an instance with given coefficient and degree.
	 * 
	 * @param coeff
	 *            Coefficient
	 * @param degree
	 *            Degree
	 */
	public Term(float coeff, int degree) {
		this.coeff = coeff;
		this.degree = degree;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object other) {
		return other != null && other instanceof Term && coeff == ((Term) other).coeff
				&& degree == ((Term) other).degree;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (degree == 0) {
			return coeff + "";
		} else if (degree == 1) {
			return coeff + "x";
		} else {
			return coeff + "x^" + degree;
		}
	}
}

/**
 * This class implements a linked list node that contains a Term instance.
 * 
 * @author runb-cs112
 *
 */
class Node {

	/**
	 * Term instance.
	 */
	Term term;

	/**
	 * Next node in linked list.
	 */
	Node next;

	/**
	 * Initializes this node with a term with given coefficient and degree,
	 * pointing to the given next node.
	 * 
	 * @param coeff
	 *            Coefficient of term
	 * @param degree
	 *            Degree of term
	 * @param next
	 *            Next node
	 */
	public Node(float coeff, int degree, Node next) {
		term = new Term(coeff, degree);
		this.next = next;
	}
	
}

/**
 * This class implements a polynomial.
 * 
 * @author runb-cs112
 *
 */
public class Polynomial {

	/**
	 * Pointer to the front of the linked list that stores the polynomial.
	 */
	Node poly;

	/**
	 * Initializes this polynomial to empty, i.e. there are no terms.
	 *
	 */
	public Polynomial() {
		poly = null;
	}

	/**
	 * Reads a polynomial from an input stream (file or keyboard). The storage
	 * format of the polynomial is:
	 * 
	 * <pre>
	 *     <coeff> <degree>
	 *     <coeff> <degree>
	 *     ...
	 *     <coeff> <degree>
	 * </pre>
	 * 
	 * with the guarantee that degrees will be in descending order. For example:
	 * 
	 * <pre>
	 *      4 5
	 *     -2 3
	 *      2 1
	 *      3 0
	 * </pre>
	 * 
	 * which represents the polynomial:
	 * 
	 * <pre>
	 * 4 * x ^ 5 - 2 * x ^ 3 + 2 * x + 3
	 * </pre>
	 * 
	 * @param br
	 *            BufferedReader from which a polynomial is to be read
	 * @throws IOException
	 *             If there is any input error in reading the polynomial
	 */
	public Polynomial(BufferedReader br) throws IOException {
		String line;
		StringTokenizer tokenizer;
		float coeff;
		int degree;

		poly = null;

		while ((line = br.readLine()) != null) {
			tokenizer = new StringTokenizer(line);
			coeff = Float.parseFloat(tokenizer.nextToken());
			degree = Integer.parseInt(tokenizer.nextToken());
			poly = new Node(coeff, degree, poly);
		}
	}

	/**
	 * Returns the polynomial obtained by adding the given polynomial p to this
	 * polynomial - DOES NOT change this polynomial
	 * 
	 * @param p
	 *            Polynomial to be added
	 * @return A new polynomial which is the sum of this polynomial and p.
	 */
	
	public Polynomial add(Polynomial p) {

		Polynomial result = new Polynomial(); // Result of addition
		
		Node temp1 = poly;
		Node temp2 = p.poly;

		//Because all polynomials are in order from 0th degree to greatest degree
		// We don't need to worry about ordering the terms correctly when adding because
		// they should be in order already.
		
		Node list = new Node(0, 0, null); // We do this to start with a "null" or zero term. It is removed before returning.
		Node listPtr = list; // Pointer starting at front of the polynomial.

		while (temp2 != null || temp1 != null) {
			if (temp2 == null) { // If temp2 is null there are no more temp2 terms to add, so we add the next temp1;
				
				listPtr.next = new Node(temp1.term.coeff, temp1.term.degree, null);
				listPtr = listPtr.next;
				temp1 = temp1.next;
				
			} else if (temp1 == null) { // If temp1 is null there are no more temp1 terms to add, so we add the next temp2.
				
				listPtr.next = new Node(temp2.term.coeff, temp2.term.degree, null);
				listPtr = listPtr.next;// After setting the list's next pointer we need to set our pointer to the latest node.
				temp2 = temp2.next;
				
			} else {

				if (temp1.term.coeff == 0) {
					temp1 = temp1.next; // Just in case we are given a 0 coefficient.
				} else if(temp2.term.coeff == 0) {
					temp2 = temp2.next; // Just in case we are given a 0 coefficient.
				} else if (temp1.term.degree > temp2.term.degree) {
					
					listPtr.next = new Node(temp2.term.coeff, temp2.term.degree, null); //Need to advance the term2 to a greater degree - so we add the term2 and go to next.
					listPtr = listPtr.next;
					temp2 = temp2.next;
					
				} else if (temp1.term.degree < temp2.term.degree) {
					
					listPtr.next = new Node(temp1.term.coeff, temp1.term.degree, null); //Need to advance the term1 to a greater degree - so we add the term1 and go to next.
					listPtr = listPtr.next;
					temp1 = temp1.next;
					
				} else if (temp1.term.degree == temp2.term.degree) {
					
					if(temp1.term.coeff + temp2.term.coeff != 0) { // Same degree so we can add the coefficients, but we don't want a 0 coefficient.
						listPtr.next = new Node(temp1.term.coeff + temp2.term.coeff, temp1.term.degree, null);
						listPtr = listPtr.next;
					}
					
					//Advance both polynomial list pointers
					temp1 = temp1.next; 
					temp2 = temp2.next;
				} else {
					break; // break here on condition where somehow it doesn't fit into other categories because it could possibly break the program. - Will cause bad return.
				}
			}
		}

		// We do this because the front node is currently a "dummy" node.
		// The next node contains the start of the polynomial.
		result.poly = list.next;
		
		// O(n + m) runtime
		return result;

	}

	/**
	 * Returns the polynomial obtained by multiplying the given polynomial p
	 * with this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p
	 *            Polynomial with which this polynomial is to be multiplied
	 * @return A new polynomial which is the product of this polynomial and p.
	 */
	public Polynomial multiply(Polynomial p) {
		if(poly == null || p.poly == null) { return new Polynomial(); }
		Polynomial res = new Polynomial();
		Node ptr = poly; // Pointer for 'this' polynomial.
		while(ptr != null) {
			Node ptr2 = p.poly; //Pointer for argument polynomial. Reset after each multiplication cycle.
			while(ptr2 != null) {
				Polynomial tempPoly = new Polynomial();
				tempPoly.poly = new Node(ptr2.term.coeff * ptr.term.coeff, ptr.term.degree + ptr2.term.degree, null); // Multiply coefficients, add degrees.
				res = res.add(tempPoly); //Doing it this way means we don't have to sort our polynomial list.
										// Automatically will combine like terms and fit things into the right spot on the polynomial.
				ptr2 = ptr2.next;
			}
			ptr = ptr.next; //Advance poly pointer.
		}
		//Return our auto-sorted and combined result.
		return res;
	}

	/**
	 * Evaluates this polynomial at the given value of x
	 * 
	 * @param x
	 *            Value at which this polynomial is to be evaluated
	 * @return Value of this polynomial at x
	 */
	public float evaluate(float x) {
		/** COMPLETE THIS METHOD **/

		float running_total = 0;
		if (poly == null) {
			return 0;
		}

		Node temp = poly;
		while (temp != null) {
			running_total += temp.term.coeff * Math.pow(x, temp.term.degree);
			temp = temp.next;
		}

		return running_total;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String retval;

		if (poly == null) {
			return "0";
		} else {
			retval = poly.term.toString();
			for (Node current = poly.next; current != null; current = current.next) {
				retval = current.term.toString() + " + " + retval;
			}
			return retval;
		}
	}
}
