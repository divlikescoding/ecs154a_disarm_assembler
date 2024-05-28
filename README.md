**How to get the assembler working**

Step 1: Clone the repository

    https://github.com/divlikescoding/ecs154a_disarm_assembler.git

Step 2: Change Working Directory to the cloned folder (ecs154a_disarm_assembler)

    cd ecs154a_disarm_assembler/
   
Step 3: Compile the assembler

    make
   
Step 4: You should now see as executable named disarm_as.	

Step 5: The usage of this assembler is similar to that of fiscas

    ./disarm_as <assembly_file> <output_file>
    example: ./disarm_as test_assembly.s test.hex

The assembler has followed the ARM assembler syntax and follows exactly the mmenomics provided to us in the assignment

Additionally, there is an examples directory where there is an example of an assembly file that does a variety of operations.

Hopefully this is helpful! Please let me know if there are any improvements or bugs!

This software is available under the following licenses:
    1. MIT