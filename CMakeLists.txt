add_llvm_loadable_module( LLVMTypeAnnotator
	annotator.cpp

	DEPENDS
	intrinsics_gen
	PLUGIN_TOOL
	opt
  )
