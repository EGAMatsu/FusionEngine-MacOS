
import os
import sys	

TESTPTH = "/home/luke-warren/Retro68-build/toolchain"
ppc_bin = "/bin/powerpc-apple-macos-"
gcc_68k = TESTPTH+ppc_bin+"gcc-12.2.0"
gpp_68k = TESTPTH+ppc_bin+"g++"
_ld_68k = TESTPTH+ppc_bin+"ld"

def is_active():
	return True
        
def get_name():
	return "MacOS Classic"


def can_build():

	return True # enabled
  
def get_opts():

	return [
	('force_32_bits','Force 32 bits binary','no')
	]
  
def get_flags():

	return [
	('builtin_zlib', 'yes'),
	('theora','no'), #use builtin openssl
	]
			


def configure(env):

	env.Append(CPPPATH=['#platform/macos_classic'])
	env["CC"]	= gcc_68k
	env["CXX"]	= gpp_68k
	env["LD"]	= _ld_68k
	env["bits"]	= "32"


	if (env["target"]=="release"):
		env.Append(CCFLAGS=['-O2','-ffast-math','-fomit-frame-pointer'])

	elif (env["target"]=="release_debug"):
		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

	elif (env["target"]=="debug"):
		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DSERVER_ENABLED'])
	env.Append(CPPFLAGS=['-DNO_THREADS'])
	env.Append(CPPFLAGS=['-DNEED_LONG_INT'])
	env.Append(LIBS=['pthread','z'])

