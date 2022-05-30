import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner; 
import java.lang.Math;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Calendar;


public class ShortestPath {
    private Integer myInf = Integer.MAX_VALUE;


    public ArrayList<Integer> dijkstra(Graph g, int node) {
        PriorityQueue<Weight> pq = new PriorityQueue<Weight>(g.graph.size(), new Comparator<Weight>() {
            @Override
            public int compare(Weight o1, Weight o2) {
                if(o1.weight < o2.weight) return -1;
                if(o1.weight > o2.weight) return 1;
                return 0;
            }
        });
        ArrayList<Integer> shortestPaths = new ArrayList<Integer>();
        for(int i=0; i<g.graph.size(); i++) {
            if(i!=node) shortestPaths.add(myInf);
            else shortestPaths.add(0);
            pq.add(new Weight(i, shortestPaths.get(i)));
        }

        Weight n;
        int tempDistance;
        while(!pq.isEmpty()) {
            n = pq.poll();
            shortestPaths.set(n.idx, n.weight);
            for(int i=0; i<g.graph.size(); i++) {
                if(i!=node && n.idx!=i && g.graph.get(n.idx).get(i)>=0 && pq.contains(new Weight(i)) && n.weight!=myInf) {
                    tempDistance = n.weight + g.graph.get(n.idx).get(i);
                    if(tempDistance < shortestPaths.get(i)) {
                        pq.remove(new Weight(i));
                        pq.add(new Weight(i, tempDistance));
                        shortestPaths.set(i, tempDistance);
                    }
                }
            }
        }

        return shortestPaths;
    }


    public ArrayList<Integer> dial(Graph g, int node) {
        Bucket bucket = new Bucket(g);
        ArrayList<Integer> shortestPaths = new ArrayList<Integer>();
        Weight w;

        for(int i=0; i<g.graph.size(); i++) {
            if(i!=node)
                shortestPaths.add(myInf);
            else {
                bucket.add(new Weight(i, 0));
                shortestPaths.add(0);
            }
        }

        int maxWeight = bucket.buckets.size();
        int idx = 0, x;
        while(true) {

            while(idx < maxWeight && bucket.bucketIsEmpty(idx)) idx++;

            if(idx == maxWeight) break;

            w = bucket.pop(idx);

            int tempDistance;
            for(int i=0; i<g.graph.size(); i++) {
                x = g.graph.get(w.idx).get(i);
                if(i!=node && i!=w.idx && x>=0) {
                    tempDistance = w.weight + x;
                    if(tempDistance < shortestPaths.get(i)) {
                        if(shortestPaths.get(i) != myInf)
                            bucket.update(new Weight(i, shortestPaths.get(i)), tempDistance);
                        else
                            bucket.add(new Weight(i, tempDistance));
                        shortestPaths.set(i, tempDistance);
                    }
                }
            }
        }

        return shortestPaths;
    }


    public ArrayList<Integer> dag(Graph g, int node) {
        ArrayList<Integer> sortedGraph = new ArrayList<Integer>();
        ArrayList<Integer> shortestPaths = new ArrayList<Integer>();
        ArrayList<Boolean> visited = new ArrayList<Boolean>();

        for (int i = 0; i < g.graph.size(); i++) {
            if (i != node)
                shortestPaths.add(myInf);
            else {
                shortestPaths.add(0);
            }
            visited.add(i, Boolean.FALSE);
        }
        topologicalSortRecursive(g, visited, sortedGraph, node);

        int tempDistance;
        for(int idx: sortedGraph) {
            for(int i=0; i<g.graph.size(); i++) {
                if(i!=node && idx!=i && g.graph.get(idx).get(i)>=0 && shortestPaths.get(idx)!=myInf) {
                    tempDistance = shortestPaths.get(idx) + g.graph.get(idx).get(i);
                    if(tempDistance < shortestPaths.get(i)) {
                        shortestPaths.set(i, tempDistance);
                    }
                }
            }
        }

        return shortestPaths;
    }


    public void topologicalSortRecursive(Graph g, ArrayList<Boolean> visited, ArrayList<Integer> sortedGraph, int weightIdx) {
        visited.set(weightIdx, Boolean.TRUE);

        for(int i=0; i<g.graph.size(); i++) {
            if(g.graph.get(weightIdx).get(i) >= 0 && visited.get(i)!=Boolean.TRUE) {
                topologicalSortRecursive(g, visited, sortedGraph, i);
            }
        }

        sortedGraph.add(0, weightIdx);
    }


    public void printShortestPaths(ArrayList<Integer> list, String path) {

        try {
            File out = new File(path);
            if (!out.createNewFile())
              System.out.println("File already exists.");
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        try {
            FileWriter myWriter = new FileWriter(path);

            for(int i=0; i<list.size(); i++)
                if(list.get(i) >= myInf)
                    myWriter.write(" -");
                else if(list.get(i)<9)
                    myWriter.write(" "+list.get(i));
                else
                    myWriter.write(" "+list.get(i));

            myWriter.close();
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }


    public void testDijkstra(String path) {
        ArrayList<Integer> list = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                list.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        double graphSize = Math.sqrt(list.size()-1);
        Graph graph = new Graph();
        int idxList = 1;
        for(int i=0; i<graphSize; i++) {
            graph.graph.add(new ArrayList<Integer>());
            for(int j=0; j<graphSize; j++) {
                graph.graph.get(i).add(list.get(idxList));
                idxList++;
            }
        }

        long startTime = Calendar.getInstance().getTimeInMillis();
        list = dijkstra(graph, list.get(0));
        long endTime = Calendar.getInstance().getTimeInMillis();
        path = "./out/" + path.replaceAll(".in", ".out").replaceAll("..out/", "");
        printShortestPaths(list, path);

        ArrayList<Integer> out = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                out.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        path = "./ref" + path.replaceAll("./out", "").replaceAll(".out", "");
        ArrayList<Integer> ref = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                ref.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        String testNr = path.replaceAll("./ref/test", "");
        if(ref.equals(out)) System.out.println("Test "+testNr+" done, time: " + (endTime - startTime) + "ms");
        else System.out.println("Test "+testNr+" failed");
    }


    public void testDial(String path) {
        ArrayList<Integer> list = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                list.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        double graphSize = Math.sqrt(list.size()-1);
        Graph graph = new Graph();
        int idxList = 1;
        for(int i=0; i<graphSize; i++) {
            graph.graph.add(new ArrayList<Integer>());
            for(int j=0; j<graphSize; j++) {
                graph.graph.get(i).add(list.get(idxList));
                idxList++;
            }
        }

        long startTime = Calendar.getInstance().getTimeInMillis();
        list = dial(graph, list.get(0));
        long endTime = Calendar.getInstance().getTimeInMillis();
        path = "./out/" + path.replaceAll(".in", ".out").replaceAll("..out/", "");
        printShortestPaths(list, path);

        ArrayList<Integer> out = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                out.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        path = "./ref" + path.replaceAll("./out", "").replaceAll(".out", "");
        ArrayList<Integer> ref = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                ref.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        String testNr = path.replaceAll("./ref/test", "");
        if(ref.equals(out)) System.out.println("Test "+testNr+" done, time: " + (endTime - startTime) + "ms");
        else System.out.println("Test "+testNr+" failed");
    }


    public void testDag(String path) {
        ArrayList<Integer> list = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                list.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        double graphSize = Math.sqrt(list.size()-1);
        Graph graph = new Graph();
        int idxList = 1;
        for(int i=0; i<graphSize; i++) {
            graph.graph.add(new ArrayList<Integer>());
            for(int j=0; j<graphSize; j++) {
                graph.graph.get(i).add(list.get(idxList));
                idxList++;
            }
        }

        long startTime = Calendar.getInstance().getTimeInMillis();
        list = dag(graph, list.get(0));
        long endTime = Calendar.getInstance().getTimeInMillis();
        path = "./out/" + path.replaceAll(".in", ".out").replaceAll("..out/", "");
        printShortestPaths(list, path);

        ArrayList<Integer> out = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                out.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        path = "./ref" + path.replaceAll("./out", "").replaceAll(".out", "");
        ArrayList<Integer> ref = new ArrayList<Integer>();
        try {
            File myObj = new File(path);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextInt()) {
                Integer data = myReader.nextInt();
                ref.add(data);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        String testNr = path.replaceAll("./ref/test", "");
        if(ref.equals(out)) System.out.println("Test "+testNr+" done, time: " + (endTime - startTime) + "ms");
        else System.out.println("Test "+testNr+" failed");
    }


    public static void main(String[] args) {

        ShortestPath t = new ShortestPath();

        if(args[0].equals("1")) {
            t.testDijkstra(args[1]);
        } else if(args[0].equals("2")) {
            t.testDial(args[1]);
        } else if(args[0].equals("3")) {
            t.testDag(args[1]);
        }
    }

}
