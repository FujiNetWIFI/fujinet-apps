#!/usr/bin/env python3
# Jan 26, 2021 18:20
# debug_output.py

import inspect

def debug_none(output):
    pass

def debug_output(output):
    """
        debug:  determine the name of the calling function
                and print it, and then print the desired output
        output: The string we wish to display
    """
    # debug.__name__
    calling_function_name = inspect.stack()[1][3]
    print(f"{calling_function_name}: {output}")



