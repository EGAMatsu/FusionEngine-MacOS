
import os
import sys	


def is_active():
	return True
        
def get_name():
	return "Server"


def can_build():

	if (os.name!="posix"):
		return False

	return True # enabled
  
def get_opts():

	return [
	('use_llvm','Use llvm compiler','no'),
	('force_32_bits','Force 32 bits binary','no')
	]
  
def get_flags():

	return [
	('builtin_zlib', 'no'),
	('theora','no'), #use builtin openssl
	]
			


def configure(env):

	env.Append(CPPPATH=['#platform/psp'])
	#if (env["use_llvm"]=="yes"):
	env["CC"]="psp-gcc"
	env["CXX"]="psp-g++"
	env["LD"]="psp-g++"

	# is64=sys.maxsize > 2**32


	env["bits"]="32"


	psp_path = os.environ["PSPDEV"]
    
	env.Append(CPPPATH=[psp_path+"/psp/sdk/include/"])
	env.Append(LIBPATH=[psp_path+"/psp/sdk/lib"])


	if (env["target"]=="release"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-fomit-frame-pointer'])

	elif (env["target"]=="release_debug"):

		env.Append(CCFLAGS=['-O2','-ffast-math','-DDEBUG_ENABLED'])

	elif (env["target"]=="debug"):

		env.Append(CCFLAGS=['-g2', '-Wall','-DDEBUG_ENABLED','-DDEBUG_MEMORY_ENABLED'])

	env.Append(CPPFLAGS=['-DNEED_LONG_INT', '-DPSP_ENABLED', '-DGLES1_ENABLED'])
	env.Append(LIBS=["GL","GLU","glut",'pthread','z',"pspdisplay", "pspge", "pspgu","pspgum","pspvfpu","pspctrl"]) #TODO detect linux/BSD!

	if (env["CXX"]=="clang++"):
		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env["CC"]="clang"
		env["LD"]="clang++"
