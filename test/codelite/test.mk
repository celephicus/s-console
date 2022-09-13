##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=test
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=C:/Users/admin/Documents/git/s-console/test/codelite
ProjectPath            :=C:/Users/admin/Documents/git/s-console/test/codelite
IntermediateDirectory  :=build-$(WorkspaceConfiguration)
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=admin
Date                   :=13/09/2022
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/Users/admin/Documents/git/s-console/test/codelite/build-$(WorkspaceConfiguration)/bin
OutputFile             :=build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=$(PreprocessorSwitch)UNITY_INCLUDE_CONFIG_H $(PreprocessorSwitch)TEST 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../src $(IncludeSwitch).. $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_up_src_sconsole.c$(ObjectSuffix) $(IntermediateDirectory)/up_test_r_stack.c$(ObjectSuffix) $(IntermediateDirectory)/up_test_u_stack.c$(ObjectSuffix) $(IntermediateDirectory)/up_test_init.c$(ObjectSuffix) $(IntermediateDirectory)/up_main.c$(ObjectSuffix) $(IntermediateDirectory)/up_test_toy_vm.c$(ObjectSuffix) $(IntermediateDirectory)/up_t_support.c$(ObjectSuffix) $(IntermediateDirectory)/up_unity.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:
	@echo Executing Pre Build commands ...
	python ..\grm.py -v -o ..\main.c ..\test*.c
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_up_src_sconsole.c$(ObjectSuffix): ../../src/sconsole.c $(IntermediateDirectory)/up_up_src_sconsole.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/src/sconsole.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_sconsole.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_src_sconsole.c$(DependSuffix): ../../src/sconsole.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_src_sconsole.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_src_sconsole.c$(DependSuffix) -MM ../../src/sconsole.c

$(IntermediateDirectory)/up_up_src_sconsole.c$(PreprocessSuffix): ../../src/sconsole.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_src_sconsole.c$(PreprocessSuffix) ../../src/sconsole.c

$(IntermediateDirectory)/up_test_r_stack.c$(ObjectSuffix): ../test_r_stack.c $(IntermediateDirectory)/up_test_r_stack.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/test_r_stack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_test_r_stack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_test_r_stack.c$(DependSuffix): ../test_r_stack.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_test_r_stack.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_test_r_stack.c$(DependSuffix) -MM ../test_r_stack.c

$(IntermediateDirectory)/up_test_r_stack.c$(PreprocessSuffix): ../test_r_stack.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_test_r_stack.c$(PreprocessSuffix) ../test_r_stack.c

$(IntermediateDirectory)/up_test_u_stack.c$(ObjectSuffix): ../test_u_stack.c $(IntermediateDirectory)/up_test_u_stack.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/test_u_stack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_test_u_stack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_test_u_stack.c$(DependSuffix): ../test_u_stack.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_test_u_stack.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_test_u_stack.c$(DependSuffix) -MM ../test_u_stack.c

$(IntermediateDirectory)/up_test_u_stack.c$(PreprocessSuffix): ../test_u_stack.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_test_u_stack.c$(PreprocessSuffix) ../test_u_stack.c

$(IntermediateDirectory)/up_test_init.c$(ObjectSuffix): ../test_init.c $(IntermediateDirectory)/up_test_init.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/test_init.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_test_init.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_test_init.c$(DependSuffix): ../test_init.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_test_init.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_test_init.c$(DependSuffix) -MM ../test_init.c

$(IntermediateDirectory)/up_test_init.c$(PreprocessSuffix): ../test_init.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_test_init.c$(PreprocessSuffix) ../test_init.c

$(IntermediateDirectory)/up_main.c$(ObjectSuffix): ../main.c $(IntermediateDirectory)/up_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_main.c$(DependSuffix): ../main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_main.c$(DependSuffix) -MM ../main.c

$(IntermediateDirectory)/up_main.c$(PreprocessSuffix): ../main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_main.c$(PreprocessSuffix) ../main.c

$(IntermediateDirectory)/up_test_toy_vm.c$(ObjectSuffix): ../test_toy_vm.c $(IntermediateDirectory)/up_test_toy_vm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/test_toy_vm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_test_toy_vm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_test_toy_vm.c$(DependSuffix): ../test_toy_vm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_test_toy_vm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_test_toy_vm.c$(DependSuffix) -MM ../test_toy_vm.c

$(IntermediateDirectory)/up_test_toy_vm.c$(PreprocessSuffix): ../test_toy_vm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_test_toy_vm.c$(PreprocessSuffix) ../test_toy_vm.c

$(IntermediateDirectory)/up_t_support.c$(ObjectSuffix): ../t_support.c $(IntermediateDirectory)/up_t_support.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/t_support.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_t_support.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_t_support.c$(DependSuffix): ../t_support.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_t_support.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_t_support.c$(DependSuffix) -MM ../t_support.c

$(IntermediateDirectory)/up_t_support.c$(PreprocessSuffix): ../t_support.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_t_support.c$(PreprocessSuffix) ../t_support.c

$(IntermediateDirectory)/up_unity.c$(ObjectSuffix): ../unity.c $(IntermediateDirectory)/up_unity.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/admin/Documents/git/s-console/test/unity.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_unity.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_unity.c$(DependSuffix): ../unity.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_unity.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_unity.c$(DependSuffix) -MM ../unity.c

$(IntermediateDirectory)/up_unity.c$(PreprocessSuffix): ../unity.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_unity.c$(PreprocessSuffix) ../unity.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


