package tests;

public class codelab_recursion {

	public static void main(String[] args) {
		
		printTriangle(5, 0);
		
	}
	
	public static void printTriangle(int n, int k){
		
		if(n > 0){
			if(n%2 == 0){
				printTriangle(n+1, k);
			}else if(k == 0){
				String prt = makeLine(n, 'O');
				System.out.println(prt);
				printTriangle(n-2, k+1);
			}else{
				String prt = "";
				prt += makeLine(k, ' ');
				prt += makeLine(n, 'O');
				System.out.println(prt);
				printTriangle(n-2, k+1);
			}
		}
		
		
		
	}
	
	public static String makeLine(int n, char c){
		String line = "";
		for(int i = 0; i < n;i++){
			line += Character.toString(c);
		}
		
		return line;
		
		
	}

}
