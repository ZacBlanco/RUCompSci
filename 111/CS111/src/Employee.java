	public class Employee{
		
		private double rate;
		private double balance;
		String name;
		
		Employee(double balance, double rate, String name){
			this.rate = rate;
			this.balance = balance;
			this.name = name;
		}
		Employee(double rate, String name){
			this.balance = 0;
			this.rate = rate;
			this.name = name;
		}
		
		public double work(double hours){
			double money = rate*hours;
			this.balance += money;
			
			return money;
		}
		
		public void spend(double money){
			this.balance -= money;
		}
		
		public void printInfo(){
			System.out.println("Name: " + name + " Rate: " + rate + " Balance: " + balance + ".");
		}
		
		
		
		
		
		
		
		
		
		
		
		
	}