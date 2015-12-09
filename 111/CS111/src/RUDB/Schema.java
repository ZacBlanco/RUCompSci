package RUDB;

import java.util.HashMap;
import java.util.Map;

public class Schema {

	private Column[] cols;
	private Map<String, Column> colMap;
	private boolean hasID;

	public Schema(Column[] c, boolean IDCol) {
		cols = c;
		colMap = new HashMap<String, Column>();
		hasID = IDCol;

		for (int i = 0; i < c.length; i++) {
			colMap.put(c[i].name, c[i]);
		}
		// if (hasID) {
		// Column idCol = new Column("ID", Integer.class);
		// colMap.put("ID", idCol);
		// }

	}
	
	public String[] col2str(){
		String[] colStr = new String[cols.length];
		
		for(int i = 0; i < colStr.length;i++){
			colStr[i] = cols[i].name;
		}
		
		return colStr;
		
	}

	public Column get(String name) {
		return colMap.get(name);
	}

	public Column[] getColumns() {
		return cols;
	}

	public boolean hasID() {
		return hasID;
	}

	public String toString(int maxFieldLength) {
		String ret = "";

		if (hasID()) {
			String line = "| ID";
			int l = line.length();
			for (int i = 0; i < maxFieldLength - l - 1; i++) {
				line += " ";
			}
			ret += line + "|";
		}

		for (Column c : cols) {
			String line = "| " + c.name;
			int l = line.length();
			for (int i = 0; i < maxFieldLength - l - 1; i++) {
				line += " ";
			}
			ret += line + "|";
		}

		return ret;
	}

}
