import base

class TestForLoop(base.CompilerTest):
    input = """
    instructions test

        variables i: int;
        /* testing the correct execution of a for loop */

        start
            for (i := 1; i < 5; i := i + 1) {
                print(i);
            }
        end
    """
    output = '1\n2\n3\n4\n'
 
