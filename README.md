**Collaboration:**
  ```
  git clone https://github.com/ProgrammingStuff11/MySPIM.git
  ```

**Get Latest Changes**
  ```
  git pull
  ```

**Commit your Changes**
  ```
  git add .
  git commit -m "Describe your changes"
  ```

**Push Changes**
  ```  
  git push
  ```

**Run Code**
  ```
  Open the terminal and run the following lines in bash:
  
  $ gcc -o spimcore spimcore.c project.c
  $ ./spimcore <filename>.asc
  ``` 
**Output based on each command**
```
r: “Register” - Display register contents
```
![alt text](Photos/image.png)

```
m: “Memory” - Display memory contents
```
![alt text](Photos/image-1.png)

```
s: “Step” – Attempt to run the instruction located at the current PC
```
![alt text](Photos/image-3.png)

```
c: “Continue” – Attempt to run all instructions, beginning with PC
```
![alt text](Photos/image-4.png)

```
h: “Halt” – Check to see if the simulation has halted
```
![alt text](Photos/image-5.png)

```
p: “Print” – Prints a copy of the input file
```
![alt text](Photos/image-6.png)

```
g: “Controls” – Display the most recent control signals
```
![alt text](Photos/image-7.png)

```
x: “Quit” – terminate the simulation
```
![alt text](Photos/image-2.png)