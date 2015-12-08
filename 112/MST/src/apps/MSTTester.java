package apps;

import java.io.IOException;
import java.util.ArrayList;

import apps.PartialTree.Arc;
import structures.*;

public class MSTTester {

	public static void main(String[] args) throws IOException {
		
		Graph g = new Graph("graph2.txt");
		ArrayList<Arc> mst = MST.execute(g, MST.initialize(g));
		System.out.println(mst.toString());
		printSize(mst);
		
		g = new Graph("graph1.txt");
		mst = MST.execute(g, MST.initialize(g));
		System.out.println(mst.toString());
		printSize(mst);
	}
	
	public static void printSize(ArrayList<Arc> a) {
		int size = 0;
		for (Arc arc : a) {
			size += arc.weight;
		}
		
		System.out.println("MST Size: " + size);
	}

}
