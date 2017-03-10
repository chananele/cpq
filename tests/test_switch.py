import base

class TestSwitch(base.CompilerTest):
    input = """
    instructions test

        start
            switch (4 * 5 + 1.0) {
                case 20: print(1); break;
                case 21: print(2); break;
                case 22: print(3); break;
                default: print(4);
            }
        end
    """
    output = '2\n'

