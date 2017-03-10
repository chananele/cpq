import base

class TestWhileLoop(base.CompilerTest):
    input="""
    instructions test
    
        variables i: int;
        /* testing the correct ececution of a while loop */

        start
            i := 1;
            while(i < 10) {
                print(i);
                i := i * 2 + 1;
            }
        end
    """
    output = '1\n3\n7\n'

