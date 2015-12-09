package RUDB;

import java.util.HashMap;
import java.util.Map;

public class DataRow {

	private Schema schema;
	public Map<Column, Object> data = new HashMap<Column, Object>();
	private boolean deleted;
	private int ID;

	public DataRow(Schema s, int ID) {
		schema = s;
		this.ID = ID;
		deleted = false;
	}

	public void delete() {
		deleted = true;
	}

	public boolean isDeleted() {
		return deleted;
	}

	public int getID() {
		return ID;
	}

	public Schema getSchema() {
		return schema;
	}

	public String toString() {
		String ret = "";

		for (Column s : data.keySet()) {
			ret += " | " + data.get(s);
		}
		return ret + " |";
	}

	public static String toString(DataRow r, int maxFieldLength) {

		String ret = "";

		if (r.schema.hasID()) {
			String line = "| " + r.getID();
			int l = line.length();
			for (int i = 0; i < maxFieldLength - l - 1; i++) {
				line += " ";
			}
			ret += line + "|";
		}
		
		for (Column s : r.schema.getColumns()) {
			String line = "| " + r.data.get(s);
			int l = line.length();
			for (int i = 0; i < maxFieldLength - l - 1; i++) {
				line += " ";
			}
			ret += line + "|";
		}

		return ret;
	}
	
	public boolean update(Column c, String value){
		if(isAllowed(c, value)){
			data.replace(c, value);
			return true;
		}else{
			return false;
		}
		
	}

	public String toString(Column[] c) {
		String ret = "| ";
		for (Column s : c) {
			if (data.containsKey(c)) {
				ret += s.type.cast(data.get(c)).toString();
			} else {
				return "Data Schema Does not contain Column";
			}
		}

		return ret + " |";
	}

	public boolean isAllowed(Column c, String s) {

		try {

			String className = c.type.getSimpleName();
			if (className.equals("int")) {
				Integer.parseInt(s);
				return true;
			} else if (className.equals("double")) {
				Double.parseDouble(s);
				return true;
			} else if (className.equals("boolean")) {
				Boolean.parseBoolean(s);
				return true;
			} else if (className.equals("String")) {
				return true;
			} else if (className.equals("char")) {
				if (s.length() == 1) {
					return true;
				} else {
					return false;
				}
			} else if (className.equals("long")) {
				Long.parseLong(s);
				return true;
			} else if (className.equals("byte")) {
				Byte.parseByte(s);
				return true;
			} else if (className.equals("short")) {
				Short.parseShort(s);
				return true;
			} else if (className.equals("float")) {
				Float.parseFloat(s);
				return true;
			} else {
				return false;
			}
		} catch (Exception e) {
			System.out.println("Error Converting String to Column Data Type");
			System.out.println(e.getMessage());
		}

		return false;

	}
}
