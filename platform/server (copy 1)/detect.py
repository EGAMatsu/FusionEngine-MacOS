
import os
import sys	

gcc_68k = "$(RETRO68)/bin/m68k-apple-macos-gcc"
gpp_68k = "$(RETRO68)/bin/m68k-apple-macos-g++"

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
	('builtin_zlib', 'no'),
	('theora','no'), #use builtin openssl
	]
			


def configure(env):

	env.Append(CPPPATH=['#platform/macos_classic'])
	env["CC"]=gcc_68k
	env["CXX"]=gpp_68k
	env["LD"]=gpp_68k

	is64=sys.maxsize > 2**32

	if (env["bits"]=="default"):
		if (is64):
			env["bits"]="64"
		else:
			env["bits"]="32"


	#if (env["tools"]=="no"):
	#	#no tools suffix
	#	env['OBJSUFFIX'] = ".nt"+env['OBJSUFFIX']
	#	env['LIBSUFFIX'] = ".nt"+env['LIBSUFFIX']


	if (env["target"]=="release"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-fomit-frame-pointer'])

	elif (env["target"]=="release_debug"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

	elif (env["target"]=="debug"):

		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DSERVER_ENABLED','-DUNIX_ENABLED'])
	env.Append(LIBS=['pthread','z'])

	if (env["CXX"]==gpp_68k):
		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env["CC"]=gcc_68k
		env["LD"]=gpp_68k

