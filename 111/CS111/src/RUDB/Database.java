package RUDB;

public class Database {

	Table[] tables = new Table[2];
	private int nTables = 0;

	public Database() {
	}

	public void addTable(Table t) {

		if (tables[nTables] == null && nTables < tables.length) {
			tables[nTables] = t;
		} else if (nTables >= tables.length) {

			Table[] temp = tables;
			tables = new Table[(nTables - 1) * 2];
			for (int i = 0; i < temp.length; i++) {
				tables[i] = temp[i];
			}

			tables[nTables] = t;

		}
		nTables++;
	}

	public Table getTable(String tableName) {
		for (int i = 0; i < tables.length; i++) {
			if (tables[i] != null) {
				if (tableName.equals(tables[i].getName())) {
					return tables[i];
				}
			}
		}

		return null;
	}
	
	public boolean TableExists(String tableName){
		for(Table t : Tables()){
			if(t.getName().equals(tableName)){
				return true;
			}
		}
		return false;
	}
	
	public Table[] Tables(){
		Table[] t = new Table[nTables];
		
		for(int i = 0; i < t.length;i++){
			t[i] = tables[i];
		}
		
		return t;
	}

	public void delete() {

	}

	public void update() {

	}

}
