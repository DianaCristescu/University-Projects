package main;

import java.util.ArrayList;

public final class OutputChild {
    private int id;
    private String lastName;
    private String firstName;
    private String city;
    private int age;
    private ArrayList<String> giftsPreferences = new ArrayList<String>();
    private Double averageScore;
    private ArrayList<Double> niceScoreHistory = new ArrayList<Double>();
    private Double assignedBudget;
    private ArrayList<Gift> receivedGifts = new ArrayList<Gift>();

    public OutputChild() {
    }

    public OutputChild(final OutputChild child) {
        id = child.getId();
        lastName = child.getLastName();
        firstName = child.getFirstName();
        age = child.getAge() + 1;
        city = child.getCity();
        niceScoreHistory = new ArrayList<Double>();
        for (Double score: child.getNiceScoreHistory()) {
            niceScoreHistory.add(score);
        }
        giftsPreferences = new ArrayList<String>();
        receivedGifts = new ArrayList<Gift>();
    }

    public OutputChild(final Child child) {
        id = child.getId();
        lastName = child.getLastName();
        firstName = child.getFirstName();
        age = child.getAge();
        city = child.getCity();
        giftsPreferences = child.getGiftsPreferences();
        niceScoreHistory = new ArrayList<Double>();
        niceScoreHistory.add(child.getNiceScore());
        receivedGifts = new ArrayList<Gift>();
    }

    public void calculateAverageScore() {
        Double s = 0.0;
        Double x = 0.0;
        if (age < 5) {
            averageScore = 10.0;
        } else if (age < 12) {
            for (Double score: niceScoreHistory) {
                s += score;
            }
            s = s / Double.valueOf(niceScoreHistory.size());
            averageScore = s;
        } else {
            for (int y = 0; y < niceScoreHistory.size(); y++) {
                s += niceScoreHistory.get(y) * Double.valueOf(y + 1);
                x += Double.valueOf(y + 1);
            }
            s = s / x;
            averageScore = s;
        }
    }

    public void applyYearlyUpdates(final ArrayList<AnnualChanges.ChildUpdate> updates) {
        for (AnnualChanges.ChildUpdate update : updates) {
            if (id == update.getId()) {

                //add new nice score
                if (update.getNiceScore() != null) {
                    niceScoreHistory.add(update.getNiceScore());
                }

                //add new gift preferences
                if (update.getGiftsPreferences() != null) {
                    for (String newPreference : update.getGiftsPreferences()) {
                        if (!giftsPreferences.contains(newPreference)) {
                            giftsPreferences.add(newPreference);
                        }
                    }
                }
            }
        }
    }

    public void copyOldGiftPreferences(final OutputChild child) {
        if (giftsPreferences.size() > 0) {
            for (String preference : child.getGiftsPreferences()) {
                if (!giftsPreferences.contains(preference)) {
                    giftsPreferences.add(preference);
                }
            }
        } else {
            giftsPreferences = child.getGiftsPreferences();
        }
    }

    public int getId() {
        return id;
    }

    public void setId(final int id) {
        this.id = id;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(final String lastName) {
        this.lastName = lastName;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(final String firstName) {
        this.firstName = firstName;
    }

    public String getCity() {
        return city;
    }

    public void setCity(final String city) {
        this.city = city;
    }

    public int getAge() {
        return age;
    }

    public void setAge(final int age) {
        this.age = age;
    }

    public ArrayList<String> getGiftsPreferences() {
        return giftsPreferences;
    }

    public void setGiftsPreferences(final ArrayList<String> giftsPreferences) {
        this.giftsPreferences = giftsPreferences;
    }

    public Double getAverageScore() {
        return averageScore;
    }

    public void setAverageScore(final Double averageScore) {
        this.averageScore = averageScore;
    }

    public ArrayList<Double> getNiceScoreHistory() {
        return niceScoreHistory;
    }

    public void setNiceScoreHistory(final ArrayList<Double> niceScoreHistory) {
        this.niceScoreHistory = niceScoreHistory;
    }

    public Double getAssignedBudget() {
        return assignedBudget;
    }

    public void setAssignedBudget(final Double assignedBudget) {
        this.assignedBudget = assignedBudget;
    }

    public ArrayList<Gift> getReceivedGifts() {
        return receivedGifts;
    }

    public void setReceivedGifts(final ArrayList<Gift> receivedGifts) {
        this.receivedGifts = receivedGifts;
    }
}
