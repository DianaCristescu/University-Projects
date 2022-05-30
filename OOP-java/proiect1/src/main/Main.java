package main;

import checker.Checker;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Class used to run the code
 */
public final class Main {

    private Main() {
        ///constructor for checkstyle
    }

    /**
     * This method is used to call the checker which calculates the score
     *
     * @param args the arguments used to call the main method
     */
    public static void main(final String[] args) {
        ObjectMapper objectMapper = new ObjectMapper();
        Input input = new Input();
        Output output = Output.getInstance();
        File inputFile;
        File outputFile;

        ArrayList<OutputChild> childrenList;
        OutputChild outputChild;

        //---Running each test---
        for (int i = 1; i <= 25; i++) {
            inputFile = new File("./tests/test" + i + ".json");
            outputFile = new File("./output/out_" + i + ".json");

            //*Get input*
            try {
                input = objectMapper.readValue(inputFile, Input.class);
            } catch (IOException e) {
                System.out.println("Error(" + i + "): " + e.getMessage());
            }
            input.addObserver(output);

            //*Round 0*
            output.setAnnualChildren(new ArrayList<Output.Children>());
            output.getAnnualChildren().add(new Output.Children());
            childrenList = output.getAnnualChildren().get(0).getChildren();

            //Add children if <= 18 years
            for (Child child : input.getInitialData().getChildren()) {
                if (child.getAge() <= 18) {
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

            boolean updatesExist;
            ArrayList<OutputChild> newChildrenList;
            //*Rounds 1 to number of years*
            for (int year = 1; year <= input.getNumberOfYears(); year++) {
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
                for (OutputChild child : childrenList) {
                    if (child.getAge() + 1 <= 18) {

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
                        if (child.getAge() <= 18) {
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
            }

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
                objectMapper.writeValue(outputFile, output);
            } catch (IOException e) {
                System.out.println("Error3: " + e.getMessage());
            }
        }

        //---Calculate score---
        Checker.calculateScore();
    }
}
