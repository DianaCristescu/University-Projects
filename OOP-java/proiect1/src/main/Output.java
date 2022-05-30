package main;

import java.util.ArrayList;

public final class Output {
    //Singleton pattern
    static Output instance = new Output();
    private ArrayList<Children> annualChildren;

    //Singleton pattern
    private Output() {

    }

    //Singleton pattern
    public static Output getInstance() {
        return instance;
    }

    public void assignBudgetAndGifts(final int year, final Double budgetUnit,
                                     final ArrayList<Gift> giftList) {
        ArrayList<OutputChild> childrenList = annualChildren.get(year).getChildren();
        Gift chosenGift;
        Double budget;
        for (OutputChild child: childrenList) {
            budget = budgetUnit * child.getAverageScore();
            child.setAssignedBudget(budget);

            for (String category: child.getGiftsPreferences()) {
                chosenGift = null;

                for (Gift gift: giftList) {
                    if (gift.getCategory().equals(category)) {
                        if (chosenGift == null
                                || Double.compare(gift.getPrice(), chosenGift.getPrice()) < 0) {
                            chosenGift = gift;
                        }
                    }
                }

                if (chosenGift != null && budget.compareTo(chosenGift.getPrice()) >= 0) {
                    budget -= chosenGift.getPrice();
                    if (Double.compare(budget, 0.0) >= 0) {
                        child.getReceivedGifts().add(chosenGift);
                    } else {
                        break;
                    }
                }
            }
        }
    }

    public ArrayList<Children> getAnnualChildren() {
        return annualChildren;
    }

    public void setAnnualChildren(final ArrayList<Children> annualChildren) {
        this.annualChildren = annualChildren;
    }

    static final class Children {
        private ArrayList<OutputChild> children = new ArrayList<OutputChild>();

        public ArrayList<OutputChild> getChildren() {
            return children;
        }

        public void setChildren(final ArrayList<OutputChild> children) {
            this.children = children;
        }
    }
}
