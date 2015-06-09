# When adding a new dependency, please update the top-level .gitignore file
# to list the dependency's destination directory.

vars = {
  'chromium_git': 'https://chromium.googlesource.com',
}

deps = {
  "src/third_party/gyp":
    Var('chromium_git') + '/external/gyp.git' + '@' + '29e94a3285ee899d14d5e56a6001682620d3778f',
}

deps_os = {
  "win": {
    "src/third_party/psyco_win32":
      Var('chromium_git') + '/chromium/deps/psyco_win32.git' + '@' + 'f5af9f6910ee5a8075bbaeed0591469f1661d868',
  },
}

hooks = [
  {
    # A change to a .gyp, .gypi, or to GYP itself should run the generator.
    "name": "gyp",
    "pattern": ".",
    "action": ['python', 'src/tools/gyp_jcef', 'src/jcef.gyp', '-I',
               'src/jcef.gypi'],
  },
]
