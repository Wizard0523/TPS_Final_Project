Welcome to my procedural dunGen!

How to demo project:

1. Install unreal engine 5.4.4 or above.

2. Clone repo locally, to UnrealProjects/<ProjectFolder> -- you will need to create the project folder, and name it anything.

3. Open Unreal Engine. Browse to find project folder. If newer than 5.4.4, then U.E. will ask you if you want to clone a copy, press yes.

4. Compile code:

![image](https://github.com/user-attachments/assets/76ae89a9-8249-441f-b189-67680548d192)<br>
Press button to compile code ^^

5. Press green button to run code.

6. While program is running. You can press CTRL + F1 to regain control of cursor.

7. Check log for details about the run:
   
![Screenshot 2024-12-13 at 11 40 03 AM](https://github.com/user-attachments/assets/42975673-4dc7-453e-a237-9ef3f1e3cfba) <br>
-- the log prints out information about the process at each stage: initializing the dungeon, and rewriting the dungeon



Questions About the Project:

* What dataset do you analyze? Are there multiple tables? For each table, answer what are the rows and what are the columns.<br>

There aren't any tables, but there could be a table that stores rooms. Each room is a row, with columns for roomID, content type, connected rooms, and I was hoping to implement different floors, and each room would have a column for the floor they are on.

The picture below was an early demo of a possible table:<br>
![Screenshot 2024-12-13 at 11 26 46 AM](https://github.com/user-attachments/assets/2beb46c2-b531-453e-8c56-8b0e12ab93fb) <br>
I no longer track depth. I also track the Z coordinate of each room now, so that rooms can be on different heigh levels. Additionally, I should track which level each floor is on with an int.

* What questions is your software meant to answer on your dataset? <br>

It's meant to create a procedural dungeon that can be used in video games. I think procedural generation is cool because where rules inter lap, something unique emerges. For me, it also answered a lot of questions about unreal engine. Specifically how C++ code can be used to spawn rooms. Also I learned how to used maps, arrays, and sets in unreal engine. 

* What algorithms and data structures did you use? <br>

For data structures I used stacks, and graphs.
For algorithms I used Depth First Search to traverse the graph, and I created a simple algorithm to prevent overlap. When the dungeon is initialized rooms are randomly generated. If a space is occupied, the current r

* What is their time and space complexity using big oh notation? <br>

For the 
Depth first search has O(N + E) complexity, where V is the number of nodes, and E is the number of edges. The contentRewriter has a loop that calls a function with a loop, so that part is big O(n^2). Since n^2 is larger than O(N + E), the complexity of my program is O(n^2).

* How can someone test your code? For example, what are some example inputs and the corresponding output you expect. <br>

To test the code you can change the room budget to adjust the number of rooms spawned. To see what is happening during runtime I used print statement to print information about what is happening in the background


* What challenges did you encounter and how did you solve them?
  
I created an infinite loop in a while loop. It froze whenever I pressed play, and it didn't create any error log. It did however save a running log, but this didn't seem to have any relevant information. I used a loop counter to break out of the loop manually after 10 loops. This allowed the program to compile, and I was able to run 10 loops, which broke, and revealed a new issue. But this approach at least allowed me to work my way through the code. I used prints to show where the code was reaching, and slowly added more print statements as I needed more information. <br>

My code became somewhat unreadable, and difficult to work with, so I added some functions to clean up the code. Using print to log helped a lot too when testing/debugging.

* What programming languages, frameworks, or technologies did you use? <br>
C++, Unreal Engine

![Screenshot 2024-12-13 at 11 25 05 AM](https://github.com/user-attachments/assets/6f8e8aed-3f93-4765-91e7-471806ad6665)
