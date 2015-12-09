package assign2;

import provided.IO;

public class NthPrime {

	public static void main(String[] args) {
		
		System.out.println("Please enter an integer of the Nth Prime you would like to see.");
		int NthP = readBoundedInt(1, Integer.MAX_VALUE, "Please enter an integer greater than 0.");
		int currPrime = 1;
		int currNumber = 2;
		int lastPrime = 0;
		
		while(currPrime <= NthP){
			
			boolean isPrime = true;
			for(int n = currNumber - 1; n > 1; n--){
				if(currNumber % n == 0){
					isPrime = false;
				}
			}
			
			if(isPrime){
				currPrime++;
				lastPrime = currNumber;
			}
			
			currNumber++;
			
		}
		
		IO.outputIntAnswer(lastPrime);

	}
	
	static int readBoundedInt(int lower, int upper, String err){
		int input;
		do{
			input = IO.readInt();
			
			
			
			if(input < lower || input > upper){
				IO.reportBadInput();
				System.out.println(err);
			}
			
		}while(input < lower || input > upper);
		
		return input;
	}

}
