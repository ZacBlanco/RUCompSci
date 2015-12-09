package assign3;

import provided.IO;

public class Averages {

	public static void main(String[] args) {

		double[] numbers = getInput();

		System.out.println("Mean: " + mean(numbers));
		System.out.println("Median: " + median(numbers));
		System.out.println("Mode: " + mode(numbers));
		IO.outputDoubleAnswer(min(numbers));
		IO.outputDoubleAnswer(max(numbers));
		IO.outputDoubleAnswer(sum(numbers));
		IO.outputDoubleAnswer(mean(numbers));
		IO.outputDoubleAnswer(median(numbers));

		double mode = mode(numbers);
		if (Double.isNaN(mode)) {
			IO.outputStringAnswer("");
		} else {

			IO.outputDoubleAnswer(mode);

		}
	}

	public static double[] getInput() {
		System.out.println("How many numbers would you like to enter?");
		int nums = readBoundedInt(1, Integer.MAX_VALUE);
		double[] numArr = new double[nums];
		for (int i = 0; i < nums; i++) {
			System.out.println("Please enter number " + (i + 1));
			numArr[i] = IO.readDouble();
		}

		return numArr;

	}

	public static void printArray(double[] a) {
		for (int i = 0; i < a.length; i++) {
			System.out.print(a[i] + " ");
		}
	}

	public static double min(double[] nums) {
		double min = Integer.MAX_VALUE;
		for (int i = 0; i < nums.length; i++) {
			if (nums[i] < min) {
				min = nums[i];
			}
		}
		return min;
	}

	public static double max(double[] nums) {
		double max = Integer.MIN_VALUE;
		for (int i = 0; i < nums.length; i++) {
			if (nums[i] > max) {
				max = nums[i];
			}
		}
		return max;
	}

	public static double median(double[] a) {
		sort(a);

		int mid = (a.length - 1) / 2;
		if (a.length % 2 == 0) {
			return (a[mid] + a[mid + 1]) / 2;
		} else {
			return a[mid];
		}

	}

	public static double sum(double[] nums) {
		double sum = 0;

		for (int i = 0; i < nums.length; i++) {
			sum += nums[i];
		}
		return sum;
	}

	public static void sort(double[] a) {
		boolean swapped = true;
		while (swapped == true) {
			swapped = false;
			for (int i = 0; i < a.length - 1; i++) {
				if (a[i] > a[i + 1]) {
					a = swap(i, i + 1, a);
					swapped = true;
				}
			}
		}
	}

	public static double[] swap(int n1, int n2, double[] a) {
		double temp = a[n1];
		a[n1] = a[n2];
		a[n2] = temp;

		return a;
	}

	public static double mean(double[] nums) {
		double s = 0;

		for (int i = 0; i < nums.length; i++) {
			s += nums[i];
		}

		return s / nums.length;

	}

	public static double mode(double[] a) {
		sort(a);

		double mode;
		for (int i = 0; i < a.length; i++) {

		}

		double[][] arr = new double[2][a.length];
		for (int i = 0; i < a.length; i++) {
			if (!contains(arr[0], a[i], 0)) {
				arr[0][i] = a[i];
			}
		}
		for (int j = 0; j < a.length; j++) {
			arr[1][indexOf(arr[0], a[j], 0)]++;
		}
		mode = max(arr[1]);
		int mInd = indexOf(arr[1], mode, 0);
		if (indexOf(arr[1], mode, mInd + 1) != -1) {
			return Double.NaN;
		} else {
			return arr[0][mInd];
		}

	}

	public static boolean contains(double[] a, double b, int ind) {
		boolean result = false;
		for (int i = ind + 1; i < a.length; i++) {
			if (a[i] == b) {
				result = true;
			}
		}
		return result;
	}

	public static int indexOf(double[] a, double b, int ind) {
		for (int i = ind; i < a.length; i++) {
			if (a[i] == b) {
				return i;
			}
		}
		return -1;
	}

	public static int readBoundedInt(int min, int max) {
		int s = IO.readInt();
		while (s < min || s > max) {
			System.out.println("Please enter a number between " + min + " and "
					+ max);
			s = IO.readInt();
		}

		return s;
	}
}
