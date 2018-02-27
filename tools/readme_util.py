from file_util import read_file


def read_readme_file(file, args):
  """ Read a README.txt and try to parse its containing version numbers """
  lines = read_file(file).split("\n")
  for line in lines:
    parts = line.split(':', 1)
    if len(parts) != 2:
      continue
    if parts[0].startswith('CEF Version'):
      args['CEF_VER'] = parts[1].strip()
      verparts = args['CEF_VER'].split('.')
      if len(verparts) >= 2:
        args['CEF_MAJOR'] = verparts[0]
        args['CEF_BUILD'] = verparts[1]
    elif parts[0].startswith('CEF URL'):
      args['CEF_URL'] = parts[1].strip()
    elif parts[0].startswith('Chromium Version'):
      args['CHROMIUM_VER'] = parts[1].strip()
    elif parts[0].startswith('Chromium URL'):
      args['CHROMIUM_URL'] = parts[1].strip()
