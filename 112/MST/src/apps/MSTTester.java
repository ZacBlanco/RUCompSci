package apps;

import java.io.IOException;
import java.util.ArrayList;
import java.util.NoSuchElementException;

import apps.PartialTree.Arc;
import structures.*;
import apps.PartialTree.*;

public class MSTTester {

	public static void main(String[] args) throws Exception {
		
		System.out.println("Testing Full execution on graph 1 through 5");
		testGraph("graph1.txt", 37);
		testGraph("graph2.txt", 7);
		testGraph("graph3.txt", 8);
		testGraph("graph4.txt", 27);
		testGraph("graph5.txt", 12);
		System.out.println();
		
		

		testPartialTreeRemove();
		
		
		
	}
	
	public static int printSize(ArrayList<Arc> a) {
		int size = 0;
		for (Arc arc : a) {
			size += arc.weight;
		}
		
		System.out.println("MST Size: " + size);
		return size;
	}
	
	public static void testGraph(String graph, int expected) throws IOException {
		Graph g = new Graph(graph);
		ArrayList<Arc> mst = MST.execute(g, MST.initialize(g));
		int sz = printSize(mst);
		System.out.println("Matches expected value of " + expected + ": " + (expected == sz));
	}

	public static void testPartialTreeRemove() throws Exception{
		PartialTreeList l = new PartialTreeList();
		
		//Test single item remove
		l.append(createPT());
		assertThat(not(l.size() != 1));
		
		l.remove();
		assertThat(not(l.size() != 0));
		
		try{
			l.remove();
			fail();
		} catch(NoSuchElementException e) { }
		
		
		PartialTree first = createPT(); 
		l.append(first);
		l.append(createPT());
		l.append(createPT());
		l.append(createPT());
		int size = l.size();
		if(size != 4){
			fail();
		}
		
		PartialTree t = l.remove();
		assertThat(l.size() == 3);
		assertThat(t == first);
		
		System.out.println("Passed Partial Tree List Test!");
	}

	
	public static void fail() throws Exception {
		throw new Exception();
	}
	public static void assertThat(boolean condition) throws Exception {
		if(!condition) {
			fail();
		}
	}
	
	public static boolean not(boolean condition) throws Exception {
		return !condition;
	}
	
	public static PartialTree createPT() throws IOException {
		Graph g = new Graph("graph5.txt");
		Vertex v = g.vertices[(int)Math.random()*1000 % g.vertices.length];
		return new PartialTree(v);
	}
	
}
