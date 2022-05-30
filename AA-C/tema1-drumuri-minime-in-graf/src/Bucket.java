import java.util.LinkedList;
import java.util.Vector;

public class Bucket {
    public Vector<LinkedList<Weight>> buckets;

    public Bucket(Graph g) {
        int maxWeight = 0;
        for(int i=0; i<g.graph.size(); i++) {
            for(int j=0; j<g.graph.size(); j++) {
                if(g.graph.get(i).get(j) > maxWeight)
                    maxWeight = g.graph.get(i).get(j);
            }
        }

        buckets = new Vector<LinkedList<Weight>>(maxWeight*g.graph.size());
        buckets.setSize(maxWeight*g.graph.size());
        for(int i = 0; i < buckets.size(); i++) {
            buckets.set(i, new LinkedList<Weight>());
        }
    }

    public void update(Weight w, int newWeight) {
        this.add(new Weight(w.idx, newWeight));
        this.remove(w);
    }

    public void add(Weight weight) {
        buckets.get(weight.weight).add(weight);
    }

    public void remove(Weight w) {
        buckets.get(w.weight).remove(w);
    }

    public boolean bucketIsEmpty(int weight) {
        return buckets.get(weight).isEmpty();
    }

    public Weight pop(int weight) {
        return buckets.get(weight).poll();
    }
}
