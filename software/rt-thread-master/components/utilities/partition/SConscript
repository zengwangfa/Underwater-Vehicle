from building import *

cwd = GetCurrentDir()
src = Glob('*.c')
CPPPATH = [cwd]

group = DefineGroup('partition', src, depend = ['RT_USING_DFS'], CPPPATH = CPPPATH)

Return('group')
