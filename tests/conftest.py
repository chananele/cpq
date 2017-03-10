import os
import pytest
from pathlib2 import Path
from distutils.spawn import find_executable

def pytest_addoption(parser):
    parser.addoption(
        '--compiler',
        action='store',
        required=True,
        help='path to the compiler',
    )

@pytest.fixture()
def context(request):
    compiler = Path(request.config.getoption('--compiler'))
    assert compiler.exists(), 'could not find the compiler executable'

    return dict(compiler=str(compiler))

