package assign3;

public class MatrixOps {
	
	public static double[][] multiply(double[][] matrix1, double[][] matrix2) {
		
		double result[][];
		
		if (matrix1.length != matrix2[0].length || matrix1[0].length != matrix2.length) {
			return null;
		}else{
			int cols = matrix1.length;
			int rows = matrix2[0].length;
			result = new double[rows][cols];
			
			for(int i = 0; i < rows;i++){
				for(int j = 0; j < cols;j++){
					result[i][j] = dot(matrix1[i], getCol(j, matrix2));
				}
			}
		}
		return result;
	}
	
	public static double dot(double[] a, double[] b){
		if(a.length != b.length){
			return Double.NaN;
		}else{
			double prod = 0;
			for(int i = 0; i < a.length; i++){
				prod += a[i]*b[i];
			}
			return prod;
		}
	}
	
	public static double[] getCol(int cn, double[][] a){
		int cLength = a.length;
		double[] col = new double[cLength];
		
		for(int i = 0; i < cLength; i++){
			col[i] = a[i][cn];
		}
		
		return col;
		
	}

}
