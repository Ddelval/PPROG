CC = gcc -g
EXE =
DISPLAY = Pixel.o Canvas.o Character.o Font.o Wlabel.o Window.o
LIB= -I Display/ -I Utility/

clean:
	rm -f *.o core $(EXE)
TestFont: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestFont.o: Testing/TestFont.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

TestLabel: %: %.o $(DISPLAY)
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -o $@ $@.o $(DISPLAY) Utility.o -lm

TestLabel.o: Testing/TestLabel.c Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Canvas.o : Display/Canvas.c Display/Canvas.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Character.o : Display/Character.c Display/Character.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Font.o : Display/Font.c Display/Font.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Pixel.o : Display/Pixel.c Display/Pixel.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Wlabel.o : Display/Wlabel.c Display/Wlabel.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Wlabel.o : Display/Window.c Display/Window.h Utility.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)

Utility.o:  Utility/Utility.c Utility/Utility.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) -c $< $(LIB)
