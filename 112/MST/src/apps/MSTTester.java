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
		
	}

}
