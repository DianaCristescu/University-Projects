import java.util.ArrayList;

public class Graph {
    public ArrayList<ArrayList<Integer>> graph;

    public void printGraph() {
        for(int i=0; i<graph.size(); i++) {
            if (graph.size() != graph.get(i).size()) {
                System.out.printf("Error");
                return;
            }
        }

        System.out.printf("     ");
        for(int i=0; i<graph.size(); i++) System.out.printf("[%d]", i);
        System.out.printf("\n");
        for(int i=0; i<graph.size(); i++) {
            System.out.printf(" [%d]", i);
            for(int j=0; j<graph.size(); j++) {
                if(graph.get(i).get(j) > 9) System.out.printf(" %d", graph.get(i).get(j));
                else if(graph.get(i).get(j) < 0) System.out.printf("  -");
                else System.out.printf("  %d", graph.get(i).get(j));
            }
            System.out.printf("\n");
        }
    }

    public Graph() {
        graph = new ArrayList<ArrayList<Integer>>();
    }

    public Graph(Graph g) {
        graph = new ArrayList<ArrayList<Integer>>();
        for(ArrayList<Integer> list: g.graph) {
            graph.add(list);
        }
    }
}
