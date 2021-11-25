# Petri Net Simulator Package 
This is a calculation package write in C++ for answering problem in Math Modeling Problem 
## Running Program
The source Code contain files "main.cpp" "PetriNet.h", you can build with compiler or just run the main.exe
## Manual
![First View](images/Annotation%202021-11-25%20160005.png)
At your first sight, you will see 4 option:
1. Patient
2. Specialist
3. Merge
4. Optional
    - With option 1,2 and 3 the Petri Net is constructed similar to the assignment demand
    - The option 4 is in order to make the user can construct the Petri Net as their need,
The input file is '.txt' and has to be stored in the same directory with main.exe
Here the Input Form:

![Input form](images/Annotation%202021-11-25%20161308.png)
- After Choosing Option you will be asked to enter the Initial Marking.

**Note that:** The input must be numberic and non-negative

- The Petri Net will be initialize and go on we will have some feature

![Input form](images/Annotation%202021-11-25%20162241.png)
1. The program prints the state of the current Place nodes and Transitionnodes. Then, it will fire the Transition nodes based on the input and print the state of Place andTransition in succession. Regarding the input, each transition should be separated by whitespaceand a period to signify the end of the input. Example: With the Patient’s Petri Net the inputshould be “start start change .” (whitespace before the period at the end is necessary) In case thetransition names entered are not in the Petri Net, the program will ignore those transitions andrun the remaining ones. Example: As the input “start abc start def change .” is entered, transition“abc” and “def” are ignored as they are not initialized yet. Therefore, the firing sequence is now“start start change .”
2. The program prints the set of reachable markings from the current marking.
3. The program prints the current state of all Place nodes.
4. The program prints the current state of all Transition nodes.
5. The program prints the current Place nodes and Transition nodes. Thenthe user enters which input to fire. The program then prints the state of the Place and Transitionnodes after the firing.
6. The program reinitializes the Petri Net to the initial marking.
7. Then the user must first set a limit on the number of tokens in a place,then the programs will proceed to create a transition system based on the input. If the operationtime of the function does not exceed a certain limit then a transition system will be printed inthe form of an adjacency list
8. If the option is “clear“ or “cls” or “Clear” or “CLEAR”, the program clears the screen.
9. 
Otherwise, their will be Invalid Choice and the program will asked you to enter again.

After that, You will be asked to be continued or not. 

