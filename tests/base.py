import sys
from pathlib2 import Path
from subprocess import Popen, PIPE

class CompilerTest(object):

    output = None
    error  = None
    ret    = 0

    def test_compiler_output(self, context):
        this = Path(__file__).absolute()
        root = Path(this).parent.parent
        compiler = context['compiler']
        interpreter = root / 'qx.py'
        
        compiled = Popen([str(compiler), '-', '-'], stdin=PIPE, stdout=PIPE)
        executed = Popen([sys.executable, str(interpreter), '-'], stdin=compiled.stdout, stdout=PIPE)

        compiled.stdin.write(self.input)
        compiled.stdin.close()

        output, error = executed.communicate()
        ret = executed.returncode

        assert output == self.output
        assert error  == self.error
        assert ret    == self.ret
 
