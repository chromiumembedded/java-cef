# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

from date_util import *
from file_util import *
from gclient_util import *
from optparse import OptionParser
import os
import re
import shlex
import subprocess
import svn_util as svn
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
    file = os.path.join(path, 'README.' +name + '.txt')
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
  data = data.replace('$JCEF_REV$', jcef_rev)
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

  write_file(os.path.join(output_dir, 'README.txt'), data)
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
parser.add_option('--output-dir', dest='outputdir', metavar='DIR',
                  help='output directory [required]')
parser.add_option('--target', dest='platform', metavar='FILE',
                  help='target platform for distribution [required]')
parser.add_option('-q', '--quiet',
                  action='store_true', dest='quiet', default=False,
                  help='do not output detailed status information')
(options, args) = parser.parse_args()

# the outputdir option is required
if options.outputdir is None or options.platform is None:
  parser.print_help(sys.stderr)
  sys.exit()
output_dir = options.outputdir

# Test the operating system.
platform = options.platform;
if (platform != 'linux32' and platform != 'linux64' and
    platform != 'macosx64' and
    platform != 'win32' and platform != 'win64'):
  print 'Unsupported target \"'+platform+'\"'
  sys.exit()

# script directory
script_dir = os.path.dirname(__file__)

# CEF root directory
jcef_dir = os.path.abspath(os.path.join(script_dir, os.pardir))

# src directory
src_dir = os.path.abspath(os.path.join(jcef_dir, os.pardir))

# retrieve url and revision information for CEF
if svn.is_checkout(jcef_dir) or svn.is_checkout(src_dir):
  jcef_info = svn.get_svn_info(jcef_dir)
  jcef_url = jcef_info['url']
  jcef_rev = jcef_info['revision']
elif git.is_checkout(jcef_dir) or git.is_checkout(src_dir):
  jcef_url = git.get_url(jcef_dir)
  jcef_rev = git.get_svn_revision(jcef_dir)
else:
  raise Exception('Not a valid checkout: %s' % (cef_dir))

date = get_date()

# Read and parse the version file (key=value pairs, one per line)
args = {}
read_readme_file(os.path.join(jcef_dir, 'third_party/cef/'+platform+'/README.txt'), args)

jcef_ver = args['CEF_VER']+'.'+jcef_rev
cef_ver = args['CEF_VER']
cef_url = args['CEF_URL']
chromium_ver = args['CHROMIUM_VER']
chromium_url = args['CHROMIUM_URL']

# create the README.TXT file
create_readme()
