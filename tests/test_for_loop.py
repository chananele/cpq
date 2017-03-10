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
    output = ''.join([str(i) + '\n' for i in xrange(1, 5)])
 
