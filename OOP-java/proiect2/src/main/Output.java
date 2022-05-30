package main;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public final class Output {
    //Singleton pattern
    static Output instance = new Output();
    private ArrayList<Children> annualChildren;

    public void write(final File outputFile) {
        ObjectMapper objectMapper = new ObjectMapper();

        //*Create the output file*
        try {
            if (!outputFile.createNewFile()) {
                outputFile.delete();
                outputFile.createNewFile();
            }
        } catch (IOException e) {
            System.out.println("Error2: " + e.getMessage());
        }

        //*Write output*
        try {
            objectMapper.writeValue(outputFile, this);
        } catch (IOException e) {
            System.out.println("Error3: " + e.getMessage());
        }
    }

    //Singleton pattern
    private Output() {

    }

    //Singleton pattern
    public static Output getInstance() {
        return instance;
    }

    public void assignBudgetAndGifts(final int year, final Double budgetUnit,
                                     final ArrayList<Gift> giftList, final String strategy) {

        ArrayList<OutputChild> childrenList;
        if (year == 0) {
            childrenList = this.childrenIdOrder(year);
        } else {
            if (strategy.equals("id")) {
                childrenList = this.childrenIdOrder(year);
            } else if (strategy.equals("niceScore")) {
                childrenList = this.childrenScoreOrder(year);
            } else {
                childrenList = this.childrenCityOrder(year);
            }
        }

        Gift chosenGift;
        Double budget;
        for (OutputChild child: childrenList) {
            budget = budgetUnit * child.getAverageScore();
            child.setAssignedBudget(budget);

            //Apply Black and Pink Elf bonus
            child.addElfBonus();
            budget = child.getAssignedBudget();

            for (String category: child.getGiftsPreferences()) {
                chosenGift = null;

                for (Gift gift: giftList) {
                    if (gift.getCategory().equals(category)) {
                        if (chosenGift == null
                                || Double.compare(gift.getPrice(), chosenGift.getPrice()) < 0) {
                            if (gift.returnQuantity() > 0) {
                                chosenGift = gift;
                            }
                        }
                    }
                }

                if (chosenGift != null && budget.compareTo(chosenGift.getPrice()) >= 0) {
                    budget -= chosenGift.getPrice();
                    if (Double.compare(budget, 0.0) >= 0) {
                        chosenGift.setQuantity(chosenGift.returnQuantity() - 1);
                        child.getReceivedGifts().add(chosenGift);
                    } else {
                        break;
                    }
                }
            }
        }
    }

    private ArrayList<OutputChild> childrenIdOrder(final int year) {
        return annualChildren.get(year).getChildren();
    }

    private ArrayList<OutputChild> childrenScoreOrder(final int year) {
        ArrayList<OutputChild> inputChildrenList = annualChildren.get(year).getChildren();
        ArrayList<OutputChild> childrenList = new ArrayList<OutputChild>();
        OutputChild chosenChild;

        while (inputChildrenList.size() > childrenList.size()) {
            chosenChild = null;
            for (OutputChild child: inputChildrenList) {
                if ((chosenChild == null
                        || child.getAverageScore().compareTo(chosenChild.getAverageScore()) > 0)
                        && !childrenList.contains(child)) {
                    chosenChild = child;
                }
            }
            childrenList.add(chosenChild);
        }

        return childrenList;
    }

    private ArrayList<OutputChild> childrenCityOrder(final int year) {
        ArrayList<OutputChild> inputChildrenList = annualChildren.get(year).getChildren();
        ArrayList<OutputChild> childrenList = new ArrayList<OutputChild>();
        Map<String, Double> cityScores = new HashMap<String, Double>();
        ArrayList<String> sortedKeyList = new ArrayList<String>();
        String chosenCity;
        String city;
        int n;

        for (OutputChild child: inputChildrenList) {
            if (!cityScores.containsKey(child.getCity())) {
                n = 0;
                city = child.getCity();
                cityScores.put(city, 0.0);
                for (OutputChild c: inputChildrenList) {
                    if (c.getCity().equals(city)) {
                        cityScores.put(city, cityScores.get(city) + c.getAverageScore());
                        n++;
                    }
                }
                cityScores.put(city, cityScores.get(city) / n);
            }
        }

        for (String key: cityScores.keySet()) {
            sortedKeyList.add(key);
        }
        Collections.sort(sortedKeyList);

        while (inputChildrenList.size() > childrenList.size()) {
            chosenCity = null;
            for (String key: sortedKeyList) {
                if ((chosenCity == null
                        || cityScores.get(key).compareTo(cityScores.get(chosenCity)) > 0)
                        && !this.alreadyPutCity(childrenList, key)) {
                    chosenCity = key;
                }
            }
            for (OutputChild child: inputChildrenList) {
                if (child.getCity().equals(chosenCity)) {
                    childrenList.add(child);
                }
            }
        }

        return childrenList;
    }

    private boolean alreadyPutCity(final ArrayList<OutputChild> childrenList, final String city) {
        for (OutputChild child: childrenList) {
            if (child.getCity().equals(city)) {
                return true;
            }
        }
        return false;
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
