package main;

import java.util.ArrayList;

public final class Input {
    private int numberOfYears;
    private Double santaBudget;
    private Double budgetUnit;
    private InitialData initialData;
    private ArrayList<AnnualChanges> annualChanges;
    //Observer pattern
    private ArrayList<Output> observers = new ArrayList<Output>();

    //Observer pattern
    public void addObserver(Output output) {
        observers.add(output);
    }

    //Observer pattern
    public void removeObserver(Output output) {
        observers.remove(output);
    }

    //Observer pattern
    public void calculateBudgetUnit(int year, final ArrayList<OutputChild> childrenList) {
        Double i = 0.0;
        for (OutputChild child: childrenList) {
            i += child.getAverageScore();
        }
        budgetUnit =  santaBudget / i;

        for(Output o: observers) {
            o.assignBudgetAndGifts(year, budgetUnit, initialData.getSantaGiftsList());
        }
    }

    public int getNumberOfYears() {
        return numberOfYears;
    }

    public void setNumberOfYears(final int numberOfYears) {
        this.numberOfYears = numberOfYears;
    }

    public Double getSantaBudget() {
        return santaBudget;
    }

    public void setSantaBudget(final Double santaBudget) {
        this.santaBudget = santaBudget;
    }

    public Double getBudgetUnit() {
        return budgetUnit;
    }

    public void setBudgetUnit(Double budgetUnit) {
        this.budgetUnit = budgetUnit;
    }

    public InitialData getInitialData() {
        return initialData;
    }

    public void setInitialData(final InitialData initialData) {
        this.initialData = initialData;
    }

    public ArrayList<AnnualChanges> getAnnualChanges() {
        return annualChanges;
    }

    public void setAnnualChanges(final ArrayList<AnnualChanges> annualChanges) {
        this.annualChanges = annualChanges;
    }

    public ArrayList<Output> getObservers() {
        return observers;
    }

    public void setObservers(final ArrayList<Output> observers) {
        this.observers = observers;
    }

    public final class InitialData {
        private ArrayList<Child> children;
        private ArrayList<Gift> santaGiftsList;

        public ArrayList<Child> getChildren() {
            return children;
        }

        public void setChildren(final ArrayList<Child> children) {
            this.children = children;
        }

        public ArrayList<Gift> getSantaGiftsList() {
            return santaGiftsList;
        }

        public void setSantaGiftsList(final ArrayList<Gift> santaGiftsList) {
            this.santaGiftsList = santaGiftsList;
        }
    }
}
