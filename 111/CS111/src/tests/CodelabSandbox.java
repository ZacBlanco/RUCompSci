package tests;

public class CodelabSandbox {

	public static void main(String[] args) {
		
		String fullName = "Arnow, David";
		String firstName = "";
		String lastName = "";
		int a = 0;
		a = fullName.indexOf(",");
		int b = 0;
		b = fullName.indexOf(" ");
		if (a > 0)
		{
		            lastName = fullName.substring(0,a);
		            firstName = fullName.substring(a+2,fullName.length());
		}
		else
		{
		            firstName = fullName.substring(0,b);
		            lastName = fullName.substring(b+1,fullName.length());
		}
		
		print(firstName);
		print(lastName);
			
		
	}
	
	
	
	public static void print(String s){
		System.out.println(s);
	}

}
