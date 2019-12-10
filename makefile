CC = gcc -g
EXE = TestFont TestWindow TestCanvas TestSize TestLabel TestSprite TestDisplay WTest TestBug TestColoring TestRoom WTestOld InvTest
OBJECTS = Attributes.o Skill.o Pixel.o Canvas.o Character.o SpriteDic.o Trigger.o TriggerDic.o Sprite.o Font.o Wlabel.o Welem.o Room.o Inventory.o Wlabic.o Window.o Entity.o Display.o Object.o ObjectDic.o
ENGINE = 
LIB= -I Display/ -I Utility/ -I Entity/ -I Inventory/

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

Trigger.o : Display/Trigger.c Display/Trigger.h Utility.o
	@echo "#--------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TriggerDic.o : Display/TriggerDic.c Display/TriggerDic.h Utility.o
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

Entity.o:  Entity/Entity.c Entity/Entity.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Attributes.o:  Inventory/Attributes.c Inventory/Attributes.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Object.o:  Inventory/Object.c Inventory/Object.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Inventory.o:  Inventory/Inventory.c Inventory/Inventory.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

ObjectDic.o:  Inventory/ObjectDic.c Inventory/ObjectDic.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Skill.o:  Inventory/Skill.c Inventory/Skill.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)