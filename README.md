# Petri Net Simulator Package 
This is a calculation package written in C++ for answering problem in Math Modeling Problem 
## Running Program
The source code contain files "main.cpp" and "PetriNet.h", you can build with compiler or just run the main.exe
## Manual
![First View](images/Annotation%202021-11-25%20160005.png)

At first sight, you will see 4 option:
1. Patient
2. Specialist
3. Merge
4. Optional
    - With option 1,2 and 3 the Petri Net is constructed similar to that the assignment demanded.
    - The option 4 is for constructing a custom Petri Net. The input file is '.txt' and has to be stored in the same directory with main.exe.

Here is the Input Form:

![Input form](images/Annotation%202021-11-25%20161308.png)
- After choosing an option you will be asked to enter the Initial Marking.

**Note that:** The input must be numberic and non-negative

- The Petri Net will be initialize and from there we will have some features

![Input form](images/Annotation%202021-11-25%20162241.png)
1. The program prints the state of the current Place nodes and Transition nodes. Then, it will fire the Transition nodes based on the input and print the state of Place and Transition in succession. Regarding the input, each transition should be separated by whitespace and a period to signify the end of the input. Example: With the Patient’s Petri Net the input should be “start start change .” (whitespace before the period at the end is necessary) In case the transition names entered are not in the Petri Net, the program will ignore those transitions and run the remaining ones. Example: As the input “start abc start def change .” is entered, transition “abc” and “def” are ignored as they are not initialized yet. Therefore, the firing sequence is now “start start change .”
2. The program prints the set of reachable markings from the current marking.
3. The program prints the current state of all Place nodes.
4. The program prints the current state of all Transition nodes.
5. The program prints the current Place nodes and Transition nodes. Then the user enters which transition to fire. The program then prints the state of the Place and Transition nodes after the firing.
6. The program reinitializes the Petri Net in accordance to its original initial marking.
7. The user must first set a limit on the number of tokens in a place, then the programs will proceed to create a transition system based on the input. If the operation time of the function does not exceed a certain limit then a transition system will be printed in the form of an adjacency list.
8. If the option is “clear“ or “cls” or “Clear” or “CLEAR”, the program clears the screen.
Otherwise, the input will be considered Invalid Choice and the program will asked you to enter again.

After that, you will be asked whether to continued or not. 

# Petri Net Visualizing 
## Motivation
Due to the graphical constraints of the programming language C++,
our group decides to use JavaScript to visualize the Petri Net that we propose in the assignment.
Despite the changes in programming language, the core idea for the Petri Net is still the same.
Visualization of the Petri Net helps the viewers have an overarching view of the whole process.
Furthermore, concurrency is better viewed in graph form of the Petri Net.
## Librabry and Dependency 
In this project we used some libraries and frameworks to construct the web structure: **Bootstrap** for designing the web UI and **Jointjs** for visualizing the Petri Net model. In addition, we also include some other related libraries. All of which have been shown at the header of the webpage.

## First View
First of all we wil come to Home Page, you can run index.html in Visualize Folder or just simply visit [Petri Network Simulator](02david20.github.io) for further update
![Home Page](images/home.png)
In the top right corner of the page, there is a navigation bar with three options which represents the three Petri Net that our group proposed in the report, which are the Patient PetriNet, Specialist Petri Net and Merged Petri Net.

Below, we have previews for the Petri Nets:

- **Patient Model:**
![Patient Model](images/patientModel.png)
- **Specialist Model:**
![Specialist Model](images/specialModel.png)
- **Merge Model:**
![Merge Model](images/mergeNet.png)

Going into separate models, we first go into file Patient.html, either through the links in the website or the file named Patient.html in the folder:

### Patient Model
![Patient](images/initP.png)
   At the top, the navigation bar now has an additional button, “Home”, which allows the user to return to the home page when clicked, the other buttons still lead to their corresponding models.
   
   For the Petri Net representing the process of patients being treated below, to best simulate, first the initial marking needs to be filled. To update the Petri Net, the user will have to press the button “Set” after they have filled out all the input slots.
   
   Setting an initial marking:
   
   After the Petri Net has been set, there are two ways in which the user can simulate the process of firing, either by pressing individual transitions or through the “AutoFire” button
- By pressing a transition, if all the conditions are satisfied for it to be enabled, a firing process of that transition will happen. This operation mainly involved consuming a token in all of its input places, and generating a token in all of its output places. This is simulated through an animation of a token going from the input places, through the transition, and then going to the output places. The animation takes some time to finish, so it is advisable to refrain from pressing a transition in quick succession (an error may occur)
- By pressing the “AutoFire” button, any transitions which are enabled will be fired at once. This process will repeat itself until the Petri Net reaches a terminal marking (a marking which has no enabled transition). This feature is best used to view concurrency in the Petri Net.
- After the user has finished viewing and interacting with the Petri Net, there is a “Reset” button to revert the Petri Net back to its original state

Same execution will be running with the Specialist Model and Merge Model
## Specialist Model
![Specialist](images/initS.png)
## Merge Model
![Merge](images/merged.png)

## Contribution
The web is built through the combined effort of our team. However, due to time constraint, the webpage may not have enough features yet, but we will update in the near future. Please visit our webpage at 
[Petri Network Simulator](02david20.github.io) for futher updates
