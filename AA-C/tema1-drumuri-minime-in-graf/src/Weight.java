public class Weight {
    public int idx;
    public int weight;

    public Weight(int idx, int weight) {
        this.idx = idx;
        this.weight = weight;
    }

    public Weight(int idx) {
        this(idx, 0);
    }

    @Override
    public boolean equals(Object o){
        if(o instanceof Weight){
            return idx == ((Weight) o).idx;
        }
        return false;
    }
}
