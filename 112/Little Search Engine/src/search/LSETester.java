package search;

public class LSETester {

	public static void main(String[] args) {
		
		LittleSearchEngine lse = new LittleSearchEngine();
		
		String words[] = {"distance.", "equi-distant", "Rabbit", "Between", "we're", "World", "World?!", "What,ever"};
		
		for(String w : words) {
			System.out.println(w + ", " + lse.getKeyWord(w));
		}
		
		
		
	}

}
