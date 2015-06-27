env = Environment(CPPFLAGS=['-Wall','-g'])

env.ParseConfig('pkg-config --cflags --libs libglademm-2.4 gconfmm-2.6')

dcmtklibs = [
  'dcmdata',
  'dcmdsig',
  'dcmimage',
  'dcmimgle',
  'dcmjpeg',
  'dcmjpls',
  'dcmnet',
  'dcmpstat',
  'dcmqrdb',
  'dcmrt',
  'dcmsr',
  'dcmtls',
  'dcmwlm',
  'i2d',
  'ijg12',
  'ijg16',
  'ijg8',
  'oflog',
  'ofstd',
]

env.Append(CPPPATH=['#',
                    '#/imagepool',
                    '#/configuration',
                    '#/widgets',
                    '#/src'],
           LIBPATH=['#/imagepool',
                    '#/configuration',
                    '#/widgets'],
           LIBS=['imagepool',
                 'configuration',
                 'widgets',
                 ] + dcmtklibs,
           CPPDEFINES = ['-DAESKULAP_DATADIR=\\"/usr/local/share/aeskulap\\"',
                      ]
           )

SConscript('src/SConscript', exports = 'env')
SConscript('imagepool/SConscript', exports = 'env')
SConscript('configuration/SConscript', exports = 'env')
SConscript('widgets/SConscript', exports = 'env')
