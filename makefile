CC = gcc -g
EXE = TestFont TestWindow TestCanvas TestSize TestLabel TestSprite TestDisplay WTest
DISPLAY = Pixel.o Canvas.o Character.o Font.o Wlabel.o Welem.o Window.o Sprite.o Room.o SpriteDic.o Wlabic.o Display.o
LIB= -I Display/ -I Utility/

all: $(EXE)

clean:
	rm -f *.o core $(EXE)

TestFont: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestFont.o: Testing/TestFont.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestLabel: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestLabel.o: Testing/TestLabel.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestDisplay: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestDisplay.o: Testing/TestDisplay.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestWindow: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestWindow.o: Testing/TestWindow.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestCanvas: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestCanvas.o: Testing/TestCanvas.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestSprite: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestSprite.o: Testing/TestSprite.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestSize: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm -lncurses

TestSize.o: Testing/TestSize.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

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

WTest: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

WTest.o: Testing/WTest.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)
