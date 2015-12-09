import java.util.Arrays;

public class recit_4_22 {

	public static void main(String[] args) {
		
		String needle = "ccccneedle";
		String[] letters = {"a", "b", "c", "d", "e", "f", "g"};
		
		String[] haystack = new String[20];
		
		for(int i = 0; i < haystack.length;i++){
			haystack[i] = letters[(int)(Math.random()*(letters.length))];
		}
		
		haystack[6] = needle;
		Arrays.sort(haystack);
		print(haystack);
		print("");
		System.out.println(binarySearch(needle, haystack));
		
		
		

	}
	public static void print(String[] a){
		for(String s: a){
			System.out.print(s + ", ");
		}
	}
	public static void print(String s){
		System.out.println(s);
	}
	public static int binarySearch(String needle, String[] haystack) {
		Arrays.sort(haystack);
		int low = 0;
		int high = haystack.length-1;

		while (high > low) {

			if (high-low >= 2) {
				
				int searchPoint = ((high - low) / 2) + low;
				if (needle.compareTo(haystack[searchPoint]) > 0) {
					low = searchPoint;
				} else if (needle.compareTo(haystack[searchPoint]) < 0) {
					high = searchPoint;

				} else if (needle.equals(haystack[searchPoint])) {
					return searchPoint;
				}
				
			}else{
				high = low;
			}

		}
		return -1;

	}

}
