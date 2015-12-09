package RUDB;

public class Column {
	
	String name;
	Class type;
	
	public Column(String name, Class type){
		this.name = name;
		this.type = type;
	}
	
	public String toString(){
		return name;
	}

}
