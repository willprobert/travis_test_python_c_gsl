import subprocess, shutil, os
from os.path import join
from string import Template
import numpy as np, pandas as pd
import pytest


SRC_DIR = "src"
SRC_DIR_TEST = "src_test"

DATA_DIR_TEST = "data_test"
TEST_OUTPUT_FILE = join(DATA_DIR_TEST, "watts_strogatz_network.txt")

# Construct the executable command
EXE = "watts_strogatz.exe {N} {k} {p_rewire}".format(N = 1000, k = 10, p_rewire = 0.1)

command = join(SRC_DIR_TEST, EXE)

def test_summation():
    np.testing.assert_equal(0, 0)

def test_summation2():
    np.testing.assert_equal(2+2, 4)

def test_summation3():
    np.testing.assert_equal(2+3, 5)


class TestClass(object):
    """
    Test class for checking 
    """
    @classmethod
    def setup_class(self):
        """
        When the class is instantiated: compile the IBM in a temporary directory
        """
        
        # Make a temporary copy of the code (remove this temporary directory if it already exists)
        shutil.rmtree(SRC_DIR_TEST, ignore_errors = True)
        shutil.copytree(SRC_DIR, SRC_DIR_TEST)
                
        # Construct the compilation command and compile
        compile_command = "make clean; make all"
        completed_compilation = subprocess.run([compile_command], 
            shell = True, cwd = SRC_DIR_TEST, capture_output = True)
        print(compile_command)
        print(completed_compilation)
        
    @classmethod
    def teardown_class(self):
        """
        Remove the temporary code directory (when this class is removed)
        """
        shutil.rmtree(SRC_DIR_TEST, ignore_errors = True)
    
    def setup_method(self):
        """
        Called before each method is run; creates a new data dir
        """
        os.mkdir(DATA_DIR_TEST)
    
    def teardown_method(self):
        """
        At the end of each method (test), remove the directory of test data
        """
        shutil.rmtree(DATA_DIR_TEST, ignore_errors = True)
    
    def test_runs(self):
        """
        Test the executable runs
        """
        
        # Call the model using baseline parameters, pipe output to file, read output file
        file_output = open(TEST_OUTPUT_FILE, "w")
        completed_run = subprocess.run([command], stdout = file_output, shell = True)
        print(completed_run)
        np.testing.assert_equal(completed_run.returncode, 0)
