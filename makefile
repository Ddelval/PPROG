CC = gcc -g
EXE = TestFont TestWindow TestCanvas TestSize TestLabel TestSprite TestDisplay WTest TestBug TestColoring TestRoom WTestOld InvTest CraftTest
OBJECTS = Attributes.o Skill.o Pixel.o Canvas.o Trigger.o Character.o Entity.o SpriteDic.o TriggerDic.o Sprite.o Font.o FontCatalog.o Wlabel.o Welem.o Room.o Inventory.o  Wlabic.o Window.o Display.o Object.o Recipe.o RecipeDic.o ObjectDic.o TrigReact.o
ENGINE =
LIB= -I Display/ -I Utility/ -I Entity/ -I Inventory/
MIDDLE = @printf "\033[1m\033[1;34m $@ \033[0;30m\033[0m\t"
all: $(EXE)

clean:
	rm -f *.o core $(EXE)

TestFont: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestFont.o: Testing/TestFont.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestColoring: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestColoring.o: Testing/TestColoring.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestBug: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestBug.o: Testing/TestBug.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestLabel: %: %.o $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestLabel.o: Testing/TestLabel.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestDisplay: %: %.o $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestDisplay.o: Testing/TestDisplay.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestWindow: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestWindow.o: Testing/TestWindow.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestCanvas: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestCanvas.o: Testing/TestCanvas.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestSprite: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestSprite.o: Testing/TestSprite.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestSize: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm -lncurses

TestSize.o: Testing/TestSize.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TestRoom: %: %.o $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm -lncurses

TestRoom.o: Testing/TestRoom.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

WTest: %: %.o  $(OBJECTS)
	$(MIDDLE)
	$(CC) -o $@ $@.o  $(OBJECTS)  Utility.o -lm

WTest.o: Testing/WTest.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

WTestOld: %: %.o $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

WTestOld.o: Testing/WTestOld.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

InvTest: %: %.o $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS)  Utility.o -lm

InvTest.o: Testing/InvTest.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

CraftTest: %: %.o $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS)  Utility.o -lm

CraftTest.o: Testing/CraftTest.c Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # # # # # # # # # # 				 Source Files 			# # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

Canvas.o : Display/Canvas.c Display/Canvas.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Character.o : Display/Character.c Display/Character.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Font.o : Display/Font.c Display/Font.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

FontCatalog.o : Display/FontCatalog.c Display/FontCatalog.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Pixel.o : Display/Pixel.c Display/Pixel.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Wlabel.o : Display/Wlabel.c Display/Wlabel.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Window.o : Display/Window.c Display/Window.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Sprite.o : Display/Sprite.c Display/Sprite.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Room.o : Display/Room.c Display/Room.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

SpriteDic.o : Display/SpriteDic.c Display/SpriteDic.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Welem.o : Display/Welem.c Display/Welem.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Wlabic.o : Display/Wlabic.c Display/Wlabic.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Display.o : Display/Display.c Display/Display.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Trigger.o : Display/Trigger.c Display/Trigger.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TriggerDic.o : Display/TriggerDic.c Display/TriggerDic.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)

TrigReact.o : Display/TrigReact.c Display/TrigReact.h Utility.o
	$(MIDDLE)
	$(CC) -c $< $(LIB)
Utility.o:  Utility/Utility.c Utility/Utility.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Entity.o:  Entity/Entity.c Entity/Entity.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Attributes.o:  Inventory/Attributes.c Inventory/Attributes.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Object.o:  Inventory/Object.c Inventory/Object.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Inventory.o:  Inventory/Inventory.c Inventory/Inventory.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

ObjectDic.o:  Inventory/ObjectDic.c Inventory/ObjectDic.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Skill.o:  Inventory/Skill.c Inventory/Skill.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

Recipe.o:  Inventory/Recipe.c Inventory/Recipe.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)

RecipeDic.o:  Inventory/RecipeDic.c Inventory/RecipeDic.h
	$(MIDDLE)
	$(CC) -c $< $(LIB)
