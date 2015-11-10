LCM_JAR = /usr/local/share/java/lcm.jar
SPACE = lcmtypes

all: state_t.hpp state_t.java lcmtypes.jar

%.hpp:
	lcm-gen -x *.lcm

%.java: %.lcm
	lcm-gen -j *.lcm

lcmtypes.jar:
	javac -cp $(LCM_JAR) $(SPACE)/*.java
	jar cf lcmtypes.jar $(SPACE)/*.class

clean:
	rm -f *.hpp *.jar
	rm -rf lcmtypes/
