package structures;

import java.util.NoSuchElementException;

class Node<T> {
	public T data;
	public Node<T> next;
	public Node(T data, Node<T> next) {
		this.data = data;
		this.next = next;
	}
	public String toString() {
		return data.toString();
	}
}

/**
 * A generic stack implementation.
 * 
 * @author ru-nb-cs111
 *
 * @param <T> Parameter type for items in the stack.
 */
public class Stack<T> {

	/**
	 * Front of linked list is top of stack.
	 */
	private Node<T> front;
	
	/**
	 * Number of items in the stack.
	 */
	private int size;
	
	/**
	 * Initializes stack to empty.
	 */
	public Stack() {
		front = null;
		size = 0;
	}
	
	/**
	 * Pushes a new item on top of stack.
	 * 
	 * @param item Item to push.
	 */
	public void push(T item) {
		front = new Node<T>(item, front);
		size++;
	}
	
	/**
	 * Pops item at top of stack and returns it.
	 * 
	 * @return Popped item.
	 * @throws NoSuchElementException If stack is empty.
	 */
	public T pop() 
	throws NoSuchElementException {
		if (front == null) {
			throw new NoSuchElementException("stack is empty!");
		}
		T data = front.data;
		front = front.next;
		size--;
		return data;
	}
	
	/**
	 * Returns item on top of stack, without popping it.
	 * 
	 * @return Item at top of stack.
	 * @throws NoSuchElementException If stack is empty.
	 */
	public T peek() 
	throws NoSuchElementException {
		if (front == null) {
			throw new NoSuchElementException("stack is empty!");
		}
		return front.data;
	}
	
	/**
	 * Tells if stack is empty.
	 * 
	 * @return True if stack is empty, false if not.
	 */
	public boolean isEmpty() {
		return size == 0;
	}
	
	/**
	 * Returns number of items in stack.
	 * 
	 * @return Number of items in stack.
	 */
	public int size() {
		return size;
	}
	
	/**
	 * Empties the stack.
	 */
	public void clear() {
		size = 0;
		front = null;
	}
	
	public String toString() {
		Node<T> curr = front;
		String out = "";
		out += "[ ";
		while(curr != null) {
			out += curr.data.toString() + ", ";
			curr = curr.next;
		}
		out += "_end_ ]";
		
		return out;
	}
  
}
