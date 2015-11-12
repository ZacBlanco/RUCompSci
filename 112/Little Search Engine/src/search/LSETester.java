package search;

import java.io.*;
import java.util.*;

public class LSETester {

	public static void main(String[] args) throws FileNotFoundException {

		LittleSearchEngine lse = new LittleSearchEngine();

		String docFile = "docs.txt"; //file containing references to all txt documents
		lse.makeIndex(docFile, "noisewords.txt"); //Index all of our documents
		Scanner words = new Scanner(System.in);

		String k1 = "", k2 = "";
		while (!k1.equals(":q")) {
			System.out.println("Enter two keywords now. Type \":q\" for keyword 1 to quit");
			System.out.print("Keyword 1:");
			k1 = words.nextLine();
			System.out.print("Keyword 2:");
			k2 = words.nextLine();

			long pre = System.nanoTime();
			ArrayList<String> results = lse.top5search(k1, k2);
			long post = System.nanoTime();
			float seconds = (post - pre) / 1000000000f;
			System.out.println(results.size() + " results in " + seconds + " seconds.");

			for (String s : results) {
				System.out.print(s + ", ");
			}
			System.out.println();

		}

	}

}
