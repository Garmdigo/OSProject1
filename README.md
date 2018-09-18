# OSProject1
COP4610 Project 1 Repository

Done: 
-General:

--Main shell loop

-Specific Parts:

--Part 1: Parsing

---Modularized The TA's code


--Part 2: Environment variables

---The function for returning an environment variable takes an input of a char* in the form '$VARIABLE' (though lower case is accepted). It removes the '$' and uses the getenv() function to return a char* string containing the value of the environment variable


--Part 3: Prompt

-Builtins:

--Exit

---Function is called from the main shell loop and takes in a timeval struct. Uses the supplied timeval struct and another generated on function call to calculate the run time of the shell and then exits with Exit(0).
