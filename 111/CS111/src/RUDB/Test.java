package RUDB;

import provided.IO;

public class Test {

	public static void main(String[] args) {

		String[] names = { "Zac", "Joe", "Bob", "Bill", "Robert", "Emily",
				"Frank", "Kyle", "Dan", "Marie" };
		String[] data = new String[2];
		double time;

		Database base = new Database();
		Column n = new Column("Name", String.class);
		Column b = new Column("Age", int.class);
		Column[] sc = { n, b };
		Schema s = new Schema(sc, true);

		Table t = new Table("People", s);
		base.addTable(t);

		int totalRows = 20;

		long start = System.nanoTime();
		for (int i = 0; i < totalRows; i++) {
			data[0] = names[getRand(0, names.length-1)];
			data[1] = Integer.toString(getRand(10, 100));

			boolean inserted = base.getTable("People").insert(data);

			if (!inserted) {
				System.out.println("Failed to insert row");
			}
		}

		long end = System.nanoTime();

		time = ((double) (end - start)) / 1000000000;

		print("Time to insert " + totalRows + " rows: " + time + " seconds.");
			int colWidth = 20;
			

			Schema displaySchema = new Schema(sc, false);

			
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
