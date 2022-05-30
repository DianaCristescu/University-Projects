package main;

import checker.Checker;
import java.io.File;
import java.util.ArrayList;

/**
 * Class used to run the code
 */
public final class Main {

    private Main() {
        ///constructor for checkstyle
    }

//    /**
//     * This method is used to call the checker which calculates the score
//     * @param args
//     *          the arguments used to call the main method
//     */

    private void runTest(final File inputFile, final File outputFile, final int i) {
        Input input = new Input();
        Output output = Output.getInstance();

        ArrayList<OutputChild> childrenList;

        //*Get input*
        input.get(inputFile);

        //part of Observer pattern
        input.addObserver(output);

        //*Round 0*
        childrenList = this.firstYear(input, output);

        //*Rounds 1 to number of years*
        for (int year = 1; year <= input.getNumberOfYears(); year++) {
            childrenList = this.otherYears(input, output, year, childrenList);
        }

        //*Create and write the output file*
        output.write(outputFile);
    }

    private ArrayList<OutputChild> firstYear(final Input input, final Output output) {
        ArrayList<OutputChild> childrenList;
        OutputChild outputChild;
        final int maxTeenAge = 18;

        output.setAnnualChildren(new ArrayList<Output.Children>());
        output.getAnnualChildren().add(new Output.Children());
        childrenList = output.getAnnualChildren().get(0).getChildren();

        //Add children if <= 18 years
        for (Child child: input.getInitialData().getChildren()) {
            if (child.getAge() <= maxTeenAge) {
                //create output child data
                outputChild = new OutputChild(child);

                //set average score
                outputChild.calculateAverageScore();

                //add child to output
                childrenList.add(outputChild);
            }
        }

        //Calculate budget unit & set assigned budget and gifts (with observer pattern)
        input.calculateBudgetUnit(0, output.getAnnualChildren().get(0).getChildren());

        //Apply Yellow Elf bonus
        for (OutputChild child: childrenList) {
            child.addElfGift(input);
        }

        return childrenList;
    }

    private ArrayList<OutputChild> otherYears(final Input input, final Output output,
                                              final int year,
                                              ArrayList<OutputChild> childrenList) {
        ArrayList<OutputChild> newChildrenList;
        OutputChild outputChild;
        boolean updatesExist;
        final int maxTeenAge = 18;

        output.getAnnualChildren().add(new Output.Children());
        newChildrenList = output.getAnnualChildren().get(year).getChildren();

        if (input.getAnnualChanges() != null
                && (year - 1) < input.getAnnualChanges().size()) {
            input.setSantaBudget(input.getAnnualChanges().get(year
                    - 1).getNewSantaBudget());
            updatesExist = true;
        } else {
            updatesExist = false;
        }

        //Apply children updates if <= 18 years
        for (OutputChild child: childrenList) {
            if (child.getAge() + 1 <= maxTeenAge) {

                //copy existing data and increment age
                outputChild = new OutputChild(child);

                //apply yearly updates
                if (updatesExist && input.getAnnualChanges().get(year
                        - 1).getChildrenUpdates() != null) {
                    outputChild.applyYearlyUpdates(input.getAnnualChanges().get(year
                            - 1).getChildrenUpdates());
                }

                //add old gift preferences
                outputChild.copyOldGiftPreferences(child);

                //set average score
                outputChild.calculateAverageScore();

                //add to list of children
                newChildrenList.add(outputChild);
            }
        }
        childrenList = newChildrenList;

        //Add new children if they exist and <= 18 years
        if (updatesExist && input.getAnnualChanges().get(year - 1).getNewChildren()
                != null) {
            for (Child child : input.getAnnualChanges().get(year - 1).getNewChildren()) {
                if (child.getAge() <= maxTeenAge) {
                    //copy child input to output
                    outputChild = new OutputChild(child);

                    //set average score
                    outputChild.calculateAverageScore();

                    //add
                    childrenList.add(outputChild);
                }
            }
        }

        //Add new gifts if they exist
        if (updatesExist && input.getAnnualChanges().get(year - 1).getNewGifts() != null) {
            for (Gift gift : input.getAnnualChanges().get(year - 1).getNewGifts()) {
                input.getInitialData().getSantaGiftsList().add(gift);
            }
        }

        //Calculate budget unit & set assigned budget and gifts (with observer pattern)
        input.calculateBudgetUnit(year, output.getAnnualChildren().get(year).getChildren());

        //Apply Yellow Elf bonus
        for (OutputChild child: childrenList) {
            child.addElfGift(input);
        }

        return childrenList;
    }

    public static void main(final String[] args) {
        File inputFile;
        File outputFile;
        final int nrOfTests = 30;

        Main main = new Main();

        //---Running each test---
        for (int i = 1; i <= nrOfTests; i++) {
            inputFile = new File("./tests/test" + i + ".json");
            outputFile = new File("./output/out_" + i + ".json");

            main.runTest(inputFile, outputFile, i);
        }

        //---Calculate score---
        Checker.calculateScore();
    }
}
