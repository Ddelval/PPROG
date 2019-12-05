CC = gcc -g
EXE = TestFont TestWindow TestCanvas TestSize TestLabel TestSprite TestDisplay WTest TestBug TestColoring TestRoom WTestOld InvTest
OBJECTS = Attributes.o Skill.o Object.o ObjectDic.o Inventory.o Entity.o Pixel.o Canvas.o Character.o Font.o Wlabel.o Welem.o Window.o Sprite.o Room.o SpriteDic.o Wlabic.o Display.o
ENGINE = 
LIB= -I Display/ -I Utility/ -I DInventory/

all: $(EXE)

clean:
	rm -f *.o core $(EXE)

TestFont: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestFont.o: Testing/TestFont.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestColoring: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestColoring.o: Testing/TestColoring.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestBug: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestBug.o: Testing/TestBug.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestLabel: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestLabel.o: Testing/TestLabel.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestDisplay: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestDisplay.o: Testing/TestDisplay.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestWindow: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestWindow.o: Testing/TestWindow.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestCanvas: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestCanvas.o: Testing/TestCanvas.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestSprite: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

TestSprite.o: Testing/TestSprite.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestSize: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm -lncurses

TestSize.o: Testing/TestSize.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestRoom: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm -lncurses

TestRoom.o: Testing/TestRoom.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

WTest: %: %.o  $(OBJECTS) 
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o  $(OBJECTS)  Utility.o -lm

WTest.o: Testing/WTest.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

WTestOld: %: %.o $(OBJECTS)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS) Utility.o -lm

WTestOld.o: Testing/WTestOld.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

InvTest: %: %.o $(OBJECTS) 
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(OBJECTS)  Utility.o -lm

InvTest.o: Testing/InvTest.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# # # # # # # # # # # 				 Source Files 			# # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

Canvas.o : Display/Canvas.c Display/Canvas.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Character.o : Display/Character.c Display/Character.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Font.o : Display/Font.c Display/Font.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Pixel.o : Display/Pixel.c Display/Pixel.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Wlabel.o : Display/Wlabel.c Display/Wlabel.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Window.o : Display/Window.c Display/Window.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Sprite.o : Display/Sprite.c Display/Sprite.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Room.o : Display/Room.c Display/Room.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

SpriteDic.o : Display/SpriteDic.c Display/SpriteDic.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Welem.o : Display/Welem.c Display/Welem.h Utility.o
	@echo "#--------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Wlabic.o : Display/Wlabic.c Display/Wlabic.h Utility.o
	@echo "#--------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Display.o : Display/Display.c Display/Display.h Utility.o
	@echo "#--------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Utility.o:  Utility/Utility.c Utility/Utility.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Attributes.o:  DInventory/Attributes.c DInventory/Attributes.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Entity.o:  DInventory/Entity.c DInventory/Entity.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Inventory.o:  DInventory/Inventory.c DInventory/Inventory.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Object.o:  DInventory/Object.c DInventory/Object.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

ObjectDic.o:  DInventory/ObjectDic.c DInventory/ObjectDic.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Skill.o:  DInventory/Skill.c DInventory/Skill.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)
