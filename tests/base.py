import os
import sys
from pathlib2 import Path
from subprocess import Popen, PIPE

def universal(string):
    if string is not None:
        return os.linesep.join(string.split('\n'))

class CompilerTest(object):

    output = None
    error  = None
    ret    = 0

    def test_compiler_output(self, context):
        here = Path(__file__).absolute()
        root = Path(here).parent.parent
        interpreter = root / 'qx.py'
        assert interpreter.exists(), 'could not find the interpreter'
        
        compiled = Popen([context['compiler'], '-', '-'], stdin=PIPE, stdout=PIPE)
        executed = Popen([sys.executable, str(interpreter), '-'], stdin=compiled.stdout, stdout=PIPE)

        compiled.stdin.write(self.input)
        compiled.stdin.close()

        output, error = executed.communicate()
        ret = executed.returncode

        assert output == universal(self.output)
        assert error  == universal(self.error)
        assert ret    == self.ret
 
