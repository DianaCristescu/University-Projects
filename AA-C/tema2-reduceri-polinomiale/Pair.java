public class Pair {
    int x;
    int y;

    public Pair() {

    }

    public Pair(int x, int y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public int hashCode()
    {
        return ((Integer)(x + y)).hashCode();
    }

    @Override
    public boolean equals(Object o)
    {
        if(o instanceof Pair 
        && ((this.x == ((Pair)o).x && this.y == ((Pair)o).y) 
        || (this.y == ((Pair)o).x && this.x == ((Pair)o).y))) {
            return true;
        }
        return false;
    }
}
