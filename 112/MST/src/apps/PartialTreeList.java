package apps;

import java.util.Iterator;
import java.util.NoSuchElementException;

import structures.Vertex;

public class PartialTreeList implements Iterable<PartialTree> {

	/**
	 * Inner class - to build the partial tree circular linked list
	 * 
	 */
	public static class Node {
		/**
		 * Partial tree
		 */
		public PartialTree tree;

		/**
		 * Next node in linked list
		 */
		public Node next;

		/**
		 * Initializes this node by setting the tree part to the given tree, and
		 * setting next part to null
		 * 
		 * @param tree
		 *            Partial tree
		 */
		public Node(PartialTree tree) {
			this.tree = tree;
			next = null;
		}
	}

	/**
	 * Pointer to last node of the circular linked list
	 */
	private Node rear;

	/**
	 * Number of nodes in the CLL
	 */
	private int size;

	/**
	 * Initializes this list to empty
	 */
	public PartialTreeList() {
		rear = null;
		size = 0;
	}

	/**
	 * Adds a new tree to the end of the list
	 * 
	 * @param tree
	 *            Tree to be added to the end of the list
	 */
	public void append(PartialTree tree) {
		Node ptr = new Node(tree);
		if (rear == null) {
			ptr.next = ptr;
		} else {
			ptr.next = rear.next;
			rear.next = ptr;
		}
		rear = ptr;
		size++;
	}

	/**
	 * Removes the tree that is at the front of the list.
	 * 
	 * @return The tree that is removed from the front
	 * @throws NoSuchElementException
	 *             If the list is empty
	 */
	public PartialTree remove() throws NoSuchElementException {
		if (rear == null) {
			throw new NoSuchElementException("Partial Tree List is Empty.");
		} else if( rear == rear.next ) {
			Node removed = rear;
			size--;
			return removed.tree;
		}
		Node removed = rear.next;
		rear.next = removed.next;
		size--;

		return removed.tree;
	}

	/**
	 * Removes the tree in this list that contains a given vertex.
	 * 
	 * @param vertex
	 *            Vertex whose tree is to be removed
	 * @return The tree that is removed
	 * @throws NoSuchElementException
	 *             If there is no matching tree
	 */
	public PartialTree removeTreeContaining(Vertex vertex)
			throws NoSuchElementException {

		if (rear == null) {
			// case where rear is null;
			throw new NoSuchElementException("PartialTreeList is Empty");

		} else if (rear.next == rear) {
			if (PTContains(rear.tree, vertex)) {
				PartialTree r = rear.tree;
				rear = null;
				size--;
				return r;
			} else {
				throw new NoSuchElementException("Vertex does not exist");
			}
		}

		Node ptr = rear.next, trailer = rear;
		if(vertex.name.equals("C")){
			System.out.println("");
		}
		do {

			if (PTContains(ptr.tree, vertex)) {
				if(ptr == rear) {
					rear = trailer;
				}
				PartialTree result = ptr.tree;
				trailer.next = ptr.next;
				size--;
				return result;
			}
			
			trailer = ptr;
			ptr = ptr.next;

		} while (trailer != rear);

		throw new NoSuchElementException("Vertex " + vertex.name + " does not exist in PTL " + this.toString());
	}

	private static boolean PTContains(PartialTree pt, Vertex v) {
		if (pt == null) {
			return false;
		} else if (v == null) {
			return false;
		}

		Vertex ptr = pt.getRoot();
		Vertex trail;

		if (ptr == null) {
			return false;
		} else if (ptr.name.equals(v.name)) {
			return true;
		} else {
				trail = ptr;
				ptr = ptr.parent;
		}

		int counter=0;
		
		while (ptr != trail) {
			if (ptr.name.equals(v.name)) {
				return true;
			}
			if(counter > 100){
				System.out.println("Uh Oh");
			}
			trail = ptr;
			ptr = ptr.parent;
			counter++;
		}
		return false;
	}

	/**
	 * Gives the number of trees in this list
	 * 
	 * @return Number of trees
	 */
	public int size() {
		return size;
	}

	/**
	 * Returns an Iterator that can be used to step through the trees in this
	 * list. The iterator does NOT support remove.
	 * 
	 * @return Iterator for this list
	 */
	public Iterator<PartialTree> iterator() {
		return new PartialTreeListIterator(this);
	}

	private class PartialTreeListIterator implements Iterator<PartialTree> {

		private PartialTreeList.Node ptr;
		private int rest;

		public PartialTreeListIterator(PartialTreeList target) {
			rest = target.size;
			ptr = rest > 0 ? target.rear.next : null;
		}

		public PartialTree next() throws NoSuchElementException {
			if (rest <= 0) {
				throw new NoSuchElementException();
			}
			PartialTree ret = ptr.tree;
			ptr = ptr.next;
			rest--;
			return ret;
		}

		public boolean hasNext() {
			return rest != 0;
		}

		public void remove() throws UnsupportedOperationException {
			throw new UnsupportedOperationException();
		}

	}
}
