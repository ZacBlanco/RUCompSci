package tests;

public class ExamPractice {

	public static void main(String[] args) {
		
		String sentence = "vizualisation";
		
		String word = "is";
		
		int cursor = 0;
		int count = 0;
		
		while(sentence.indexOf(word, cursor) > 0){
			count++;
			cursor = sentence.indexOf(word, cursor) + word.length();
		}
		
		print("" + count);
		
	}
	
	public static void print(String s){
		System.out.println(s);
	}

}
