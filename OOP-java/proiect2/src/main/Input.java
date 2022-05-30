package main;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import com.fasterxml.jackson.databind.ObjectMapper;

public final class Input {
    private int numberOfYears;
    private Double santaBudget;
    private Double budgetUnit;
    private InitialData initialData;
    private ArrayList<AnnualChanges> annualChanges;

    public void get(final File inputFile) {
        ObjectMapper objectMapper = new ObjectMapper();
        Input input = new Input();

        //*Read input from file*
        try {
            input = objectMapper.readValue(inputFile, Input.class);
            numberOfYears = input.numberOfYears;
            santaBudget = input.santaBudget;
            budgetUnit = input.budgetUnit;
            initialData = input.initialData;
            annualChanges = input.annualChanges;
        } catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    //Observer pattern
    private ArrayList<Output> observers = new ArrayList<Output>();

    //Observer pattern
    public void addObserver(final Output output) {
        observers.add(output);
    }

    //Observer pattern
    public void removeObserver(final Output output) {
        observers.remove(output);
    }

    //Observer pattern
    public void calculateBudgetUnit(final int year, final ArrayList<OutputChild> childrenList) {
        Double i = 0.0;
        String strategy = "id";

        for (OutputChild child: childrenList) {
            i += child.getAverageScore();
        }
        budgetUnit =  santaBudget / i;

        for (Output o: observers) {
            if (year > 0) {
                strategy = annualChanges.get(year - 1).getStrategy();
            }
            o.assignBudgetAndGifts(year, budgetUnit, initialData.getSantaGiftsList(), strategy);
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

    public void setBudgetUnit(final Double budgetUnit) {
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
