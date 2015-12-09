import provided.IO;

public class Recit2_25 {

	public static void main(String[] args) {

		// Digits();
		horzPyramid();

	}

	public static void Digits() {

		System.out.println("Please enter an integer");
		int num = IO.readInt();

		char[] ch = Integer.toString(num).toCharArray();
		for (int q = 0; q < ch.length; q++) {
			System.out.println(ch[q]);
		}

	}

	public static void vertPyramid() {

		int levels;
		System.out.println("How many levels?");
		levels = IO.readInt();

		for (int i = 0; i < levels; i++) {

			int ast = (i * 2) + 1;
			int spaces = ((levels - 1) * 2) + 1 - ast;

			String asterisk = "";
			String spaceStr = " ";

			for (int j = 0; j < ast; j++) {
				asterisk += "*";
			}
			for (int k = 0; k < spaces / 2; k++) {
				spaceStr += " ";
			}

			System.out.println((spaceStr) + asterisk);
		}

	}

	public static void horzPyramid() {
		int levels;

		System.out.println("How many levels?");
		levels = IO.readInt();

		for (int i = 0; i <= levels; i++) {
			int level = i;
			for (int j = 0; j < level; j++) {
				System.out.print("*");
			}
			System.out.println();
		}
		
		for (int i = levels -1; i > 0; i--) {
			int level = i;
			for (int j = 0; j < level; j++) {
				System.out.print("*");
			}
			System.out.println();

		}

	}

}
