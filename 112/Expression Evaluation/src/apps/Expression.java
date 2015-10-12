package apps;

import java.io.IOException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.StringTokenizer;

import structures.Stack;

public class Expression {

	/**
	 * Expression to be evaluated
	 */
	String expr;                
    
	/**
	 * Scalar symbols in the expression 
	 */
	ArrayList<ScalarSymbol> scalars;   
	
	/**
	 * Array symbols in the expression
	 */
	ArrayList<ArraySymbol> arrays;
    
	/**
	 * Positions of opening brackets
	 */
	ArrayList<Integer> openingBracketIndex; 
    
	/**
	 * Positions of closing brackets
	 */
	ArrayList<Integer> closingBracketIndex; 

    /**
     * String containing all delimiters (characters other than variables and constants), 
     * to be used with StringTokenizer
     */
    public static final String delims = " \t*+-/()[]";
    
    /**
     * Initializes this Expression object with an input expression. Sets all other
     * fields to null.
     * 
     * @param expr Expression
     */
    public Expression(String expr) {
        this.expr = expr;
        scalars = null;
        arrays = null;
        openingBracketIndex = null;
        closingBracketIndex = null;
    }

    /**
     * Matches parentheses and square brackets. Populates the openingBracketIndex and
     * closingBracketIndex array lists in such a way that closingBracketIndex[i] is
     * the position of the bracket in the expression that closes an opening bracket
     * at position openingBracketIndex[i]. For example, if the expression is:
     * <pre>
     *    (a+(b-c))*(d+A[4])
     * </pre>
     * then the method would return true, and the array lists would be set to:
     * <pre>
     *    openingBracketIndex: [0 3 10 14]
     *    closingBracketIndex: [8 7 17 16]
     * </pe>
     * 
     * See the FAQ in project description for more details.
     * 
     * @return True if brackets are matched correctly, false if not
     */
    public boolean isLegallyMatched() {
    	// COMPLETE THIS METHOD
    	
    	if(expr == null) {
    		return false;
    	}
    	
    	boolean result =  false;
    	ArrayList<Integer> openingBrackets = new ArrayList<Integer>(), closingBrackets = new ArrayList<Integer>();
    	Stack<Character> brackets = new Stack<Character>();
    	char[] exprArray = expr.toCharArray();
    	for(int i = 0; i < exprArray.length; i++) {
    		try {
	    		if( exprArray[i] == '[' || exprArray[i] == '(') {
	    			brackets.push(exprArray[i]);
	    			openingBrackets.add(i);
	    		} else if ( exprArray[i] == ']' && brackets.peek() == '[' ) {
	    			brackets.pop();
	    			closingBrackets.add(i);
	    		} else if( exprArray[i] == ')' && brackets.peek() == '(') {
	    			brackets.pop();
	    			closingBrackets.add(i);
	    		}
    		} catch (Exception e) {
    			return false;
    		}
    	}
    	if(brackets.isEmpty()) {
    		result = true;
    		openingBracketIndex = openingBrackets;
    		closingBracketIndex = closingBrackets;
    	}
    	return result;
    }

    /**
     * Populates the scalars and arrays lists with symbols for scalar and array
     * variables in the expression. For every variable, a SINGLE symbol is created and stored,
     * even if it appears more than once in the expression.
     * At this time, the constructors for ScalarSymbol and ArraySymbol
     * will initialize values to zero and null, respectively.
     * The actual values will be loaded from a file in the loadSymbolValues method.
     */
    public void buildSymbols() {
    	// COMPLETE THIS METHOD
    	ScalarSymbol s;
    	ArraySymbol as;
    	char[] exprArray = expr.toCharArray();
    	scalars = new ArrayList<ScalarSymbol>();
    	arrays = new ArrayList<ArraySymbol>();
    	String currentSymbol = "";
    	for(int i = 0; i < exprArray.length; i++) {
    		if(!delims.contains("" + exprArray[i])) {
    			if (!Character.isDigit(exprArray[i])) {
    				currentSymbol += exprArray[i];
    			}
    		} else if (delims.contains("" + exprArray[i])) {
    			if(currentSymbol.length() < 1) {
    				continue;
    			} else {
    				if (exprArray[i] == '[' && currentSymbol.length() > 0) {
    					as = new ArraySymbol(currentSymbol);
    					if(!arrays.contains(as)) {
    						arrays.add(as);
    					}
						currentSymbol = "";
    				} else if(currentSymbol.length() > 0) {
    					s = new ScalarSymbol(currentSymbol);
    					if(!scalars.contains(s)){
    						scalars.add(s);
    					}
						currentSymbol = "";

    				}
    			}
    		}
    	}
    	
		s = new ScalarSymbol(currentSymbol);
		if(!scalars.contains(s)){
			scalars.add(s);
		}
		currentSymbol = "";
    	return;
    }
    
    /**
     * Loads values for symbols in the expression
     * 
     * @param sc Scanner for values input
     * @throws IOException If there is a problem with the input 
     */
    public void loadSymbolValues(Scanner sc) 
    throws IOException {
        while (sc.hasNextLine()) {
            StringTokenizer st = new StringTokenizer(sc.nextLine().trim());
            int numTokens = st.countTokens();
            String sym = st.nextToken();
            ScalarSymbol ssymbol = new ScalarSymbol(sym);
            ArraySymbol asymbol = new ArraySymbol(sym);
            int ssi = scalars.indexOf(ssymbol);
            int asi = arrays.indexOf(asymbol);
            if (ssi == -1 && asi == -1) {
            	continue;
            }
            int num = Integer.parseInt(st.nextToken());
            if (numTokens == 2) { // scalar symbol
                scalars.get(ssi).value = num;
            } else { // array symbol
            	asymbol = arrays.get(asi);
            	asymbol.values = new int[num];
                // following are (index,val) pairs
                while (st.hasMoreTokens()) {
                    String tok = st.nextToken();
                    StringTokenizer stt = new StringTokenizer(tok," (,)");
                    int index = Integer.parseInt(stt.nextToken());
                    int val = Integer.parseInt(stt.nextToken());
                    asymbol.values[index] = val;              
                }
            }
        }
    }
    
    /**
     * Evaluates the expression, using RECURSION to evaluate subexpressions and to evaluate array 
     * subscript expressions. (Note: you can use one or more private helper methods
     * to implement the recursion.)
     * 
     * @return Result of evaluation
     */
    public float evaluate() {
    	return evaluate(expr);
    }
    
    private float evaluate(String expression) {
    	char[] exprArray = expression.toCharArray();
    	Stack<Float> operands = new Stack<Float>();
    	Stack<Character> operators = new Stack<Character>();
    	
    	//First we're going to tokenize by symbols, literal values, and subexpressions;
    	String cToken = "";
    	for(int i = 0; i < exprArray.length; i++) {
    		if(!delims.contains(""+exprArray[i])) {
    			cToken += exprArray[i];
    		} else if (exprArray[i] == '(' ) {
    			int b2 = findMatchingBrace(i, expression);
    			String sub = expression.substring(i + 1,  b2);
    			String eval = "" + evaluate(sub);
    			cToken = eval;
    			i = b2;
    		}else if( exprArray[i] == '[') {
    			int b2 = findMatchingBrace(i, expression);
    			String sub = expression.substring(i + 1,  b2);
    			int eval = (int)evaluate(sub);	
    			String val;
    			i=b2;
    			ArraySymbol as = arrays.get(arrays.indexOf(new ArraySymbol(cToken)));
    			int[] values = as.values;
				if(values == null) {
					throw new Error("Array values are null");
				} else if(eval < 0 || eval >= values.length) {
    				// Throw error
    				throw new Error("Array index is out of bounds. Index is " + eval + ". Array is symbol: " + as.name);
    			} else {
    				val = "" + values[eval];
    			}
    			cToken = val;
    			
    		} else if (exprArray[i] != ' ') {
    			
    			// This means we've hit an operator, let's first push our latest operand to the stack.
    			// If this is a variable then we'll need to get it's value to store it.

    			operands.push(getScalarSymbolValue(cToken));
    			if(!operators.isEmpty() && operators.peek() == '-') {
    				operators.pop();
    				operands.push(operands.pop() * -1.0f);
    				operators.push('+');
    			}
    			// If the operator we're about to push is of lesser precedence than the top
    			// We need to pop the top two operands, the top operator, operate on those two
    			// Then we can push the newest operand and operator.
					while(!operators.isEmpty() && comparePrecedence(operators.peek(), exprArray[i]) >= 0) {
 	    				float opand2 = operands.pop();
	    				float opand1 = operands.pop();
	    				Character operator = operators.pop();
	    				operands.push(operate(operator, opand1, opand2));
					}
					operators.push(exprArray[i]);
					
    			cToken = "";
    		}
    	}
    	// Need to perform a bit of manipulation for this last one.
    	// Always remember to include our last operand from the stack. There should never be an operator afterwards.
    	
    	operands.push(getScalarSymbolValue(cToken));
    	
    	if(operands.size() - 1 == operators.size()) {
    		while(!operators.isEmpty()){
    			float op2 = operands.pop();
    			float op1 = operands.pop();
    			char op = operators.pop();
    			operands.push(operate(op, op1, op2));
        	}
    	} else {
    		throw new Error("stack sizes not correct");
    	}
    	return operands.pop();
    }
    
    private float getScalarSymbolValue(String token) {
    	int symbolIndex = scalars.indexOf(new ScalarSymbol(token));
    	float f;
		if(symbolIndex != -1) {
			f = scalars.get(symbolIndex).value;
			
		} else {
			//Let's try to assume this is a float value and push it to the operands
			try {
				f = Float.parseFloat(token);
			} catch (NumberFormatException e) {
				throw new Error("Could not parse token " + token);
			}
		}
		
		return f;
    }
    
    // Given our first argument, a, if it takes lesser precedence to b, then we return -1
    //Greater precedence is +1, equal precedence is 0.
    private int comparePrecedence(char a, char b) {
    	int result = 0;
    	if(a == '+' || a == '-') {
    		if(b == '*' || b == '/') {
    			result = -1;
    		} else {
    			result = 0;
    		}
    	} else if (a == '*' || a == '/') {
    		if(b == '*' || b == '/') {
    			result = 0;
    		} else {
    			result = 1;
    		}
    	}
    	return result;
    }
    
    private float operate(char operator, float a, float b) {
    	switch (operator) {
    	case '+': return a + b;
    	case '-': return a - b;
    	case '*': return a * b;
    	case '/': return a / b;
    	default: throw new Error("Operator " + operator + " not recognized");
    	}
    }
    
    private static int findMatchingBrace(int braceLocation, String expression) {
    	
    	Stack<Character> braces = new Stack<Character>();
    	braces.push(expression.charAt(braceLocation));
    	
    	for(int i = braceLocation + 1; i < expression.length(); i++) {
    		if(expression.charAt(i) == '(' || expression.charAt(i) == '[') {
    			braces.push(expression.charAt(i));
    		} else if(expression.charAt(i) == ']' || expression.charAt(i) == ')') {
    			if(braces.peek() == '(' && expression.charAt(i) == ')') {
    				braces.pop();
    			} else if( braces.peek() == '[' && expression.charAt(i) == ']') {
    				braces.pop();
    			} else {
    				throw new Error("Bad Braces at expression index " + i);
    			}
    			
    			if(braces.isEmpty()) {
    				return i;
    			}
    		}
    	}
    	throw new Error("Could not find matching brace for brace at index " + braceLocation + " of expression.");
    }

    /**
     * Utility method, prints the symbols in the scalars list
     */
    public void printScalars() {
        for (ScalarSymbol ss: scalars) {
            System.out.println(ss);
        }
    }
    
    /**
     * Utility method, prints the symbols in the arrays list
     */
    public void printArrays() {
    	for (ArraySymbol as: arrays) {
    		System.out.println(as);
    	}
    }

}
