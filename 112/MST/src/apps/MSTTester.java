package apps;

import java.io.IOException;
import java.util.ArrayList;

import apps.PartialTree.Arc;
import structures.*;

public class MSTTester {

	public static void main(String[] args) throws IOException {
		
		testGraph("graph1.txt", 37);
		testGraph("graph2.txt", 7);
		testGraph("graph3.txt", 8);
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
		System.out.println(mst.toString());
		int sz = printSize(mst);
		System.out.println("Matches expected value of " + expected + ": " + (expected == sz));
	}

}
