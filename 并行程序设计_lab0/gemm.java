import java.util.Scanner;
import java.util.Random;

public class gemm {
public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		System.out.println("input M:");
		int x = input.nextInt();
		System.out.println("input N:");
		int y = input.nextInt();
		System.out.println("input K:");
		int z = input.nextInt();

		double[][] a =new double[x][y];
		double[][] b =new double[y][z];
		double[][] c =new double[x][z];
		
        Random random = new Random();

		System.out.println();
        System.out.println("matrix_1:");
        for (int i = 0; i < a.length; i++) {
	        for (int j = 0; j < a[i].length; j++) {
		        a[i][j] =100* random.nextDouble();
		        System.out.print(a[i][j] + " ");
	        }  
	        System.out.println();
        }
   
		System.out.println();
        System.out.println("matrix_2:");
        for (int i = 0; i < b.length; i++) {
	        for (int j = 0; j < b[i].length; j++) {
		        b[i][j] = 100* random.nextDouble();
		        System.out.print(b[i][j] + " ");
	        }
	    System.out.println();
        }

		long start=System.currentTimeMillis();

		for (int i = 0; i < a.length; i++) {
			for (int j = 0; j < a[i].length; j++) {
				for (int k = 0; k < b[j].length; k++) {
					c[i][k] += a[i][j] * b[j][k];
				}	
			}
		}
		long end=System.currentTimeMillis();

		
		System.out.println();
        System.out.println("result:");
		for (double[] row : c){
			for (double cloumn : row) {
				System.out.print(cloumn + " ");
			}
			System.out.println();
		}
		System.out.println("using time:"  + (end-start)+" ms");
		System.out.println();
	}
}