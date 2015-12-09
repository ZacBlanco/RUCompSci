import provided.IO;


public class recit_3_11 {

	public static void main(String[] args) {
		
		barGraph();
	}
	
	public static void barGraph(){
		
		System.out.println("Number of bars");
		int bars = readInt(0, Integer.MAX_VALUE);
		
		int[] bs = new int[bars];
		for(int i = 0; i < bars; i++){
			System.out.println("Enter number for bar " + (i + 1));
			bs[i] = readInt(0, Integer.MAX_VALUE);
		}
		
		System.out.println("Bar Graph Values are: ");
		for(int j = 0; j < bs.length; j++){
			System.out.print(bs[j] + " ");
		}
		
		System.out.println();
		
		printGraph(bs);
		
		
		
	}
	
	
	public static int readInt(int min, int max){
		
		int r = IO.readInt();
		while(r < min || r > max){
			System.out.println("Try again");
			r = IO.readInt();
		}
		
		return r;
		
	}
	
	public static void printGraph(int[] bars){
		
		int lines = max(bars);
		
		for(int i = 0; i < lines; i++){
			for(int j = 0; j < bars.length; j++){
				
				if((lines - i) > bars[j]){
					System.out.print("   ");
				}else if(lines - i <= bars[j]){
					System.out.print("[] ");
				}
			}
			System.out.println();
		}
		
		
		
		
	}
	
	public static int max(int[] r){
		int m = Integer.MIN_VALUE;
		
		for(int i = 0; i < r.length; i++){
			if(r[i] > m){
				m = r[i];
			}
		}
		
		return m;
	}
	
	
	
	
	
}
