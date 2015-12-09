package RUDB;

import provided.IO;

public class SQLDatabase {

	public static void main(String[] args) {

		int colWidth = 20;
		
		//Create A table for the database.
		//Any kind of table can be created using primitive classes (and Strings).
		Database base = new Database();
		Column n = new Column("Name", String.class);
		Column b = new Column("Age", int.class);
		Column w = new Column("Wage", double.class);
		Column a = new Column("Address", String.class);
		Column[] sc = { n, b, w, a };
		Schema s = new Schema(sc, true);
		Schema displaySchema = new Schema(sc, false);
		Table t = new Table("People", s);
		base.addTable(t);
		
		
		//Database and query reading logic
		
		String lastWord = "";
		print("Welcome to RUDB! To Exit please type \"exit\"");
		print("");
		print("Below are the fields for the relation named: \"" +t.getName() + "\"" );
		print("");
		print(displaySchema.toString(colWidth));
		print("");
		while (!lastWord.equals("exit")) {
			print("Please enter a command, or exit");
			lastWord = IO.readString();
			Query q = Query.readQuery(lastWord);

			if (!lastWord.equals("exit")) {
				if (q == null || q.error()) {
					print("There was an error parsing your query");
				} else {
					print("Processing Query...");
					boolean r = false;
					if (q.isSelect()) {
						r = Select(q, base);
					} else if (q.isInsert()) {
						r = Insert(q, base);
					} else if (q.isUpdate()) {
						r = Update(q, base);
					} else if (q.isDelete()) {
						r = Delete(q, base);
					} else {
						print("Internal Query Parsing Error. Contact Pawel");
					}

					print("Query returned successful: " + r);
				}
			}
		}

		print("Thanks for using RUDB. Exiting now...");

	}

	//A simple print method to reduce amount of typing.
	public static void print(String s) {
		System.out.println(s);
	}
	
	public static boolean Update(Query q, Database b){
		boolean ret = false;
		
		if(b.TableExists(q.getRelationName())){
			if(q.getWhereID() == -1){
				ret = b.getTable(q.getRelationName()).update(q.getUpdateField(), q.getUpdateValue());
			}else{
				ret = b.getTable(q.getRelationName()).update(q.getUpdateField(), q.getUpdateValue(), q.getWhereID());
			}
		}else{
			ret = false;
		}
		
		
		return ret;
	}

	public static boolean Select(Query q, Database b) {

		if (b.TableExists(q.getRelationName())) {
			DataRow[] rows;
			if (q.whereID == -1) {
				rows = b.getTable(q.getRelationName()).select(
						q.getSelectFields());
			} else {
				rows = new DataRow[1];
				rows[0] = b.getTable(q.getRelationName()).select(
						q.getSelectFields(), q.getWhereID());
			}

			if (rows != null && rows[0] != null) {

				String header = rows[0].getSchema().toString(20);
				String underline = "";
				for (int i = 0; i < header.length(); i++) {
					underline += "-";
				}

				print(header);
				print(underline);

				for (DataRow r : rows) {
					print(DataRow.toString(r, 20));
				}

				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	public static boolean Insert(Query q, Database b) {
		boolean r = false;
		if (b.TableExists(q.getRelationName())) {
			r = b.getTable(q.getRelationName()).insert(q.getInsertValues());
		} else {
			r = false;
		}
		return r;
	}
	
	public static boolean Delete(Query q, Database b){
		boolean r = false;
		if(b.TableExists(q.getRelationName())){
			
			if(q.whereID != -1){
				r = b.getTable(q.getRelationName()).delete(q.whereID);
			}else{
				r = b.getTable(q.getRelationName()).delete();
			}
		}
		
		return r;
	}

	public static int getRand(int min, int max) {
		return (int) ((Math.random() * max) + min);
	}


}
