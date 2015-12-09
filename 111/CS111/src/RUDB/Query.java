package RUDB;

import java.io.*;
import java.util.*;

/**
 * The Query class represents a parsed SQL query as specified by Milestone 1
 * documentation. <br>
 * <br>
 * <u>Basic Rules</u> <br>
 * NO SPACING allowed in the following cases
 * <ol>
 * <li>
 * In <b>field names</b> or <b>values inserted</b> with the INSERT query. If you
 * wish to have two (or more) word phrases use underscores.</li>
 * <li>
 * Between <b>field names</b> or <b>values inserted</b> with the INSERT query.
 * Use a comma and do not place any sort of spacing before or after the comma.</li>
 * <li>
 * Around an <b>'=' sign</b> there should be no spaces as well. This applies to
 * the 'SET' clause of UPDATE as well as the 'ID' part of WHERE clause.</li>
 * <li>
 * Between the terminating semicolon ';' and the preceeding statement</li>
 * <li>
 * Between the 'VALUES' keyword and actual values in parentheses</li>
 * </ol>
 * <br>
 * Remember that queries must end with a semicolon! <br>
 * <br>
 * If you violate any of these rules your query will be considered invalid and
 * will not be parsed. <br>
 * <br>
 * 
 * <u>Good Query Examples</u>
 * <ul>
 * <li><tt>SELECT * FROM employees;</tt></li>
 * <li><tt>SELECT field1,field2 FROM employees;</tt></li>
 * <li><tt>SELECT field1,field2 FROM employees WHERE id=2;</tt></li>
 * <li><tt>INSERT INTO employees VALUES (tom,jones,20,$344);</tt></li>
 * <li><tt>UPDATE employees SET last=jordan;</tt></li>
 * <li><tt>UPDATE employees SET last=jordan WHERE id=5;</tt></li>
 * <li><tt>DELETE FROM employees;</tt></li>
 * <li><tt>DELETE FROM employees WHERE id=6;</tt></li>
 * </ul>
 * 
 * <br>
 * 
 * <u>BAD Query Examples</u>
 * <ul>
 * <li><tt>SELECT * FROM employees ; // violates rule 4</tt></li>
 * <li><tt>SELECT field1, field2 FROM employees; // violates rule 1</tt></li>
 * <li><tt>SELECT field1,last name FROM employees; // violates rule 2</tt></li>
 * <li>
 * <tt>SELECT field1,field2 FROM employees WHERE id =2; // violates rule 3</tt></li>
 * <li>
 * <tt>INSERT INTO employees VALUES (tom joseph,jones smith,20,$344); // violates rule 1</tt>
 * </li>
 * <li><tt>UPDATE employees SET last=jordan // semicolon missing!</tt></li>
 * <li><tt>UPDATE employees SET last= jordan WHERE id=5; // violates rule 3</tt>
 * </li>
 * <li>
 * <tt>INSERT INTO employees VALUES(tom,jones,20,$344); // violates rule 5</tt></li>
 * </ul>
 * 
 * <br>
 * 
 * @author Pawel Szczurko (pszczurko@gmail.com)
 * @version 1.0, April 2015
 */
public class Query {
	/* ########## Hello Query class instance vars ########### */
	private static BufferedReader br = new BufferedReader(
			new InputStreamReader(System.in));

	private boolean isSelect, isUpdate, isDelete, isInsert;
	protected boolean error;
	protected String relationName;
	protected int whereID;

	/*---------- Query class instance vars -----------*/

	/* ########### Query constructors ########### */
	/**
	 * Default constructor. Meant for internal object creation. Not meant to be
	 * used by an outside user.
	 */
	protected Query() {
		initialize();
	}

	/**
	 * Constructor that creates query based on provided type. Meant for internal
	 * object creation. Not meant to be used by an outside user.
	 * 
	 * @param type
	 *            (required) char type specifying query type. As follows
	 *            <ul>
	 *            <li>'s' - SELECT query</li>
	 *            <li>'i' - INSERT query</li>
	 *            <li>'d' - DELETE query</li>
	 *            <li>'u' - UPDATE query</li>
	 *            </ul>
	 */
	protected Query(char type) {
		initializeType(type);
	}

	/*---------- Query constructors -----------*/

	/* ########### Query public non-static methods ########### */
	/**
	 * Once query has been parsed, this returns a String array with the fields
	 * found in a SELECT query. <br>
	 * <br>
	 * <b>NOTE:</b> Make sure to first verify that your parsed query is a SELECT
	 * query. If it is not, this method returns <tt>null</tt>.
	 * 
	 * @return String array of SELECT fields if current query in fact is a
	 *         SELECT query, <tt>null</tt> otherwise.
	 */
	public String[] getSelectFields() {
		System.out.println("WARNING, THIS IS NOT A SELECT QUERY!!");
		return null;
	}

	/**
	 * Once query has been parsed, this returns a String array with the values
	 * found in INSERT query. <br>
	 * <br>
	 * <b>NOTE:</b> Make sure to first verify that your parsed query is an
	 * INSERT query. If it is not, this method returns <tt>null</tt>.
	 * 
	 * @return String array of INSERT values if current query in fact is an
	 *         INSERT query, <tt>null</tt> otherwise.
	 */
	public String[] getInsertValues() {
		System.out.println("WARNING, THIS IS NOT AN INSERT QUERY!!");
		return null;
	}

	/**
	 * Once query has been parsed, this returns a String representing the
	 * 'field' in the UPDATE query. <br>
	 * <br>
	 * Ex: For the query "UPDATE employees SET last=jordan;", this method will
	 * return 'last' since it is the 'field' specified in this query. <br>
	 * <br>
	 * <b>NOTE:</b> Make sure to first verify that your parsed query is an
	 * UPDATE query. If it is not, this method returns <tt>null</tt>.
	 * 
	 * @return String representation of the 'field' specified in the current
	 *         query if it is an UPDATE query, returns <tt>null</tt> otherwise.
	 */
	public String getUpdateField() {
		System.out.println("WARNING, THIS IS NOT AN UPDATE QUERY!!");
		return null;
	}

	/**
	 * Once query has been parsed, this returns a String representing the
	 * 'value' in the UPDATE query. <br>
	 * <br>
	 * Ex: For the query "UPDATE employees SET last=jordan;", this method will
	 * return 'jordan' since it is the 'value' specified in this query. <br>
	 * <br>
	 * <b>NOTE:</b> Make sure to first verify that your parsed query is an
	 * UPDATE query. If it is not, this method returns <tt>null</tt>.
	 * 
	 * @return String representation of the 'value' specified in the current
	 *         query if it is an UPDATE query, returns <tt>null</tt> otherwise.
	 */
	public String getUpdateValue() {
		System.out.println("WARNING, THIS IS NOT AN UPDATE QUERY!!");
		return null;
	}

	/**
	 * Meant to be used along with <tt>static readQuery(String query)</tt>. If
	 * an error occured when parsing a query, this method will return true.
	 *
	 * @return boolean with 'true' if an error in query parsing occurred,
	 *         'false' otherwise.
	 */
	protected boolean error() {
		return error;
	}

	/**
	 * Fetches the 'ID' field specified in the WHERE part of the query.
	 *
	 * @return int representing the 'ID' field if one was found, returns
	 *         <tt>-1</tt> if no WHERE clause was present.
	 */
	public int getWhereID() {
		return whereID;
	}

	/**
	 * Fetches the relation/table name from the query.
	 *
	 * @return String representing the relation/table name specified in the
	 *         query. If an error occurred or no relation/table name was found
	 *         then <tt>NOT FOUND</tt> is returned.
	 */
	public String getRelationName() {
		return relationName;
	}

	/**
	 * Method used to check if parsed query is a SELECT query
	 *
	 * @return boolean that returns 'true' if it is a SELECT query, 'false'
	 *         otherwise.
	 */
	public boolean isSelect() {
		return isSelect;
	}

	/**
	 * Method used to check if parsed query is an UPDATE query
	 *
	 * @return boolean that returns 'true' if it is an UPDATE query, 'false'
	 *         otherwise.
	 */
	public boolean isUpdate() {
		return isUpdate;
	}

	/**
	 * Method used to check if parsed query is a DELETE query
	 *
	 * @return boolean that returns 'true' if it is a DELETE query, 'false'
	 *         otherwise.
	 */
	public boolean isDelete() {
		return isDelete;
	}

	/**
	 * Method used to check if parsed query is an INSERT query
	 *
	 * @return boolean that returns 'true' if it is an INSERT query, 'false'
	 *         otherwise.
	 */
	public boolean isInsert() {
		return isInsert;
	}

	/*---------- Query public non-static methods -----------*/

	/* ########### Query public STATIC methods ########### */
	/**
	 * Static method that repeatedly prompts a user for a query until a
	 * correctly formatted query is inputted. The method returns a Query object
	 * in the same way the the IO.java module returns the requested type. An
	 * exit token needs to be specified in order to exit from the query read. <br>
	 * <br>
	 * Basic usage: Query myQuery = Query.readQuery("exit", true);
	 *
	 * @param exitToken
	 *            String representation of the token to be used in order to exit
	 * @param ignoreCase
	 *            boolean indicating whether to ignore case for the exit token
	 * @return Query type is returned if query identified, <tt>null</tt> is
	 *         returned if exit token is encountered.
	 */
	public static Query readQuery(String exitToken, boolean ignoreCase) {
		Query finalQuery = null;
		boolean localError = false;
		while (true) {
			if (localError) {
				System.out.println("Wrong query input. Try again.");
			}
			localError = true;
			String q = getInput();
			boolean exitRead = (ignoreCase ? q.equalsIgnoreCase(exitToken) : q
					.equals(exitToken));
			if (exitRead) {
				return null;
			}
			finalQuery = readQuery(q);
			if (finalQuery == null) {
				continue;
			} else {
				break;
			}

		}
		return finalQuery;
	}

	/**
	 * Static method that analyzes the passed query and attempts to parse and
	 * return it in Query object form. This method might be useful for
	 * developing so that way a query can be typed into the source code. <br>
	 * <br>
	 * The main difference between the other readQuery method and this one is
	 * that there is no continuous prompting for a new query so this method will
	 * return <tt>null</tt> if an incorrectly formatted query has been passed. <br>
	 * <br>
	 * Basic usage: Query myQuery = Query.readQuery("select * from employees;");
	 *
	 * @param q
	 *            String representation of query to be interpreted
	 * @return Query type is returned, a return value of <tt>null</tt> will be
	 *         present if an incorrectly formatted query has been passed.
	 */
	public static Query readQuery(String q) {
		Query finalQuery = null;

		if (q.length() < 6) {
			// query should be longer than 6 characters
			return finalQuery;
		}

		StringTokenizer tok = new StringTokenizer(q.trim());
		if (tok.countTokens() < 3) {
			// min tokens for delete query (shortest query)
			// ex: delete from employees;
			return finalQuery;
		}

		String qType = tok.nextToken();
		if (qType.length() < 6) {
			// select, delete, update, insert are all of 6 characters
			return finalQuery;
		}

		qType = qType.toLowerCase();
		if (qType.equals("select")) {
			finalQuery = new Select(tok);
		} else if (qType.equals("delete")) {
			finalQuery = new Delete(tok);
		} else if (qType.equals("insert")) {
			finalQuery = new Insert(tok);
		} else if (qType.equals("update")) {
			finalQuery = new Update(tok);
		}

		if (finalQuery != null && finalQuery.error()) {
			// error has occured within child parsing
			return null;
		}
		return finalQuery;
	}

	/*---------- Query public STATIC methods -----------*/

	/* ########### Query private/protected helper methods ########### */
	private static String getInput() {
		while (true) {
			System.out.print("Enter sql> ");
			try {
				return br.readLine();
			} catch (IOException e) {
				// shouldn't happen
			}
		}
	}

	private void initialize() {
		whereID = -1;
		relationName = "NOT FOUND";
		error = false;
		isSelect = false;
		isUpdate = false;
		isDelete = false;
		isInsert = false;
	}

	private void initializeType(char type) {
		initialize();
		switch (type) {
		case 's':
			isSelect = true;
			break;
		case 'u':
			isUpdate = true;
			break;
		case 'd':
			isDelete = true;
			break;
		case 'i':
			isInsert = true;
			break;
		}
	}

	protected void parseWhere(StringTokenizer tok) {
		String potentialWhere = tok.nextToken();
		potentialWhere = potentialWhere.toLowerCase();
		if (potentialWhere.equals("where") && tok.hasMoreTokens()) {
			String potentialID = tok.nextToken();
			int equalSignPos = potentialID.indexOf("=");
			int semicolenPos = potentialID.indexOf(";");
			if (equalSignPos != -1 && semicolenPos != -1) {
				String id = potentialID.substring(equalSignPos + 1,
						semicolenPos);
				try {
					this.whereID = Integer.parseInt(id);
				} catch (NumberFormatException e) {
					this.error = true;
				} catch (Exception e) {
					this.error = true;
				}
			} else {
				this.error = true;
			}
		} else {
			this.error = true;
		}
	}

	/*---------- Query private/protected helper methods -----------*/

	private static class Select extends Query {
		private ArrayList<String> fields;
		private StringTokenizer tok;

		public Select(StringTokenizer tok) {
			super('s');
			fields = new ArrayList<String>();
			// at this point 'tok' does not have the first 'select' token
			this.tok = tok;
			parseFields();
			if (this.error) {
				// there was an error parsing fields
				return;
			}
			// at this point next token should be relation name
			String relationName = tok.nextToken();
			if (relationName.indexOf(";") != -1 && !tok.hasMoreTokens()) {
				// no where clause
				relationName = relationName.replace(";", "");
				this.relationName = relationName;
			} else if (tok.hasMoreTokens()) {
				// where clause present
				this.relationName = relationName;
				parseWhere(tok);
			} else {
				this.error = true;
			}
		}

		public String[] getSelectFields() {
			return fields.toArray(new String[fields.size()]);
		}

		private void parseFields() {
			String potentialFields = tok.nextToken();
			String potentialFrom = tok.nextToken();
			potentialFrom = potentialFrom.toLowerCase();
			if (potentialFrom.equals("from")) {
				populateFieldsList(potentialFields);
			} else {
				this.error = true;
			}
		}

		private void populateFieldsList(String fls) {
			StringTokenizer fieldsTok = new StringTokenizer(fls, ",");
			while (fieldsTok.hasMoreTokens()) {
				fields.add(fieldsTok.nextToken());
			}
		}
	}

	private static class Update extends Query {
		private StringTokenizer tok;
		private FieldValue fv;

		public Update(StringTokenizer tok) {
			super('u');
			// at this point query doesn't hold 'update' token
			this.tok = tok;
			this.fv = null;
			// min token requirement is 3 so this access is ok
			String potentialRelName = tok.nextToken();
			this.relationName = potentialRelName;
			String potentialSet = tok.nextToken();
			potentialSet = potentialSet.toLowerCase();
			if (potentialSet.equals("set") && tok.hasMoreTokens()) {
				String potentialSetClause = tok.nextToken();
				if (potentialSetClause.indexOf(";") != -1) {
					// no where clause
					potentialSetClause = potentialSetClause.replace(";", "");
					parseFieldVal(potentialSetClause);
				} else if (tok.hasMoreTokens()) {
					parseFieldVal(potentialSetClause);
					// parse where clause
					parseWhere(tok);
				} else {
					this.error = true;
				}
			} else {
				this.error = true;
			}
		}

		public String getUpdateField() {
			return fv.getField();
		}

		public String getUpdateValue() {
			return fv.getValue();
		}

		private void parseFieldVal(String fieldVal) {
			StringTokenizer fvTok = new StringTokenizer(fieldVal, "=");
			if (fvTok.countTokens() != 2) {
				this.error = true;
			} else {
				this.fv = new FieldValue(fvTok.nextToken(), fvTok.nextToken());
			}
		}
	}

	private static class Delete extends Query {
		private StringTokenizer tok;

		public Delete(StringTokenizer tok) {
			super('d');
			// at this point the query does not hold the 'delete' token
			this.tok = tok;
			// can access tokens as below because min number of tokens is 3
			// anyway
			String potentialFrom = tok.nextToken();
			potentialFrom = potentialFrom.toLowerCase();
			String potentialRelName = tok.nextToken();
			this.relationName = potentialRelName;
			if (potentialFrom.equals("from") && tok.hasMoreTokens()) {
				parseWhere(tok);
			} else if (potentialFrom.equals("from") && !tok.hasMoreTokens()) {
				this.relationName = this.relationName.replace(";", "");
			} else {
				this.error = true;
			}
		}
	}

	private static class Insert extends Query {
		private StringTokenizer tok;
		private ArrayList<String> insertValues;

		public Insert(StringTokenizer tok) {
			super('i');
			insertValues = new ArrayList<String>();
			// at this point no 'insert' token present
			this.tok = tok;

			String potentialInto = tok.nextToken();
			potentialInto = potentialInto.toLowerCase();
			String potentialRelName = tok.nextToken();
			this.relationName = potentialRelName;

			if (potentialInto.equals("into") && tok.countTokens() == 2) {
				String potentialValKeyword = tok.nextToken();
				potentialValKeyword = potentialValKeyword.toLowerCase();
				String potentialValues = tok.nextToken();
				if (potentialValKeyword.equals("values")) {
					parseValues(potentialValues);
				} else {
					this.error = true;
				}
			} else {
				this.error = true;
			}
		}

		public String[] getInsertValues() {
			return insertValues.toArray(new String[insertValues.size()]);
		}

		private void parseValues(String vals) {
			int openParPos = vals.indexOf("(");
			int closeParPos = vals.indexOf(")");
			if (openParPos == -1 || closeParPos == -1) {
				this.error = true;
				return;
			}
			vals = vals.substring(openParPos + 1, closeParPos);
			StringTokenizer valsTok = new StringTokenizer(vals, ",");
			if (valsTok.countTokens() <= 1) {
				this.error = true;
				return;
			}
			while (valsTok.hasMoreTokens()) {
				insertValues.add(valsTok.nextToken());
			}
		}
	}

	private static class FieldValue {
		private String field;
		private String value;

		public FieldValue(String f, String v) {
			field = f;
			value = v;
		}

		public String getField() {
			return field;
		}

		public String getValue() {
			return value;
		}
	}
}
