import provided.IO;

public class recit_4_1 {

	public static void main(String[] args) {
		
		Employee e = new Employee(0.0, 15.0, "Zac");
		e.work(30*15*12);
		e.printInfo();
		
		
		
		boolean result = true;
		
		System.out.println("Please Enter Word 1:");
		String w1 = IO.readString();
		System.out.println("Please Enter Word 2:");
		String w2 = IO.readString();
		
		if(w1.length() == w2.length()){
			
			for(int i = 0; i < w1.length();i++){
				
				char letter = w1.charAt(i);
				int occ1 = countOccurences(letter, w1);
				int occ2 = countOccurences(letter, w2);
				
				if(occ2 != occ1){
					result = false;
				}	
			}
		
			
		}else{
			result = false;
		}
		IO.outputBooleanAnswer(result);
		
	}
	
	
	public static int countOccurences(char letter, String word){
		int count = 0;
		for(int i = 0; i < word.length(); i++){
			if(word.charAt(i) == letter){
				count++;
			}
		}
		return count;
		
	}
	
	

}
