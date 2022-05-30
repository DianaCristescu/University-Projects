import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class Registre extends Task{    
    ArrayList<Pair> graph = new ArrayList<>();
    ArrayList<Integer> result = new ArrayList<>();
    boolean found = false;
    String answer = null;
    int n, m, k, k_max;

    @Override
    public void solve() throws IOException, InterruptedException {
        this.readProblemData();
        for (k = 1; k <= k_max; k++) {
            this.formulateOracleQuestion();
            this.decipherOracleAnswer();
            if (found) {
                break;
            }
        }
        this.writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        Scanner scanner = new Scanner(System.in);

        //Get input data
        n = scanner.nextInt();
        m = scanner.nextInt();
        k_max = scanner.nextInt();
        for (int i = 0; i < m; i++) {
            graph.add(new Pair(scanner.nextInt(), scanner.nextInt()));
        }
        scanner.close();
    }

    @Override
    public void formulateOracleQuestion() throws IOException {
        File out = new File("sat.cnf");
        FileWriter myWriter = new FileWriter(out);
        String formula = "p cnf " + n*k + " ";
        String clauses = "";
        int nr_clauses = 0;
        int x;

        //Makes sure all variables have a register (#1) -> O(n*k)
        for (int v = 1; v <= n; v++) {
            for (int i = 0; i < k; i++) {
                x = (n * i + v);
                clauses += x + " ";
            }
            clauses += "0";
            clauses += "\n";
            nr_clauses++;
        }

        //Checks if a variable is in 2 registers (#2) -> O(n*k*(k-1))
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                if (i != j) {
                    for (int v = 1; v <= n; v++) {
                        x = - (n * i + v);
                        clauses += x + " ";
                        x = - (n * j + v);
                        clauses += x + " 0";
                        clauses += "\n";
                        nr_clauses++;
                    }
                }
            }
        }

        //Checks if 2 variables that have a connection aren't in the same register (#3) -> O(!n*k)
        for (int v = 1; v <= n; v++) {
            for (int w = v + 1; w <= n; w++) {
                if (graph.contains(new Pair(v, w))) {
                    for (int i = 0; i < k; i++) {
                        x = - (n * i + v);
                        clauses += x + " ";
                        x = - (n * i + w);
                        clauses += x + " 0";
                        clauses += "\n";
                        nr_clauses++;
                    }
                }
            }
        }

        //Ask oracle
        formula += nr_clauses + "\n" + clauses;
        out.createNewFile();
        myWriter.write(formula);
        myWriter.close();
        try {
            this.askOracle();
        } catch (InterruptedException e) {
            System.out.println("Exception: " + e);
        }
        out.delete();
    }

    @Override
    public void decipherOracleAnswer() throws IOException {
        File in = new File("sat.sol");
        Scanner scanner = new Scanner(in);
        int v, register, variable;

        //If an answer has been found
        if (scanner.nextBoolean()) {
            int result[] = new int[n];
            answer = "True\n";
            v = scanner.nextInt();

            //Puts the registre at index variable in result 
            while (scanner.hasNextInt()) {
                v = scanner.nextInt();
                if (v > 0) {
                    variable = v % n;
                    register = (int)(v / n) + 1;
                    if (variable == 0) {
                        variable = n;
                        register--;
                    }
                    result[variable - 1] = register;
                }
            }

            //Saves the answer
            for (int i = 0; i < n; i++) {
                answer += result[i] + " ";
            }
            answer += "\n";
        }
        scanner.close();
        in.delete();
    }

    @Override
    public void writeAnswer() throws IOException {
        //Print answer
        if (answer == null) {
            System.out.println("False");
        } else {
            System.out.printf(answer);
        }
    }

    public static void main(String[] args) {
        Registre registre = new Registre();
        try {
            registre.solve();
        } catch (IOException e1) {
            System.out.println("Exception: " + e1);
        } catch (InterruptedException e2) {
            System.out.println("Exception: " + e2);
        }
    }
}