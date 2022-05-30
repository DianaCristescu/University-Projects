import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class Retele extends Task{
    ArrayList<Pair> graph = new ArrayList<>();
    String answer;
    boolean calledFromReclame = false;
    int n;
    int m;
    int k;

    @Override
    public void solve() throws IOException, InterruptedException {
        this.readProblemData();
        this.formulateOracleQuestion();
        this.decipherOracleAnswer();
        this.writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        Scanner scanner = new Scanner(System.in);

        //Get input data
        if (!scanner.hasNext()) {
            scanner.close();
            File in = new File("reclame.out");
            scanner = new Scanner(in);
            calledFromReclame = true;
        }

        n = scanner.nextInt();
        m = scanner.nextInt();
        k = scanner.nextInt();
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

        //Makes sure all positions in a clique are occupied (#1) -> O(n*k)
        for (int i = 0; i < k; i++) { 
            for (int j = 1; j <= n; j++) {
                x = (n * i + j);
                clauses += x + " ";
            }
            clauses += "0";
            clauses += "\n";
            nr_clauses++;
        }

        //Checks if all the persons in the clique have a connection (#2) -> O(!k*!n)
        for (int i = 0; i < k; i++) {
            for (int j = i + 1; j < k; j++) {
                for (int v = 1; v <= n; v++) {
                    for (int w = v + 1; w <= n; w++) {
                        if (!graph.contains(new Pair(v, w))) {
                            x = - (n * i + v);
                            clauses += x + " ";
                            x = - (n * j + w);
                            clauses += x + " 0";
                            clauses += "\n";
                            x = - (n * j + v);
                            clauses += x + " ";
                            x = - (n * i + w);
                            clauses += x + " 0";
                            clauses += "\n";
                            nr_clauses++;
                        }
                    }
                }
            }
        }

        //Makes sure one person doesn't occupy 2 spots in a clique (#3) -> O(!k*n)
        for (int i = 0; i < k; i++) {
            for (int j = i + 1; j < k; j++) {
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

        //Checks if 2 persons are on the same spot in the clique (#4) -> O(!n*k)
        for (int i = 0; i < k; i++) {
            for (int v = 1; v <= n; v++) {
                for (int w = v + 1; w <= n; w++) {
                    x = - (n * i + v);
                    clauses += x + " ";
                    x = - (n * i + w);
                    clauses += x + " 0";
                    clauses += "\n";
                    nr_clauses++;
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
        int v;

        answer = scanner.nextLine();
        answer += "\n";
        if (answer.equals("True\n")) {
            v = scanner.nextInt();

            //Compose answer
            while (scanner.hasNextInt()) {
                v = scanner.nextInt();
                if (v > 0) {
                    v %= n;
                    if (v == 0) {
                        v = n;
                    }
                    answer += v + " ";
                }
            }

            answer += "\n";
        }
        scanner.close();
        in.delete();
    }

    @Override
    public void writeAnswer() throws IOException {
        //Print answer
        if (calledFromReclame) {
            FileWriter fileWriter = new FileWriter("reclame.in");
            fileWriter.write(answer);
            fileWriter.close();
        } else {
            System.out.printf(answer);
        }
    }

    public static void main(String[] args) {
        Retele retea = new Retele();
        try {
            retea.solve();
        } catch (IOException e1) {
            System.out.println("Exception: " + e1);
        } catch (InterruptedException e2) {
            System.out.println("Exception: " + e2);
        }
    }
}