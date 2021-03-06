package search;

import java.io.*;
import java.util.*;

public class LSETester {

	public static void main(String[] args) throws FileNotFoundException {

		LittleSearchEngine lse = new LittleSearchEngine();

		String docFile = "docs.txt"; //file containing references to all txt documents
		
//		AliceCh1.txt
//		WowCh1.txt
//		books/mice.txt
//		books/oedip10.txt
//		books/scrlt11.txt
//		books/sleep10.txt
//		books/stoker-dracula-168.txt
//		books/swift-gullivers-728.txt
//		books/wizrd_oz.txt
//		books/zenda10.txt
		lse.makeIndex(docFile, "noisewords.txt"); //Index all of our documents
		Scanner words = new Scanner(System.in);
		
		lse.loadKeyWords("pohlx.txt");
		lse.loadKeyWords("Tyger.txt");
		lse.loadKeyWords("jude.txt");
		
		
		String k1 = "", k2 = "";
		while (true) {
			System.out.println("Enter two keywords now. Type \":q\" for either keyword to quit");
			System.out.print("Keyword 1:");
			k1 = words.nextLine();
			if(k1.equals(":q")) {
				break;
			}
			System.out.print("Keyword 2:");
			k2 = words.nextLine();
			if(k2.equals(":q")) {
				break;
			}
			
			
			long pre = System.nanoTime();
			ArrayList<String> results = lse.top5search(k1, k2);
			long post = System.nanoTime();
			float seconds = (post - pre) / 1000000000f;
			if(results != null){
				System.out.println(results.size() + " results in " + seconds + " seconds.");
				for (String s : results) {
					System.out.print(s + ", ");
				}
			} else {
				System.out.println("0 results in " + seconds + " seconds.");
			}
			System.out.println();

		}
		
		System.out.println("Thank you for using the Little Search Engine!");

	}

}
