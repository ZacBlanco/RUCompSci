package assign2;

import provided.IO;

public class Scores {

	public static void main(String[] args) {

		System.out.println("Number of judges?");
		int nJudges = readBoundedInt(3, Integer.MAX_VALUE,
				"Please Enter an Integer greater than 2.");
		double[] scores = new double[nJudges];

		for (int i = 0; i < scores.length; i++) {
			System.out.println("Please Enter a score for judge " + (i + 1));
			scores[i] = readBoundedDouble(0.0, 10.0,
					"Please enter a value between 0 and 10");
		}
		
		double min = min(scores);
		double max = max(scores);
		
		double scoreSum = 0;
		for(int j = 0; j < scores.length; j++){
			scoreSum += scores[j];
		}
		
		
		double finalScore = (scoreSum - min - max)/(nJudges-2);
		
		

		System.out.println("Final Score " + finalScore);
		IO.outputDoubleAnswer(finalScore);

	}

	static int readBoundedInt(int lower, int upper, String err) {
		boolean valid = false;
		int val = IO.readInt();
		while (!valid) {
			if (val >= lower && val <= upper) {
				valid = true;
			} else {
				System.out.println(err);
				IO.reportBadInput();
				val = IO.readInt();
			}
		}
		return val;

	}

	static double readBoundedDouble(double lower, double upper, String err) {
		boolean valid = false;
		double val = IO.readDouble();
		while (!valid) {
			if (val >= lower && val <= upper) {
				valid = true;
			} else {
				System.out.println(err);
				IO.reportBadInput();
				val = IO.readDouble();
			}
		}
		return val;

	}

	static double max(double[] s) {
		double m = Double.MIN_VALUE;

		for (int i = 0; i < s.length; i++) {
			if (s[i] > m) {
				m = s[i];
			}
		}
		return m;
	}
	
	static double min(double[] s) {
		double m = Double.MAX_VALUE;

		for (int i = 0; i < s.length; i++) {
			if (s[i] < m) {
				m = s[i];
			}
		}
		return m;
	}

}
