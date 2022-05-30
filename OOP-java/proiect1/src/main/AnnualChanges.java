package main;

import java.util.ArrayList;

public final class AnnualChanges {
    private Double newSantaBudget;
    private ArrayList<Gift> newGifts;
    private ArrayList<Child> newChildren;
    private ArrayList<ChildUpdate> childrenUpdates;

    public Double getNewSantaBudget() {
        return newSantaBudget;
    }

    public void setNewSantaBudget(final Double newSantaBudget) {
        this.newSantaBudget = newSantaBudget;
    }

    public ArrayList<Gift> getNewGifts() {
        return newGifts;
    }

    public void setNewGifts(final ArrayList<Gift> newGifts) {
        this.newGifts = newGifts;
    }

    public ArrayList<Child> getNewChildren() {
        return newChildren;
    }

    public void setNewChildren(final ArrayList<Child> newChildren) {
        this.newChildren = newChildren;
    }

    public ArrayList<ChildUpdate> getChildrenUpdates() {
        return childrenUpdates;
    }

    public void setChildrenUpdates(final ArrayList<ChildUpdate> childrenUpdates) {
        this.childrenUpdates = childrenUpdates;
    }

    static final class ChildUpdate {
        private int id;
        private Double niceScore;
        private ArrayList<String> giftsPreferences;

        public int getId() {
            return id;
        }

        public void setId(final int id) {
            this.id = id;
        }

        public Double getNiceScore() {
            return niceScore;
        }

        public void setNiceScore(final Double niceScore) {
            this.niceScore = niceScore;
        }

        public ArrayList<String> getGiftsPreferences() {
            return giftsPreferences;
        }

        public void setGiftsPreferences(final ArrayList<String> giftsPreferences) {
            this.giftsPreferences = giftsPreferences;
        }
    }
}
