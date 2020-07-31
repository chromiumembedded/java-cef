# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

from __future__ import absolute_import
from __future__ import print_function
from date_util import *
from file_util import *
from optparse import OptionParser
import os
import re
from readme_util import read_readme_file
import shlex
import subprocess
import git_util as git
import sys
import zipfile


def get_readme_component(name):
  """ Loads a README file component. """
  paths = []
  # platform directory
  paths.append(os.path.join(script_dir, 'distrib', platform))

  # shared directory
  paths.append(os.path.join(script_dir, 'distrib'))

  # load the file if it exists
  for path in paths:
    file = os.path.join(path, 'README.' + name + '.txt')
    if path_exists(file):
      return read_file(file)

  raise Exception('Readme component not found: ' + name)


def create_readme():
  """ Creates the README.TXT file. """
  # gather the components
  header_data = get_readme_component('header')
  mode_data = get_readme_component('standard')
  redistrib_data = get_readme_component('redistrib')
  footer_data = get_readme_component('footer')

  # format the file
  data = header_data + '\n\n' + mode_data + '\n\n' + redistrib_data + '\n\n' + footer_data
  data = data.replace('$JCEF_URL$', jcef_url)
  data = data.replace('$JCEF_REV$', jcef_commit_hash)
  data = data.replace('$JCEF_VER$', jcef_ver)
  data = data.replace('$CEF_URL$', cef_url)
  data = data.replace('$CEF_VER$', cef_ver)
  data = data.replace('$CHROMIUM_URL$', chromium_url)
  data = data.replace('$CHROMIUM_VER$', chromium_ver)
  data = data.replace('$DATE$', date)

  if platform == 'win32':
    platform_str = 'Windows 32-bit'
  elif platform == 'win64':
    platform_str = 'Windows 64-bit'
  elif platform == 'macosx64':
    platform_str = 'Mac OS-X 64-bit'
  elif platform == 'linux32':
    platform_str = 'Linux 32-bit'
  elif platform == 'linux64':
    platform_str = 'Linux 64-bit'

  data = data.replace('$PLATFORM$', platform_str)

  write_file(os.path.join(output_dir, 'README.txt'), data.encode('utf-8'))
  if not options.quiet:
    sys.stdout.write('Creating README.TXT file.\n')


# cannot be loaded as a module
if __name__ != "__main__":
  sys.stderr.write('This file cannot be loaded as a module!')
  sys.exit()

# parse command-line options
disc = """
This utility builds the JCEF README.txt for the distribution.
"""

parser = OptionParser(description=disc)
parser.add_option(
    '--output-dir',
    dest='outputdir',
    metavar='DIR',
    help='output directory [required]')
parser.add_option(
    '--platform',
    dest='platform',
    help='target platform for distribution [required]')
parser.add_option(
    '-q',
    '--quiet',
    action='store_true',
    dest='quiet',
    default=False,
    help='do not output detailed status information')
(options, args) = parser.parse_args()

# the outputdir option is required
if options.outputdir is None or options.platform is None:
  parser.print_help(sys.stderr)
  sys.exit(1)
output_dir = options.outputdir

# Test the operating system.
platform = options.platform
if (platform != 'linux32' and platform != 'linux64' and
    platform != 'macosx64' and platform != 'win32' and platform != 'win64'):
  print('Unsupported target \"' + platform + '\"')
  sys.exit(1)

# script directory
script_dir = os.path.dirname(__file__)

# JCEF root directory
jcef_dir = os.path.abspath(os.path.join(script_dir, os.pardir))

# Read and parse the CEF version file.
args = {}
read_readme_file(os.path.join(jcef_dir, 'jcef_build', 'README.txt'), args)

# retrieve url and revision information for CEF
if not git.is_checkout(jcef_dir):
  raise Exception('Not a valid checkout: %s' % (jcef_dir))

jcef_commit_number = git.get_commit_number(jcef_dir)
jcef_commit_hash = git.get_hash(jcef_dir)
jcef_url = git.get_url(jcef_dir)
jcef_ver = '%s.%s.%s.%s+g%s' % (args['CEF_MAJOR'], args['CEF_MINOR'],
                                args['CEF_PATCH'], jcef_commit_number,
                                jcef_commit_hash[:7])

date = get_date()

cef_ver = args['CEF_VER']
cef_url = args['CEF_URL']
chromium_ver = args['CHROMIUM_VER']
chromium_url = args['CHROMIUM_URL']

# create the README.TXT file
create_readme()
