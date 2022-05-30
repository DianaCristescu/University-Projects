Cristescu Diana-Andreea 324CD
Project 1

Main structure:

1.For loop that cycles all 25 tests:
    1.Making the input and output file paths
    2.Getting the input into the Input class with ObjectMapper
    3.Round 0 begins:
        1.Setting up the output Object 
        2.Iterate trough input children:
            1.If they are younger or 18 years:
                1.Copy input data
                2.Calculate average score
                3.Add new OutputChild to the list
        3.Calculate budget unit & set assigned budget and gifts
          (with observer pattern) 
    4.For loop that runs rounds 1 to nr. of years:
        1.Setting up the output Object 
        2.Testing if updates exist
        3.Iterating trough old children to update their data:
            1.If they are younger or 18 years:
                1.Copy existing data and increment age
                2.Apply yearly updates if they exist
                3.Add old gift preferences
                4.Calculate average score
                5.Add new OutputChild to the list
        4.Add new children if they exist and <= 18 years:
            1.Copy input data
            2.Calculate average score
            3.Add new OutputChild to the list
        5.Add new gifts if they exist
        6.Calculate budget unit & set assigned budget and gifts
          (with observer pattern)
    5.Create the output file
    6.Write output
2.Calculate score