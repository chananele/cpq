import os
import pytest
from pathlib2 import Path
from distutils.spawn import find_executable

def pytest_addoption(parser):

    here = Path(__file__).absolute()
    root = Path(here).parent.parent
    compiler = str(root / 'bin' / 'cpq')

    parser.addoption(
        '--compiler',
        action='store',
        default=compiler,
        help='path to the compiler',
    )

@pytest.fixture()
def context(request):
    compiler = request.config.getoption('--compiler')
    if not find_executable(compiler):
        raise RuntimeError('could not find the compiler executable')

    return dict(compiler=compiler)

