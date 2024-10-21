Include = -I Include
IncludeStack = -I HellStack/Include
COMP_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
COMP_FLAGS += $(Include)
COMP_FLAGS += $(IncludeStack)
CC = g++

Sources = $(wildcard $(Source)/*.cpp)
StackSources = $(wildcard $(HellSource)/*.cpp)
Objects = $(patsubst $(Source)/%.cpp,$(Build)/%.o,$(Sources))
StackObjects = $(patsubst $(HellSource)/%.cpp, $(HellBuild)/%.o, $(StackSources))

SourcesAss = $(wildcard $(SourceAss)/*.cpp)
ObjectsAss = $(patsubst $(SourceAss)/%.cpp,$(Build)/%ass.o,$(SourcesAss))

Cpp = $(wildcard *.cpp)
H = $(wildcard *.h)

Build = Build
Source = Source
SourceAss = Source/Ass
HellSource = HellStack/Source
HellBuild = HellStack/Build
Exe = SPU.exe

all: $(Build)/$(Exe)

ass: $(Build)/ass.exe

$(HellBuild)/%.o : $(HellSource)/%.cpp | $(HellBuild)
	$(CC) $(COMP_FLAGS) $< -c -o $@

$(Build)/%.o : $(Source)/%.cpp | $(Build)
	$(CC) $(COMP_FLAGS) $< -c -o $@

$(Build)/%ass.o : $(SourceAss)/%.cpp | $(Build)
	$(CC) $(COMP_FLAGS) $< -c -o $@

$(Build)/$(Exe): $(Objects) $(StackObjects)| $(Build)
	$(CC) $(COMP_FLAGS) -o $(Build)/$(Exe) $(Objects) $(StackObjects)

$(Build)/ass.exe: $(ObjectsAss) | $(Build)
	$(CC) $(COMP_FLAGS) -o $(Build)/ass.exe $(ObjectsAss)

$(Build):
	mkdir -p $(Build)

$(HellBuild):
	mkdir -p $(HellBuild)

comb: all ass runass run

cleanup:
ifdef Cpp
	mkdir -p Source
	mv *.cpp Source
endif

ifdef H
	mkdir -p Include
	mv *.h Include
endif
	rm -rf *.o

run:
	$(Build)/$(Exe)

runass:
	$(Build)/ass.exe

clean:
	rm -rf ./$(Build)
	rm -rf ./$(HellBuild)


test:
	echo $(Sources)
	echo $(StackObjects)



