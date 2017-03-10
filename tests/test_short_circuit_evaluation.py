import base

class TestShortCircuiting(base.CompilerTest):
    input="""
    instructions test
    
        start
            if (2 > 1 || (1 / 0) == 4) then {
                print(1);
            } else {
                print(2);
            }
        end
    """
    output='1\n'

