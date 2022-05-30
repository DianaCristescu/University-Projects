import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class Reclame extends Task{
    ArrayList<Pair> graph = new ArrayList<>();
    String tgraph = "";
    boolean notFound = true;
    String answer = "";
    int n;
    int m;
    int k;

    @Override
    public void solve() throws IOException, InterruptedException {
        this.readProblemData();
        k = n;
        while (notFound) {
            k--;
            this.formulateOracleQuestion();
            this.decipherOracleAnswer();
        }
        this.writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        Scanner scanner = new Scanner(System.in);

        //Get input data
        n = scanner.nextInt();
        m = scanner.nextInt();
        for (int i = 0; i < m; i++) {
            graph.add(new Pair(scanner.nextInt(), scanner.nextInt()));
        }
        scanner.close();

        //Make transpose graph
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                if (!graph.contains(new Pair(i, j))) {
                    tgraph += i + " " + j + "\n";
                }
            }
        }
    }

    @Override
    public void formulateOracleQuestion() throws IOException {
        Retele retea = new Retele();
        File out = new File("reclame.out");
        File in = new File("reclame.in");
        FileWriter myWriter = new FileWriter(out);
        String inputData = "";

        //Print transpose graph
        int x = (n*(n-1))/2 - m;
        inputData += n + " " + x + " " + k;
        inputData += "\n";
        inputData += tgraph;
        in.createNewFile();
        out.createNewFile();
        myWriter.write(inputData);
        myWriter.close();

        //Get the nodes that aren't in the essential group if it exists
        try {
            retea.solve();
        } catch (InterruptedException e) {
            System.out.println("Exception: " + e);
        }
    }

    @Override
    public void decipherOracleAnswer() throws IOException {
        File in = new File("reclame.in");
        Scanner scanner = new Scanner(in);

        //If oracle found a group
        if (scanner.nextBoolean()) {
            notFound = false;
            ArrayList<Integer> notInGroup = new ArrayList<>();

            //Save the nodes that aren't in the essential group
            while (scanner.hasNextInt()) {
                notInGroup.add(scanner.nextInt());
            }
            scanner.close();

            //Formulate answer
            for (int i = 1; i <= n; i++) {
                if (!notInGroup.contains(i)) {
                    answer += i + " ";
                }
            }
        }
    }

    @Override
    public void writeAnswer() throws IOException {
        System.out.printf(answer + "\n");
    }

    public static void main(String[] args) {
        Reclame retea = new Reclame();
        try {
            retea.solve();
        } catch (IOException e1) {
            System.out.println("Exception: " + e1);
        } catch (InterruptedException e2) {
            System.out.println("Exception: " + e2);
        }
    }
}