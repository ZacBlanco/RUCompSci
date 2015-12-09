package assign2;

import provided.IO;

public class TwoSmallest {

	public static void main(String[] args) {

		System.out.println("Please Enter your terminating value");
		double termValue = IO.readDouble();
		double smallest = Double.MAX_VALUE;
		double sSmallest = Double.MAX_VALUE;
		int nVals = 0;

		double lastVal;

		do {

			lastVal = IO.readDouble();

			while (lastVal == termValue && nVals < 2) {
				IO.reportBadInput();
				System.out
						.println("Please enter at least 2 values that are not the terminting value");
				lastVal = IO.readDouble();
			}
			if (lastVal != termValue) {
				if (lastVal <= smallest) {
					sSmallest = smallest;
					smallest = lastVal;
				} else if (lastVal > smallest && lastVal < sSmallest) {
					sSmallest = lastVal;
				}
			}

			if (lastVal != termValue) {
				nVals++;
			}

		} while (lastVal != termValue);
		System.out.println("Two smallest values");
		IO.outputDoubleAnswer(smallest);
		IO.outputDoubleAnswer(sSmallest);

	}

}
