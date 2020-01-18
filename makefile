.SECONDARY:
CC= @gcc-8 -g  #-fsanitize=address -fno-omit-frame-pointer
EXE:=WorldTest TestCombat GameTest

##ASAN_OPTIONS=detect_leaks=1 ./GameTest 2>err.txt

EXE_O:=$(patsubst %,Compile_obj/%.o,$(EXE))
DISPLAY:=$(foreach F,$(wildcard Display/*.c),$(subst Display/, Compile_obj/Display/, $(subst .c,.o, $F)))

ENTITY:=$(foreach F,$(wildcard Entity/*.c),$(subst Entity/, Compile_obj/Entity/, $(subst .c,.o, $F)))

INVENTORY:=$(foreach F,$(wildcard Inventory/*.c),$(subst Inventory/, Compile_obj/Inventory/, $(subst .c,.o, $F)))

WORLD:=$(foreach F,$(wildcard World/*.c),$(subst World/, Compile_obj/World/, $(subst .c,.o, $F)))

UTILITY:=$(foreach F,$(wildcard Utility/*.c),$(subst Utility/, Compile_obj/Utility/, $(subst .c,.o, $F)))

COMBAT:=$(foreach F,$(wildcard Combat/*.c),$(subst Combat/, Compile_obj/Combat/, $(subst .c,.o, $F)))

OBJECTS:= $(DISPLAY) $(ENTITY) $(INVENTORY) $(WORLD) $(UTILITY) $(COMBAT)

MIDDLE = @printf "Generated: \033[1m\033[1;34m $@ \033[0;30m\033[0m\t\n"
LIB= -I Display/ -I Utility/ -I Entity/ -I Inventory/ -I World/ -I Combat/

all: $(EXE_O)


clean:
	find . -name '*.o' -delete
	rm -f *.o core $(EXE)



#Testing
Compile_obj/%.o: Testing/%.c $(OBJECTS)
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)
	$(CC) -o  $(subst Compile_obj/,,$(subst .o,,$@))  $@   $(OBJECTS)  -lm

#Display
Compile_obj/Display/%.o: Display/%.c Display/%.h
	@mkdir -p Compile_obj/Display/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

#Entity
Compile_obj/Entity/%.o: Entity/%.c Entity/%.h
	@mkdir -p Compile_obj/Entity/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

#Inventory
Compile_obj/Inventory/%.o: Inventory/%.c Inventory/%.h
	@mkdir -p Compile_obj/Inventory/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

#World
Compile_obj/World/%.o: World/%.c World/%.h
	@mkdir -p Compile_obj/World/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

#Utility
Compile_obj/Utility/%.o: Utility/%.c Utility/%.h
	@mkdir -p Compile_obj/Utility/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

#Combat
Compile_obj/Combat/%.o: Combat/%.c Combat/%.h
	@mkdir -p Compile_obj/Combat/
	$(MIDDLE)
	$(CC) -c  $< -o $@ $(LIB)

run: GameTest
	./GameTest
