import base

class TestPrint(base.CompilerTest):
    input = """
    instructions printing
    start
    print(1);
    end
    """
    output = '1\n'

