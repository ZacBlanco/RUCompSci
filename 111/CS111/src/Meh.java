
public class Meh {

	public static void main(String[] args) {
		
		String s = "gigi the gato";
		
		int l = s.length();
		int c = 0;
		
		while(s.length() > 0){
			
			s = s.replace(Character.toString(s.charAt(0)), "");
			if(l - s.length() >= 2){
				c += l - s.length();
			}
			l = s.length();
			
		}
		
		System.out.println(c);
		
		
	}

}
