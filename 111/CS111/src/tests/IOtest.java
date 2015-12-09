package tests;

import provided.IO;

public class IOtest {

	public static void main(String[] args) {
		while (true) {
			System.out.println("Enter number for int sqrt");
			System.out.println(intSqrt(IO.readInt()));
		}
	}

	public static int intSqrt(int num) {
		if (num >= 0) {

			for (int i = 1; i < num; i++) {
				int sqr = i * i;
				int sqrm = (i - 1) * (i - 1);
				if ((sqr > num) && (sqrm <= num)) {
					return (i - 1);
				}
			}
		} else {
			return -1;
		}

		return -1;
	}

}
