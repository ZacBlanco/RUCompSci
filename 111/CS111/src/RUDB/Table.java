package RUDB;

public class Table {

	private Schema schema;
	private DataRow[] rows;
	private int indexNum = 0;
	private String name;

	public Table(String name, Schema s) {
		this.name = name;
		schema = s;
		rows = new DataRow[2];
	}

	public String getName() {
		return name;
	}

	public int getNumRows() {
		return indexNum + 1;
	}

	private DataRow newRow() {

		DataRow dr = new DataRow(schema, getCurrID());
		indexNum++;
		return dr;
	}

	private int getCurrID() {
		return indexNum + 1;
	}

	public boolean insert(String[] qData) {
		// Check if amount of data is equal to the number of columns for our
		// table
		if (qData.length == schema.getColumns().length) {
			// Now we can try to add the data to a new dataRow for each column,
			// casting to the columns class type.

			Column[] cols = schema.getColumns();
			DataRow r = newRow();
			try {
				for (int i = 0; i < cols.length; i++) {
					if (r.isAllowed(cols[i], qData[i])) {
						Object item = qData[i];
						r.data.put(cols[i], item);
					} else {
						indexNum--;
						return false;
					}
				}

				// Here we make sure that the ID is equal to the array's index
				// so we can find it in O(1) time by the where clause. Also
				// ensures proper indexing
				if (r.getID() == indexNum) {
					if (indexNum < rows.length) {
						rows[indexNum - 1] = r;
					} else {
						expandRows();
						rows[indexNum - 1] = r;
					}

					return true;
				} else {
					indexNum--;
					return false;
				}
			} catch (Exception e) {
				System.out.println("Error Inserting data" + e.getMessage());
				indexNum--;
				return false;
			}
		} else {
			return false;
		}
	}

	public DataRow select(String[] cols, int id) {
		if (colsExist(cols)) {
			Column[] c = Str2Col(cols);
			Schema ret = new Schema(c, wantsID(cols));

			if ((id - 1) < indexNum && (id - 1) >= 0) {
				DataRow oldRow = rows[id - 1];
				if (oldRow.isDeleted()) {
					System.out
							.println("ID specified has been deleted permanantly.");
					return null;
				} else {
					DataRow newRow = selectCols(ret, oldRow);

					return newRow;
				}

			} else {
				System.out
						.println("The ID you entered is not part of this table");
				return null;
			}

		} else {
			System.out.println("One of the columns specified does not exist.");
			return null;
		}

	}

	public DataRow[] select(String[] cols) {

		if (colsExist(cols)) {
			Column[] c = Str2Col(cols);
			Schema ns = new Schema(c, wantsID(cols));
			DataRow[] allRows = Rows();
			DataRow[] r = new DataRow[allRows.length];
			if (r.length > 0) {

				for (int i = 0; i < r.length; i++) {
					r[i] = selectCols(ns, allRows[i]);
				}

				return r;
			} else {
				System.out.println("There are no rows to return here.");
				return null;
			}

		} else {
			System.out.println("One of the columns specified does not exist.");
			return null;
		}

	}

	public boolean delete() {

		DataRow[] ndRows = Rows();

		if (ndRows.length > 0) {

			for (int i = 0; i < ndRows.length; i++) {
				ndRows[i].delete();
			}

			return true;
		} else {
			System.out.println("This table has no rows to return.");
			return false;
		}

	}

	public boolean delete(int whereID) {

		if ((whereID - 1) < indexNum && whereID > 0) {

			DataRow[] rs = allRows();

			DataRow dr = rs[whereID - 1];

			if (dr.getID() == whereID && !dr.isDeleted()) {
				dr.delete();
				return true;
			} else {
				System.out.println("Record with ID: " + whereID
						+ " does not exist.");
				return false;
			}

		} else {
			return false;
		}

	}

	public boolean update(String field, String value) {
		DataRow[] rs = Rows();

		if (field.toLowerCase().equals("id")) {
			System.out.println("Can't modify the id");
			return false;
		} else {

			for (int i = 0; i < rs.length; i++) {
				Column c = Str2Col(field);
				boolean valid = rs[i].update(c, value);

				if (!valid) {
					System.out
							.println("Field/Value combination not of correct datatype.");
					return false;
				}
			}
		}
		return true;

	}

	public boolean update(String field, String value, int id) {
		if (id - 1 < indexNum) {

			DataRow row = rows[id - 1];
			if (!row.isDeleted()) {
				boolean valid = row.update(Str2Col(field), value);
				if (!valid) {
					System.out
							.println("Field/Value pair not of valid datatype.");
					return false;
				}
				return valid;
			}

		}
		System.out.println("ID does not exist in table");
		return false;
	}

	private boolean wantsID(String[] cols) {
		for (int i = 0; i < cols.length; i++) {
			if (cols[i].equals("ID") || cols[i].equals("*")) {
				return true;
			}
		}
		return false;
	}

	private DataRow selectCols(Schema newSchema, DataRow r) {
		DataRow newRow = new DataRow(newSchema, r.getID());
		Column[] cols = newSchema.getColumns();
		for (int i = 0; i < cols.length; i++) {
			newRow.data.put(cols[i], r.data.get(cols[i]));
		}
		return newRow;

	}

	private boolean colsExist(String[] cols) {
		if (cols.length > 0 && !cols[0].equals("*")) {
			for (int i = 0; i < cols.length; i++) {
				if (schema.get(cols[i]) == null
						&& !cols[i].toLowerCase().equals("id")) {
					return false;
				}
			}
		}

		return true;
	}

	private boolean colExists(String col) {
		if (schema.get(col) == null && !col.toLowerCase().equals("id")) {
			return false;
		} else {
			return true;
		}
	}

	private boolean isSelectAll(String[] cols) {
		if (cols.length == 1 && cols[0].equals("*")) {
			return true;
		} else {
			return false;
		}
	}

	public Column Str2Col(String col) {
		if (colExists(col)) {
			if (col.toLowerCase().equals("id")) {
				System.out.println("Can't modify the ID column");
				return null;
			}else{
				Column c = schema.get(col);
				return c;
			}
		}
		return null;
	}

	public Column[] Str2Col(String[] cols) {
		if (colsExist(cols)) {
			if (isSelectAll(cols)) {
				return schema.getColumns();
			}

			Column[] c;
			if (wantsID(cols)) {
				c = new Column[cols.length - 1];
			} else {
				c = new Column[cols.length];
			}

			// Use a k to make sure we skip over the ID column
			// allows us to remove ID and ensure rest of columns present
			int k = 0;
			for (int i = 0; k < c.length; i++) {
				if (!cols[i].equals("ID")) {
					c[k] = schema.get(cols[i]);
				} else {
					k--;
				}
				k++;
			}

			return c;
		} else {
			return null;
		}

	}

	private void expandRows() {
		DataRow[] r = new DataRow[rows.length * 2];
		for (int i = 0; i < rows.length; i++) {
			r[i] = rows[i];
		}

		rows = r;
	}

	public Schema getSchema() {
		return schema;
	}

	// private because this will return deleted rows.
	private DataRow[] Rows() {
		DataRow[] r;
		int ndRowCount = 0;

		for (int i = 0; i < indexNum; i++) {
			if (!rows[i].isDeleted()) {
				ndRowCount++;
			}
		}

		r = new DataRow[ndRowCount];

		int cInd = 0;
		for (int i = 0; i < indexNum; i++) {
			if (!rows[i].isDeleted()) {
				r[cInd] = rows[i];
				cInd++;
			}

		}
		return r;
	}

	private DataRow[] allRows() {
		DataRow[] r = new DataRow[indexNum];

		for (int i = 0; i < r.length; i++) {
			r[i] = rows[i];
		}

		return r;

	}

}
