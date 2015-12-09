import provided.IO;

public class Recit3_25 {

	public static void main(String[] args) {
		//prob1();
		
		String s = IO.readString();
		char[] l = s.toCharArray();
		
		for(int i = 0; i< l.length/2;i++){
			char t = l[i];
			l[i] = l[l.length - 1 - i];
			l[l.length - i - 1] = t;
		}
		
		System.out.println(toString(l));
	}
	
	public static String toString(char[] a){
		String s = "";
		for(int i = 0; i < a.length; i++){
			s += a[i];
		}
		return s;
	}
	
	

	public static void prob1() {
		System.out.println("Rows:");
		int rows = IO.readInt();
		System.out.println("Columns: ");
		int cols = IO.readInt();

		int[][] mat = new int[rows][cols];

		for (int i = 0; i < rows; i++) {
			System.out.println("Enter Vals for row " + (i + 1));
			for (int j = 0; j < cols; j++) {
				mat[i][j] = IO.readInt();
			}
		}

		double[] means = new double[mat.length];
		for (int i = 0; i < mat.length; i++) {
			means[i] = mean(mat[i]);
		}

		int max = maxInd(means) + 1;
		System.out.println("Max: " + max);

	}

	public static double mean(int[] nums) {
		int sum = 0;

		for (int i = 0; i < nums.length; i++) {
			sum += nums[i];
		}

		return ((double) sum) / nums.length;
	}

	public static int maxInd(double[] nums) {
		int maxI = -1;
		double maxVal = Double.MIN_VALUE;

		for (int i = 0; i < nums.length; i++) {
			if (nums[i] > maxVal) {
				maxI = i;
				maxVal = nums[i];
			}
		}

		return maxI;
	}

	public static void prob2() {

	}

	public static void flip(int[] a) {

		for (int i = 0; i > a.length / 2; i++) {
			int temp = a[i];
			a[i] = a[a.length - 1 - i];
			a[a.length - 1 - i] = temp;
		}

	}

}
